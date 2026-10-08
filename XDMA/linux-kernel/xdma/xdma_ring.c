
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
		pr_info("map sgl failed, sgt 0x%p.\n", sgt);
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

