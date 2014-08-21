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
#include <string.h>
#include <pthread.h>
#include <mailbox.h>

void main_init(void);
void main_delete(int id);
void *main_thread(void *);

int main(int argc, char **argv)
{
	int mbox1, mbox2, mbox3, mbox4, mbox5;;
	static pthread_t t1, t2, t3, t4, t5;

	main_init();

printf("CREATE ...\n");
	// mbox1 = mbox_create("thread1", get_owner(), 0);
	pthread_create(&t1, 0, main_thread, (void *)&mbox1);
	// mbox2 = mbox_create("thread2", get_owner(), 0);
	pthread_create(&t2, 0, main_thread, (void *)&mbox2);
	mbox3 = mbox_create("thread2", get_owner(), 0);
	mbox4 = mbox_create("thread3", get_owner(), 0);
	mbox5 = mbox_create("thread4", get_owner(), 0);

	mbox_show();
printf("mbox1=%d, mbox2=%d mbox3=%d mbox4=%d mbox5=%d\n", mbox1, mbox2, mbox3, mbox4, mbox5);

	main_delete(mbox3);
	main_delete(mbox2);
	main_delete(mbox2);
	main_delete(mbox4);
	main_delete(mbox5);
	main_delete(mbox1);
	main_delete(mbox5);

	mbox1 = mbox_create("control", get_owner(), 0);

	mail_t *mailp = mail_create(mbox1, MT_INFO, 0, 0);

	printf("TungstenBox(tm) is alive!!!\n");
	exit(0);
}

void *main_thread(void *arg)
{
	static int instance = 0;
	char name[32];			/* BUG!!! */

	sprintf(name, "%s%d", "thread", ++instance);
	*(int *)arg = mbox_create(name, get_owner(), 0);
	printf("Hello World %s\n", name);

	return(0);
}

void main_delete(int id)
{
	int status;

printf("DELETE ... %d ", id);
	status = mbox_delete(id);
printf("status=%d\n", status);
	mbox_show();
}

void main_init(void)
{
printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
	set_owner();
printf("%s: this_owner=%X\n", __FUNCTION__, get_owner());
}
