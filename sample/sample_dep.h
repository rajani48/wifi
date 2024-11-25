#ifndef _SAMPLE_DEP_H_
#define _SAMPLE_DEP_H_

#include <sys/types.h>
#include <sys/param.h> 
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/sysctl.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/callout.h>

#include "chdev_drv.h"

typedef void (*callout_cb)(void *);

static inline void
callout_reset_dbg(struct callout *my_callout, int hz, callout_cb cb, void *arg)
{
    char buf[128];
    snprintf(buf, 128, "%s is activating timer callout=%p, ticks=%d cb=%p arg=%p"
                    , __func__, (void*)my_callout, hz, (void*)cb, (void*) arg);
    wifi_dbg(buf, __func__);
    callout_reset(my_callout, hz, cb, arg);
}

#ifdef callout_reset
#undef callout_reset
#endif

#define callout_reset(my_callout, hz, cb, arg) \
        callout_reset_dbg(my_callout, hz, cb, arg)


#endif /* _SAMPLE_DEP_H_ */
