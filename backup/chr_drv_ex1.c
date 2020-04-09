#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

#define mem_size 1024

dev_t dev=0;
static struct cdev my_cdev; 
static struct class *dev_class;
uint8_t *kernel_buffer;

static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *flip, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *flip, const char *buf, size_t len, loff_t *off);

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read  = my_read,
	.write = my_write,
	.open  = my_open,
	.release = my_release,
};


static int __init chr_driver_init(void); 
static int __init chr_driver_exit(void); 


static int __init chr_driver_init(void){
	//allocating major number
	if ((alloc_chrdev_region(&dev,0,1,"my_dev"))<0){
		printk(KERN_INFO"canot allocate major number");
		return -1;
	}
	printk(KERN_INFO"Major = %d  Minor = %d \n", MAJOR(dev), MINOR(dev));
	
	// creating a cdev structure
	
	cdev_init(&my_cdev,&fops);
	
	// Adding character device to system 
	if ((cdev_add(&my_cdev,dev,1)<0)){
		printk(KERN_INFO"cannot add the device to the system...");
		goto r_class;
	}
	
	// creating struct class
	
	if((dev_class = class_create(THIS_MODULE,"my_class")) == NULL){
		printk(KERN_INFO" cannot create the struct class ... \n" );
		goto r_class;
	}
	
	// creating device
	if((device_create(dev_class,NULL,dev,NULL,"my_device"))==NULL){
		printk(KERN_INFO" cannot create the device");
		goto r_device;
	}
	
	printk(KERN_INFO" Device is created properly, device driver insert ... done");
	printk(KERN_INFO "init.1 \n");
	return 0;
	
	 r_device:
				class_destroy(dev_class);
					printk(KERN_INFO "init.2 \n");
					return -1;

	
	 
	 
	 r_class: 
				unregister_chrdev_region(dev,1);
				printk(KERN_INFO "init.3 \n");
				return -1;
 }
 
 
 static int __exit chr_driver_exit(void){
	 //we should destroy our device
	 device_destroy(dev_class,dev);
	 class_destroy(dev_class);
	 cdev_del(&my_cdev);
	 unregister_chrdev_region(dev,1);
	 printk(KERN_INFO "Devie driver is removed successfully");
	 return 0;
 }
 
 static int my_open(struct inode *inode, struct file *file){
		
		/*Creating Physical memory*/
        if((kernel_buffer = kmalloc(mem_size , GFP_KERNEL)) == 0){
            printk(KERN_INFO "Cannot allocate memory in kernel\n");
            return -1;
        }
        printk(KERN_INFO "Device File Opened...!!!\n");
        return 0;
 }
 
 static int my_release(struct inode *inode, struct file *file){
	 kfree(kernel_buffer);
	 printk(KERN_INFO "Device File closed ... \n");
	 return 0;
 }
 
 static ssize_t my_read(struct file *flip, char __user *buf, size_t len, loff_t *off){
	 copy_to_user(buf,
	 kernel_buffer,mem_size);
	 printk(KERN_INFO "Data has been read successfuly... \n");
	 return mem_size;
 }
 
 static ssize_t my_write(struct file *flip, const char *buf, size_t len, loff_t *off){
	 
	 copy_from_user(kernel_buffer,buf,len);
	 printk(KERN_INFO "Data has been written successfuly ... \n");
	 return len;
 }
 
 module_init(chr_driver_init);
 module_exit(chr_driver_exit);
 
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("saeed");
 MODULE_DESCRIPTION("the character device driver");
 
 



