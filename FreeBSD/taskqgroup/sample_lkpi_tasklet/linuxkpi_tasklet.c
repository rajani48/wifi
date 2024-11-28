#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>

static struct tasklet_struct my_tasklet;

#if 0
/* For tasklet_init */
static void my_tasklet_handler(unsigned long data)
{
    printk(KERN_INFO "Executing deferred tasklet with data: %ld\n", data);
}
#endif

static void my_tasklet_handler(struct tasklet_struct *t)
{
    printf("Executing deferred tasklet\n");
}


static int load_module(void)
{
    //int data = 42;

    // Initialize tasklet with handler function and data
    tasklet_setup(&my_tasklet, my_tasklet_handler);
#if 0 
    //tasklet_init(&my_tasklet, my_tasklet_handler, data);
#endif

    // Schedule the tasklet to run
    tasklet_schedule(&my_tasklet);

    printf("Module loaded, tasklet scheduled\n");
    return 0;
}

static void unload_module(void)
{
    // Kill the tasklet before unloading the module
    tasklet_kill(&my_tasklet);

    printk(KERN_INFO "Module unloaded, tasklet killed\n");
}


SYSINIT(linuxkpi_gtask_init, SI_SUB_DRIVERS, SI_ORDER_MIDDLE, load_module, NULL);
SYSUNINIT(linuxkpi_gtask_uninit, SI_SUB_DRIVERS, SI_ORDER_MIDDLE, unload_module, NULL);

MODULE_VERSION(linuxkpi_gtask_module, 1);
