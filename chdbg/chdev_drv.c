#include <sys/types.h>
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/conf.h>
#include <sys/uio.h>
#include <sys/sysctl.h>
#include <sys/systm.h>

#include "chdev_drv.h"

#define DEVICE_NAME "chdbg"
//#define DEBUG_LOG_ENABLED 1
#define MAX_LINES 100
#define MAX_LINE_LEN 128

//static int debug_enabled = DEBUG_LOG_ENABLED;

static char dbg_arr[MAX_LINES][MAX_LINE_LEN];
static int i = 0;
//static int wc = 0;

void wifi_dbg(const char *mod, const char *msg)
{
#if 0
    if (debug_enabled) {
        snprintf(dbg_arr, 128, "%s : %s\n", mod, msg);
        printf("Debug: %s\n", dbg_arr);
    }
#endif
    snprintf(dbg_arr[i], MAX_LINE_LEN, "i=%d  %s : %s", i, mod, msg);
    i = (i + 1) % MAX_LINES;

  /*  if (wc < MAX_LINES) {
        wc++;
    }*/
}

static int
my_device_open(struct cdev *dev, int flags, int fmt, struct thread *td)
{
    uprintf("Device opened\n");
    return 0;
}

static int
my_device_release(struct cdev *dev, int flags, int fmt, struct thread *td)
{
    uprintf("Device closed\n");
    return 0;
}

static int
my_device_read(struct cdev *dev, struct uio *uio, int ioflag)
{
    int j = 0;
    int err = 0;
    //int start = 0;
    size_t amt;
    char buf[MAX_LINES * MAX_LINE_LEN] = {0};

    //start = (i + 1 + MAX_LINES - wc) % MAX_LINES;

    for(j = 0; j < MAX_LINES; j++) {
        //strlcat(buf, dbg_arr[(start + j) % MAX_LINES], sizeof(buf));
        strlcat(buf, dbg_arr[j], sizeof(buf));
	strlcat(buf, "\n", sizeof(buf));
    }

    amt = strlen(buf);
    err = uiomove(buf, amt, uio);

    return err;
}

static struct cdevsw my_device_cdevsw = {
    .d_version = D_VERSION,
    .d_open = my_device_open,
    .d_close = my_device_release,
    .d_read = my_device_read,
    .d_name = DEVICE_NAME,
};

static struct cdev *my_device = NULL;

static int chdbg_init(void)
{
    uprintf("Char Driver Init\n");

    int err = make_dev_p(MAKEDEV_CHECKNAME | MAKEDEV_WAITOK, 
		    &my_device, &my_device_cdevsw, 0,
		    UID_ROOT, GID_WHEEL, 0600, DEVICE_NAME);
    if (err != 0) {
        uprintf("Failed to create device\n");
	return err;
    }

    return 0;
}

static int chdbg_exit(void)
{
    uprintf("Char Driver Exit\n");

    if (my_device)
        destroy_dev(my_device);

    return 0;
}


static int
chdbg_handler(struct module *m, int event, void *arg)
{
    switch(event)
    {
        case MOD_LOAD:
            return chdbg_init();
        case MOD_UNLOAD:
            return chdbg_exit();
        default:
            return EOPNOTSUPP;
    }
}

static moduledata_t ch_dbg_data = {
    "chdbg",
    chdbg_handler,
    NULL
};

DECLARE_MODULE(chdbg, ch_dbg_data, SI_SUB_DRIVERS, SI_ORDER_MIDDLE);
//DEV_MODULE(chdrv, my_driver_handler, NULL);
MODULE_VERSION(chdbg, 1);
