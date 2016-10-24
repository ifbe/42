.PHONY:bsd
bsd:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 bsd/bsd.memory.file.c
	gcc -c -fPIC -O2 bsd/bsd.memory.mmio.c
	gcc -c -fPIC -O2 bsd/bsd.special.folder.c
	gcc -c -fPIC -O2 bsd/bsd.special.random.c
	gcc -c -fPIC -O2 system.c
.PHONY:linux
linux:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 linux/linux.interface.bluetooth.c
	gcc -c -fPIC -O2 linux/linux.interface.i2c.c
	gcc -c -fPIC -O2 linux/linux.interface.gpio.c
	gcc -c -fPIC -O2 linux/linux.interface.pci.c
	gcc -c -fPIC -O2 linux/linux.interface.spi.c
	gcc -c -fPIC -O2 linux/linux.interface.uart.c
	gcc -c -fPIC -O2 linux/linux.interface.usb.c
	gcc -c -fPIC -O2 linux/linux.interface.wifi.c
	gcc -c -fPIC -O2 linux/linux.memory.file.c
	gcc -c -fPIC -O2 linux/linux.memory.mmio.c
	gcc -c -fPIC -O2 linux/linux.net.client.c
	gcc -c -fPIC -O2 linux/linux.net.server.c
	gcc -c -fPIC -O2 linux/linux.special.folder.c
	gcc -c -fPIC -O2 linux/linux.special.process.c
	gcc -c -fPIC -O2 linux/linux.special.random.c
	gcc -c -fPIC -O2 linux/linux.special.signal.c
	gcc -c -fPIC -O2 linux/linux.special.time.c
	gcc -c -fPIC -O2 system.c
.PHONY:mac
mac:
	rm -f linux.*.o win.*.o
	clang -c -O2 mac/mac.interface.bluetooth.c
	clang -c -O2 mac/mac.interface.i2c.c
	clang -c -O2 mac/mac.interface.gpio.c
	clang -c -O2 mac/mac.interface.pci.c
	clang -c -O2 mac/mac.interface.spi.c
	clang -c -O2 mac/mac.interface.uart.c
	clang -c -O2 mac/mac.interface.usb.c
	clang -c -O2 mac/mac.interface.wifi.c
	clang -c -O2 mac/mac.memory.file.c
	clang -c -O2 mac/mac.memory.mmio.c
	clang -c -O2 mac/mac.net.client.c
	clang -c -O2 mac/mac.net.server.c
	clang -c -O2 mac/mac.special.folder.c
	clang -c -O2 mac/mac.special.random.c
	clang -c -O2 mac/mac.special.signal.c
	clang -c -O2 mac/mac.special.time.c
	clang -c -O2 system.c
.PHONY:win
win:
	rm -f linux.*.o mac.*.o
	gcc -c -O2 win/win.interface.bluetooth.c
	gcc -c -O2 win/win.interface.i2c.c
	gcc -c -O2 win/win.interface.gpio.c
	gcc -c -O2 win/win.interface.pci.c
	gcc -c -O2 win/win.interface.spi.c
	gcc -c -O2 win/win.interface.uart.c
	gcc -c -O2 win/win.interface.usb.c
	gcc -c -O2 win/win.interface.wifi.c
	gcc -c -O2 win/win.memory.file.c
	gcc -c -O2 win/win.memory.virtual.c
	gcc -c -O2 win/win.net.client.c
	gcc -c -O2 win/win.net.server.c
	gcc -c -O2 win/win.special.folder.c
	gcc -c -O2 win/win.special.process.c
	gcc -c -O2 win/win.special.random.c
	gcc -c -O2 win/win.special.signal.c
	gcc -c -O2 win/win.special.time.c
	gcc -c -O2 system.c
clean:
	rm -f *.o *.a
