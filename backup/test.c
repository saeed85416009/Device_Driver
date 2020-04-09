#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>      //kmalloc
#include <linux/uaccess.h>   // copy_from_user / copy_to_user
#include <linux/ioctl.h>
#include <linux/proc_fs.h>

#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/interrupt.h>
#include <asm/io.h>

#include <linux/kthread.h>  // creating the thread
#include <linux/delay.h>

#define mem_size 1024

/* define
TASKLET_DYNAMICALLY
TASKLET_STATICALLY

SIMPLE_SPIN_LOCK
SIMPLE_INTRRUPT

SIMPLE_THREAD
THREAD_DYNAMICALLY
THREAD_STATICALLY
*/
#define SIMPLE_SPIN_LOCK
#define SIMPLE_THREAD
#define SIMPLE_INTRRUPT
#define TASKLET_DYNAMICALLY
#define THREAD_STATICALLY

/* IOCTL  */

//define ioctl code
#define WR_DATA _IOW('a', 'a', int32_t*)
#define RD_DATA _IOR('a', 'b', int32_t*)

#ifdef SIMPLE_SPIN_LOCK

// static way
DEFINE_SPINLOCK(chr_spinlock);

/* end of spin lock ... */

#endif

int32_t val = 0;



dev_t dev=0;
static struct cdev my_cdev; 
static struct class *dev_class;
uint8_t *kernel_buffer;

/* proc */
char chr_array[400]= " I am Saeed the writer of this device driver";
static int len = 1;

/* spin lock */
static struct task_struct *chr_thread1;
static struct task_struct *chr_thread2;

//thread
static struct task_struct *chr_thread;
unsigned long chr_spinlock_variable = 0;




//prototype
static int my_open(struct inode *inode, struct file *file);
static int my_release(struct inode *inode, struct file *file);
static ssize_t my_read(struct file *flip, char __user *buf, size_t len, loff_t *off);
static ssize_t my_write(struct file *flip, const char *buf, size_t len, loff_t *off);

static int open_proc(struct inode *inode, struct file *file);
static int release_proc(struct inode *inode, struct file *file);
static ssize_t read_proc(struct file *flip, char __user *buf, size_t len, loff_t *off);
static ssize_t write_proc(struct file *flip, const char *buf, size_t len, loff_t *off);

static int __init chr_driver_init(void); 
static int __init chr_driver_exit(void); 

static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg);

//thread
int thrd_func1 (void *p);
int thrd_func2 (void *p);

// second half of interrupt
struct tasklet_struct *tasklet;
void tasklet_func(unsigned long data);

/*  end */




static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read  = my_read,
	.write = my_write,
	.open  = my_open,
	.unlocked_ioctl = chr_ioctl, 
	.release = my_release,
};

static struct file_operations proc_fops = {
	.owner 		= THIS_MODULE,
	.read  		= read_proc,
	.write   	= write_proc,
	.open     	= open_proc,
	.release 	= release_proc,
};










#ifdef SIMPLE_INTRRUPT	
/* interrupt */
#define IRQ_NO 1

#ifdef TASKLET_STATICALLY
// declare tasklet statically
DECLARE_TASKLET(tasklet, tasklet_func, 1);
#endif

// tasklet function body
void tasklet_func(unsigned long data){
	spin_lock_irq(&chr_spinlock);
	chr_spinlock_variable++;
	printk (KERN_INFO "executing the tasklet function : data = %lu ...chr_spinlock_variable=%lu \n", data, chr_spinlock_variable);
	spin_unlock_irq(&chr_spinlock);
}
// first half of interrupt
static irqreturn_t irq_handler(int irq, void *dev_id){
	spin_lock_irq(&chr_spinlock);
	chr_spinlock_variable++;
	printk (KERN_INFO "keyboard: interrupt occured chr_spinlock_variable = %lu ...\n", chr_spinlock_variable);
	spin_unlock_irq(&chr_spinlock);
	tasklet_schedule(tasklet);
	return IRQ_HANDLED;
} 

/* end of interrupt */
#endif



/* thread ..... */
#ifdef SIMPLE_THREAD

//body functions
int thrd_func1 (void *p){
	int i = 0;
	while(!kthread_should_stop()){
		if(!spin_is_locked(&chr_spinlock)){
		printk(KERN_INFO " spinlock is not locked in thread function 1 ...\n");
		}
		spin_lock(&chr_spinlock);
		if(spin_is_locked(&chr_spinlock)){
		printk(KERN_INFO " spinlock is locked in thread function 1 ...\n");
		}
	 chr_spinlock_variable++;
	 printk(KERN_INFO " in thread function 1 = %lu ...\n", chr_spinlock_variable);
		spin_unlock(&chr_spinlock);
		msleep(1000);
	}
return 0;
}

int thrd_func2 (void *p){
	int i = 0;
	while(!kthread_should_stop()){
		if(!spin_is_locked(&chr_spinlock)){
		printk(KERN_INFO " spinlock is not locked in thread function 2 ...\n");
		}
		spin_lock(&chr_spinlock);
		if(spin_is_locked(&chr_spinlock)){
		printk(KERN_INFO " spinlock is locked in thread function 2 ...\n");
		}
	 chr_spinlock_variable++;
	 printk(KERN_INFO " in thread function 2 = %lu ...\n", chr_spinlock_variable);
		spin_unlock(&chr_spinlock);
		msleep(1000);
	}
return 0;
}
#endif 

/* end of thread .... */






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
	
	/* create proc entry */	
	proc_create("my_chr_proc", 0666, NULL, &proc_fops);
	/*  end of proc */
	
#ifdef SIMPLE_INTRRUPT	
	/* interrup request */
	if (request_irq(IRQ_NO, irq_handler, IRQF_SHARED, "my_device", (void *)(irq_handler))){
		printk(KERN_INFO" cannot create the irq");
		goto irq;
	}

#ifdef TASKLET_DYNAMICALLY	
	// tasklet dynamically
	tasklet = kmalloc(sizeof(struct tasklet_struct),GFP_KERNEL);
	if (tasklet){
		 printk(KERN_INFO "tasklet: Cannot allocate memory in kernel\n");
            goto irq;
        
	}
	tasklet_init(tasklet,tasklet_func,1); //dynamic method
	/* end of interrupt request */
#endif

#endif

 /* thread ........*/
 #ifdef THREAD_STATICALLY
	chr_thread1 = kthread_create(thrd_func1, NULL, "chr thread 1");
	if (chr_thread1){
		wake_up_process(chr_thread1); // we will start the kernel thread by writing wake up process
	}else{
		printk(KERN_INFO "thread1: Cannot create thread  in kernel\n");
            goto r_device; // it will destroy the class
        
	}

	chr_thread2 = kthread_create(thrd_func2, NULL, "chr thread 2");
	if (chr_thread2){
		wake_up_process(chr_thread2); // we will start the kernel thread by writing wake up process
	}else{
		printk(KERN_INFO "thread2: Cannot create thread  in kernel\n");
            goto r_device; // it will destroy the class
        
	} 
#endif
	
#ifdef THREAD_DYNAMICALLY
	// dynamically thread
	chr_thread = kthread_run(thrd_func1, NULL, "my_device");
	if (chr_thread){
			printk(KERN_INFO "thread: successfully created in kernel\n");
			}else{
		printk(KERN_INFO "thread: Cannot create thread  in kernel\n");
            goto r_device; // it will destroy the class
        
	}
#endif

/* end of thread ...*/
	
	printk(KERN_INFO" Device is created properly, device driver insert ... done");
	printk(KERN_INFO "init.1 \n");
	return 0;
	
	 r_device:  // it will destroy the class
				class_destroy(dev_class);
					printk(KERN_INFO "init.2 \n");
					return -1;

	
	 
	 
	 r_class: 
				unregister_chrdev_region(dev,1);
				printk(KERN_INFO "init.3 \n");
				return -1;

#ifdef SIMPLE_INTRRUPT				
	irq:
				free_irq(IRQ_NO, (void *)(irq_handler));
#endif
 }
 
 
 static int __exit chr_driver_exit(void){
	//we should destroy our device
	device_destroy(dev_class,dev);
	class_destroy(dev_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev,1);
#ifdef SIMPLE_INTRRUPT	
	free_irq(IRQ_NO, (void *)(irq_handler));
	tasklet_kill(tasklet);
#endif
#ifdef SIMPLE_THREAD
	kthread_stop(chr_thread1);
	kthread_stop(chr_thread2);
#endif
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
	 copy_to_user(buf,kernel_buffer,mem_size);
	 printk(KERN_INFO "Data has been read successfuly... \n");
	 return mem_size;
 }
 
 static ssize_t my_write(struct file *flip, const char *buf, size_t len, loff_t *off){
	 
	 copy_from_user(kernel_buffer,buf,len);
	 printk(KERN_INFO "Data has been written successfuly ... \n");
	 return len;
 }
 
 static long chr_ioctl(struct file *file, unsigned int cmd, unsigned long arg){
	 switch(cmd){
		 case WR_DATA:
				 copy_from_user(&val,(int32_t*) arg,sizeof(val));
				 printk(KERN_INFO "val = %d\n", val);
				 break;
		 case RD_DATA:
				 copy_to_user( (int32_t*)arg, &val, sizeof(val));
				 printk(KERN_INFO "val = %d\n", val);
				 break;
		 
	 }
	 return 0;
 }
 
 
 /*  proc */
 static int open_proc(struct inode *inode, struct file *file){
	 printk(KERN_INFO "my proc file is opened ... \n");
	return 0;
	 }
static int release_proc(struct inode *inode, struct file *file){
	printk(KERN_INFO "my proc file is released ... \n");
	return 0;
}


static ssize_t read_proc(struct file *flip, char __user *buf, size_t len, loff_t *off){
	
	/* in this function we write data from our driver which is statically written,chr_array
    to the buffer, buf, that user will be entered in the user space application
	*/ 	
	printk(KERN_INFO "my proc file is reading ... \n");
	if(len){
		len=0;
		}else {
		len=1;
		return 0;		
		}
		copy_to_user(buf,chr_array,40);
		
		return len;
}

static ssize_t write_proc(struct file *flip, const char *buf, size_t len, loff_t *off){
	
	printk(KERN_INFO "my proc file is writing ... \n");
	copy_from_user(chr_array,buf,len);
	return len;
}

 /* end proc */
 
 module_init(chr_driver_init);
 module_exit(chr_driver_exit);
 
 MODULE_LICENSE("GPL");
 MODULE_AUTHOR("saeed");
 MODULE_DESCRIPTION("the character device driver");
 
 



