DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC -I..


baresrc = \
bareos/bare.flow.shell.c \
bareos/bare.flow.socket.c \
bareos/bare.flow.uart.c \
bareos/bare.flow.watcher.c \
bareos/bare.lock.semaphore.c \
bareos/bare.lock.mutex.c \
bareos/bare.memory.file.c \
bareos/bare.memory.folder.c \
bareos/bare.memory.portio.c \
bareos/bare.multitask.coroutine.c \
bareos/bare.multitask.thread.c \
bareos/bare.multitask.process.c \
bareos/bare.multitask.virtualize.c \
bareos/bare.syscall.42.c \
bareos/bare.syscall.unix.c \
bareos/bare.system.power.c \
bareos/bare.system.random.c \
bareos/bare.system.signal.c \
bareos/bare.system.time.c \
system.c
bareobj = $(patsubst %.c,%.o,$(baresrc))


myossrc = \
system.c
myosobj = $(patsubst %.c,%.o,$(myossrc))


efisrc = \
efiapp/efi.flow.socket.c \
efiapp/efi.flow.watcher.c \
efiapp/efi.memory.file.c \
efiapp/efi.memory.folder.c \
efiapp/efi.memory.portio.c \
efiapp/efi.system.power.c \
efiapp/efi.multitask.coroutine.c \
efiapp/efi.multitask.thread.c \
efiapp/efi.multitask.process.c \
efiapp/efi.multitask.virtualize.c \
efiapp/efi.system.random.c \
efiapp/efi.system.signal.c \
efiapp/efi.system.shell.c \
efiapp/efi.system.time.c \
system.c
efiobj = $(patsubst %.c,%.o,$(efisrc))


bsdsrc = \
bsdapp/bsd.memory.file.c \
bsdapp/bsd.memory.folder.c \
bsdapp/bsd.memory.mmio.c \
bsdapp/bsd.system.power.c \
bsdapp/bsd.multitask.coroutine.c \
bsdapp/bsd.multitask.thread.c \
bsdapp/bsd.multitask.process.c \
bsdapp/bsd.multitask.virtualize.c \
bsdapp/bsd.system.random.c \
bsdapp/bsd.system.signal.c \
bsdapp/bsd.system.time.c \
system.c
bsdobj = $(patsubst %.c,%.o,$(bsdsrc))


linuxsrc = \
linuxapp/linux.flow.epoll.c \
linuxapp/linux.flow.socket.c \
linuxapp/linux.flow.ptmx.c \
linuxapp/linux.flow.tty.c \
linuxapp/linux.memory.file.c \
linuxapp/linux.memory.folder.c \
linuxapp/linux.memory.mmio.c \
linuxapp/linux.system.power.c \
linuxapp/linux.multitask.coroutine.c \
linuxapp/linux.multitask.thread.c \
linuxapp/linux.multitask.process.c \
linuxapp/linux.multitask.virtualize.c \
linuxapp/linux.system.random.c \
linuxapp/linux.system.signal.c \
linuxapp/linux.system.time.c \
system.c
linuxobj = $(patsubst %.c,%.o,$(linuxsrc))


macsrc = \
macapp/mac.flow.kqueue.c \
macapp/mac.flow.socket.c \
macapp/mac.flow.ptmx.c \
macapp/mac.flow.tty.c \
macapp/mac.memory.file.c \
macapp/mac.memory.folder.c \
macapp/mac.memory.mmio.c \
macapp/mac.system.power.c \
macapp/mac.multitask.coroutine.c \
macapp/mac.multitask.thread.c \
macapp/mac.multitask.process.c \
macapp/mac.multitask.virtualize.c \
macapp/mac.system.random.c \
macapp/mac.system.signal.c \
macapp/mac.system.time.c \
system.c
macobj = $(patsubst %.c,%.o,$(macsrc))


winsrc = \
winapp/win.flow.iocp.c \
winapp/win.flow.socket.c \
winapp/win.flow.cmd.c \
winapp/win.flow.com.c \
winapp/win.memory.file.c \
winapp/win.memory.folder.c \
winapp/win.system.power.c \
winapp/win.multitask.coroutine.c \
winapp/win.multitask.thread.c \
winapp/win.multitask.process.c \
winapp/win.multitask.virtualize.c \
winapp/win.system.random.c \
winapp/win.system.signal.c \
winapp/win.system.time.c \
system.c
winobj = $(patsubst %.c,%.o,$(winsrc))

.PHONY:bare
bare: $(bareobj)

.PHONY:myos
myos: $(myosobj)

.PHONY:efi
efiapp: $(efiobj)

.PHONY:bsd
bsd: $(bsdobj)

.PHONY:linux
linux: $(linuxobj)

.PHONY:mac
mac: $(macobj)

.PHONY:win
win: $(winobj)

%.o: %.c
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libsoft0/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi


clean:
	rm -f *.o *.a
