#ifndef _SAMPLE_DEP_H_
#define _SAMPLE_DEP_H_

#include <sys/types.h>
#include <sys/param.h> 
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/sysctl.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/malloc.h>
#include <sys/mutex.h>

#include "chdev_drv.h"

#define mtx_lock_dbg(my_mutex) {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "mtx_lock=" #my_mutex); \
	mtx_lock(my_mutex);		\
}

#endif /* _SAMPLE_DEP_H_ */
