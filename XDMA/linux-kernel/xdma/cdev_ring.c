#include <linux/types.h>
#include <asm/cacheflush.h>
#include <linux/slab.h>
#include <linux/aio.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(3, 16, 0)
#include <linux/uio.h>
#endif
#include "libxdma_api.h"
#include "xdma_cdev.h"
#include "cdev_sgdma.h"
#include "xdma_thread.h"
#include "xdma_ring_uapi.h"
#include "cdev_ring.h"
#include "xdma_ring.h"

int ioctl_do_ring_registration(struct xdma_engine *engine, unsigned long arg, struct xdma_file_context* ctx) {
	int rv;
	mutex_lock(&ctx->ring_lock);
	if (ctx->ring) {
		pr_info("Ring Buffer already registered\n");
		rv = -EBUSY;
		goto release_ring_lock;
	}

	struct xdma_ring_ioctl ring_ioctl;

	if (copy_from_user(&ring_ioctl, (void __user*)arg, sizeof(struct xdma_ring_ioctl))) {
		pr_info("Failed to copy xdma_ring_ioctl from user space 0x%lx\n", arg);
		rv = -EFAULT;
		goto release_ring_lock;
	}
	pr_info("Register: ptr=%#llx slots=%u slot_bytes=%u page_size=%lu\n",
        (unsigned long long)ring_ioctl.ptr,
        ring_ioctl.slot_count,
        ring_ioctl.slot_bytes,
        (unsigned long)PAGE_SIZE);
	if (!ring_ioctl.slot_count || !ring_ioctl.slot_bytes) {
		pr_info("Ring buffer params not initilized.\n");
		rv = -EINVAL;
		goto release_ring_lock;
	}
	if (ring_ioctl.ptr > ULONG_MAX) {
		pr_info("Registration ring pointer larger than ULONG MAX");
		rv = -EINVAL;
		goto release_ring_lock;
	}
	unsigned long base = (unsigned long) ring_ioctl.ptr;
	if (!IS_ALIGNED(base, PAGE_SIZE) || !IS_ALIGNED(ring_ioctl.slot_bytes, PAGE_SIZE)) {
		pr_info("register: base or slot size is not page aligned\n");
		rv = -EINVAL;
		goto release_ring_lock;
	}
	size_t total_bytes;
	unsigned long end;
	if (check_mul_overflow((size_t)ring_ioctl.slot_count, (size_t)ring_ioctl.slot_bytes, &total_bytes) ||
		check_add_overflow(base, (unsigned long) total_bytes, &end)) {
		rv = -EOVERFLOW;
		pr_info("registration overflow check failed.\n");
		goto release_ring_lock;
	}
	if (!access_ok((void __user *)base, total_bytes)) {
		pr_info("registration access check failed.\n");
		rv = -EFAULT;
		goto release_ring_lock;
	}

	struct xdma_ring* ring = kzalloc(sizeof(struct xdma_ring), GFP_KERNEL);
	if (!ring) {
		pr_info("Failed to allocate xdma ring\n");
		rv = -ENOMEM;
		goto release_ring_lock;
	}
	ring->slot_bytes = ring_ioctl.slot_bytes;
	ring->slot_count = ring_ioctl.slot_count;
	ring->user_base = ring_ioctl.ptr;
	ring->withdraw_queue = kcalloc(ring->slot_count, sizeof(unsigned int), GFP_KERNEL);
	if (!ring->withdraw_queue) {
		pr_info("Failed to allocate xdma ring withdraw queue.\n");
		rv = -ENOMEM;
		goto ring_cleanup;
	}
	ring->withdraw_queue_head = ring->withdraw_queue;
	ring->withdraw_queue_tail = ring->withdraw_queue;
	ring->slots = kcalloc(ring->slot_count, sizeof(struct xdma_ring_slot), GFP_KERNEL);
	if (!ring->slots) {
		pr_info("Failed to allocate xdma ring slots\n");
		rv = -ENOMEM;
		goto queue_cleanup;
	}

	int write = (engine->dir == DMA_TO_DEVICE);
	int prepared_slots = 0;
	for (int i = 0; i < ring_ioctl.slot_count; i++) {
		char __user* buf = (char __user*) (ring_ioctl.ptr + (size_t)i * ring_ioctl.slot_bytes);
		rv = check_transfer_align(engine, buf, ring_ioctl.slot_bytes, 0, 1);
		if (rv) {
			pr_info("Invalid transfer alignment detected on ring buffer slot %u.\n", i);
			goto slots_cleanup;
		}
		struct xdma_ring_slot* slot = &ring->slots[i];
		slot->i = i; 
		slot->dma_mapped = false;
		struct xdma_io_cb* cb = &slot->io;
		cb->buf = buf;
		cb->len = ring_ioctl.slot_bytes;
		cb->ep_addr = 0;
		cb->write = write;
		cb->req = NULL;
		rv = char_sgdma_map_user_buf_to_sgl(cb, write, true);
		if (rv < 0) {
			goto slots_cleanup;
		}
		prepared_slots++;
		rv = xdma_register_slot(slot, ctx->xcdev->xdev, engine->dir); // Returns non-zero on error
		if (rv)
			goto slots_cleanup;

	}
	ctx->ring = ring;
	mutex_unlock(&ctx->ring_lock);
	return 0;

slots_cleanup:
	for (int i = 0; i < prepared_slots; i++) {
		struct xdma_ring_slot* slot = &ring->slots[i];
		if (slot->dma_mapped) 
			xdma_unregister_slot(slot, ctx->xcdev->xdev, engine->dir);
		char_sgdma_unmap_user_buf(&slot->io, write);
	}
	kfree(ring->slots);
queue_cleanup:
	kfree(ring->withdraw_queue);
ring_cleanup:
	kfree(ring);
release_ring_lock:
	mutex_unlock(&ctx->ring_lock);
	return rv;
}

int ring_destroy_locked(struct xdma_file_context *ctx) {
	struct xdma_ring* ring = ctx->ring;
	for (int i = 0; i < ring->slot_count; i++) {
		struct xdma_ring_slot* slot = &ring->slots[i];
		xdma_unregister_slot(slot, ctx->xcdev->xdev, ctx->xcdev->engine->dir);
		char_sgdma_unmap_user_buf(&slot->io, (ctx->xcdev->engine->dir == DMA_TO_DEVICE));
	}
	kfree(ring->withdraw_queue);
	kfree(ring->slots);
	kfree(ctx->ring);
	ctx->ring = NULL;
	return 0;
}

int ioctl_do_ring_unregistration(struct xdma_file_context* ctx) {
	int rv;
	mutex_lock(&ctx->ring_lock);
	if (!ctx->ring) {
		pr_info("Ring buffer not registered.\n");
		rv = -ENOENT;
		goto ring_unlock;
	}
	struct xdma_ring* ring = ctx->ring;
	for (int i = 0; i < ring->slot_count; i++) {
		struct xdma_ring_slot* slot = &ring->slots[i];
		if (slot->state != USER_OWNED) {
			rv = -EBUSY;
			goto ring_unlock;
		}
	}
	rv = ring_destroy_locked(ctx);
ring_unlock:
	mutex_unlock(&ctx->ring_lock);
	return rv;
}

int ioctl_do_ring_slot_submit(struct xdma_engine *engine, unsigned long arg, struct xdma_file_context* ctx) {
	// int rv;
	struct xdma_ring_slot_ioctl slot_ioctl;

	if (copy_from_user(&slot_ioctl, (void __user*)arg, sizeof(struct xdma_ring_slot_ioctl))) {
		pr_info("Failed to copy xdma_ring_ioctl from user space 0x%lx\n", arg);
		return -EFAULT;
	}
	mutex_lock(&ctx->ring_lock);
	if (!ctx->ring) {
		pr_info("Attempted slot submission on unregistered ring.\n");
		mutex_unlock(&ctx->ring_lock);
		return -ENOENT;
	}
	if (!slot_ioctl.xfer_bytes || slot_ioctl.xfer_bytes > ctx->ring->slot_bytes) {
		pr_info("Slot submit: Invalid xfer_bytes: %u\n", slot_ioctl.xfer_bytes);
		mutex_unlock(&ctx->ring_lock);
		return -EINVAL;
	}
	if (slot_ioctl.slot_index >= ctx->ring->slot_count) {
		pr_info("Slot submit: Invalid slot id: %u out of %u slots.\n", slot_ioctl.slot_index, ctx->ring->slot_count);
		mutex_unlock(&ctx->ring_lock);
		return -EINVAL;
	}
	struct xdma_ring_slot* slot = &ctx->ring->slots[slot_ioctl.slot_index];
	if (slot->state != USER_OWNED) {
		pr_info("Requested submission slot already busy\n");
		mutex_unlock(&ctx->ring_lock);
		return -EBUSY;
	}
	slot->state = SUBMITTED;
	mutex_unlock(&ctx->ring_lock);
	slot->submission_id = slot_ioctl.xfer_id;
	slot->submitted_bytes = slot_ioctl.xfer_bytes;

	dma_sync_sg_for_device(&ctx->xcdev->xdev->pdev->dev, slot->io.sgt.sgl, slot->io.sgt.orig_nents, engine->dir);

	slot->state = IN_USE;
	// TODO: non-blocking transfer and support for xfer_bytes != slot_bytes
	ssize_t res = 0;
	int timeout = (engine->dir == DMA_TO_DEVICE) ? h2c_timeout * 1000 : c2h_timeout * 1000;
	res = slot->submitted_bytes;//xdma_xfer_slot_submit(engine, slot, ctx->xcdev->xdev, timeout);
	if (res < 0) {
		slot->state = XFER_FAIL;
		slot->withdraw_bytes = 0;
	} else {
		slot->withdraw_bytes = res;
		slot->state = FOR_WITHDRAW;
	}
	slot->completion_status = (res < 0) ? res : 0;
	mutex_lock(&ctx->ring_lock);
	ctx->ring->queued_slot_cnt++;
	ctx->ring->withdraw_queue_tail[0] = slot->i;
	if (++ctx->ring->withdraw_queue_tail >= &ctx->ring->withdraw_queue[ctx->ring->slot_count])
		ctx->ring->withdraw_queue_tail = ctx->ring->withdraw_queue;
	mutex_unlock(&ctx->ring_lock);
	return 0;
}

int ioctl_do_ring_slot_withdraw(unsigned long arg, struct xdma_file_context* ctx) {
	int rv = 0; 
	mutex_lock(&ctx->ring_lock);
	if (!ctx->ring) {
		pr_err("Withdraw request with no registered ring.\n");
		rv = -ENOENT;
		goto release_ring;
	}
	if (ctx->ring->queued_slot_cnt > ctx->ring->slot_count) {
		pr_err("Overwithdrawn slots.\n");
		rv = -EINVAL;
		goto release_ring;
	}
	if (ctx->ring->queued_slot_cnt == 0) {
		pr_info("No slots prepared for withdraw.\n");
		rv = -EAGAIN;
		goto release_ring;
	}
	struct xdma_ring_slot* slot = &ctx->ring->slots[*ctx->ring->withdraw_queue_head];
	
	struct xdma_ring_slot_ioctl slot_ioctl;
	slot_ioctl.slot_index = slot->i;
	slot_ioctl.xfer_id = slot->submission_id;
	switch (slot->state)
	{
	case FOR_WITHDRAW:
		slot_ioctl.xfer_bytes = slot->withdraw_bytes;
		break;
	case XFER_FAIL:
		slot_ioctl.xfer_bytes = 0;
		break;
	default:
		pr_info("invalid slot queued for withdraw.\n");
		rv = -EFAULT;
		goto release_ring;
	}
	slot_ioctl.status = slot->completion_status;
	dma_sync_sg_for_cpu(&ctx->xcdev->xdev->pdev->dev, slot->io.sgt.sgl, slot->io.sgt.orig_nents, ctx->xcdev->engine->dir);
	if (copy_to_user((void __user*)arg, &slot_ioctl, sizeof(struct xdma_ring_slot_ioctl))) {
		pr_info("Failed to copy xdma_ring_ioctl to user space 0x%lx\n", arg);
		rv = -EFAULT;
		goto release_ring;
	}
	slot->state = USER_OWNED;
	if (++ctx->ring->withdraw_queue_head >= &ctx->ring->withdraw_queue[ctx->ring->slot_count])
		ctx->ring->withdraw_queue_head = ctx->ring->withdraw_queue;
	ctx->ring->queued_slot_cnt--;
	
release_ring:
	mutex_unlock(&ctx->ring_lock);
	return rv;
}