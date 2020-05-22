#include <linux/types.h>        /* For ulong and other fancy types*/
#include <linux/kernel.h>       /* I don't know why */
#include <linux/init.h>         /* module_init, module_exit */
#include <linux/module.h>       /* version info, MODULE_LICENSE, MODULE_AUTHOR, printk() */
#include <linux/moduleparam.h>  /* Maybe for module init command lines parameters */
#include "device_file.h"
#include "gpio_irq.h"

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("JDat Deface, YL3AKC");

module_param(gpioBase, ulong, 0444);
//module_param(gpioBase, ulong, S_IWUSR | S_IRGRP | S_IROTH);
MODULE_PARM_DESC(gpioBase, "I/O peripheral register base. 0x20000000 for RPi1, 0x30000000 for RPi2 and later");

/*===============================================================================================*/
static int simple_driver_init(void)
{
    int result = 0;
    
    //gpioBase = 0x3f000000;
    
    printk( KERN_NOTICE "DVB char driver: Initializing char device\n" );
    result = register_device();
    printk( KERN_NOTICE "DVB char driver: Initializing IRQ\n" );
    result = register_irq();
    return result;
}
/*-----------------------------------------------------------------------------------------------*/
static void simple_driver_exit(void)
{
    printk( KERN_NOTICE "DVB char driver: Exiting\n" );
    unregister_device();
    unregister_irq();
}
/*===============================================================================================*/

module_init(simple_driver_init);
module_exit(simple_driver_exit);
