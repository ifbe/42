.PHONY:bsd
bsd:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 bsd/bsd.event.c
	gcc -c -fPIC -O2 bsd/bsd.file.c
	gcc -c -fPIC -O2 bsd/bsd.memory.c
	gcc -c -fPIC -O2 bsd/bsd.random.c
	gcc -c -fPIC -O2 system.c
.PHONY:linux
linux:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 linux/linux.event.fake.c
	gcc -c -fPIC -O2 linux/linux.event.net.c
	gcc -c -fPIC -O2 linux/linux.interface.bluetooth.c
	gcc -c -fPIC -O2 linux/linux.interface.i2c.c
	gcc -c -fPIC -O2 linux/linux.interface.pci.c
	gcc -c -fPIC -O2 linux/linux.interface.spi.c
	gcc -c -fPIC -O2 linux/linux.interface.usb.c
	gcc -c -fPIC -O2 linux/linux.interface.wifi.c
	gcc -c -fPIC -O2 linux/linux.memory.file.c
	gcc -c -fPIC -O2 linux/linux.memory.mmio.c
	gcc -c -fPIC -O2 linux/linux.special.folder.c
	gcc -c -fPIC -O2 linux/linux.special.random.c
	gcc -c -fPIC -O2 linux/linux.special.socket.c
	gcc -c -fPIC -O2 system.c
.PHONY:mac
mac:
	rm -f linux.*.o win.*.o
	clang -c -O2 mac/mac.event.c
	clang -c -O2 mac/mac.file.c
	clang -c -O2 mac/mac.folder.c
	clang -c -O2 mac/mac.memory.c
	clang -c -O2 mac/mac.random.c
	clang -c -O2 mac/mac.socket.c
	clang -c -O2 system.c
.PHONY:win
win:
	rm -f linux.*.o mac.*.o
	gcc -c -O2 win/win.event.net.c
	gcc -c -O2 win/win.memory.file.c
	gcc -c -O2 win/win.memory.virtual.c
	gcc -c -O2 win/win.memory.process.c
	gcc -c -O2 win/win.special.folder.c
	gcc -c -O2 win/win.special.random.c
	gcc -c -O2 win/win.special.socket.c
	gcc -c -O2 system.c
clean:
	rm -f *.o *.a
