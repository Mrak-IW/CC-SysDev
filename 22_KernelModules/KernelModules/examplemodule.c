#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int __init hi(void)
{
	printk(KERN_INFO "MODTEST: Example module being loaded.\n");
	return 0;
}

static void __exit bye(void)
{
	printk(KERN_INFO "MODTEST: Example module being unloaded.\n");
}

module_init(hi);
module_exit(bye);

MODULE_AUTHOR("Ostware");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Description of the example module.");