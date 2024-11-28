#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/malloc.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/gtaskqueue.h>

static struct taskqgroup *my_taskqgroup;
static struct grouptask my_gtask;

static void my_gtask_fn(void *arg)
{
    printf("Group task executed\n");
}

static int load_module(void)
{
    printf("Loading Grouptask sample module\n");

    // Create a taskgroup
    my_taskqgroup = taskqgroup_create("my_taskqgroup", 1, 0);

    // Initialize the grouptask
    GROUPTASK_INIT(&my_gtask, 0, my_gtask_fn, NULL);

    // Attach the task to softirq group, CPU 0
    taskqgroup_attach_cpu(my_taskqgroup, &my_gtask, NULL, 0, NULL, NULL, "sample_tqgroup");

    // Enqueue the task for execution
    GROUPTASK_ENQUEUE(&my_gtask);

    return 0;
}

static void unload_module(void)
{
    printf("Unloading Grouptask sample module\n");

    // Detach the task from the softirq group
    taskqgroup_detach(my_taskqgroup, &my_gtask);

    taskqgroup_destroy(my_taskqgroup);
}

SYSINIT(my_gtask_init, SI_SUB_DRIVERS, SI_ORDER_MIDDLE, load_module, NULL);
SYSUNINIT(my_gtask_uninit, SI_SUB_DRIVERS, SI_ORDER_MIDDLE, unload_module, NULL);

MODULE_VERSION(bsd_gtask_module, 1);
