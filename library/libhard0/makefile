compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:efi
efi:
	$(compiler) $(cflag) efi/efi.wire.bt.c
	$(compiler) $(cflag) efi/efi.wire.i2c.c
	$(compiler) $(cflag) efi/efi.wire.spi.c
	$(compiler) $(cflag) efi/efi.wire.usb.c
	$(compiler) $(cflag) efi/efi.wire.wifi.c

.PHONY:mips
mips:
	$(compiler) $(cflag) bare/cpu/mips/mips.fp.c
	$(compiler) $(cflag) device.c

.PHONY:mips64
mips64:
	$(compiler) $(cflag) bare/cpu/mips64/mips64.fp.c
	$(compiler) $(cflag) device.c




#---------------------arm32-------------------------
.PHONY:arm
arm:
	$(compiler) $(cflag) bare/cpu/arm/arm.fp.c
	$(compiler) $(cflag) device.c

linuxarm:arm
	$(compiler) $(cflag) linux/linux.wire.bt.c
	$(compiler) $(cflag) linux/linux.wire.i2c.c
	$(compiler) $(cflag) linux/linux.wire.gpio.c
	$(compiler) $(cflag) linux/linux.wire.pci.c
	$(compiler) $(cflag) linux/linux.wire.spi.c
	$(compiler) $(cflag) linux/linux.wire.usb.c
	$(compiler) $(cflag) linux/linux.wire.wifi.c

linuxpi32:arm
	$(compiler) $(cflag) rpi/rpi.wiringpi.c
	$(compiler) $(cflag) linux/linux.wire.bt.c
	$(compiler) $(cflag) linux/linux.wire.i2c.c
	$(compiler) $(cflag) linux/linux.wire.pci.c
	$(compiler) $(cflag) linux/linux.wire.spi.c
	$(compiler) $(cflag) linux/linux.wire.usb.c
	$(compiler) $(cflag) linux/linux.wire.wifi.c




#---------------------arm64-------------------------
.PHONY:arm64
arm64:
	$(compiler) $(cflag) bare/cpu/arm64/arm64.fp.c
	$(compiler) $(cflag) device.c

linuxarm64:arm64
	$(compiler) $(cflag) linux/linux.wire.bt.c
	$(compiler) $(cflag) linux/linux.wire.gpio.c
	$(compiler) $(cflag) linux/linux.wire.i2c.c
	$(compiler) $(cflag) linux/linux.wire.pci.c
	$(compiler) $(cflag) linux/linux.wire.spi.c
	$(compiler) $(cflag) linux/linux.wire.usb.c
	$(compiler) $(cflag) linux/linux.wire.wifi.c

linuxpi64:arm64
	$(compiler) $(cflag) linux/linux.wire.bt.c
	$(compiler) $(cflag) linux/linux.wire.gpio.c
	$(compiler) $(cflag) linux/linux.wire.i2c.c
	$(compiler) $(cflag) linux/linux.wire.pci.c
	$(compiler) $(cflag) linux/linux.wire.spi.c
	$(compiler) $(cflag) linux/linux.wire.usb.c
	$(compiler) $(cflag) linux/linux.wire.wifi.c




#---------------------x86-------------------------
.PHONY:x86
x86:
	$(compiler) $(cflag) bare/cpu/x86/x86.endian.c
	$(compiler) $(cflag) bare/cpu/x86/x86.fp.c
	$(compiler) $(cflag) bare/cpu/x86/x86.port.c
	$(compiler) $(cflag) device.c

.PHONY:barex86
barex86:x86
	$(compiler) $(cflag) bare/bare.c

.PHONY:linuxx86
linuxx86:x86

.PHONY:macx86
macx86:x86

.PHONY:winx86
winx86:x86
	$(compiler) $(cflag) bare/cpu/x86/x86.endian.c
	$(compiler) $(cflag) bare/cpu/x86/x86.fp.c
	$(compiler) $(cflag) bare/cpu/x86/x86.port.c
	$(compiler) $(cflag) device.c

.PHONY:x64
x64:
	$(compiler) $(cflag) bare/cpu/x64/x64.endian.c
	$(compiler) $(cflag) bare/cpu/x64/x64.fp.c
	$(compiler) $(cflag) bare/cpu/x64/x64.port.c
	$(compiler) $(cflag) device.c




#---------------------x64-------------------------
.PHONY:barex64
barex64:x64
	$(compiler) $(cflag) bare/cpu/x64/x64.asm.c
	$(compiler) $(cflag) bare/cpu/x64/x64.page.c
	$(compiler) $(cflag) bare/cpu/x64/x64.gdt.c
	#$(compiler) $(cflag) -mgeneral-regs-only bare/cpu/x64/x64.idt.c
	$(compiler) $(cflag) bare/cpu/x64/x64.idt.c
	$(compiler) $(cflag) bare/irq/8259.c
	#$(compiler) $(cflag) bare/irq/apic.c
	$(compiler) $(cflag) bare/bus/input/8042.c
	$(compiler) $(cflag) bare/bus/pci/pci.c
	$(compiler) $(cflag) bare/timer/825x.c
	$(compiler) $(cflag) bare/timer/rtc.c
	$(compiler) $(cflag) bare/wire/bare.wire.bt.c
	$(compiler) $(cflag) bare/wire/bare.wire.gpio.c
	$(compiler) $(cflag) bare/wire/bare.wire.i2c.c
	$(compiler) $(cflag) bare/wire/bare.wire.spi.c
	$(compiler) $(cflag) bare/wire/bare.wire.uart.c
	$(compiler) $(cflag) bare/wire/bare.wire.usb.c
	$(compiler) $(cflag) bare/wire/bare.wire.wifi.c

.PHONY:linuxx64
linuxx64:x64
	$(compiler) $(cflag) linux/linux.wire.bt.c
	$(compiler) $(cflag) linux/linux.wire.i2c.c
	$(compiler) $(cflag) linux/linux.wire.gpio.c
	$(compiler) $(cflag) linux/linux.wire.pci.c
	$(compiler) $(cflag) linux/linux.wire.spi.c
	$(compiler) $(cflag) linux/linux.wire.usb.c
	$(compiler) $(cflag) linux/linux.wire.wifi.c

.PHONY:macx64
macx64:x64
	$(compiler) $(cflag) mac/mac.wire.bt.c
	$(compiler) $(cflag) mac/mac.wire.i2c.c
	$(compiler) $(cflag) mac/mac.wire.gpio.c
	$(compiler) $(cflag) mac/mac.wire.pci.c
	$(compiler) $(cflag) mac/mac.wire.spi.c
	$(compiler) $(cflag) mac/mac.wire.usb.c
	$(compiler) $(cflag) mac/mac.wire.wifi.c

.PHONY:winx64
winx64:x64
	$(compiler) $(cflag) win/win.wire.bt.c
	$(compiler) $(cflag) win/win.wire.i2c.c
	$(compiler) $(cflag) win/win.wire.gpio.c
	$(compiler) $(cflag) win/win.wire.pci.c
	$(compiler) $(cflag) win/win.wire.spi.c
	$(compiler) $(cflag) win/win.wire.usb.c
	$(compiler) $(cflag) win/win.wire.wifi.c

clean:
	rm -f *.o *.a
