#ifndef _XDMA_RING_H_
#define _XDMA_RING_H_

struct xdma_ring_slot;
int xdma_register_slot(struct xdma_ring_slot* slot, struct xdma_dev* xdev, enum dma_data_direction dir);
int xdma_unregister_slot(struct xdma_ring_slot* slot, struct xdma_dev* xdev, enum dma_data_direction dir);

#endif // _XDMA_RING_H_