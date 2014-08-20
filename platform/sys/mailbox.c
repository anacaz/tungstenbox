/*
 * TungstenBox(tm) the original!
 *
 * Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED.
 *
 * rfa - 140820
 */
#include <stdint.h>
#include <stdlib.h>
#include <mailbox.h>

static mailbox_t *mailbox_master = 0;

/*
 * TODO!  Mail messages should be alloc'd and free'd as linked messages using the mbox
 *	  as the master list pointer.
 */

/*
 * Mailbox APIs.
 */
int mailbox_init(unsigned int owner, size_t size, void (*alert)(void))
{
	mailbox_t *mbp;

	if (!(mbp = malloc(sizeof(*mbp))))
		return(0);
	mbp->id = mailbox_assign();
	mbp->owner = owner;
	mbp->size = size;
	mbp->unread = mbp->used = 0;
	if (!(mbp->mbox = malloc(size)))
	{
		free(mbp);
		return(0);
	}
	mbp->alert = alert;
	mbp->link = mailbox_master;
	mailbox_master = mbp;
	return(1);
}

/*
 * Assign a unique mailbox id.
 */
unsigned int mailbox_assign(void)
{
	static unsigned int mailbox_id = 0;
	return(++mailbox_id);
}

int mailbox_send(unsigned int owner, void *mail)
{
	return(0);
}

/*
 * Messages are removed on read.
 */
void *mailbox_read(unsigned int id)
{
	return(0);
}
