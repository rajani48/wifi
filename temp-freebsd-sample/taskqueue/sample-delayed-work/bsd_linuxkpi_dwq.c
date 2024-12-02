#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/workqueue.h>

static struct workqueue_struct *my_workqueue;
static struct delayed_work my_dwork;


static void my_work_function(struct work_struct *work)
{
	printk(KERN_INFO "Executing delayed deferred work function\n");
}

static int wq_module_load(void)
{
	// Initialize the delayed work item with handler
	INIT_DELAYED_WORK(&my_dwork, my_work_function);

        // Create a custom workqueue - single threaded workqueue
	//my_workqueue = alloc_workqueue("my_dworkqueue", WQ_UNBOUND | WQ_HIGHPRI, 0);
	my_workqueue = create_workqueue("my_dworkqueue");

	// Schedule the work to execute after a delay of 10 seconds
	queue_delayed_work(my_workqueue, &my_dwork, 10 * HZ);

	printk(KERN_INFO "Module loaded\n");
	return 0;
}

static void wq_module_unload(void)
{
	// Cancel any pending or running delayed work and wait for completion 
	cancel_delayed_work_sync(&my_dwork);
	// Destroy the custom workqueue
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

