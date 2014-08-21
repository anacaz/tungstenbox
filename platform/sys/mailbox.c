/*
 * TungstenBox(tm) the original!
 *
 * Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED.
 *
 * rfa - 140820-21
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <mailbox.h>

#define FORWARD	/* */

FORWARD static mbox_t *mbox_find(unsigned int id);
FORWARD static unsigned int mbox_assign(void);

static mbox_t *mbox_master = 0;

/*
 * TODO!  Mail messages should be alloc'd and free'd as linked messages using the mbox
 *	  as the master list pointer. -- DONE
 */

/*
 * Mailbox APIs.
 */

/*
 * Create a new mailbox based upon the caller specified attributes.
 *
 * Return the mailbox id upon success, return -1 on error.
 */
int mbox_create(char *name, unsigned int owner, void (*client)(void))
{
	mbox_t *mbp;

	printf("Creating mailbox \"%s\" ... ", name);
	if (!(mbp = malloc(sizeof(*mbp))))
	{
		printf("Error: unable to allocate resource!!!\n");
		return(-1);
	}
	if (!(mbp->name = strdup(name)))
	{
		printf("Error: unable to duplicate name!!!\n");
		free(mbp);
		return(-1);
	}
	mbp->id = mbox_assign();
	mbp->owner = owner;
	mbp->mbox = 0;
	mbp->client = client;
	mbp->link = mbox_master;
	mbox_master = mbp;
	printf("id=%03d owner=%08X mbox=%p client=%p\n", mbp->id, mbp->owner, mbp->mbox, mbp->client);
	return(mbp->id);
}

/*
 * Delete the caller specified mailbox.
 *
 * Return 0 on success, return -1 on error.
 */
int mbox_delete(unsigned int id)
{
	mbox_t *mbp, *target;

	if (!(target = mbox_find(id)))
	{
		return(-1);
	}
	for (mbp = mbox_master; mbp->link && mbp->link != target; mbp = mbp->link)
		;
	if (mbox_master == target)
	{
		mbox_master = mbox_master->link;	/* Remove the top entry from the list */
	}
	else if (mbp->link)
	{
		if (mbp->link != target)
		{
			printf("%s: link IS active and can't find the target!!!\n", __FUNCTION__);
			return(-1);
		}
		mbp->link = mbp->link->link;		/* Remove the link from the list */
	}
	else if (mbp == target)
	{
		mbox_master = 0;			/* Zero out the master */
	}
	else
	{
		printf("%s: link is NOT active and can't find the target!!!\n", __FUNCTION__);
		printf("%s: mbox_master=%p target=%p mbp=%p\n", __FUNCTION__, mbox_master, target, mbp);
		return(-1);
	}
	free(target->name);
	free(target);
	return(0);
}

/*
 * Find the mailbox associated with the caller specified mailbox id.
 */
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

	if (!mbox_master)
	{
		printf("%s: No mailboxes found!!!\n", __FUNCTION__);
		return;
	}
	for (mbp = mbox_master; mbp; mbp = mbp->link)
	{
// printf("SHOW mbp=%p ", mbp);
// printf("mbp->name=%s ", mbp->name);
// printf("mbp->link=%p\n", mbp->link);
		printf("\t%-20s id=%03d owner=%08X mbox=%p client=%p\n", mbp->name,
			mbp->id, mbp->owner, mbp->mbox, mbp->client);
	}
}

/*
 * Assign a unique mbox id.
 */
static unsigned int mbox_assign(void)
{
	static unsigned int mbox_id = 0;

	++mbox_id;
// printf("%s: mbox_id=%d\n", __FUNCTION__, mbox_id);
	return(mbox_id);
}

/*
 * Mail APIs.
 */
int mail_send(unsigned int owner, void *mail)
{
	return(0);
}

/*
 * Messages are removed on read.
 */
void *mail_read(unsigned int id)
{
	return(0);
}

void *mail_create()
{
	return(0);
}
