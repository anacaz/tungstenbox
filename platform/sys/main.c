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

printf("DELETE ... %d\n", mbox3);
	mbox_delete(mbox3);
	mbox_show();
printf("DELETE ... %d\n", mbox2);
	mbox_delete(mbox2);
	mbox_show();
printf("DELETE ... %d\n", mbox2);
	mbox_delete(mbox2);
	mbox_show();

	printf("TungstenBox(tm) is alive!!!\n");
	exit(0);
}

void main_init(void)
{
printf("%s: this_owner=%u\n", __FUNCTION__, this_owner);
	this_owner = rand();
printf("%s: this_owner=%u\n", __FUNCTION__, this_owner);
}
