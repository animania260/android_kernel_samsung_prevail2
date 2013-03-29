/*
 *  arch/arm/include/asm/localtimer.h
 *
 *  Copyright (C) 2004-2005 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#ifndef __ASM_ARM_LOCALTIMER_H
#define __ASM_ARM_LOCALTIMER_H

<<<<<<< HEAD
#include <linux/interrupt.h>

=======
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
struct clock_event_device;

/*
 * Setup a per-cpu timer, whether it be a local timer or dummy broadcast
 */
void percpu_timer_setup(void);

<<<<<<< HEAD
=======
/*
 * Called from assembly, this is the local timer IRQ handler
 */
asmlinkage void do_local_timer(struct pt_regs *);


>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
#ifdef CONFIG_LOCAL_TIMERS

#ifdef CONFIG_HAVE_ARM_TWD

#include "smp_twd.h"

<<<<<<< HEAD
#define local_timer_stop(c)	twd_timer_stop((c))
=======
#define local_timer_ack()	twd_timer_ack()
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y

#else

/*
<<<<<<< HEAD
 * Stop the local timer
 */
void local_timer_stop(struct clock_event_device *);
=======
 * Platform provides this to acknowledge a local timer IRQ.
 * Returns true if the local timer IRQ is to be processed.
 */
int local_timer_ack(void);
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y

#endif

/*
 * Setup a local timer interrupt for a CPU.
 */
int local_timer_setup(struct clock_event_device *);

#else

static inline int local_timer_setup(struct clock_event_device *evt)
{
	return -ENXIO;
}
<<<<<<< HEAD

static inline void local_timer_stop(struct clock_event_device *evt)
{
}
=======
>>>>>>> msm-linux-3.0.y/korg/linux-3.0.y
#endif

#endif
