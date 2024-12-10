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

#include <chdbg/chdev_drv.h>

#define callout_reset_dbg(my_callout, hz, cb, arg) {    \
        char buf[128];                                  \
        snprintf(buf, 128, "%s is activating timer", __func__); \
        wifi_dbg(buf, "callout=" #my_callout ", ticks=" #hz ", cb=" #cb ", arg=" #arg); \
        callout_reset(my_callout, hz, cb, arg);         \
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
        taskqueue_enqueue((tq), (task));  \
}
#endif /* _SAMPLE_DEP_H_ */
