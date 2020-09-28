CC:=gcc
CF:=-c -O2 -fPIC




#---------------------arm32-------------------------
.PHONY:arm
arm:
	$(CC) $(CF) toarm/cpu32/arm.fp.c
	$(CC) $(CF) device.c

linuxarm:arm
	$(CC) $(CF) inlinux/hardware.c
	$(CC) $(CF) inlinux/linux.wire.bt.c
	$(CC) $(CF) inlinux/linux.wire.i2c.c
	$(CC) $(CF) inlinux/linux.wire.gpio.c
	$(CC) $(CF) inlinux/linux.wire.pci.c
	$(CC) $(CF) inlinux/linux.wire.spi.c
	$(CC) $(CF) inlinux/linux.wire.usb.c
	$(CC) $(CF) inlinux/linux.wire.wifi.c

linuxpi32:arm
	$(CC) $(CF) inlinux/hardware.c
	$(CC) $(CF) toarm/lib/rpi.wiringpi.c
	$(CC) $(CF) inlinux/linux.wire.bt.c
	$(CC) $(CF) inlinux/linux.wire.i2c.c
	$(CC) $(CF) inlinux/linux.wire.pci.c
	$(CC) $(CF) inlinux/linux.wire.spi.c
	$(CC) $(CF) inlinux/linux.wire.usb.c
	$(CC) $(CF) inlinux/linux.wire.wifi.c




#---------------------arm64-------------------------
.PHONY:arm64
arm64:
	$(CC) $(CF) toarm/cpu64/arm64.fp.c
	$(CC) $(CF) device.c

barepi64:arm64
	$(CC) $(CF) toarm/pi34.c
	$(CC) $(CF) toarm/rpi/rpi.sd.c
	$(CC) $(CF) toarm/rpi/rpi.mbox.c
	$(CC) $(CF) toarm/rpi/rpi.systmr.c
	$(CC) $(CF) toarm/rpi/rpi.gpio.c
	$(CC) $(CF) toarm/rpi/rpi.uart.c
	$(CC) $(CF) toarm/rpi/rpi.i2c.c
	$(CC) $(CF) toarm/rpi/rpi.spi.c

linuxarm64:arm64
	$(CC) $(CF) inlinux/hardware.c
	$(CC) $(CF) inlinux/linux.wire.bt.c
	$(CC) $(CF) inlinux/linux.wire.gpio.c
	$(CC) $(CF) inlinux/linux.wire.i2c.c
	$(CC) $(CF) inlinux/linux.wire.pci.c
	$(CC) $(CF) inlinux/linux.wire.spi.c
	$(CC) $(CF) inlinux/linux.wire.usb.c
	$(CC) $(CF) inlinux/linux.wire.wifi.c

linuxpi64:arm64
	$(CC) $(CF) inlinux/hardware.c
	$(CC) $(CF) inlinux/linux.wire.bt.c
	$(CC) $(CF) inlinux/linux.wire.gpio.c
	$(CC) $(CF) inlinux/linux.wire.i2c.c
	$(CC) $(CF) inlinux/linux.wire.pci.c
	$(CC) $(CF) inlinux/linux.wire.spi.c
	$(CC) $(CF) inlinux/linux.wire.usb.c
	$(CC) $(CF) inlinux/linux.wire.wifi.c




#---------------------mips32-------------------------
.PHONY:mips
mips:
	$(CC) $(CF) tomips/cpu32/mips.fp.c
	$(CC) $(CF) device.c




#---------------------mips64-------------------------
.PHONY:mips64
mips64:
	$(CC) $(CF) tomips/cpu64/mips64.fp.c
	$(CC) $(CF) device.c




#---------------------x86-------------------------
.PHONY:x86
x86:
	$(CC) $(CF) tox86/cpu32/x86.endian.c
	$(CC) $(CF) tox86/cpu32/x86.fp.c
	$(CC) $(CF) tox86/cpu32/x86.port.c
	$(CC) $(CF) device.c

.PHONY:linuxx86
linuxx86:x86

.PHONY:macx86
macx86:x86

.PHONY:winx86
winx86:x86
	$(CC) $(CF) tox86/cpu32/x86.endian.c
	$(CC) $(CF) tox86/cpu32/x86.fp.c
	$(CC) $(CF) tox86/cpu32/x86.port.c
	$(CC) $(CF) device.c




#---------------------x64-------------------------
.PHONY:x64
x64:
	$(CC) $(CF) tox86/cpu64/x64.endian.c
	$(CC) $(CF) tox86/cpu64/x64.fp.c
	$(CC) $(CF) tox86/cpu64/x64.port.c
	$(CC) $(CF) device.c

.PHONY:barex64
barex64:x64
	$(CC) $(CF) tox86/oldhw.c
	$(CC) $(CF) tox86/cpu64/x64.asm.c
	$(CC) $(CF) tox86/cpu64/x64.paging.c
	$(CC) $(CF) tox86/cpu64/x64.gdtmake.c
	yasm -a x86 -m amd64 -f elf64 -o x64.gdthelp.o tox86/cpu64/x64.gdthelp.asm
	$(CC) $(CF) -mgeneral-regs-only tox86/cpu64/x64.idt.c
	yasm -a x86 -m amd64 -f elf64 -o x64.idthelp.o tox86/cpu64/x64.idthelp.asm
	$(CC) $(CF) tox86/irq/8259.c
	$(CC) $(CF) tox86/irq/apic.c
	$(CC) $(CF) tox86/bus/input/8042.c
	$(CC) $(CF) tox86/bus/pci/pci-port.c
	$(CC) $(CF) tox86/bus/pci/pci-mmio.c
	$(CC) $(CF) tox86/usbhost/ehci-driver.c
	$(CC) $(CF) tox86/usbhost/xhci-driver.c
	$(CC) $(CF) tox86/usbgadget/usb.c
	$(CC) $(CF) tox86/usbgadget/usbhid-driver.c
	$(CC) $(CF) tox86/usbgadget/usbstor-driver.c
	$(CC) $(CF) tox86/stor/ide-driver.c
	$(CC) $(CF) tox86/stor/ahci-driver.c
	$(CC) $(CF) tox86/stor/nvme-driver.c
	$(CC) $(CF) tox86/timer/825x.c
	$(CC) $(CF) tox86/timer/rtc.c
	$(CC) $(CF) tox86/wire/bare.wire.bt.c
	$(CC) $(CF) tox86/wire/bare.wire.gpio.c
	$(CC) $(CF) tox86/wire/bare.wire.i2c.c
	$(CC) $(CF) tox86/wire/bare.wire.spi.c
	$(CC) $(CF) tox86/wire/bare.wire.uart.c
	$(CC) $(CF) tox86/wire/bare.wire.wifi.c

.PHONY:efix64
efix64:x64
	$(CC) $(CF) inefi/efi.wire.bt.c
	$(CC) $(CF) inefi/efi.wire.gpio.c
	$(CC) $(CF) inefi/efi.wire.i2c.c
	$(CC) $(CF) inefi/efi.wire.spi.c
	$(CC) $(CF) inefi/efi.wire.uart.c
	$(CC) $(CF) inefi/efi.wire.usb.c
	$(CC) $(CF) inefi/efi.wire.wifi.c
	$(CC) $(CF) inefi/hardware.c

.PHONY:linuxx64
linuxx64:x64
	$(CC) $(CF) inlinux/hardware.c
	$(CC) $(CF) inlinux/linux.wire.bt.c
	$(CC) $(CF) inlinux/linux.wire.i2c.c
	$(CC) $(CF) inlinux/linux.wire.gpio.c
	$(CC) $(CF) inlinux/linux.wire.pci.c
	$(CC) $(CF) inlinux/linux.wire.spi.c
	$(CC) $(CF) inlinux/linux.wire.usb.c
	$(CC) $(CF) inlinux/linux.wire.wifi.c

.PHONY:macx64
macx64:x64
	$(CC) $(CF) inmac/hardware.c
	$(CC) $(CF) inmac/mac.wire.bt.c
	$(CC) $(CF) inmac/mac.wire.i2c.c
	$(CC) $(CF) inmac/mac.wire.gpio.c
	$(CC) $(CF) inmac/mac.wire.pci.c
	$(CC) $(CF) inmac/mac.wire.spi.c
	$(CC) $(CF) inmac/mac.wire.usb.c
	$(CC) $(CF) inmac/mac.wire.wifi.c

.PHONY:winx64
winx64:x64
	$(CC) $(CF) inwin/hardware.c
	$(CC) $(CF) inwin/win.wire.bt.c
	$(CC) $(CF) inwin/win.wire.i2c.c
	$(CC) $(CF) inwin/win.wire.gpio.c
	$(CC) $(CF) inwin/win.wire.pci.c
	$(CC) $(CF) inwin/win.wire.spi.c
	$(CC) $(CF) inwin/win.wire.usb.c
	$(CC) $(CF) inwin/win.wire.wifi.c

clean:
	rm -f *.o *.a
