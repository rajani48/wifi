#include "sample_dep.h"

static struct callout my_callout;

static void my_timer_cb(void *arg)
{
    printf("callout executed\n");
    wifi_dbg("executing timer", __func__);
}

static int sample_module_init(void)
{
    printf("Sample module loaded\n");
    callout_init(&my_callout, 1);
    callout_reset_dbg(&my_callout, 5000, my_timer_cb, NULL);

    return 0;
}

static int sample_module_exit(void)
{
    printf("Sample module unloaded\n");
    callout_drain(&my_callout);
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
