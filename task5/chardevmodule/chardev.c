#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "chardev"
#define CLASS_D_NAME "chardev_class"
#define CHARDEV_NAME "chardrv"
#define BUF_LEN 256


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Zakharov");
MODULE_DESCRIPTION("First CharDev");
   
       
int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char *, size_t, loff_t *);
static ssize_t device_write(struct file *, const char *, size_t, loff_t *);
             
static int Device_Open = 0;
static int temp = 0;   
static char msg[BUF_LEN];
static dev_t dev;
static struct cdev c_dev; // Global variable for the character device structure
static struct class * cl; // Global variable for the device class

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.read = device_read,
	.write = device_write,
	.open = device_open,
	.release = device_release
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

int init_module(void) {
	if (alloc_chrdev_region(&dev, 0, 1, DEVICE_NAME) < 0)
		return -1;

	if ((cl = class_create(THIS_MODULE, CLASS_D_NAME)) == NULL) {
		unregister_chrdev_region(dev, 1);
		return -1;
	}
	if (device_create(cl, NULL, dev, NULL, CHARDEV_NAME) == NULL) {
		class_destroy(cl);
		unregister_chrdev_region(dev, 1);
		return -1;
	}

	cdev_init(&c_dev, &fops);

	if (cdev_add(&c_dev, dev, 1) == -1) {
		device_destroy(cl, dev);
		class_destroy(cl);
		unregister_chrdev_region(dev, 1);
		return -1;
	}

	return 0;
}

void cleanup_module(void) {
	cdev_del(&c_dev);
  	device_destroy(cl, dev);
  	class_destroy(cl);
  	unregister_chrdev_region(dev, 1);
  	printk(KERN_INFO "Module: ofcd unregistered");
}


static int device_open(struct inode * inode, struct file * file) {
	if (Device_Open)
		return -EBUSY;

	Device_Open++;
	try_module_get(THIS_MODULE);

	return 0;
}


static int device_release(struct inode * inode, struct file * file) {
	Device_Open--;                
	module_put(THIS_MODULE);
	return 0;
}

static ssize_t device_read(struct file * filp, char * buffer, size_t len, loff_t * offset) {
  	if(len > temp)
		len = temp;

	temp = temp - len;
	copy_to_user(buffer, msg, len);

	if(len == 0)
		temp = len;
	
	printk(KERN_INFO "Read:  %s\n", msg);
 
	return len;
}

static ssize_t device_write(struct file * filp, const char * buffer, size_t len, loff_t * off) {
  	copy_from_user(msg, buffer, len);
	len = len;
	temp = len;

	printk(KERN_INFO "Write:  %s\n", msg);
	return len;
}
