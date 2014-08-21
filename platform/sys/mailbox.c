/*
 * TungstenBox(tm) the original!
 *
 * Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED.
 *
 * rfa - 140820
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mailbox.h>

#define FORWARD	/* */

FORWARD static mbox_t *mbox_find(unsigned int id);

static mbox_t *mbox_master = 0;

/*
 * TODO!  Mail messages should be alloc'd and free'd as linked messages using the mbox
 *	  as the master list pointer. -- DONE
 */

/*
 * Mailbox APIs.
 */
int mbox_create(char *name, unsigned int owner, void (*client)(void))
{
	mbox_t *mbp;

	if (!(mbp = malloc(sizeof(*mbp))))
		return(0);
	if (!(mbp->name = strdup(name)))
	{
		free(mbp);
		return(0);
	}
	mbp->id = mbox_assign();
	mbp->owner = owner;
	mbp->mbox = 0;
	mbp->client = client;
	mbp->link = mbox_master;
	mbox_master = mbp;
	return(mbp->id);
}

void mbox_delete(unsigned int id)
{
	mbox_t *mbp, *target;

	if (!(target = mbox_find(id)))
	{
printf("%s: id %d not found\n", __FUNCTION__, id);
		return;
	}
	for (mbp = mbox_master; mbp->link && mbp->link != target; mbp = mbp->link)
		;
	if (mbp->link)
	{
		if (mbp->link == target)
		{
			mbp->link = mbp->link->link;
		}
	}
	else if (mbp == target)
	{
		mbox_master = 0;
	}
	free(target);
}

static mbox_t *mbox_find(unsigned int id)
{
	mbox_t *mbp;

	for (mbp = mbox_master; mbp; mbp = mbp->link)
	{
		if (mbp->id == id)
		{
			return(mbp);
		}
	}
	return(0);
}

void mbox_show(void)
{
	mbox_t *mbp;

	for (mbp = mbox_master; mbp; mbp = mbp->link)
	{
		printf("%s: %s id=%X owner=%X mbox=%X client=%X\n", __FUNCTION__, mbp->name,
			mbp->id, mbp->owner, (unsigned int)mbp->mbox, (unsigned int)mbp->client);
	}
}

/*
 * Assign a unique mbox id.
 */
unsigned int mbox_assign(void)
{
	static unsigned int mbox_id = 0;

	++mbox_id;
printf("%s: mbox_id=%d\n", __FUNCTION__, mbox_id);
	return(mbox_id);
}

int mbox_send(unsigned int owner, void *mail)
{
	return(0);
}

/*
 * Messages are removed on read.
 */
void *mbox_read(unsigned int id)
{
	return(0);
}

void *mail_form()
{
	return(0);
}
