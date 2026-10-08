#ifndef _CDEV_RING_H_
#define _CDEV_RING_H_

#include <linux/ioctl.h>
#include "libxdma.h"

// #ifdef __KERNEL__
enum xdma_slot_state {
	USER_OWNED			= 1,
	SUBMITTED			= 2,
	IN_USE				= 3,
	FOR_WITHDRAW		= 4,
	XFER_FAIL			= 5
};

struct xdma_ring_slot {
	unsigned int 			i;
	enum xdma_slot_state	state;
	struct xdma_io_cb		io;
	// struct mutex			slot_lock;
	size_t					submitted_bytes;
	size_t					withdraw_bytes;
	unsigned long			submission_id;
	int						completion_status;
	bool					dma_mapped;
};

struct xdma_ring {
	unsigned long 			user_base;
	size_t					slot_bytes;
	unsigned int			slot_count;
	unsigned int			queued_slot_cnt;
	struct xdma_ring_slot*	slots;
	unsigned int*			withdraw_queue;
	unsigned int*			withdraw_queue_head;
	unsigned int*			withdraw_queue_tail;
};

struct xdma_cdev;

struct xdma_file_context {
	struct xdma_cdev* 	xcdev;
	struct xdma_ring* 	ring;
	struct mutex 		ring_lock;
};
// #endif 

int ioctl_do_ring_registration(struct xdma_engine *engine, unsigned long arg, struct xdma_file_context* ctx);
int ioctl_do_ring_unregistration(struct xdma_file_context* ctx);
int ioctl_do_ring_slot_submit(struct xdma_engine *engine, unsigned long arg, struct xdma_file_context* ctx);
int ioctl_do_ring_slot_withdraw(unsigned long arg, struct xdma_file_context* ctx);
int ring_destroy_locked(struct xdma_file_context *ctx);


#endif //_CDEV_RING_H_