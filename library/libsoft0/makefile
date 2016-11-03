.PHONY:bsd
bsd:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 system.c
	gcc -c -fPIC -O2 bsd/bsd.memory.file.c
	gcc -c -fPIC -O2 bsd/bsd.memory.mmio.c
	gcc -c -fPIC -O2 bsd/bsd.system.folder.c
	gcc -c -fPIC -O2 bsd/bsd.system.random.c
.PHONY:linux
linux:
	rm -f mac.*.o win.*.o
	gcc -c -fPIC -O2 system.c
	gcc -c -fPIC -O2 linux/linux.memory.file.c
	gcc -c -fPIC -O2 linux/linux.memory.mmio.c
	gcc -c -fPIC -O2 linux/linux.net.client.c
	gcc -c -fPIC -O2 linux/linux.net.server.c
	gcc -c -fPIC -O2 linux/linux.system.folder.c
	gcc -c -fPIC -O2 linux/linux.system.process.c
	gcc -c -fPIC -O2 linux/linux.system.random.c
	gcc -c -fPIC -O2 linux/linux.system.signal.c
	gcc -c -fPIC -O2 linux/linux.system.time.c
	gcc -c -fPIC -O2 linux/linux.wire.bt.c
	gcc -c -fPIC -O2 linux/linux.wire.i2c.c
	gcc -c -fPIC -O2 linux/linux.wire.gpio.c
	gcc -c -fPIC -O2 linux/linux.wire.pci.c
	gcc -c -fPIC -O2 linux/linux.wire.spi.c
	gcc -c -fPIC -O2 linux/linux.wire.uart.c
	gcc -c -fPIC -O2 linux/linux.wire.usb.c
	gcc -c -fPIC -O2 linux/linux.wire.wifi.c
.PHONY:mac
mac:
	rm -f linux.*.o win.*.o
	clang -c -O2 system.c
	clang -c -O2 mac/mac.memory.file.c
	clang -c -O2 mac/mac.memory.mmio.c
	clang -c -O2 mac/mac.net.client.c
	clang -c -O2 mac/mac.net.server.c
	clang -c -O2 mac/mac.system.folder.c
	clang -c -O2 mac/mac.system.random.c
	clang -c -O2 mac/mac.system.signal.c
	clang -c -O2 mac/mac.system.time.c
	clang -c -O2 mac/mac.wire.bt.c
	clang -c -O2 mac/mac.wire.i2c.c
	clang -c -O2 mac/mac.wire.gpio.c
	clang -c -O2 mac/mac.wire.pci.c
	clang -c -O2 mac/mac.wire.spi.c
	clang -c -O2 mac/mac.wire.uart.c
	clang -c -O2 mac/mac.wire.usb.c
	clang -c -O2 mac/mac.wire.wifi.c
.PHONY:win
win:
	rm -f linux.*.o mac.*.o
	gcc -c -O2 system.c
	gcc -c -O2 win/win.memory.file.c
	gcc -c -O2 win/win.memory.virtual.c
	gcc -c -O2 win/win.net.client.c
	gcc -c -O2 win/win.net.server.c
	gcc -c -O2 win/win.system.folder.c
	gcc -c -O2 win/win.system.process.c
	gcc -c -O2 win/win.system.random.c
	gcc -c -O2 win/win.system.signal.c
	gcc -c -O2 win/win.system.time.c
	gcc -c -O2 win/win.wire.bt.c
	gcc -c -O2 win/win.wire.i2c.c
	gcc -c -O2 win/win.wire.gpio.c
	gcc -c -O2 win/win.wire.pci.c
	gcc -c -O2 win/win.wire.spi.c
	gcc -c -O2 win/win.wire.uart.c
	gcc -c -O2 win/win.wire.usb.c
	gcc -c -O2 win/win.wire.wifi.c
clean:
	rm -f *.o *.a
