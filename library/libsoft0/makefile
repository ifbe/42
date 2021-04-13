DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC -I..


baresrc = \
bare/bare.flow.socket.c \
bare/bare.flow.watcher.c \
bare/bare.lock.semaphore.c \
bare/bare.lock.mutex.c \
bare/bare.memory.file.c \
bare/bare.memory.folder.c \
bare/bare.memory.portio.c \
bare/bare.multitask.coroutine.c \
bare/bare.multitask.thread.c \
bare/bare.multitask.process.c \
bare/bare.multitask.virtualize.c \
bare/bare.syscall.c \
bare/bare.system.power.c \
bare/bare.system.random.c \
bare/bare.system.signal.c \
bare/bare.system.shell.c \
bare/bare.system.time.c \
system.c
bareobj = $(patsubst %.c,%.o,$(baresrc))


efisrc = \
efi/efi.flow.socket.c \
efi/efi.flow.watcher.c \
efi/efi.memory.file.c \
efi/efi.memory.folder.c \
efi/efi.memory.portio.c \
efi/efi.system.power.c \
efi/efi.multitask.coroutine.c \
efi/efi.multitask.thread.c \
efi/efi.multitask.process.c \
efi/efi.multitask.virtualize.c \
efi/efi.system.random.c \
efi/efi.system.signal.c \
efi/efi.system.shell.c \
efi/efi.system.time.c \
system.c
efiobj = $(patsubst %.c,%.o,$(efisrc))


bsdsrc = \
bsd/bsd.memory.file.c \
bsd/bsd.memory.folder.c \
bsd/bsd.memory.mmio.c \
bsd/bsd.system.power.c \
bsd/bsd.multitask.coroutine.c \
bsd/bsd.multitask.thread.c \
bsd/bsd.multitask.process.c \
bsd/bsd.multitask.virtualize.c \
bsd/bsd.system.random.c \
bsd/bsd.system.signal.c \
bsd/bsd.system.time.c \
system.c
bsdobj = $(patsubst %.c,%.o,$(bsdsrc))


linuxsrc = \
linux/linux.flow.epoll.c \
linux/linux.flow.socket.c \
linux/linux.flow.ptmx.c \
linux/linux.flow.tty.c \
linux/linux.memory.file.c \
linux/linux.memory.folder.c \
linux/linux.memory.mmio.c \
linux/linux.system.power.c \
linux/linux.multitask.coroutine.c \
linux/linux.multitask.thread.c \
linux/linux.multitask.process.c \
linux/linux.multitask.virtualize.c \
linux/linux.system.random.c \
linux/linux.system.signal.c \
linux/linux.system.time.c \
system.c
linuxobj = $(patsubst %.c,%.o,$(linuxsrc))


macsrc = \
mac/mac.flow.kqueue.c \
mac/mac.flow.socket.c \
mac/mac.flow.ptmx.c \
mac/mac.flow.tty.c \
mac/mac.memory.file.c \
mac/mac.memory.folder.c \
mac/mac.memory.mmio.c \
mac/mac.system.power.c \
mac/mac.multitask.coroutine.c \
mac/mac.multitask.thread.c \
mac/mac.multitask.process.c \
mac/mac.multitask.virtualize.c \
mac/mac.system.random.c \
mac/mac.system.signal.c \
mac/mac.system.time.c \
system.c
macobj = $(patsubst %.c,%.o,$(macsrc))


winsrc = \
win/win.flow.iocp.c \
win/win.flow.socket.c \
win/win.flow.cmd.c \
win/win.flow.com.c \
win/win.memory.file.c \
win/win.memory.folder.c \
win/win.system.power.c \
win/win.multitask.coroutine.c \
win/win.multitask.thread.c \
win/win.multitask.process.c \
win/win.multitask.virtualize.c \
win/win.system.random.c \
win/win.system.signal.c \
win/win.system.time.c \
system.c
winobj = $(patsubst %.c,%.o,$(winsrc))

.PHONY:bare
bare: $(bareobj)

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
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<

clean:
	rm -f *.o *.a
