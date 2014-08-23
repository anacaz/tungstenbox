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

FORWARD void thread_init(void);
FORWARD void thread_reset(int tid);
FORWARD int thread_new(void (*startup)(void *));
FORWARD void *thread_start(void *);
FORWARD void thread_default(void *arg);
FORWARD void thread_show(void);
FORWARD void thread_print(unsigned int tid);
FORWARD void thread_stop(void *arg);

FORWARD static int thread_find(int tid);

static threadctl_t threads[CONFIG_THREADMAX];

void thread_init(void)
{
	int index;

	for (index = 0; index < CONFIG_THREADMAX; ++index)
	{
		THREAD_RESET(index);
	}
}

void thread_reset(int tid)
{
	int index;

	if ((index = thread_find(tid)) == -1)
		return;
	THREAD_RESET(index);
}

/*
 * This routine bundles all of the house keeping for thread launch control and management
 * in one place.
 */
int thread_new(void (*startup)(void *))
{
	threadctl_t *tp;
	int index;

	index = thread_find(-1);
printf("%s: %d = thread_find(-1)\n", __FUNCTION__, index);
	// if ((index = thread_find(-1)) == -1)
	if (index == -1)
	{
		printf("error: no more thread slots available!!!\n");
		return(-1);
	}
	/*
	 * Each mail box is created in the thread_start() start up routine.
	 */
	tp = &threads[index];
	tp->tid = index;
	snprintf(tp->name, 31, "thread-%d", tp->tid);
	tp->func = startup;					/* Load the startup function */
	pthread_create(&tp->t, 0, thread_start, (void *)tp);
	return(tp->tid);
}

/*
 * This is the startup wrapper function that will pass control to the user specified thread
 * function from thread_new().
 */
void *thread_start(void *arg)
{
	threadctl_t *tp;

	tp = (threadctl_t *)arg;
	tp->mid = mbox_create(tp->name, tp->tid, 0);		/* Create the thread mailbox */
printf("creating ... %d = mbox_create(%s, %08X, %d)\n", tp->mid, tp->name, tp->tid, 0);
	if (tp->func)
		(*tp->func)(tp);				/* Pass control to the thread function */
	thread_stop(tp);					/* Stop the thread */
	return(0);
}

void thread_default(void *arg)
{
	threadctl_t *tp;
	unsigned int seconds;

	tp = (threadctl_t *)arg;
	seconds = 1;
	printf("**** %s: %s sleeping for %d second(s)\n", __FUNCTION__, tp->name, seconds);
	sleep(seconds);
}

void thread_show(void)
{
	void mbox_show(unsigned int mid);
	int index;
	int count;

	for (count = index = 0; index < CONFIG_THREADMAX; ++index)
	{
		threadctl_t *tp = &threads[index];

		if (tp->tid == -1)
			continue;
		++count;
		printf("thread(%d): t(%p) mid(%d) func(%p) %s\n", 
			tp->tid, &tp->t, tp->mid, tp->func, tp->name);
		mbox_show(tp->mid);
	}
	printf("%d active threads found\n", count);
}

void thread_print(unsigned int tid)
{
	threadctl_t *tp;
	int index;

	if ((index = thread_find(-1)) == -1)
	{
		printf("error: invalid tid %d\n", tid);
		return;
	}
	tp = &threads[index];
	printf("thread(%-16s): id(%d) t(%p) mid(%d) func(%p)\n", 
		tp->name, tp->tid, &tp->t, tp->mid, tp->func);
}

void thread_stop(void *arg)
{
	threadctl_t *tp;
	int index;

	tp = (threadctl_t *)arg;
printf("THREAD STOP: %s\n", tp->name);
	tp->status = mbox_delete(tp->mid);
	printf("%d = mbox_delete(%d)\n", tp->status, tp->mid);
	thread_reset(tp->tid);
	pthread_exit(&tp->status);
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

	for (index = 0; index < CONFIG_THREADMAX; ++index)
	{
		if (threads[index].tid == tid)
			return(index);
	}
	return(-1);
}
