compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) bare/bare.flow.socket.c
	$(compiler) $(cflag) bare/bare.flow.watcher.c
	$(compiler) $(cflag) bare/bare.memory.file.c
	$(compiler) $(cflag) bare/bare.memory.folder.c
	$(compiler) $(cflag) bare/bare.memory.portio.c
	$(compiler) $(cflag) bare/bare.system.power.c
	$(compiler) $(cflag) bare/bare.system.process.c
	$(compiler) $(cflag) bare/bare.system.random.c
	$(compiler) $(cflag) bare/bare.system.signal.c
	$(compiler) $(cflag) bare/bare.system.shell.c
	$(compiler) $(cflag) bare/bare.system.time.c
	$(compiler) $(cflag) system.c

.PHONY:efi
efi:
	rm -f *.o
	$(compiler) $(cflag) efi/efi.flow.socket.c
	$(compiler) $(cflag) efi/efi.flow.watcher.c
	$(compiler) $(cflag) efi/efi.memory.file.c
	$(compiler) $(cflag) efi/efi.memory.folder.c
	$(compiler) $(cflag) efi/efi.memory.portio.c
	$(compiler) $(cflag) efi/efi.system.power.c
	$(compiler) $(cflag) efi/efi.system.process.c
	$(compiler) $(cflag) efi/efi.system.random.c
	$(compiler) $(cflag) efi/efi.system.signal.c
	$(compiler) $(cflag) efi/efi.system.shell.c
	$(compiler) $(cflag) efi/efi.system.time.c
	$(compiler) $(cflag) system.c

.PHONY:bsd
bsd:
	rm -f *.o
	$(compiler) $(cflag) bsd/bsd.memory.file.c
	$(compiler) $(cflag) bsd/bsd.memory.folder.c
	$(compiler) $(cflag) bsd/bsd.memory.mmio.c
	$(compiler) $(cflag) bsd/bsd.system.power.c
	$(compiler) $(cflag) bsd/bsd.system.process.c
	$(compiler) $(cflag) bsd/bsd.system.random.c
	$(compiler) $(cflag) bsd/bsd.system.signal.c
	$(compiler) $(cflag) bsd/bsd.system.time.c
	$(compiler) $(cflag) system.c

.PHONY:linux
linux:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.flow.epoll.c
	$(compiler) $(cflag) linux/linux.flow.socket.c
	$(compiler) $(cflag) linux/linux.flow.ptmx.c
	$(compiler) $(cflag) linux/linux.flow.tty.c
	$(compiler) $(cflag) linux/linux.memory.file.c
	$(compiler) $(cflag) linux/linux.memory.folder.c
	$(compiler) $(cflag) linux/linux.memory.mmio.c
	$(compiler) $(cflag) linux/linux.system.power.c
	$(compiler) $(cflag) linux/linux.system.process.c
	$(compiler) $(cflag) linux/linux.system.random.c
	$(compiler) $(cflag) linux/linux.system.signal.c
	$(compiler) $(cflag) linux/linux.system.time.c
	$(compiler) $(cflag) system.c

.PHONY:mac
mac:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.flow.kqueue.c
	$(compiler) $(cflag) mac/mac.flow.socket.c
	$(compiler) $(cflag) mac/mac.flow.ptmx.c
	$(compiler) $(cflag) mac/mac.flow.tty.c
	$(compiler) $(cflag) mac/mac.memory.file.c
	$(compiler) $(cflag) mac/mac.memory.folder.c
	$(compiler) $(cflag) mac/mac.memory.mmio.c
	$(compiler) $(cflag) mac/mac.system.power.c
	$(compiler) $(cflag) mac/mac.system.process.c
	$(compiler) $(cflag) mac/mac.system.random.c
	$(compiler) $(cflag) mac/mac.system.signal.c
	$(compiler) $(cflag) mac/mac.system.time.c
	$(compiler) $(cflag) system.c

.PHONY:win
win:
	rm -f *.o
	$(compiler) $(cflag) win/win.flow.iocp.c
	$(compiler) $(cflag) win/win.flow.socket.c
	$(compiler) $(cflag) win/win.flow.cmd.c
	$(compiler) $(cflag) win/win.flow.com.c
	$(compiler) $(cflag) win/win.memory.file.c
	$(compiler) $(cflag) win/win.memory.folder.c
	$(compiler) $(cflag) win/win.system.power.c
	$(compiler) $(cflag) win/win.system.process.c
	$(compiler) $(cflag) win/win.system.random.c
	$(compiler) $(cflag) win/win.system.signal.c
	$(compiler) $(cflag) win/win.system.time.c
	$(compiler) $(cflag) system.c
clean:
	rm -f *.o *.a
