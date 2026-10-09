
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/vmalloc.h>

#include "libxdma.h"
#include "libxdma_api.h"
#include "cdev_sgdma.h"
#include "xdma_thread.h"
#include "xdma_ring.h"
#include "cdev_ring.h"

int xdma_register_slot(struct xdma_ring_slot* slot, struct xdma_dev* xdev, enum dma_data_direction dir) {
	int nents;
	struct sg_table *sgt = &slot->io.sgt;
	struct scatterlist *sgl = sgt->sgl;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 16, 0)
		nents = pci_map_sg(xdev->pdev, sg, sgt->orig_nents, dir);
#else
		nents = dma_map_sg(&xdev->pdev->dev, sgl, sgt->orig_nents, dir);
#endif
	if (!nents) {
		pr_debug("map sgl failed, sgt 0x%p.\n", sgt);
		return -EIO;
	}
	sgt->nents = nents;
	slot->dma_mapped = true;

	dma_sync_sg_for_cpu(&xdev->pdev->dev, sgl, sgt->orig_nents, dir);
	slot->state = USER_OWNED;
	return 0;
}

int xdma_unregister_slot(struct xdma_ring_slot* slot, struct xdma_dev* xdev, enum dma_data_direction dir) {
	struct sg_table *sgt = &slot->io.sgt;
#if LINUX_VERSION_CODE < KERNEL_VERSION(5, 16, 0)
	pci_unmap_sg(xdev->pdev, sgt->sgl, sgt->orig_nents, dir);
#else
	dma_unmap_sg(&xdev->pdev->dev, sgt->sgl, sgt->orig_nents, dir);
#endif
	sgt->nents = 0;
	slot->dma_mapped = false;
	return 0;
}

ssize_t xdma_xfer_slot_submit(struct xdma_engine* engine, struct xdma_ring_slot* slot, 
                              void* dev_hndl, int timeout_ms) {
	struct xdma_dev *xdev = (struct xdma_dev *)dev_hndl;
	int rv = 0, tfer_idx = 0, i;
	ssize_t done = 0;
    struct sg_table* sgt = &slot->io.sgt;
	struct scatterlist *sg = sgt->sgl;
	int nents;
	struct xdma_request_cb *req = NULL;
    uint64_t ep_addr = slot->io.ep_addr;

	if (!dev_hndl)
		return -EINVAL;

	if (debug_check_dev_hndl(__func__, xdev->pdev, dev_hndl) < 0)
		return -EINVAL;

	if (!engine) {
		pr_err("dma engine NULL\n");
		return -EINVAL;
	}

	if (engine->magic != MAGIC_ENGINE) {
		pr_err("%s has invalid magic number %lx\n", engine->name,
		       engine->magic);
		return -EINVAL;
	}

	if (xdma_device_flag_check(xdev, XDEV_FLAG_OFFLINE)) {
		pr_debug("xdev 0x%p, offline.\n", xdev);
		return -EBUSY;
	}

	req = xdma_init_request(sgt, ep_addr);
	if (!req) {
		rv = -ENOMEM;
		goto unmap_sgl;
	}

	dbg_tfr("%s, len %u sg cnt %u.\n", engine->name, req->total_len,
		req->sw_desc_cnt);

	sg = sgt->sgl;
	nents = req->sw_desc_cnt;
	mutex_lock(&engine->desc_lock);

	while (nents) {
		unsigned long flags;
		struct xdma_transfer *xfer;

		/* build transfer */
		rv = transfer_init(engine, req, &req->tfer[0]);
		if (rv < 0) {
			mutex_unlock(&engine->desc_lock);
			goto unmap_sgl;
		}
		xfer = &req->tfer[0];

		/* last transfer for the given request? */
		nents -= xfer->desc_num;
		if (!nents) {
			xfer->last_in_request = 1;
			xfer->sgt = sgt;
		}

		dbg_tfr("xfer, %u, ep 0x%llx, done %lu, sg %u/%u.\n", xfer->len,
			req->ep_addr, done, req->sw_desc_idx, req->sw_desc_cnt);

#ifdef __LIBXDMA_DEBUG__
		transfer_dump(xfer);
#endif

		rv = transfer_queue(engine, xfer);
		if (rv < 0) {
			mutex_unlock(&engine->desc_lock);
			pr_debug("unable to submit %s, %d.\n", engine->name, rv);
			goto unmap_sgl;
		}

		if (engine->cmplthp)
			xdma_kthread_wakeup(engine->cmplthp);

		if (timeout_ms > 0)
			swait_event_interruptible_timeout_exclusive(xfer->wq,
				(xfer->state != TRANSFER_STATE_SUBMITTED),
				msecs_to_jiffies(timeout_ms));
		else
			swait_event_interruptible_exclusive(xfer->wq,
				(xfer->state != TRANSFER_STATE_SUBMITTED));

		spin_lock_irqsave(&engine->lock, flags);

		switch (xfer->state) {
		case TRANSFER_STATE_COMPLETED:
			spin_unlock_irqrestore(&engine->lock, flags);

			rv = 0;
			dbg_tfr("transfer %p, %u, ep 0x%llx compl, +%lu.\n",
				xfer, xfer->len, req->ep_addr - xfer->len,
				done);

			/* For C2H streaming use writeback results */
			if (engine->streaming &&
			    engine->dir == DMA_FROM_DEVICE) {
				struct xdma_result *result = xfer->res_virt;

				for (i = 0; i < xfer->desc_cmpl; i++)
					done += result[i].length;

				/* finish the whole request */
				if (engine->eop_flush && (xfer->flags & XFER_FLAG_ST_C2H_EOP_RCVED))
					nents = 0;
			} else
				done += xfer->len;

			break;
		case TRANSFER_STATE_FAILED:
			pr_debug("xfer 0x%p,%u, failed, ep 0x%llx.\n", xfer,
				xfer->len, req->ep_addr - xfer->len);
			spin_unlock_irqrestore(&engine->lock, flags);

#ifdef __LIBXDMA_DEBUG__
			transfer_dump(xfer);
			sgt_dump(sgt);
#endif
			rv = -EIO;
			break;
		default:
			/* transfer can still be in-flight */
			pr_debug("xfer 0x%p,%u, s 0x%x timed out, ep 0x%llx.\n",
				xfer, xfer->len, xfer->state, req->ep_addr);
			rv = engine_status_read(engine, 0, 1);
			if (rv < 0) {
				pr_err("Failed to read engine status\n");
			} else if (rv == 0) {
				//engine_status_dump(engine);
				rv = transfer_abort(engine, xfer);
				if (rv < 0) {
					pr_err("Failed to stop engine\n");
				} else if (rv == 0) {
					rv = xdma_engine_stop(engine);
					if (rv < 0)
						pr_err("Failed to stop engine\n");
				}
			}
			spin_unlock_irqrestore(&engine->lock, flags);

#ifdef __LIBXDMA_DEBUG__
			transfer_dump(xfer);
			sgt_dump(sgt);
#endif
			rv = -ETIMEDOUT;
			break;
		}

		engine->desc_used -= xfer->desc_num;
		transfer_destroy(xdev, xfer);

		/* use multiple transfers per request if we could not fit
		 * all data within single descriptor chain.
		 */
		tfer_idx++;

		if (rv < 0) {
			mutex_unlock(&engine->desc_lock);
			goto unmap_sgl;
		}
	} /* while (sg) */
	mutex_unlock(&engine->desc_lock);

unmap_sgl:
	if (req)
		xdma_request_free(req);

	/* as long as some data is processed, return the count */
	return done ? done : rv;
}
