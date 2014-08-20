
CC=	gcc
CFILES=	main.c mailbox.c
OFILES=	$(CFILES:%.c=%.o)
CFLAGS=	-DLINUX -I. -I../include -I/usr/include

TARGETS	= sys

all: clean $(TARGETS)

sys: $(OFILES)
	$(CC) $(CFLAGS) -o $@ $(OFILES)

.c.o: 
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o $(TARGETS)

