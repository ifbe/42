CC:=gcc
CF:="-c -O2 -fPIC -I.."
DIR:=$(shell pwd)
SRC= \
data/bignode_self.c \
data/bignode_slot.c \
data/rel_0jump.c \
data/rel_1jump.c \
data/rel_2jump.c \
data/rel_3jump.c \
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
setup/setup.subcmd.c \
setup/setup.compiler.c \
setup/setup.kernel.c \
setup/setup.myml.c \
setup/setup.mython.c \
setup/setup.guiapp.c \
setup/setup.term.c \
bootup.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)

%.o: %.c
	#A=$$(stat -c %Y $(DIR)/$(notdir $@));
	#B=$$(stat -c %Y $<);
	#$(eval tmp=$(shell if [ $(DIR)/$(notdir $@) -nt $< ]; then echo gt; else echo lt; fi))
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libboot1/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi

clean:
	rm -f *.o *.a *.so
