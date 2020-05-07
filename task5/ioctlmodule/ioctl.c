/*
 * kbleds.c − Blink keyboard leds until the module is unloaded.(modified for > 4.15)
 */
#include <linux/module.h>
#include <linux/configfs.h>
#include <linux/init.h>
#include <linux/tty.h>          /* For fg_console, MAX_NR_CONSOLES */
#include <linux/kd.h>           /* For KDSETLED */
#include <linux/vt.h>
#include <linux/console_struct.h>       /* For vc_cons */
#include <linux/vt_kern.h>
#include <linux/timer.h>
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/fs.h>
#include <linux/string.h>

#define BLINK_DELAY   HZ
#define ALL_LEDS_ON   0x07
#define RESTORE_LEDS  0xFF
#define BUFF_SIZE 8
#define PROC_FILENAME "kbledctrl"
 
MODULE_DESCRIPTION("Controll keyboard's LEDs");
MODULE_LICENSE("GPL");
struct timer_list my_timer;
struct tty_driver *my_driver;
static int _kbledstatus = 0;
static int ledcod = 0;

static struct kobject * kb_kobject;
 
static ssize_t get_led_num(struct kobject * kobj, struct kobj_attribute * attr, char * buf) {
        return sprintf(buf, "%d\n", ledcod);
}
 
static ssize_t set_led_num(struct kobject * kobj, struct kobj_attribute * attr, const char * buf, size_t count) {
        sscanf(buf, "%du", &ledcod);
        return count;
}
 
static struct kobj_attribute foo_attribute =__ATTR(test, 0660, get_led_num, set_led_num);

static int init_sysfs_oper(void) {
	int error = 0;
 
        pr_debug("Module initialized successfully \n");
 
        kb_kobject = kobject_create_and_add(PROC_FILENAME, kernel_kobj);
        if(!kb_kobject)
                return -ENOMEM;
 
        error = sysfs_create_file(kb_kobject, &foo_attribute.attr);
        if (error)
                pr_debug("failed to create the foo file in /sys/kernel/systest \n");
 
        return error;
}


static void my_timer_func(struct timer_list *ptr) {
        int *pstatus = &_kbledstatus;

        if (*pstatus == ledcod)
                *pstatus = RESTORE_LEDS;
        else
                *pstatus = ledcod;

        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            *pstatus);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);
}

static int __init kbleds_init(void) {
	int error = init_sysfs_oper();
	
        int i;
        printk(KERN_INFO "kbleds: loading\n");
        printk(KERN_INFO "kbleds: fgconsole is %x\n", fg_console);

        for (i = 0; i < MAX_NR_CONSOLES; i++) {
                if (!vc_cons[i].d)
                        break;
                printk(KERN_INFO "poet_atkm: console[%i/%i] #%i, tty %lx\n", i,
                       MAX_NR_CONSOLES, vc_cons[i].d->vc_num,
                       (unsigned long)vc_cons[i].d->port.tty);
        }

        printk(KERN_INFO "kbleds: finished scanning consoles\n");
        my_driver = vc_cons[fg_console].d->port.tty->driver;
        printk(KERN_INFO "kbleds: tty driver magic %x\n", my_driver->magic);

       
        timer_setup(&my_timer, my_timer_func, 0);
        my_timer.expires = jiffies + BLINK_DELAY;
        add_timer(&my_timer);

        return error;
}

static void __exit kbleds_cleanup(void) {
        printk(KERN_INFO "kbleds: unloading...\n");
        del_timer(&my_timer);
        (my_driver->ops->ioctl) (vc_cons[fg_console].d->port.tty, KDSETLED,
                            RESTORE_LEDS);

	pr_debug ("Module un initialized successfully \n");
        kobject_put(kb_kobject);
}


module_init(kbleds_init);
module_exit(kbleds_cleanup);
