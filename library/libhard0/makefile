compiler ?= gcc
cflag ?= -c -O2 -fPIC




#---------------------arm32-------------------------
.PHONY:arm
arm:
	$(compiler) $(cflag) toarm/cpu32/arm.fp.c
	$(compiler) $(cflag) device.c

linuxarm:arm
	$(compiler) $(cflag) inlinux/hardware.c
	$(compiler) $(cflag) inlinux/linux.wire.bt.c
	$(compiler) $(cflag) inlinux/linux.wire.i2c.c
	$(compiler) $(cflag) inlinux/linux.wire.gpio.c
	$(compiler) $(cflag) inlinux/linux.wire.pci.c
	$(compiler) $(cflag) inlinux/linux.wire.spi.c
	$(compiler) $(cflag) inlinux/linux.wire.usb.c
	$(compiler) $(cflag) inlinux/linux.wire.wifi.c

linuxpi32:arm
	$(compiler) $(cflag) torpi/hardware.c
	$(compiler) $(cflag) torpi/rpi.wiringpi.c
	$(compiler) $(cflag) inlinux/linux.wire.bt.c
	$(compiler) $(cflag) inlinux/linux.wire.i2c.c
	$(compiler) $(cflag) inlinux/linux.wire.pci.c
	$(compiler) $(cflag) inlinux/linux.wire.spi.c
	$(compiler) $(cflag) inlinux/linux.wire.usb.c
	$(compiler) $(cflag) inlinux/linux.wire.wifi.c




#---------------------arm64-------------------------
.PHONY:arm64
arm64:
	$(compiler) $(cflag) toarm/cpu64/arm64.fp.c
	$(compiler) $(cflag) device.c

linuxarm64:arm64
	$(compiler) $(cflag) inlinux/hardware.c
	$(compiler) $(cflag) inlinux/linux.wire.bt.c
	$(compiler) $(cflag) inlinux/linux.wire.gpio.c
	$(compiler) $(cflag) inlinux/linux.wire.i2c.c
	$(compiler) $(cflag) inlinux/linux.wire.pci.c
	$(compiler) $(cflag) inlinux/linux.wire.spi.c
	$(compiler) $(cflag) inlinux/linux.wire.usb.c
	$(compiler) $(cflag) inlinux/linux.wire.wifi.c

linuxpi64:arm64
	$(compiler) $(cflag) inlinux/hardware.c
	$(compiler) $(cflag) inlinux/linux.wire.bt.c
	$(compiler) $(cflag) inlinux/linux.wire.gpio.c
	$(compiler) $(cflag) inlinux/linux.wire.i2c.c
	$(compiler) $(cflag) inlinux/linux.wire.pci.c
	$(compiler) $(cflag) inlinux/linux.wire.spi.c
	$(compiler) $(cflag) inlinux/linux.wire.usb.c
	$(compiler) $(cflag) inlinux/linux.wire.wifi.c




#---------------------mips32-------------------------
.PHONY:mips
mips:
	$(compiler) $(cflag) tomips/cpu32/mips.fp.c
	$(compiler) $(cflag) device.c




#---------------------mips64-------------------------
.PHONY:mips64
mips64:
	$(compiler) $(cflag) tomips/cpu64/mips64.fp.c
	$(compiler) $(cflag) device.c




#---------------------x86-------------------------
.PHONY:x86
x86:
	$(compiler) $(cflag) tox86/cpu32/x86.endian.c
	$(compiler) $(cflag) tox86/cpu32/x86.fp.c
	$(compiler) $(cflag) tox86/cpu32/x86.port.c
	$(compiler) $(cflag) device.c

.PHONY:linuxx86
linuxx86:x86

.PHONY:macx86
macx86:x86

.PHONY:winx86
winx86:x86
	$(compiler) $(cflag) tox86/cpu32/x86.endian.c
	$(compiler) $(cflag) tox86/cpu32/x86.fp.c
	$(compiler) $(cflag) tox86/cpu32/x86.port.c
	$(compiler) $(cflag) device.c




#---------------------x64-------------------------
.PHONY:x64
x64:
	$(compiler) $(cflag) tox86/cpu64/x64.endian.c
	$(compiler) $(cflag) tox86/cpu64/x64.fp.c
	$(compiler) $(cflag) tox86/cpu64/x64.port.c
	$(compiler) $(cflag) device.c

.PHONY:barex64
barex64:x64
	$(compiler) $(cflag) tox86/oldhw.c
	$(compiler) $(cflag) tox86/cpu64/x64.asm.c
	$(compiler) $(cflag) tox86/cpu64/x64.page.c
	yasm -a x86 -m amd64 -f elf64 -o x64.gdt.o tox86/cpu64/x64.gdt.asm
	$(compiler) $(cflag) -mgeneral-regs-only -mno-red-zone tox86/cpu64/x64.idt.c
	$(compiler) $(cflag) tox86/irq/8259.c
	#$(compiler) $(cflag) tox86/irq/apic.c
	$(compiler) $(cflag) tox86/bus/input/8042.c
	$(compiler) $(cflag) tox86/bus/pci/pci.c
	$(compiler) $(cflag) tox86/timer/825x.c
	$(compiler) $(cflag) tox86/timer/rtc.c
	$(compiler) $(cflag) tox86/wire/bare.wire.bt.c
	$(compiler) $(cflag) tox86/wire/bare.wire.gpio.c
	$(compiler) $(cflag) tox86/wire/bare.wire.i2c.c
	$(compiler) $(cflag) tox86/wire/bare.wire.spi.c
	$(compiler) $(cflag) tox86/wire/bare.wire.uart.c
	$(compiler) $(cflag) tox86/wire/bare.wire.usb.c
	$(compiler) $(cflag) tox86/wire/bare.wire.wifi.c

.PHONY:efix64
efix64:x64
	$(compiler) $(cflag) tox86/cpu64/x64.asm.c
	$(compiler) $(cflag) tox86/cpu64/x64.page.c
	$(compiler) $(cflag) tox86/cpu64/x64.gdt.c
	$(compiler) $(cflag) -mgeneral-regs-only tox86/cpu64/x64.idt.c
	$(compiler) $(cflag) tox86/irq/8259.c
	#$(compiler) $(cflag) tox86/irq/apic.c
	$(compiler) $(cflag) tox86/bus/input/8042.c
	$(compiler) $(cflag) tox86/bus/pci/pci.c
	$(compiler) $(cflag) tox86/timer/825x.c
	$(compiler) $(cflag) tox86/timer/rtc.c
	$(compiler) $(cflag) inefi/efi.wire.bt.c
	$(compiler) $(cflag) inefi/efi.wire.gpio.c
	$(compiler) $(cflag) inefi/efi.wire.i2c.c
	$(compiler) $(cflag) inefi/efi.wire.spi.c
	$(compiler) $(cflag) inefi/efi.wire.uart.c
	$(compiler) $(cflag) inefi/efi.wire.usb.c
	$(compiler) $(cflag) inefi/efi.wire.wifi.c
	$(compiler) $(cflag) inefi/hardware.c

.PHONY:linuxx64
linuxx64:x64
	$(compiler) $(cflag) inlinux/hardware.c
	$(compiler) $(cflag) inlinux/linux.wire.bt.c
	$(compiler) $(cflag) inlinux/linux.wire.i2c.c
	$(compiler) $(cflag) inlinux/linux.wire.gpio.c
	$(compiler) $(cflag) inlinux/linux.wire.pci.c
	$(compiler) $(cflag) inlinux/linux.wire.spi.c
	$(compiler) $(cflag) inlinux/linux.wire.usb.c
	$(compiler) $(cflag) inlinux/linux.wire.wifi.c

.PHONY:macx64
macx64:x64
	$(compiler) $(cflag) inmac/hardware.c
	$(compiler) $(cflag) inmac/mac.wire.bt.c
	$(compiler) $(cflag) inmac/mac.wire.i2c.c
	$(compiler) $(cflag) inmac/mac.wire.gpio.c
	$(compiler) $(cflag) inmac/mac.wire.pci.c
	$(compiler) $(cflag) inmac/mac.wire.spi.c
	$(compiler) $(cflag) inmac/mac.wire.usb.c
	$(compiler) $(cflag) inmac/mac.wire.wifi.c

.PHONY:winx64
winx64:x64
	$(compiler) $(cflag) inwin/hardware.c
	$(compiler) $(cflag) inwin/win.wire.bt.c
	$(compiler) $(cflag) inwin/win.wire.i2c.c
	$(compiler) $(cflag) inwin/win.wire.gpio.c
	$(compiler) $(cflag) inwin/win.wire.pci.c
	$(compiler) $(cflag) inwin/win.wire.spi.c
	$(compiler) $(cflag) inwin/win.wire.usb.c
	$(compiler) $(cflag) inwin/win.wire.wifi.c

clean:
	rm -f *.o *.a
