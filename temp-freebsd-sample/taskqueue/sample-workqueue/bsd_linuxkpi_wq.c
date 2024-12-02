#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>

static struct workqueue_struct *my_workqueue;
static struct work_struct my_work;


static void my_work_function(struct work_struct *work)
{
	printk(KERN_INFO "Executing deferred work function\n");
}

static int wq_module_load(void)
{
	// Initialize work struct
	INIT_WORK(&my_work, my_work_function);

        // Create a custom workqueue - single threaded workqueue
	my_workqueue = alloc_workqueue("my_workqueue", WQ_UNBOUND | WQ_HIGHPRI, 0);
//	my_workqueue = create_workqueue("my_workqueue");

	// Schedule work
	queue_work(my_workqueue, &my_work);

	printk(KERN_INFO "Module loaded\n");
	return 0;
}

static void wq_module_unload(void)
{
	// Cleanup workqueue
	flush_workqueue(my_workqueue);
	destroy_workqueue(my_workqueue);

	printk(KERN_INFO "Module unloaded\n");
}

/* Module event handler */
static int wq_module_handler(struct module *mod, int event, void *arg)
{
    switch(event)
    {
        case MOD_LOAD:
            return wq_module_load();
        case MOD_UNLOAD:
            wq_module_unload();
        default:
            return EOPNOTSUPP;
    }
}

/* Declare the module */
static moduledata_t wq_mod = {
    "example_workqueue",
    wq_module_handler,
    NULL
};

DECLARE_MODULE(example_workqueue, wq_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);

