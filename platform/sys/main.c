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
#include <mailbox.h>

void main_init(void);
void main_delete(int id);

static unsigned int this_owner = 0;

int main(int argc, char **argv)
{
	int mbox1, mbox2, mbox3, mbox4, mbox5;;

	main_init();

printf("CREATE ...\n");
	mbox1 = mbox_create("mailbox", this_owner, 0);
	mbox2 = mbox_create("sent", this_owner, 0);
	mbox3 = mbox_create("trash", this_owner, 0);
	mbox4 = mbox_create("alert", this_owner, 0);
	mbox5 = mbox_create("info", this_owner, 0);

	mbox_show();
printf("mbox1=%d, mbox2=%d mbox3=%d mbox4=%d mbox5=%d\n", mbox1, mbox2, mbox3, mbox4, mbox5);

	main_delete(mbox3);
	main_delete(mbox2);
	main_delete(mbox2);
	main_delete(mbox4);
	main_delete(mbox5);
	main_delete(mbox1);
	main_delete(mbox5);

	printf("TungstenBox(tm) is alive!!!\n");
	exit(0);
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
printf("%s: this_owner=%u\n", __FUNCTION__, this_owner);
	this_owner = rand();
printf("%s: this_owner=%u\n", __FUNCTION__, this_owner);
}
