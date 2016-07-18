.PHONY:android
android:
	rm -f bsd.*.o linux.*.o mac.*.o win.*.o
	gcc -c -fPIC -O2 android/android.interface.bluetooth.c
	gcc -c -fPIC -O2 android/android.interface.i2c.c
	gcc -c -fPIC -O2 android/android.interface.gpio.c
	gcc -c -fPIC -O2 android/android.interface.pci.c
	gcc -c -fPIC -O2 android/android.interface.spi.c
	gcc -c -fPIC -O2 android/android.interface.uart.c
	gcc -c -fPIC -O2 android/android.interface.usb.c
	gcc -c -fPIC -O2 android/android.interface.wifi.c
	gcc -c -fPIC -O2 android/android.memory.file.c
	gcc -c -fPIC -O2 android/android.memory.mmio.c
	gcc -c -fPIC -O2 android/android.net.epoll.c
	gcc -c -fPIC -O2 android/android.net.socket.c
	gcc -c -fPIC -O2 android/android.special.folder.c
	gcc -c -fPIC -O2 android/android.special.random.c
	gcc -c -fPIC -O2 system.c
.PHONY:bsd
bsd:
	rm -f android.*o linux.*.o mac.*.o win.*.o
	gcc -c -fPIC -O2 bsd/bsd.memory.file.c
	gcc -c -fPIC -O2 bsd/bsd.memory.mmio.c
	gcc -c -fPIC -O2 bsd/bsd.special.folder.c
	gcc -c -fPIC -O2 bsd/bsd.special.random.c
	gcc -c -fPIC -O2 system.c
.PHONY:linux
linux:
	rm -f android.*.o bsd.*.o mac.*.o win.*.o
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
	gcc -c -fPIC -O2 linux/linux.net.epoll.c
	gcc -c -fPIC -O2 linux/linux.net.socket.c
	gcc -c -fPIC -O2 linux/linux.special.folder.c
	gcc -c -fPIC -O2 linux/linux.special.random.c
	gcc -c -fPIC -O2 system.c
.PHONY:mac
mac:
	rm -f android.*.o bsd.*.o linux.*.o win.*.o
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
	clang -c -O2 mac/mac.net.kqueue.c
	clang -c -O2 mac/mac.net.socket.c
	clang -c -O2 mac/mac.special.folder.c
	clang -c -O2 mac/mac.special.random.c
	clang -c -O2 system.c
.PHONY:win
win:
	rm -f android.*.o bsd.*.o linux.*.o mac.*.o
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
	gcc -c -O2 win/win.memory.process.c
	gcc -c -O2 win/win.net.iocp.c
	gcc -c -O2 win/win.net.socket.c
	gcc -c -O2 win/win.special.folder.c
	gcc -c -O2 win/win.special.random.c
	gcc -c -O2 system.c
clean:
	rm -f *.o *.a
