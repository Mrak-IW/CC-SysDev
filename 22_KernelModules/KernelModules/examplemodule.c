#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>

static int testParam = 42;
static const char moduleMarker[] = "MODTEST";

module_param(testParam, int, 0644);
MODULE_PARM_DESC(testParam, "Description of the parameter.");

static int __init paramModuleHello(void)
{
    printk(KERN_INFO "%s: moduleParam module being loaded.\n", moduleMarker);
    printk(KERN_INFO "%s: Initial value of testParam = %d.\n", moduleMarker, testParam);
    return 0;
}

static void __exit paramModuleBye(void)
{
    printk(KERN_INFO "%s: moduleParam module being unloaded.\n", moduleMarker);
    printk(KERN_INFO "%s: Final value of testParam = %d.\n", moduleMarker, testParam);
}

module_init(paramModuleHello);
module_exit(paramModuleBye);

MODULE_AUTHOR("Ostware/Denis Maslov");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Description of the example module.");