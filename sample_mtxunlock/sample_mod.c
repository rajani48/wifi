#include "sample_dep.h"

static struct mtx my_mutex;
static int shared_data = 0;

static void handler_odd(void) {
	wifi_dbg("Executing Function", __func__);
        mtx_lock(&my_mutex);

        if(shared_data%2 !=0){
                printf("Odd Number : %d\n", shared_data);
        }
        mtx_unlock_dbg(&my_mutex);
}

static void handler_even(void) {
	wifi_dbg("Executing Function", __func__);
        mtx_lock(&my_mutex);

        if(shared_data%2 ==0){
                printf("Even Number : %d\n", shared_data);
        }
        mtx_unlock_dbg(&my_mutex);
}

static void process_num(void) {

        for (shared_data = 1; shared_data <= 10; shared_data++) {

                if(shared_data % 2 == 0)
                        handler_even();
                else
                        handler_odd();
        }
}

static int sample_module_init(void)
{
    printf("Sample module loaded\n");
    process_num();
    return 0;
}

static int sample_module_exit(void)
{
    printf("Sample module unloaded\n");
    mtx_destroy(&my_mutex);
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
