#
# Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED
#
CC=	gcc
CFILES=	main.c mailbox.c
OFILES=	$(CFILES:%.c=%.o)
CFLAGS=	-DLINUX -I. -I../include -I/usr/include

TARGETS	= sys

all: $(TARGETS)

sys: $(OFILES)
	$(CC) $(CFLAGS) -o $@ $(OFILES)

.c.o: 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o $(TARGETS)

