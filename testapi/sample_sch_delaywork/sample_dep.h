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

//Linux Header file
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/workqueue.h>
#include <linux/jiffies.h>

#define schedule_delayed_work_dbg(my_delaywork, expire) {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "schedulw_delayed_work=" #my_delaywork ", expire=" #expire); \
	schedule_delayed_work(my_delaywork, expire);		\
}

#endif /* _SAMPLE_DEP_H_ */
