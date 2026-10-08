#ifndef _XDMA_RING_H_
#define _XDMA_RING_H_

struct xdma_ring_slot;
int xdma_register_slot(struct xdma_ring_slot* slot, struct xdma_dev* xdev, enum dma_data_direction dir);
int xdma_unregister_slot(struct xdma_ring_slot* slot, struct xdma_dev* xdev, enum dma_data_direction dir);
ssize_t xdma_xfer_slot_submit(struct xdma_engine* engine, struct xdma_ring_slot* slot, 
                              void* dev_hndl, int timeout_ms);
#endif // _XDMA_RING_H_