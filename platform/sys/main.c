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

int *tidlist = 0;

int main(int argc, char **argv)
{
	int index;

	if (!--argc)
	{
		printf("Usage: %s <count>\n", *argv);
		exit(0);
	}

	int count = strtol(*++argv, (char **)0, 0);
	printf("count = %d\n", count);

	if (!(tidlist = (int *)malloc(sizeof(int)*count)))
	{
		printf("unable to allocate tidlist\n");
		exit(-1);
	}

	main_init();
	thread_init();

	for (index = 0; index < count; ++index)
	{
		tidlist[index] = -1;
	}

printf("CREATE ...\n");
	for (index = 0; index < count; ++index)
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
	for (index = 0; index < count; ++index)
	{
		int status;

		printf("deleting ... ");
		status = thread_stop(tidlist[index]);
	}

	int mbox1;

	mbox1 = mbox_create("control", get_owner(), 0);

	mail_t *mailp = mail_create(mbox1, MT_INFO, 0, 0);

	printf("TungstenBox(tm) isalive!!!\n");
	exit(0);
}

void main_init(void)
{
printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
	set_owner();
printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
}
