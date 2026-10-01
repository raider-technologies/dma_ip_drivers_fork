#ifndef __XDMA_RING_BUFFER_UAPI_H__
#define __XDMA_RING_BUFFER_UAPI_H__

#include "linux/ioctl.h"
#include "linux/types.h"

struct xdma_ring_ioctl {
	__aligned_u64   ptr;
	__u32           slot_count;
	__u32           slot_bytes;
};

#define IOCTL_XDMA_REGISTER_RING	_IOW('q', 0x40, struct xdma_ring_ioctl)
#define IOCTL_XDMA_UNREGISTER_RING	_IOR('q', 0x41, int)
#define IOCTL_XDMA_SUBMIT_SLOT		_IOR('q', 0x42, int)
#define IOCTL_XDMA_WITHDRAW_SLOT	_IOR('q', 0x43, int)

#endif // __XDMA_RING_BUFFER_UAPI_H__