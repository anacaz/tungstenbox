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
#include <time.h>

typedef struct mbox mbox_t;
typedef struct mail mail_t;

struct mbox
{
	mbox_t		*link;
	char		*name;
	unsigned int	id;			/* Identifier */
	unsigned int	owner;			/* Owner */
	mail_t		*mbox;			/* Mailbox buffer */
	void		(*client)(void);	/* Mailbox call back handler */
};

/*
 * The mail messages themselves are completely self contained.  One-to-many, many-to-one, and
 * one-to-one message transmission.  The system crosses the "plane" with mail messages and
 * guaranteed delivery methods.
 */

typedef enum mailtype mailtype_e;

enum mailtype { MT_INFO, MT_ALERT, MT_EVENT, MT_ALARM, MT_FAULT };

typedef struct mailhdr mailhdr_t;

struct mailhdr
{
	unsigned int	from;		/* Sender id */
	mailtype_e	type;		/* Mail type */
	time_t		time;		/* Timestamp */
};

struct mail
{
	mail_t		*next;			/* Next available message */
	mailhdr_t	hdr;
	size_t		size;
	void		*body;			/* Mail body */
};

/*
 * Mailbox APIs.
 */
int mbox_create(char *name, unsigned int owner, void (*client)(void));
int mbox_send(unsigned int owner, void *mail);
void *mbox_read(unsigned int id);
void mbox_list(void);
void mbox_show(unsigned int mid);
int mbox_delete(unsigned int id);

/*
 * Mail message APIs.
 */
mail_t *mail_create(unsigned int id, mailtype_e type, void *body, size_t size);

/*
 * Ownership and authentication routines.
 */
unsigned int set_owner(void);
unsigned int get_owner(void);
int confirm_owner(unsigned int owner);

#endif /* !_mailbox.h */
