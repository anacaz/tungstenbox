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

#include <thread.h>
#include <mailbox.h>

void main_init(void);
// void *main_thread(void *);

int tidlist[25];

int main(int argc, char **argv)
{
	int index;

	main_init();
	thread_init();

	for (index = 0; index < 25; ++index)
	{
		tidlist[index] = -1;
	}

printf("CREATE ...\n");
	for (index = 0; index < 10; ++index)
	{
		/*
		 * Each mail box is created in the main_thread() start up routine.
		 */
		tidlist[index] = thread_new(thread_start);
	}

	sleep(2);
	thread_show();
	// mbox_show();

	/*
	 * BUG!!!  This only deletes the mailboxes now we need to delete the threads!!!
	 */
	for (index = 0; index < 10; ++index)
	{
		int status;

		printf("deleting ... ");
		status = thread_stop(tidlist[index]);
		if (status == -1)
			printf("\n");
	}

	int mbox1;

	mbox1 = mbox_create("control", get_owner(), 0);

	mail_t *mailp = mail_create(mbox1, MT_INFO, 0, 0);

	printf("TungstenBox(tm) isalive!!!\n");
	exit(0);
}

#if 0
void *main_thread(void *arg)
{
	threadctl_t *tp;
	unsigned int owner = get_owner();

	tp = (threadctl_t *)arg;
	snprintf(tp->name, 31, "thread-%d", tp->tid);
	tp->mid = mbox_create(tp->name, owner, 0);
printf("creating ... %d = mbox_create(%s, %08X, %d)\n", tp->mid, tp->name, owner, 0);

	return(0);
}
#endif

void main_init(void)
{
printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
	set_owner();
printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
}
