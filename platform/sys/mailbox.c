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
#include <pthread.h>

#define FORWARD	/* */

FORWARD static mbox_t *mbox_find(unsigned int mid);
FORWARD static unsigned int mbox_assign(void);
FORWARD static void mbox_default(void *arg);

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
static pthread_mutex_t mbox_lock;

int mbox_create(char *name, unsigned int owner, void (*client)(void *))
{
	static char *inbox = "-inbox";
	mbox_t *mbp;
	int err;

	printf("Creating mailbox \"%s\" ... ", name);
	printf("Waiting for lock %s ...\n", name);
	if ((err = pthread_mutex_lock(&mbox_lock)))
	{
		printf("error(%d): %s unable to acuire lock!!!\n", err, name);
		return(-1);
	}
	printf("Got lock %s\n", name);
	if (!(mbp = malloc(sizeof(*mbp))))
	{
		printf("error: %s unable to allocate resource!!!\n", name);
		pthread_mutex_unlock(&mbox_lock);
		return(-1);
	}
	if (!(mbp->name = strdup(name)))
	{
		printf("error: unable to strdup %s!!!\n", name);
		free(mbp);
		pthread_mutex_unlock(&mbox_lock);
		return(-1);
	}
	if (!(mbp->name = realloc(mbp->name, strlen(inbox))))
	{
		printf("error: unable to realloc %s!!!\n", name);
		free(mbp);
		pthread_mutex_unlock(&mbox_lock);
		return(-1);
	}
	strcat(mbp->name, inbox);
	mbp->id = mbox_assign();
printf("%s: %s mbp->id=%d\n", __FUNCTION__, mbp->name, mbp->id);
	mbp->owner = owner;
	mbp->mbox = 0;
	mbp->client = client ? client : mbox_default;		/* Set the default client routine */
	mbp->link = mbox_master;
	mbox_master = mbp;
printf("id=%03d owner=%08X mbox=%p client=%p\n", mbp->id, mbp->owner, mbp->mbox, mbp->client);
	pthread_mutex_unlock(&mbox_lock);
	return(mbp->id);
}

/*
 * On the off chance that a mail client is not specified by the caller of mbox_create then
 * this routine will be the default mail client.
 */
static void mbox_default(void *arg)
{
	mail_t *mail = (mail_t *)arg;

	printf("%s: mail=%p\n", __FUNCTION__, mail);
	switch (mail->hdr.type)
	{
	case MT_INFO: printf("type: MT_INFO "); break;
	case MT_ALERT: printf("type: MT_ALERT "); break;
	case MT_EVENT: printf("type: MT_EVENT "); break;
	case MT_ALARM: printf("type: MT_ALARM "); break;
	case MT_FAULT: printf("type: MT_FAULT "); break;
	default: printf("type: unknown "); break;
	}
	printf("\n");
}

/*
 * Delete the caller specified mailbox.
 *
 * Return 0 on success, return -1 on error.
 */
int mbox_delete(unsigned int mid)
{
	mbox_t *mbp, *target;

	if (!(target = mbox_find(mid)))
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

void mbox_list(void)
{
	mbox_t *mbp;

	if (!mbox_master)
	{
		printf("%s: No mailboxes found!!!\n", __FUNCTION__);
		return;
	}
	for (mbp = mbox_master; mbp; mbp = mbp->link)
	{
		printf("\t%-20s id=%03d owner=%08X mbox=%p client=%p\n", mbp->name,
			mbp->id, mbp->owner, mbp->mbox, mbp->client);
	}
}

void mbox_show(unsigned int mid)
{
	mbox_t *mbp;

	if (!(mbp = mbox_find(mid)))
		return;
	printf("mailbox(%d) owner=%08X mbox=%p client=%p %s\n",
		mbp->id, mbp->owner, mbp->mbox, mbp->client, mbp->name);
}

/*
 * Find the mailbox associated with the caller specified mailbox id.
 */
static mbox_t *mbox_find(unsigned int mid)
{
	mbox_t *mbp;

// printf("%s: mid=%d\n", __FUNCTION__, mid);
	for (mbp = mbox_master; mbp; mbp = mbp->link)
	{
		if (mbp->id == mid)
		{
// printf("%s: mbp=%X mid=%d\n", __FUNCTION__, (int)mbp, mid);
			return(mbp);
		}
	}
// printf("%s: mbp=%X mid=%d\n", __FUNCTION__, 0, mid);
	return(0);
}

/*
 * Assign a unique mbox id.
 */
static unsigned int mbox_assign(void)
{
	static unsigned int mbox_id = 0;

	++mbox_id;
printf("<<<< %s: mbox_id=%d\n", __FUNCTION__, mbox_id);
	return(mbox_id);
}

/*
 * Mail APIs.
 */
/*
 * Create a mail message from caller specified mailbox.
 */
mail_t *mail_create(unsigned int id, mailtype_e type, void *body, size_t size)
{
	mbox_t *mbp;
	mail_t *mailp;

	if (!(mbp = mbox_find(id)))
	{
		printf("error: sender mbox id %d is invalid!!!\n", id);
		return(0);
	}

	printf("Creating mail message from owner(%08X)  mbox(%s) ... \n", mbp->owner, mbp->name);
	if (!(mailp = (mail_t *)malloc(sizeof(*mailp))))
	{
		printf("error: unable to allocate resource!!!\n");
		return(0);
	}

	return(mailp);
}

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

#if 0
static unsigned int this_owner = 0;

unsigned int set_owner(void)
{
	if (!this_owner)
		this_owner = rand();
	return(get_owner());
}

unsigned int get_owner(void)
{
	return(this_owner);
}

int confirm_owner(unsigned int owner)
{
	return(owner == this_owner);
}
#endif
