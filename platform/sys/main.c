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
#include <mailbox.h>

void main_init(void);
void *main_thread(void *);

typedef struct threadctl threadctl_t;

struct threadctl
{
	pthread_t	t;			/* pthread instance */
	char		name[32];		/* thread name */
	int		tid;			/* thread identifier */
	int		mid;			/* thread's mail box */
};

#define THREADMAX	25

static threadctl_t threads[THREADMAX];

int main(int argc, char **argv)
{
	int tid;

	main_init();

printf("CREATE ...\n");
	for (tid = 0; tid < THREADMAX; ++tid)
	{
		/*
		 * Each mail box is created in the main_thread() start up routine.
		 */
		pthread_create(&threads[tid].t, 0, main_thread, (void *)&threads[tid]);
		threads[tid].tid = tid;
	}

	sleep(2);
	// mbox_show();

	/*
	 * BUG!!!  This only deletes the mailboxes now we need to delete the threads!!!
	 */
	for (tid = 0; tid < THREADMAX; ++tid)
	{
		int status;

		printf("deleting ... ");
		status = mbox_delete(threads[tid].mid);
		printf("%d = mbox_delete(%d)\n", status, threads[tid].mid);
	}

	int mbox1;

	mbox1 = mbox_create("control", get_owner(), 0);

	mail_t *mailp = mail_create(mbox1, MT_INFO, 0, 0);

	printf("TungstenBox(tm) isalive!!!\n");
	exit(0);
}

void *main_thread(void *arg)
{
	threadctl_t *tp;
	unsigned int owner = get_owner();

	tp = (threadctl_t *)arg;
	sprintf(tp->name, "%s-%d", "thread", tp->tid);
	tp->mid = mbox_create(tp->name, owner, 0);
printf("creating ... %d = mbox_create(%s, %08X, %d)\n", tp->mid, tp->name, owner, 0);

	return(0);
}

#define THREAD_RESET(X) \
do \
{ \
	threads[tid].tid = -1; \
	threads[tid].name[0] = '\0'; \
	threads[tid].mid = 0; \
} while (0)

void main_init(void)
{
	int tid;

	for (tid = 0; tid < THREADMAX; ++tid)
	{
		THREAD_RESET(tid);
	}

printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
	set_owner();
printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
}
