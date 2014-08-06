/**
 * @file
 * @brief
 *
 * @date 21.11.2013
 * @author Andrey Kokorev
 */

#ifndef _KERNEL_SCHEDEE_H_
#define _KERNEL_SCHEDEE_H_


#include <kernel/sched/sched_strategy.h>
#include <kernel/thread/signal.h>
#include <kernel/sched/waitq.h>

enum schedee_result {
	SCHEDEE_EXIT,
	SCHEDEE_REPEAT
};

struct schedee {
	enum schedee_result                (*prepare)(struct schedee *prev,
			struct schedee *n, struct runq *rq);

	void               *(*run)(void *); /**< Start routine */
	void               *run_arg;        /**< Argument to be passed to run */

	struct sched_attr  sched_attr;      /**< Scheduler-private data pointer */

	unsigned int       active;       /**< Running on a CPU. TODO SMP-only. */
	unsigned int       ready;        /**< Managed by the scheduler. */
	unsigned int       waiting;      /**< Waiting for an event. */

	spinlock_t         lock;         /**< Protects wait state and others. */

	struct sigstate    sigstate;     /**< Pending signal(s). */
	struct waitq_link  waitq_link;
};

#endif /* _KERNEL_SCHEDEE_H_ */
