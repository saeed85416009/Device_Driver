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
	{ 0x971081b7, "cdev_del" },
	{ 0x242c5130, "device_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x6582b2f7, "wake_up_process" },
	{ 0xd69ca7ff, "kthread_create_on_node" },
	{ 0xca52963a, "proc_create" },
	{ 0x1dbb4dfd, "class_destroy" },
	{ 0x5adbc918, "device_create" },
	{ 0xd7a5092d, "__class_create" },
	{ 0x579ef545, "cdev_add" },
	{ 0xd63a4ffe, "cdev_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0xee7f2c95, "kmem_cache_alloc_trace" },
	{ 0x988fdb69, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x6b10bee1, "_copy_to_user" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x56470118, "__warn_printk" },
	{ 0x1000e51, "schedule" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x952664c5, "do_exit" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x6df1aaf1, "kernel_sigaction" },
	{ 0xea6a457d, "current_task" },
	{ 0xc5850110, "printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");

