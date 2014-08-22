#ifndef _thread_h
#define _thread_h
/*
 * This file describes the thread control interface and APIs.
 *
 * Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED.
 *
 * rfa - 140822
 */

typedef struct threadctl threadctl_t;

struct threadctl
{
	pthread_t	t;			/* pthread instance */
	char		name[32];		/* thread name */
	int		tid;			/* thread identifier */
	int		mid;			/* thread's mail box */
	void		*(*func)(void *);	/* startup routine */
};

#define THREAD_RESET(X) \
do \
{ \
	threads[X].tid = -1; \
	threads[X].name[0] = '\0'; \
	threads[X].mid = 0; \
} while (0)

void thread_init(void);
void thread_reset(int tid);
int thread_new(void *(*startup)(void *));
void *thread_start(void *);
void thread_show(void);
int thread_stop(int tid);

#endif /* !_thread_h */
