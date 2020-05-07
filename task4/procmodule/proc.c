#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define BUFF_SIZE 256
#define PROC_FILENAME "myproc"

int len, temp;
 
char * msg = NULL;
 
static ssize_t read_proc(struct file * filp, char * buf, size_t count, loff_t * offp) {
	if(count > temp)
		count = temp;

	temp = temp - count;
	copy_to_user(buf, msg, count);

	if(count == 0)
		temp = len;
 
	return count;
}
 
static ssize_t write_proc(struct file * filp, const char * buf, size_t count, loff_t * offp) {
	copy_from_user(msg, buf, count);
	
	len = count;
	temp = len;
	return count;
}

struct file_operations proc_fops = {
	read: read_proc,
	write: write_proc
};
 
static void create_new_proc_entry(void) {
	proc_create(PROC_FILENAME, 0, NULL, &proc_fops);
	msg = kmalloc(BUFF_SIZE * sizeof(char), GFP_KERNEL);
}



static int __init first_module_init(void) {
	create_new_proc_entry();
	printk(KERN_INFO "Hello, World!\n");
	return 0;
}

static void __exit first_module_exit(void) {
	remove_proc_entry(PROC_FILENAME, NULL);
    	kfree(msg);
	printk(KERN_INFO "Goodbye, World!\n");
}

module_init(first_module_init);
module_exit(first_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roman Zakharov");
MODULE_DESCRIPTION("A simple example Linux module.");
MODULE_VERSION("0.01");
