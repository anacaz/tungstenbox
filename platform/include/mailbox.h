#ifndef _mailbox_h
#define _mailbox_h
/*
 * TungstenBox(tm) the original!
 *
 * Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED.
 *
 * rfa - 140820
 */
#include <stdint.h>

typedef struct mailbox mailbox_t;

struct mailbox
{
	mailbox_t	*link;
	unsigned int	id;			/* Identifier */
	unsigned int	owner;			/* Owner */
	size_t		size;			/* Mailbox capacity (in bytes) */
	size_t		used;			/* Used space (in bytes) */
	unsigned int	unread;			/* Unread messages (count) */
	void		*mbox;			/* Mailbox buffer */
	void		(*alert)(void);		/* Mailbox call back handler */
};

/*
 * Mailbox APIs.
 */
int mailbox_init(unsigned int owner, size_t size, void (*alert)(void));
int mailbox_send(unsigned int owner, void *mail);
void *mailbox_read(unsigned int id);
unsigned int mailbox_assign(void);
void mailbox_show(void);

#endif /* !_mailbox.h */
