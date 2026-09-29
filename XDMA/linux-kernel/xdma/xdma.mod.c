#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const char ____versions[]
__used __section("__versions") =
	"\x18\x00\x00\x00\xfd\xe1\xe8\x87"
	"pci_save_state\0\0"
	"\x14\x00\x00\x00\x3b\x4a\x51\xc1"
	"free_irq\0\0\0\0"
	"\x18\x00\x00\x00\x59\xb5\x0a\xc8"
	"swake_up_one\0\0\0\0"
	"\x1c\x00\x00\x00\xe4\xc3\xb2\xd0"
	"get_user_pages_fast\0"
	"\x1c\x00\x00\x00\x2b\x2f\xec\xe3"
	"alloc_chrdev_region\0"
	"\x1c\x00\x00\x00\x48\x9f\xdb\x88"
	"__check_object_size\0"
	"\x1c\x00\x00\x00\xeb\x5d\xe0\x9c"
	"flush_dcache_page\0\0\0"
	"\x18\x00\x00\x00\x49\x54\x3c\x34"
	"param_ops_uint\0\0"
	"\x1c\x00\x00\x00\x60\x73\x8b\x1a"
	"pci_enable_device\0\0\0"
	"\x1c\x00\x00\x00\x8f\x18\x02\x7f"
	"__msecs_to_jiffies\0\0"
	"\x20\x00\x00\x00\xb2\xb0\xed\xd9"
	"pci_enable_device_mem\0\0\0"
	"\x14\x00\x00\x00\xfd\x7c\xcf\x7a"
	"pci_iomap\0\0\0"
	"\x20\x00\x00\x00\x8e\x8c\x00\x1a"
	"pci_alloc_irq_vectors\0\0\0"
	"\x14\x00\x00\x00\x6e\x4a\x6e\x65"
	"snprintf\0\0\0\0"
	"\x18\x00\x00\x00\x36\xf2\xb6\xc5"
	"queue_work_on\0\0\0"
	"\x18\x00\x00\x00\x86\x50\xc8\xc8"
	"sg_free_table\0\0\0"
	"\x20\x00\x00\x00\xb5\x41\x87\x60"
	"__init_swait_queue_head\0"
	"\x14\x00\x00\x00\xbf\x0f\x54\x92"
	"finish_wait\0"
	"\x18\x00\x00\x00\xf1\x40\xce\x42"
	"class_destroy\0\0\0"
	"\x20\x00\x00\x00\x0a\xe0\x02\xad"
	"__pci_register_driver\0\0\0"
	"\x18\x00\x00\x00\x80\x30\x79\x8f"
	"pci_disable_msi\0"
	"\x1c\x00\x00\x00\xf1\xaa\xf1\x6d"
	"kernel_sigaction\0\0\0\0"
	"\x1c\x00\x00\x00\x90\xa7\xfd\x27"
	"pci_request_regions\0"
	"\x10\x00\x00\x00\x7e\xa4\x29\x48"
	"memcpy\0\0"
	"\x18\x00\x00\x00\xc3\x8d\x9f\x77"
	"remap_pfn_range\0"
	"\x10\x00\x00\x00\xba\x0c\x7a\x03"
	"kfree\0\0\0"
	"\x20\x00\x00\x00\x95\xd4\x26\x8c"
	"prepare_to_wait_event\0\0\0"
	"\x1c\x00\x00\x00\x6e\x64\xf7\xb3"
	"kthread_should_stop\0"
	"\x14\x00\x00\x00\x44\x43\x96\xe2"
	"__wake_up\0\0\0"
	"\x18\x00\x00\x00\x3f\x1f\x34\x47"
	"pci_irq_vector\0\0"
	"\x1c\x00\x00\x00\x8b\x31\xd7\x06"
	"kmem_cache_create\0\0\0"
	"\x20\x00\x00\x00\x0b\x05\xdb\x34"
	"_raw_spin_lock_irqsave\0\0"
	"\x1c\x00\x00\x00\xe9\xef\x28\x39"
	"__per_cpu_offset\0\0\0\0"
	"\x18\x00\x00\x00\x64\xbd\x8f\xba"
	"_raw_spin_lock\0\0"
	"\x20\x00\x00\x00\xa9\x13\xe8\xc0"
	"pci_unregister_driver\0\0\0"
	"\x18\x00\x00\x00\x8c\x89\xd4\xcb"
	"fortify_panic\0\0\0"
	"\x18\x00\x00\x00\xe2\xd8\x00\xe7"
	"wake_up_process\0"
	"\x10\x00\x00\x00\x7e\x3a\x2c\x12"
	"_printk\0"
	"\x20\x00\x00\x00\x21\xca\x28\x2b"
	"prepare_to_swait_event\0\0"
	"\x18\x00\x00\x00\x81\xc8\x24\x1d"
	"___ratelimit\0\0\0\0"
	"\x1c\x00\x00\x00\xad\x8a\xdd\x8d"
	"schedule_timeout\0\0\0\0"
	"\x14\x00\x00\x00\x51\x0e\x00\x01"
	"schedule\0\0\0\0"
	"\x1c\x00\x00\x00\xcb\xf6\xfd\xf0"
	"__stack_chk_fail\0\0\0\0"
	"\x1c\x00\x00\x00\x54\xfc\xbb\x6c"
	"__arch_copy_to_user\0"
	"\x14\x00\x00\x00\xfc\x11\x89\x61"
	"numa_node\0\0\0"
	"\x1c\x00\x00\x00\x3b\x03\x71\x22"
	"kmem_cache_alloc\0\0\0\0"
	"\x18\x00\x00\x00\xa8\x85\xf9\xb3"
	"sg_alloc_table\0\0"
	"\x14\x00\x00\x00\x09\x5a\x32\xd5"
	"cdev_add\0\0\0\0"
	"\x1c\x00\x00\x00\x1e\xcc\x4d\x27"
	"pci_find_capability\0"
	"\x18\x00\x00\x00\x75\x79\x48\xfe"
	"init_wait_entry\0"
	"\x18\x00\x00\x00\x21\x10\xf2\x0e"
	"pci_enable_msi\0\0"
	"\x14\x00\x00\x00\xd2\x19\xbc\x57"
	"down_write\0\0"
	"\x14\x00\x00\x00\x25\x7a\x80\xce"
	"up_write\0\0\0\0"
	"\x1c\x00\x00\x00\xc3\xc9\xb0\x43"
	"preempt_schedule\0\0\0\0"
	"\x18\x00\x00\x00\x7f\x79\x91\x60"
	"synchronize_rcu\0"
	"\x20\x00\x00\x00\x8e\x83\xd5\x92"
	"request_threaded_irq\0\0\0\0"
	"\x18\x00\x00\x00\xb1\x5b\xa6\xc7"
	"device_create\0\0\0"
	"\x18\x00\x00\x00\x2b\x3b\xb2\x4e"
	"class_create\0\0\0\0"
	"\x18\x00\x00\x00\x28\xd4\x5b\x5c"
	"finish_swait\0\0\0\0"
	"\x14\x00\x00\x00\x4b\x8d\xfa\x4d"
	"mutex_lock\0\0"
	"\x18\x00\x00\x00\x58\x14\x27\xb9"
	"kmem_cache_free\0"
	"\x18\x00\x00\x00\xb6\xfa\xf1\xad"
	"dma_alloc_attrs\0"
	"\x20\x00\x00\x00\x56\xe5\x60\xf4"
	"pci_read_config_word\0\0\0\0"
	"\x28\x00\x00\x00\x7a\xc6\x64\xfc"
	"pci_aer_clear_nonfatal_status\0\0\0"
	"\x18\x00\x00\x00\xd9\xe8\xa1\x53"
	"_find_next_bit\0\0"
	"\x1c\x00\x00\x00\xa0\x40\x32\x5e"
	"__cpu_online_mask\0\0\0"
	"\x18\x00\x00\x00\xd4\xb0\x96\x47"
	"kthread_stop\0\0\0\0"
	"\x1c\x00\x00\x00\xeb\x16\xf2\xfe"
	"_raw_spin_trylock\0\0\0"
	"\x18\x00\x00\x00\x9f\x0c\xfb\xce"
	"__mutex_init\0\0\0\0"
	"\x24\x00\x00\x00\x70\xce\x5c\xd3"
	"_raw_spin_unlock_irqrestore\0"
	"\x14\x00\x00\x00\x27\xfd\x87\xd8"
	"pci_iounmap\0"
	"\x1c\x00\x00\x00\x19\x4a\x2f\x2f"
	"pci_restore_state\0\0\0"
	"\x10\x00\x00\x00\xad\x64\xb7\xdc"
	"memset\0\0"
	"\x18\x00\x00\x00\x9c\xea\xe1\x1a"
	"pci_set_master\0\0"
	"\x14\x00\x00\x00\xd5\xe3\x7d\x01"
	"nr_cpu_ids\0\0"
	"\x20\x00\x00\x00\x54\xea\xa5\xd9"
	"__init_waitqueue_head\0\0\0"
	"\x18\x00\x00\x00\xab\x21\x47\x27"
	"kthread_bind\0\0\0\0"
	"\x34\x00\x00\x00\x2b\xe5\x26\x90"
	"pcie_capability_clear_and_set_word_unlocked\0"
	"\x10\x00\x00\x00\xa6\x50\xba\x15"
	"jiffies\0"
	"\x20\x00\x00\x00\x7b\xe2\xf2\x5b"
	"kthread_create_on_node\0\0"
	"\x20\x00\x00\x00\x9d\x70\x4c\x4a"
	"dma_set_coherent_mask\0\0\0"
	"\x10\x00\x00\x00\xfd\xf9\x3f\x3c"
	"sprintf\0"
	"\x14\x00\x00\x00\xb4\xf7\x2a\x7a"
	"cpu_number\0\0"
	"\x18\x00\x00\x00\x0b\xb9\x41\x79"
	"dma_free_attrs\0\0"
	"\x10\x00\x00\x00\x97\x82\x9e\x99"
	"vfree\0\0\0"
	"\x24\x00\x00\x00\x33\xb3\x91\x60"
	"unregister_chrdev_region\0\0\0\0"
	"\x18\x00\x00\x00\x38\xf0\x13\x32"
	"mutex_unlock\0\0\0\0"
	"\x1c\x00\x00\x00\xaf\x00\x90\x67"
	"pci_release_regions\0"
	"\x10\x00\x00\x00\xe4\x15\xe2\xfb"
	"sg_next\0"
	"\x14\x00\x00\x00\x44\xb0\xe3\x13"
	"__folio_put\0"
	"\x1c\x00\x00\x00\x0e\xf9\x12\x45"
	"kobject_set_name\0\0\0\0"
	"\x18\x00\x00\x00\x52\xc1\x33\xb7"
	"device_destroy\0\0"
	"\x20\x00\x00\x00\x28\xe1\xa4\x12"
	"__arch_copy_from_user\0\0\0"
	"\x1c\x00\x00\x00\xc6\x25\x50\x0e"
	"set_page_dirty_lock\0"
	"\x1c\x00\x00\x00\x17\xcb\xb6\x4a"
	"pci_disable_device\0\0"
	"\x1c\x00\x00\x00\xef\x6d\x5c\xa6"
	"alt_cb_patch_nops\0\0\0"
	"\x18\x00\x00\x00\x60\x70\x9d\xb2"
	"pcie_set_readrq\0"
	"\x18\x00\x00\x00\x8b\xe0\xa8\xfb"
	"dma_set_mask\0\0\0\0"
	"\x1c\x00\x00\x00\xbf\x59\x7d\x17"
	"dma_unmap_sg_attrs\0\0"
	"\x18\x00\x00\x00\x4c\x72\x3c\xa2"
	"kmalloc_trace\0\0\0"
	"\x20\x00\x00\x00\xe9\x01\xa7\xab"
	"pci_read_config_byte\0\0\0\0"
	"\x10\x00\x00\x00\x8f\x68\xee\xd6"
	"vmalloc\0"
	"\x20\x00\x00\x00\xc3\x24\x4b\x64"
	"pci_write_config_word\0\0\0"
	"\x1c\x00\x00\x00\x34\x4b\xb5\xb5"
	"_raw_spin_unlock\0\0\0\0"
	"\x20\x00\x00\x00\x47\x03\x69\x66"
	"pci_free_irq_vectors\0\0\0\0"
	"\x14\x00\x00\x00\x2b\x8c\x7b\x27"
	"cdev_init\0\0\0"
	"\x14\x00\x00\x00\x45\x3a\x23\xeb"
	"__kmalloc\0\0\0"
	"\x18\x00\x00\x00\xc7\xa2\xd5\x42"
	"kmalloc_caches\0\0"
	"\x14\x00\x00\x00\x7a\x51\x6d\x32"
	"cdev_del\0\0\0\0"
	"\x1c\x00\x00\x00\x1c\x2b\xcd\x59"
	"kmem_cache_destroy\0\0"
	"\x1c\x00\x00\x00\xf9\x6f\xac\x2a"
	"dma_map_sg_attrs\0\0\0\0"
	"\x14\x00\x00\x00\xd3\x85\x33\x2d"
	"system_wq\0\0\0"
	"\x18\x00\x00\x00\x8a\x10\xd2\xe6"
	"module_layout\0\0\0"
	"\x00\x00\x00\x00\x00\x00\x00\x00";

MODULE_INFO(depends, "");

MODULE_ALIAS("pci:v000010EEd00009048sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009044sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009042sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009041sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd0000903Fsv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009038sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009028sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009034sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009024sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009032sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009012sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00009011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008012sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008024sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008028sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008032sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008034sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00008038sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007011sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007012sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007014sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007018sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007021sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007022sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007024sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007028sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007031sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007032sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007034sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00007038sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006828sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006830sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006928sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006930sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006A28sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006A30sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00006D30sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004808sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004828sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004908sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004A28sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00004B28sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v000010EEd00002808sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D0Fd0000F000sv*sd*bc*sc*i*");
MODULE_ALIAS("pci:v00001D0Fd0000F001sv*sd*bc*sc*i*");

MODULE_INFO(srcversion, "342F49C1FD0E6E52C3A58D0");
