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

//linux header
#include <linux/init.h>
#include <linux/timer.h>
#include <linux/jiffies.h>

#define mod_timer_dbg(my_timer, expire) {	\
	char buf[128];					\
	snprintf(buf, 128, "%s is activating timer", __func__);	\
	wifi_dbg(buf, "mod_timer=" #my_timer ", expire=" #expire); \
	mod_timer(my_timer, expire);		\
}

#endif /* _SAMPLE_DEP_H_ */
