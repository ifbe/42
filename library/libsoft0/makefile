compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.

.PHONY:bare
bare:
	rm -f bsd.*.o linux.*.o mac.*.o win.*.o
	$(compiler) $(cflag) bare/bare.flow.net.socket.c
	$(compiler) $(cflag) bare/bare.flow.sound.c
	$(compiler) $(cflag) bare/bare.flow.vision.c
	$(compiler) $(cflag) bare/bare.memory.file.c
	$(compiler) $(cflag) bare/bare.memory.folder.c
	$(compiler) $(cflag) bare/bare.memory.managed.c
	$(compiler) $(cflag) bare/bare.memory.portio.c
	$(compiler) $(cflag) bare/bare.system.power.c
	$(compiler) $(cflag) bare/bare.system.process.c
	$(compiler) $(cflag) bare/bare.system.random.c
	$(compiler) $(cflag) bare/bare.system.signal.c
	$(compiler) $(cflag) bare/bare.system.thread.c
	$(compiler) $(cflag) bare/bare.system.time.c
	$(compiler) $(cflag) bare/bare.system.watcher.c
	$(compiler) $(cflag) bare/bare.wire.bt.c
	$(compiler) $(cflag) bare/bare.wire.i2c.c
	$(compiler) $(cflag) bare/bare.wire.spi.c
	$(compiler) $(cflag) bare/bare.wire.uart.c
	$(compiler) $(cflag) bare/bare.wire.usb.c
	$(compiler) $(cflag) bare/bare.wire.wifi.c
	$(compiler) $(cflag) system.c

.PHONY:bsd
bsd:
	rm -f bare.*.o linux.*.o mac.*.o win.*.o
	$(compiler) $(cflag) bsd/bsd.memory.file.c
	$(compiler) $(cflag) bsd/bsd.memory.folder.c
	$(compiler) $(cflag) bsd/bsd.memory.managed.c
	$(compiler) $(cflag) bsd/bsd.memory.mmio.c
	$(compiler) $(cflag) bsd/bsd.system.power.c
	$(compiler) $(cflag) bsd/bsd.system.process.c
	$(compiler) $(cflag) bsd/bsd.system.random.c
	$(compiler) $(cflag) bsd/bsd.system.signal.c
	$(compiler) $(cflag) bsd/bsd.system.thread.c
	$(compiler) $(cflag) bsd/bsd.system.time.c
	$(compiler) $(cflag) system.c

.PHONY:linux
linux:
	rm -f bare.*.o bsd.*.o mac.*.o win.*.o
	$(compiler) $(cflag) linux/linux.flow.socket.c
	$(compiler) $(cflag) linux/linux.flow.vision.v4l2.c
	$(compiler) $(cflag) linux/linux.flow.vision.other.c
	$(compiler) $(cflag) linux/linux.flow.sound.pulseaudio.c
	$(compiler) $(cflag) linux/linux.memory.file.c
	$(compiler) $(cflag) linux/linux.memory.folder.c
	$(compiler) $(cflag) linux/linux.memory.managed.c
	$(compiler) $(cflag) linux/linux.memory.mmio.c
	$(compiler) $(cflag) linux/linux.system.power.c
	$(compiler) $(cflag) linux/linux.system.process.c
	$(compiler) $(cflag) linux/linux.system.random.c
	$(compiler) $(cflag) linux/linux.system.shell.c
	$(compiler) $(cflag) linux/linux.system.signal.c
	$(compiler) $(cflag) linux/linux.system.thread.c
	$(compiler) $(cflag) linux/linux.system.time.c
	$(compiler) $(cflag) linux/linux.system.watcher.c
	$(compiler) $(cflag) linux/linux.wire.bt.c
	$(compiler) $(cflag) linux/linux.wire.i2c.c
	$(compiler) $(cflag) linux/linux.wire.gpio.c
	$(compiler) $(cflag) linux/linux.wire.pci.c
	$(compiler) $(cflag) linux/linux.wire.spi.c
	$(compiler) $(cflag) linux/linux.wire.uart.c
	$(compiler) $(cflag) linux/linux.wire.usb.c
	$(compiler) $(cflag) linux/linux.wire.wifi.c
	$(compiler) $(cflag) system.c

.PHONY:mac
mac:
	rm -f bare.*.o bsd.*.o linux.*.o win.*.o
	$(compiler) $(cflag) mac/mac.flow.net.socket.c
	$(compiler) $(cflag) mac/mac.flow.sound.xxx.c
	$(compiler) $(cflag) mac/mac.flow.vision.xxx.c
	$(compiler) $(cflag) mac/mac.memory.file.c
	$(compiler) $(cflag) mac/mac.memory.folder.c
	$(compiler) $(cflag) mac/mac.memory.managed.c
	$(compiler) $(cflag) mac/mac.memory.mmio.c
	$(compiler) $(cflag) mac/mac.system.power.c
	$(compiler) $(cflag) mac/mac.system.process.c
	$(compiler) $(cflag) mac/mac.system.random.c
	$(compiler) $(cflag) mac/mac.system.signal.c
	$(compiler) $(cflag) mac/mac.system.thread.c
	$(compiler) $(cflag) mac/mac.system.time.c
	$(compiler) $(cflag) mac/mac.wire.bt.c
	$(compiler) $(cflag) mac/mac.wire.i2c.c
	$(compiler) $(cflag) mac/mac.wire.gpio.c
	$(compiler) $(cflag) mac/mac.wire.pci.c
	$(compiler) $(cflag) mac/mac.wire.spi.c
	$(compiler) $(cflag) mac/mac.wire.uart.c
	$(compiler) $(cflag) mac/mac.wire.usb.c
	$(compiler) $(cflag) mac/mac.wire.wifi.c
	$(compiler) $(cflag) system.c

.PHONY:win
win:
	rm -f linux.*.o mac.*.o
	$(compiler) $(cflag) win/win.flow.socket.c
	$(compiler) $(cflag) win/win.flow.sound.waveinout.c
	g++ $(cflag) -fno-rtti win/win.flow.vision.directshow.cpp
	$(compiler) $(cflag) win/win.memory.file.c
	$(compiler) $(cflag) win/win.memory.folder.c
	$(compiler) $(cflag) win/win.memory.managed.c
	$(compiler) $(cflag) win/win.system.power.c
	$(compiler) $(cflag) win/win.system.process.c
	$(compiler) $(cflag) win/win.system.random.c
	$(compiler) $(cflag) win/win.system.shell.c
	$(compiler) $(cflag) win/win.system.signal.c
	$(compiler) $(cflag) win/win.system.thread.c
	$(compiler) $(cflag) win/win.system.time.c
	$(compiler) $(cflag) win/win.system.watcher.c
	$(compiler) $(cflag) win/win.wire.bt.c
	$(compiler) $(cflag) win/win.wire.i2c.c
	$(compiler) $(cflag) win/win.wire.gpio.c
	$(compiler) $(cflag) win/win.wire.pci.c
	$(compiler) $(cflag) win/win.wire.spi.c
	$(compiler) $(cflag) win/win.wire.uart.c
	$(compiler) $(cflag) win/win.wire.usb.c
	$(compiler) $(cflag) win/win.wire.wifi.c
	$(compiler) $(cflag) system.c
clean:
	rm -f *.o *.a
