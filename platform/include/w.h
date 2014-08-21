#ifndef _w_h
#define _w_h
/*
 * TungstenBox(tm) the original!
 *
 * Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED.
 *
 * rfa - 140820-21
 */

/*
 * This might need to be ASCII only.
 */
struct mfginfo
{
	char		serial[128];
	unsigned char	macadr[6];
	unsigned char	rsakey[4096/8];			/* BUG!!! This is only one key!!! */
	time_t		builddate;
};

#endif /* !_w_h */
