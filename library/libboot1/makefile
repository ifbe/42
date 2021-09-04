CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC= \
data/bignode.c \
data/relation.c \
data/linklist.c \
data/rbtree.c \
inout/in.ev.c \
inout/in.str.c \
inout/out.str.c \
inout/out.trace.c \
loop/loop.exiter.c \
loop/loop.poller.c \
loop/loop.pulser.c \
loop/loop.waiter.c \
setup/setup.compiler.c \
setup/setup.kernel.c \
setup/setup.myml.c \
setup/setup.mython.c \
setup/setup.guiapp.c \
bootup.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)

%.o: %.c
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<

clean:
	rm -f *.o *.a *.so
