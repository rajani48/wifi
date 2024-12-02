#include "sample_dep.h"
#include "chdev_drv.h"

static struct timer_list my_timer;

static void my_timer_cb(struct timer_list *t);

static void my_timer_cb(struct timer_list *t)
{
    printf("***Timer Expired and call back function called***\n");
    wifi_dbg("Executing timer", __func__);
    mod_timer_dbg(&my_timer, jiffies + msecs_to_jiffies(1000));
}

static int sample_module_init(void)
{
    printf("Sample module loaded\n");
    timer_setup(&my_timer, my_timer_cb, 0);
    mod_timer_dbg(&my_timer, jiffies + msecs_to_jiffies(1000));

    return 0;
}

static int sample_module_exit(void)
{
    printf("Sample module unloaded\n");
    del_timer(&my_timer);
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
