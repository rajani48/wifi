#include "sample_dep.h"
/* Task function that will be executed by taskqueue */
static void my_task_fn(void *context, int pending)
{
    int *data = (int *)context;
    printf("Task executed with data: %d and pending count: %d\n", *data, pending);
}

/* struct to hold taskqueue and task details */
struct taskqueue *my_tq;
struct task my_task;
int data;

static int sample_module_init(void)
{
    printf("Loading example taskqueue module\n");
    int error;

    /* Initialize data */
    data = 10;

    /* Create taskqueue */
    my_tq = taskqueue_create("my_taskqueue", M_WAITOK,
                    taskqueue_thread_enqueue, &my_tq);
    if (my_tq == NULL) {
        printf("Failed to create taskqueue\n");
        return ENOMEM;
    }

    /* Start a thread for the taskqueue */
   error = taskqueue_start_threads(&my_tq, 1, PI_NET, "my_taskqueue_thread");
    if (error) {
        printf("Failed to start taskqueue thread\n");
        taskqueue_free(my_tq);
        return error;
    }
    /* Initialize the task*/
    TASK_INIT(&my_task, 0, my_task_fn, &data);

    /* Enqueue task (i.e.,my_task) to taskqueue (i.e., my_tq) */
    taskqueue_enqueue_dbg(my_tq, &my_task);
    if (error) {
        printf("Failed to enqueue taskq\n");
        taskqueue_free(my_tq);
        return error;
    }
    return 0;
}

static int sample_module_exit(void)
{
    printf("Unloading example taskqueue module\n");

    /* Drain the taskqueue to ensure it completes before freeing the taskqueue */
    taskqueue_drain(my_tq, &my_task);

    /* Free the taskqueue */
    taskqueue_free(my_tq);

    return 0;
}

static int sample_module_handler(struct module *m, int event, void *arg)
{
    switch(event)
    {
        case MOD_LOAD:
            return sample_module_init();
        case MOD_UNLOAD:
            return sample_module_exit();
        default:
            return EOPNOTSUPP;
    }
}

static moduledata_t sample_mod = {
    "sample_module",
    sample_module_handler,
    NULL
};

DECLARE_MODULE(sample_module, sample_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);

MODULE_DEPEND(sample_module, chdbg, 1, 1, 1);
