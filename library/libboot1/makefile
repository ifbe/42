CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC= \
data/bignode.c \
data/relation.c \
data/linklist.c \
data/rbtree.c \
loop/loop.exiter.c \
loop/loop.poller.c \
loop/loop.pulser.c \
loop/loop.waiter.c \
std/stdev.c \
std/stdin.c \
std/stdout.c \
work/work.compile.c \
work/work.kernel.c \
work/work.myml.c \
work/work.mython.c \
work/work.guiapp.c \
bootup.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)

%.o: %.c
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<

clean:
	rm -f *.o *.a *.so
