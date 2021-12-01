DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC -I..




#---------------------else-------------------------
anyelse_c = \
anyusb/ehci/ehci-driver.c \
anyusb/xhci/xhci-driver.c
anyelse_o = $(patsubst %.c, %.o, $(anyelse_c))

mmc_c = \
anymmc/usbhost/sdhci.c \
anymmc/usbperi/sdcard.c
mmc_o = $(patsubst %.c, %.o, $(mmc_c))





#---------------------arm32-------------------------
arm_c = \
toarm/cpu32/arm.fp.c \
device.c
arm_o = $(patsubst %.c, %.o, $(arm_c))

linuxarm_c = \
inlinux/linux.wire.bt.c \
inlinux/linux.wire.i2c.c \
inlinux/linux.wire.gpio.c \
inlinux/linux.wire.pci.c \
inlinux/linux.wire.spi.c \
inlinux/linux.wire.usb.c \
inlinux/linux.wire.wifi.c \
inlinux/hardware.c
linuxarm_o = $(patsubst %.c, %.o, $(linuxarm_c))

linuxpi32_c = \
libgpio/rpi.wiringpi.c \
inlinux/linux.wire.bt.c \
inlinux/linux.wire.i2c.c \
inlinux/linux.wire.pci.c \
inlinux/linux.wire.spi.c \
inlinux/linux.wire.usb.c \
inlinux/linux.wire.wifi.c \
inlinux/hardware.c
linuxpi32_o = $(patsubst %.c, %.o, $(linuxpi32_c))

linuxarm:$(arm_o) $(linuxarm_o)

linuxpi32:$(arm_o) $(linuxpi32_o)




#---------------------arm64-------------------------
arm64_c = \
toarm/cpu64/arm64.fp.c \
toarm/cpu64/arm64.asm.c \
toarm/cpu64/arm64.boot.c \
toarm/cpu64/arm64.paging.c \
toarm/cpu64/arm64.exc.c \
toarm/cpu64/arm64.cache.c \
toarm/cpu64/arm64.syscall.caller.c \
toarm/cpu64/arm64.syscall.handler.c \
device.c
arm64_o = $(patsubst %.c, %.o, $(arm64_c))

barepi64_c = \
toarm/irq/irq.gic4.c \
toarm/timer/generictimer.c \
toarm/rpi/rpi.mbox.c \
toarm/rpi/rpi.pmic.c \
toarm/rpi/rpi.watchdog.c \
toarm/rpi/rpi.systmr.c \
toarm/rpi/rpi.sdhci.c \
toarm/rpi/rpi.sdhost.c \
toarm/rpi/rpi.gpio.c \
toarm/rpi/rpi.i2c.c \
toarm/rpi/rpi.spi.c \
toarm/rpi/rpi.uart.c \
toarm/rpi/rpi.pcie.c \
toarm/rpi/rpi.usb.innerxhci.c \
toarm/rpi/rpi.usb.dwc2.c \
anyusb/xhci/xhci-driver.c \
toarm/barepi34.c
barepi64_o = $(patsubst %.c, %.o, $(barepi64_c))

linuxarm64_c = \
inlinux/linux.wire.bt.c \
inlinux/linux.wire.gpio.c \
inlinux/linux.wire.i2c.c \
inlinux/linux.wire.pci.c \
inlinux/linux.wire.spi.c \
inlinux/linux.wire.usb.c \
inlinux/linux.wire.wifi.c \
inlinux/hardware.c
linuxarm64_o = $(patsubst %.c, %.o, $(linuxarm64_c))

linuxpi64_c = \
inlinux/linux.wire.bt.c \
inlinux/linux.wire.gpio.c \
inlinux/linux.wire.i2c.c \
inlinux/linux.wire.pci.c \
inlinux/linux.wire.spi.c \
inlinux/linux.wire.usb.c \
inlinux/linux.wire.wifi.c \
inlinux/hardware.c
linuxpi64_o = $(patsubst %.c, %.o, $(linuxpi64_c))

barepi64:$(arm64_o) $(barepi64_o)
	$(AS) -c toarm/cpu64/arm64.boothelp.s -o $(DIR)/arm64.boothelp.o
	$(AS) -c toarm/cpu64/arm64.exchelp.s -o $(DIR)/arm64.exchelp.o

linuxarm64:$(arm64_o) $(linuxarm64_o)

linuxpi64:$(arm64_o) $(linuxpi64_o)




#---------------------mips32-------------------------
mips_c = \
tomips/cpu32/mips.fp.c \
device.c
mips_o = $(patsubst %.c, %.o, $(mips_c))




#---------------------mips64-------------------------
mips64_c = \
tomips/cpu64/mips64.fp.c \
device.c
mips64_o = $(patsubst %.c, %.o, $(mips64_c))




#---------------------x86-------------------------
x86_c = \
tox86/cpu32/x86.endian.c \
tox86/cpu32/x86.fp.c \
tox86/cpu32/x86.port.c \
device.c
x86_o = $(patsubst %.c, %.o, $(x86_c))

winx86_c = \
tox86/cpu32/x86.endian.c \
tox86/cpu32/x86.fp.c \
tox86/cpu32/x86.port.c \
device.c
winx86_o = $(patsubst %.c, %.o, $(winx86_c))

.PHONY:linuxx86
linuxx86:$(x86_o)

.PHONY:macx86
macx86:$(x86_o)

.PHONY:winx86
winx86:$(x86_o) $(winx86_o)




#---------------------x64-------------------------
x64_c = \
tox86/cpu64/x64.endian.c \
tox86/cpu64/x64.fp.c \
tox86/cpu64/x64.port.c \
device.c
x64_o = $(patsubst %.c, %.o, $(x64_c))

barex64_c = \
tox86/cpu64/x64.paging.c \
tox86/cpu64vt/x64.vt.c \
tox86/cpu64svm/x64.svm.c \
tox86/bus/input/8042.c \
tox86/bus/ethernet/e1000.c \
tox86/bus/pci/pci-port.c \
tox86/bus/pci/pci-mmio.c \
tox86/bus/pci/pci.ahci.c \
tox86/bus/pci/pci.xhci.c \
tox86/irq/irq.c \
tox86/irq/8259/8259.c \
tox86/irq/apic/apic.io.c \
tox86/irq/apic/apic.local.c \
tox86/irq/apic/apic.timer.c \
tox86/pmic/power.c \
tox86/stor/ide-driver.c \
tox86/stor/ahci-driver.c \
tox86/stor/nvme-driver.c \
tox86/timer/825x.c \
tox86/timer/rtc.c \
tox86/wire/bare.wire.bt.c \
tox86/wire/bare.wire.gpio.c \
tox86/wire/bare.wire.i2c.c \
tox86/wire/bare.wire.spi.c \
tox86/wire/bare.wire.uart.c \
tox86/wire/bare.wire.wifi.c \
tox86/barepc.c
barex64_o = $(patsubst %.c, %.o, $(barex64_c))

efix64_c = \
inefi/efi.wire.bt.c \
inefi/efi.wire.gpio.c \
inefi/efi.wire.i2c.c \
inefi/efi.wire.spi.c \
inefi/efi.wire.uart.c \
inefi/efi.wire.usb.c \
inefi/efi.wire.wifi.c \
inefi/hardware.c
efix64_o = $(patsubst %.c, %.o, $(efix64_c))

linuxx64_c = \
inlinux/linux.wire.bt.c \
inlinux/linux.wire.i2c.c \
inlinux/linux.wire.gpio.c \
inlinux/linux.wire.pci.c \
inlinux/linux.wire.spi.c \
inlinux/linux.wire.usb.c \
inlinux/linux.wire.wifi.c \
inlinux/hardware.c
linuxx64_o = $(patsubst %.c, %.o, $(linuxx64_c))

macx64_c = \
inmac/mac.wire.bt.c \
inmac/mac.wire.i2c.c \
inmac/mac.wire.gpio.c \
inmac/mac.wire.pci.c \
inmac/mac.wire.spi.c \
inmac/mac.wire.usb.c \
inmac/mac.wire.wifi.c \
inmac/hardware.c
macx64_o = $(patsubst %.c, %.o, $(macx64_c))

winx64_c = \
inwin/win.wire.bt.c \
inwin/win.wire.i2c.c \
inwin/win.wire.gpio.c \
inwin/win.wire.pci.c \
inwin/win.wire.spi.c \
inwin/win.wire.usb.c \
inwin/win.wire.wifi.c \
inwin/hardware.c
winx64_o = $(patsubst %.c, %.o, $(winx64_c))

.PHONY:barex64
barex64:$(x64_o) $(barex64_o) $(anyelse_o)
	$(CC) $(CF) tox86/cpu64/x64.asm.c                             -o $(DIR)/x64.asm.o
	yasm -a x86 -m amd64 -f elf64 tox86/cpu64/x64.asmhelp.asm     -o $(DIR)/x64.asmhelp.o
	$(CC) $(CF) tox86/cpu64/x64.boot.c                            -o $(DIR)/x64.boot.o
	yasm -a x86 -m amd64 -f elf64 tox86/cpu64/x64.boothelp.asm    -o $(DIR)/x64.boothelp.o
	$(CC) $(CF) tox86/cpu64/x64.gdt.c                             -o $(DIR)/x64.gdt.o
	yasm -a x86 -m amd64 -f elf64 tox86/cpu64/x64.gdthelp.asm     -o $(DIR)/x64.gdthelp.o
	$(CC) $(CF) -mgeneral-regs-only tox86/cpu64/x64.idt.c         -o $(DIR)/x64.idt.o
	yasm -a x86 -m amd64 -f elf64 tox86/cpu64/x64.idthelp.asm     -o $(DIR)/x64.idthelp.o
	$(CC) $(CF) -mgeneral-regs-only tox86/cpu64/x64.syscall.caller.c     -o $(DIR)/x64.syscall.caller_c.o
	yasm -a x86 -m amd64 -f elf64 tox86/cpu64/x64.syscall.caller.asm -o $(DIR)/x64.syscall.caller_asm.o
	$(CC) $(CF) -mgeneral-regs-only tox86/cpu64/x64.syscall.handler.c     -o $(DIR)/x64.syscall.handler_c.o
	yasm -a x86 -m amd64 -f elf64 tox86/cpu64/x64.syscall.handler.asm -o $(DIR)/x64.syscall.handler_asm.o

.PHONY:efix64
efix64:$(x64_o) $(efix64_o)

.PHONY:linuxx64
linuxx64:$(x64_o) $(linuxx64_o)

.PHONY:macx64
macx64:$(x64_o) $(macx64_o)

.PHONY:winx64
winx64:$(x64_o) $(winx64_o)

%.o: %.c
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libhard0/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi


clean:
	rm -f *.o *.a
