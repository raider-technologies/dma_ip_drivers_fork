#ifndef __XDMA_RING_BUFFER_UAPI_H__
#define __XDMA_RING_BUFFER_UAPI_H__

#include "linux/ioctl.h"
#include "linux/types.h"

struct xdma_ring_ioctl {
	__aligned_u64   ptr;
	__u32           slot_count;
	__u32           slot_bytes;
};

struct xdma_ring_slot_ioctl {
	__u32 			slot_index; 
	__u32			xfer_id; // for book-keeping to ensure ording
	__s32			status; // 0 for success/submit, negative for failure on withdraw
	__u32			xfer_bytes; // Num actual bytes to transfer for write
};

#define IOCTL_XDMA_REGISTER_RING	_IOW('q', 0x40, struct xdma_ring_ioctl)
#define IOCTL_XDMA_UNREGISTER_RING	_IO('q', 0x41)
#define IOCTL_XDMA_SUBMIT_SLOT		_IOW('q', 0x42, struct xdma_ring_slot_ioctl)
#define IOCTL_XDMA_WITHDRAW_SLOT	_IOR('q', 0x43, struct xdma_ring_slot_ioctl)

#endif // __XDMA_RING_BUFFER_UAPI_H__