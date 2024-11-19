#include <sys/types.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/sysctl.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/callout.h>

#include "chdev_drv.h"

static struct callout my_callout;

#if 0
static void sample_fun(void)
{
    wifi_dbg("sample_mod1", __func__);
    wifi_dbg("sample_mod2", __func__);
    wifi_dbg("sample_mod3", __func__);
    wifi_dbg("sample_mod4", __func__);
    wifi_dbg("sample_mod5", __func__);
    wifi_dbg("sample_mod6", __func__);
    wifi_dbg("sample_mod7", __func__);
    wifi_dbg("sample_mod8", __func__);
    wifi_dbg("sample_mod9", __func__);
    wifi_dbg("sample_mod10", __func__);
    wifi_dbg("sample_mod11", __func__);
    wifi_dbg("sample_mod12", __func__);
    wifi_dbg("sample_mod13", __func__);
    wifi_dbg("sample_mod14", __func__);
}
#endif

#define callout_reset_dbg(my_callout, hz, cb, arg) do {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "callout=" #my_callout ", ticks=" #hz ", cb=" #cb ", arg=" #arg); \
	callout_reset(my_callout, hz, cb, arg);		\
} while (0)


static void my_timer_cb(void *arg)
{
    printf("callout executed\n");// counter:%d\n", ++counter);
    wifi_dbg("executing timer", __func__);
}

static int sample_module_init(void)
{
    printf("Sample module loaded\n");

//    wifi_dbg("sample_mod", __func__);
//    sample_fun();
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


static int
sample_module_handler(struct module *m, int event, void *arg)
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

/* Declare the module */
static moduledata_t sample_mod = {
    "sample_module",
    sample_module_handler,
    NULL
};

DECLARE_MODULE(sample_module, sample_mod, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);

MODULE_DEPEND(sample_module, chdbg, 1, 1, 1);
