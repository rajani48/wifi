#ifndef _SAMPLE_DEP_H_
#define _SAMPLE_DEP_H_

#include <sys/types.h>
#include <sys/param.h> 
#include <sys/kernel.h>
#include <sys/module.h>
#include <sys/sysctl.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/taskqueue.h>
#include "chdev_drv.h"
/*
#define taskqueue_enqueue_dbg(my_tq, my_task) { \
        char buf[128]; \
        snprintf(buf, 128, "%s is activating timer", __func__); \
        wifi_dbg(buf, "taskqueue=" #my_tq ", my_task=" #my_task); \
	taskqueue_enqueue((my_tq), (my_task)); \
}

*/
#define taskqueue_enqueue_dbg(tq, task) {  \
        char buf[128]; \
        snprintf(buf, 128, "%s is activating timer", __func__); \
        wifi_dbg(buf, "taskqueue=" #tq ", my_task=" #task); \
        return taskqueue_enqueue((tq), (task));  \
} 
#endif/* _SAMPLE_DEP_H_ */
