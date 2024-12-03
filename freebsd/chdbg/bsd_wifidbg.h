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
#include <sys/taskqueue.h>
#include <sys/callout.h>

#include "chdev_drv.h"

//linux header
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <linux/delay.h>
#include <linux/workqueue.h>

#define callout_reset_dbg(my_callout, hz, cb, arg) {    \
        char buf[128];                                  \
        snprintf(buf, 128, "%s is activating timer", __func__); \
        wifi_dbg(buf, "callout=" #my_callout ", ticks=" #hz ", cb=" #cb ", arg=" #arg); \
        callout_reset(my_callout, hz, cb, arg);         \
}

#define schedule_delayed_work_dbg(my_delaywork, expire) {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "schedulw_delayed_work=" #my_delaywork ", expire=" #expire); \
	schedule_delayed_work(my_delaywork, expire);		\
}

#define mod_timer_dbg(my_timer, expire) {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "mod_timer=" #my_timer ", expire=" #expire); \
	mod_timer(my_timer, expire);		\
}

#define mtx_lock_dbg(my_mutex) {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "mtx_lock=" #my_mutex); \
	mtx_lock(my_mutex);		\
}

#define mtx_unlock_dbg(my_mutex) {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "mtx_unlock=" #my_mutex); \
	mtx_unlock(my_mutex);		\
}

#define taskqueue_enqueue_dbg(tq, task) {  \
        char buf[128]; \
        snprintf(buf, 128, "%s is activating timer", __func__); \
        wifi_dbg(buf, "taskqueue=" #tq ", my_task=" #task); \
        return taskqueue_enqueue((tq), (task));  \
}
#endif /* _SAMPLE_DEP_H_ */
