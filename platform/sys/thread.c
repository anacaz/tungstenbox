/*
 * TungstenBox(tm) main --
 *
 * Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED
 *
 * rfa - 140820
 */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include <config.h>
#include <thread.h>
#include <mailbox.h>

#define FORWARD	/* */

FORWARD void thread_reset(int tid);
FORWARD void *thread_start(void *);
FORWARD int thread_new(void *(*startup)(void *));

FORWARD static int thread_find(int tid);

static threadctl_t threads[CONFIG_THREADMAX];

void thread_reset(int tid)
{
	int index;

	if ((index = thread_find(-1)) == -1)
	{
		THREAD_RESET(index);
	}
}

/*
 * This routine bundles all of the house keeping for thread launch control and management
 * in one place.
 */
int thread_new(void *(*startup)(void *))
{
	int index;
	threadctl_t *tp;

	if ((index = thread_find(-1)) == -1)
	{
		printf("error: no more thread slots available!!!\n");
		return;
	}
	/*
	 * Each mail box is created in the thread_start() start up routine.
	 */
	tp = &threads[index];
	tp->tid = index;
	snprintf(tp->name, 31, "thread-%d", tp->tid);
	if (!startup)
		startup = thread_start;
	pthread_create(tp->t, 0, startup, (void *)tp);
	return(tp->tid);
}

void *thread_start(void *arg)
{
	threadctl_t *tp;
	unsigned int owner = get_owner();

	tp = (threadctl_t *)arg;
	tp->mid = mbox_create(tp->name, owner, 0);
printf("creating ... %d = mbox_create(%s, %08X, %d)\n", tp->mid, tp->name, owner, 0);
	return(0);
}

void thread_stop()
{
}

/*
 * Find the caller specified thread id within the thread control array 'threads'.  Return
 * the array offset on success or -1 on failure.
 *
 * To located an unused thread slot then search for thread id '-1'.  All reset or unused
 * thread slots will reset the thread id to -1.
 */
static int thread_find(int tid)
{
	int index;

	for (index = 0; index = CONFIG_THREADMAX; ++index)
		if (threads[index].tid == tid)
			return(index);
	return(-1);
}
