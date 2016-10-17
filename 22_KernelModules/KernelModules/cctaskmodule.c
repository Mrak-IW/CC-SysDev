#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define MAXLEN 255

static const char moduleMarker[] = "MODTEST";
static const char devName[] = "ccchardev";

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static char m[MAXLEN];

static int my_open(struct inode *i, struct file *f)
{
	m[0] = 't';
	m[1] = 'e';
	m[2] = 's';
	m[3] = 't';
	m[4] = '\n';

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
	printk(KERN_INFO "%s: Driver read(%lld: %s)\n", moduleMarker, *off, buf);

    
    if(*off > 0)
        return 0; /* End of file */

    if (copy_to_user(buf, m, 5))
        return -EFAULT;

    *off = 5;
    return 5;
}
static ssize_t my_write(struct file *f, const char __user *buf, size_t len,	loff_t *off)
{
	unsigned int actualLen = len - len / MAXLEN;
	char str[MAXLEN];
	if (copy_from_user(str, buf, actualLen) != 0)
	{
		return -EFAULT;
	}
	str[actualLen] = 0;

	printk(KERN_INFO "%s: Driver write(%d: %s)\n", moduleMarker, len, str);
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
		printk(KERN_INFO "%s: module registeration ERROR.\n", moduleMarker);
		return PTR_ERR(cl);
	}
	if (IS_ERR(dev_ret = device_create(cl, NULL, first, NULL, devName)))
	{
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		printk(KERN_INFO "%s: module registeration ERROR.\n", moduleMarker);
		return PTR_ERR(dev_ret);
	}
	cdev_init(&c_dev, &pugs_fops);
	if ((ret = cdev_add(&c_dev, first, 1)) < 0)
	{
		device_destroy(cl, first);
		class_destroy(cl);
		unregister_chrdev_region(first, 1);
		printk(KERN_INFO "%s: module registeration ERROR.\n", moduleMarker);
		return ret;
	}

	printk(KERN_INFO "%s: module registered.\n", moduleMarker);
	return 0;
}


static void __exit bye(void)
{
    cdev_del(&c_dev);
	device_destroy(cl, first);
	class_destroy(cl);
	unregister_chrdev_region(first, 1);
	printk(KERN_INFO "%s: module unregistered.\n", moduleMarker);
}

module_init(hello);
module_exit(bye);

MODULE_AUTHOR("Ostware/Denis Maslov");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("CC - character driver");