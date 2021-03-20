CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC= \
work/work.compile.c \
work/work.kernel.c \
work/work.myml.c \
work/work.mython.c \
loop/loop.exiter.c \
loop/loop.poller.c \
loop/loop.pulser.c \
loop/loop.realer.c \
loop/loop.waiter.c \
std/stdev.c \
std/stdin.c \
std/stdout.c \
mgr/mgrnode.c \
mgr/mgrwire.c \
bootup.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)

%.o: %.c
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<

clean:
	rm -f *.o *.a *.so
