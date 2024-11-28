#include <sys/param.h>
#include <sys/module.h>
#include <sys/kernel.h>
#include <sys/taskqueue.h>
#include <sys/systm.h>
#include <sys/malloc.h>

/* Task function that will be executed by taskqueue */
static void my_task_fn(void *context, int pending)
{
    int *data = (int *)context;
    printf("Task executed with data: %d and pending count: %d\n", *data, pending);
}

/* struct to hold taskqueue and task details */
static struct {
    struct taskqueue *my_tq;
    struct task my_task;
    int data;
} example_taskqueue;

/* Module load function*/
static int example_module_load(void)
{
    printf("Loading example taskqueue module\n");
    int error;

    /* Initialize data */
    example_taskqueue.data = 10;

    /* Create taskqueue */
    example_taskqueue.my_tq = taskqueue_create("my_taskqueue", M_WAITOK,
		    taskqueue_thread_enqueue, &example_taskqueue.my_tq);

    if (example_taskqueue.my_tq == NULL)
    {
        printf("Failed to create taskqueue\n");
	return ENOMEM;
    }

    /* Start a thread for the taskqueue */
    error = taskqueue_start_threads(&example_taskqueue.my_tq, 1, PI_NET, "my_taskqueue_thread");
    if (error)
    {
        printf("Failed to start taskqueue thread\n");
	taskqueue_free(example_taskqueue.my_tq);
        return error;
    }

    /* Initialize the task*/
    TASK_INIT(&example_taskqueue.my_task, 0, my_task_fn, &example_taskqueue.data);

    /* Enqueue task (i.e.,my_task) to taskqueue (i.e., my_tq) */
    error = taskqueue_enqueue(example_taskqueue.my_tq, &example_taskqueue.my_task);
    if (error)
    {
        printf("Failed to enqueue taskq\n");
        taskqueue_free(example_taskqueue.my_tq);
        return error;
    }

    return 0;
}

/* Module unload function */
static int example_module_unload(void)
{
    printf("Unloading example taskqueue module\n");

    /* Drain the taskqueue to ensure it completes before freeing the taskqueue */
    taskqueue_drain(example_taskqueue.my_tq, &example_taskqueue.my_task);

    /* Free the taskqueue */
    taskqueue_free(example_taskqueue.my_tq);

    return 0; 
}

/* Module event handler */
static int example_module_handler(struct module *mod, int event, void *arg)
{
    switch(event)
    {
        case MOD_LOAD:
            return example_module_load();
        case MOD_UNLOAD:
            return example_module_unload();
        default:
            return EOPNOTSUPP;
    }
}

/* Declare the module */
static moduledata_t example_mod = {
    "example_taskqueue",
    example_module_handler,
    NULL
};

DECLARE_MODULE(example_taskqueue, example_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
