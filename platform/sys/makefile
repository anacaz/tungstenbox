#
# Copyright(c) 2014 Anacaz Networks, Inc., ALL RIGHTS RESERVED
#
CC=	gcc
CFILES=	main.c mailbox.c thread.c
OFILES=	$(CFILES:%.c=%.o)
CFLAGS=	-DLINUX -I. -I../include -I/usr/include
LFLAGS= -lmon -L../apps/monitor

TARGETS	= sys

all: $(TARGETS)

sys: $(OFILES)
	make -C ../apps/monitor
	$(CC) $(CFLAGS) $(LFLAGS) -o $@ $(OFILES)

.c.o: 
	$(CC) $(CFLAGS) -c $<

clean:
	make -C ../apps/monitor clean
	rm -rf *.o $(TARGETS)

