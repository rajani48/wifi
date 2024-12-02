#include "sample_dep.h"

static void work_handler(struct work_struct *work);

static DECLARE_DELAYED_WORK(my_delaywork, work_handler);

static void work_handler(struct work_struct *work) {
	printf("***Delayed Work handler Executed***\n");
	wifi_dbg("Executing Function", __func__);
}

static int sample_module_init(void)
{
    printf("Sample module loaded\n");
    schedule_delayed_work_dbg(&my_delaywork, msecs_to_jiffies(100)); //1 seconds delay
    return 0;
}

static int sample_module_exit(void)
{
    printf("Sample module unloaded\n");
    cancel_delayed_work_sync(&my_delaywork);
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
