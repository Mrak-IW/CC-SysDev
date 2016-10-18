#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/errno.h>
#include <linux/kthread.h>
#include <linux/delay.h>

#define MAXLEN 255

static const char moduleMarker[] = "MODTEST";
static const char devName[] = "ccchardev";

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static struct task_struct *thread_st;

static char m[MAXLEN + 1];
unsigned int actualLen;

// Function executed by kernel thread
static int thread_fn(void *unused)
{
    // Allow the SIGKILL signal
    allow_signal(SIGKILL);
    while (!kthread_should_stop())
    {
        printk(KERN_INFO "%s: Thread Running\n", moduleMarker);
        ssleep(5);
        // Check if the signal is pending
        if (signal_pending(current))
            break;
    }
    printk(KERN_INFO "%s: Thread Stopping\n", moduleMarker);
    do_exit(0);
    return 0;
}

static int my_open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "%s: Driver open()\n", moduleMarker);
	return 0;
}
static int my_close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "%s: Driver close()\n", moduleMarker);
	return 0;
}

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{   
    if(*off > 0)
        return 0; /* End of file */

    if (copy_to_user(buf, m, actualLen))
        return -EFAULT;

    *off = actualLen;

    printk(KERN_INFO "%s: Driver read(%lld: %s)\n", moduleMarker, *off, buf);

    return actualLen;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len,	loff_t *off)
{
	actualLen = (len > MAXLEN ? len % MAXLEN : len) + 1;
	if (copy_from_user(m, buf, actualLen) != 0)
	{
		return -EFAULT;
	}
	m[actualLen] = 0;

	printk(KERN_INFO "%s: Driver write(%d: %s)\n", moduleMarker, len, m);
	return len;
}

static struct file_operations pugs_fops =
{
	.owner = THIS_MODULE,
	.open = my_open,
	.release = my_close,
	.read = my_read,
	.write = my_write
};

static int __init hello(void)
{
    int ret;
	struct device *dev_ret;

	if ((ret = alloc_chrdev_region(&first, 0, 1, devName)) < 0)
	{
		printk(KERN_INFO "%s: module registeration ERROR.\n", moduleMarker);
		return ret;
	}
	if (IS_ERR(cl = class_create(THIS_MODULE, "chardrv")))
	{
		unregister_chrdev_region(first, 1);
		printk(KERN_INFO "%s: class_create ERROR.\n", moduleMarker);
		return PTR_ERR(cl);
	}
	if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, devName)))
	{
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		printk(KERN_INFO "%s: device_create ERROR.\n", moduleMarker);
		return PTR_ERR(dev_ret);
	}
	cdev_init(&c_dev, &pugs_fops);
	if ((ret = cdev_add(&c_dev, first, 1)) < 0)
	{
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		printk(KERN_INFO "%s: cdev_add ERROR.\n", moduleMarker);
		return ret;
	}

	sprintf(m, "test\n");
	m[MAXLEN] = 0;


	printk(KERN_INFO "%s: Creating Thread\n", moduleMarker);
	thread_st = kthread_run(thread_fn, NULL, "mythread");
	if (thread_st)
		printk(KERN_INFO "Thread created successfully\n");
	else
		printk(KERN_ERR "Thread creation failed\n");

	printk(KERN_INFO "%s: module registered.\n", moduleMarker);
	return 0;
}


static void __exit bye(void)
{
	printk(KERN_INFO "%s: Cleaning Up\n", moduleMarker);

    cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);

	if (thread_st)
	{
		kthread_stop(thread_st);
		printk(KERN_INFO "%s: Thread stopped", moduleMarker);
	}
	printk(KERN_INFO "%s: module unregistered.\n", moduleMarker);
	printk(KERN_INFO "%s: - - - - - - - - - - -\n", moduleMarker);
}

module_init(hello);
module_exit(bye);

MODULE_AUTHOR("Ostware/Denis Maslov");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CC - character driver");