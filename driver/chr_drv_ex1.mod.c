#include <linux/build-salt.h>
#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(.gnu.linkonce.this_module) = {
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

static const struct modversion_info ____versions[]
__used __section(__versions) = {
	{ 0x56994e6, "module_layout" },
	{ 0x1ded2093, "kthread_stop" },
	{ 0x82072614, "tasklet_kill" },
	{ 0x971081b7, "cdev_del" },
	{ 0x242c5130, "device_destroy" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x1dbb4dfd, "class_destroy" },
	{ 0x6582b2f7, "wake_up_process" },
	{ 0xd69ca7ff, "kthread_create_on_node" },
	{ 0x9545af6d, "tasklet_init" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xca52963a, "proc_create" },
	{ 0x5adbc918, "device_create" },
	{ 0xd7a5092d, "__class_create" },
	{ 0x579ef545, "cdev_add" },
	{ 0xd63a4ffe, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xfaef0ed, "__tasklet_schedule" },
	{ 0x47941711, "_raw_spin_lock_irq" },
	{ 0xf9a482f9, "msleep" },
	{ 0xdbf17652, "_raw_spin_lock" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x577fbd20, "pv_ops" },
	{ 0xee7f2c95, "kmem_cache_alloc_trace" },
	{ 0x988fdb69, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x56470118, "__warn_printk" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

