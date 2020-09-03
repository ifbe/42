CC:=gcc
CF:=-c -O2 -fPIC -I.

.PHONY:bare
bare:
	rm -f *.o
	$(CC) $(CF) bare/bare.flow.socket.c
	$(CC) $(CF) bare/bare.flow.watcher.c
	$(CC) $(CF) bare/bare.memory.file.c
	$(CC) $(CF) bare/bare.memory.folder.c
	$(CC) $(CF) bare/bare.memory.portio.c
	$(CC) $(CF) bare/bare.system.power.c
	$(CC) $(CF) bare/bare.system.process.c
	$(CC) $(CF) bare/bare.system.random.c
	$(CC) $(CF) bare/bare.system.signal.c
	$(CC) $(CF) bare/bare.system.shell.c
	$(CC) $(CF) bare/bare.system.time.c
	$(CC) $(CF) system.c

.PHONY:efi
efiapp:
	rm -f *.o
	$(CC) $(CF) efi/efi.flow.socket.c
	$(CC) $(CF) efi/efi.flow.watcher.c
	$(CC) $(CF) efi/efi.memory.file.c
	$(CC) $(CF) efi/efi.memory.folder.c
	$(CC) $(CF) efi/efi.memory.portio.c
	$(CC) $(CF) efi/efi.system.power.c
	$(CC) $(CF) efi/efi.system.process.c
	$(CC) $(CF) efi/efi.system.random.c
	$(CC) $(CF) efi/efi.system.signal.c
	$(CC) $(CF) efi/efi.system.shell.c
	$(CC) $(CF) efi/efi.system.time.c
	$(CC) $(CF) system.c

.PHONY:bsd
bsd:
	rm -f *.o
	$(CC) $(CF) bsd/bsd.memory.file.c
	$(CC) $(CF) bsd/bsd.memory.folder.c
	$(CC) $(CF) bsd/bsd.memory.mmio.c
	$(CC) $(CF) bsd/bsd.system.power.c
	$(CC) $(CF) bsd/bsd.system.process.c
	$(CC) $(CF) bsd/bsd.system.random.c
	$(CC) $(CF) bsd/bsd.system.signal.c
	$(CC) $(CF) bsd/bsd.system.time.c
	$(CC) $(CF) system.c

.PHONY:linux
linux:
	rm -f *.o
	$(CC) $(CF) linux/linux.flow.epoll.c
	$(CC) $(CF) linux/linux.flow.socket.c
	$(CC) $(CF) linux/linux.flow.ptmx.c
	$(CC) $(CF) linux/linux.flow.tty.c
	$(CC) $(CF) linux/linux.memory.file.c
	$(CC) $(CF) linux/linux.memory.folder.c
	$(CC) $(CF) linux/linux.memory.mmio.c
	$(CC) $(CF) linux/linux.system.power.c
	$(CC) $(CF) linux/linux.system.process.c
	$(CC) $(CF) linux/linux.system.random.c
	$(CC) $(CF) linux/linux.system.signal.c
	$(CC) $(CF) linux/linux.system.time.c
	$(CC) $(CF) system.c

.PHONY:mac
mac:
	rm -f *.o
	$(CC) $(CF) mac/mac.flow.kqueue.c
	$(CC) $(CF) mac/mac.flow.socket.c
	$(CC) $(CF) mac/mac.flow.ptmx.c
	$(CC) $(CF) mac/mac.flow.tty.c
	$(CC) $(CF) mac/mac.memory.file.c
	$(CC) $(CF) mac/mac.memory.folder.c
	$(CC) $(CF) mac/mac.memory.mmio.c
	$(CC) $(CF) mac/mac.system.power.c
	$(CC) $(CF) mac/mac.system.process.c
	$(CC) $(CF) mac/mac.system.random.c
	$(CC) $(CF) mac/mac.system.signal.c
	$(CC) $(CF) mac/mac.system.time.c
	$(CC) $(CF) system.c

.PHONY:win
win:
	rm -f *.o
	$(CC) $(CF) win/win.flow.iocp.c
	$(CC) $(CF) win/win.flow.socket.c
	$(CC) $(CF) win/win.flow.cmd.c
	$(CC) $(CF) win/win.flow.com.c
	$(CC) $(CF) win/win.memory.file.c
	$(CC) $(CF) win/win.memory.folder.c
	$(CC) $(CF) win/win.system.power.c
	$(CC) $(CF) win/win.system.process.c
	$(CC) $(CF) win/win.system.random.c
	$(CC) $(CF) win/win.system.signal.c
	$(CC) $(CF) win/win.system.time.c
	$(CC) $(CF) system.c
clean:
	rm -f *.o *.a
