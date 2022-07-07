DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC -I..




#---------------------arm32-------------------------
arm_c = \
barecore/toarm/cpu32/arm.fp.c \
device.c
arm_o = $(patsubst %.c, %.o, $(arm_c))

linuxarm_c = \
inoscore/inlinux/linux.wire.bt.c \
inoscore/inlinux/linux.wire.i2c.c \
inoscore/inlinux/linux.wire.gpio.c \
inoscore/inlinux/linux.wire.pci.c \
inoscore/inlinux/linux.wire.spi.c \
inoscore/inlinux/linux.wire.usb.c \
inoscore/inlinux/linux.wire.wifi.c \
inoscore/inlinux/hardware.c
linuxarm_o = $(patsubst %.c, %.o, $(linuxarm_c))

linuxpi32_c = \
inoslib/libgpio/rpi.wiringpi.c \
inoscore/inlinux/linux.wire.bt.c \
inoscore/inlinux/linux.wire.i2c.c \
inoscore/inlinux/linux.wire.pci.c \
inoscore/inlinux/linux.wire.spi.c \
inoscore/inlinux/linux.wire.usb.c \
inoscore/inlinux/linux.wire.wifi.c \
inoscore/inlinux/hardware.c
linuxpi32_o = $(patsubst %.c, %.o, $(linuxpi32_c))

linuxarm:$(arm_o) $(linuxarm_o)

linuxpi32:$(arm_o) $(linuxpi32_o)




#---------------------arm64-------------------------
arm64_c = \
barecore/toarm/cpu64/arm64.fp.c \
barecore/toarm/cpu64/arm64.asm.c \
device.c
arm64_o = $(patsubst %.c, %.o, $(arm64_c))

barepi64_c = \
barecore/toarm/cpu64el0/arm64.syscall.caller.c \
barecore/toarm/cpu64el1/arm64.boot.c \
barecore/toarm/cpu64el1/arm64.paging.c \
barecore/toarm/cpu64el1/arm64.exc.c \
barecore/toarm/cpu64el1/arm64.cache.c \
barecore/toarm/cpu64el1/arm64.syscall.handler.c \
barecore/toarm/irq/irq.gic4.c \
barecore/toarm/timer/generictimer.c \
barecore/toarm/rpi/rpi.mbox.c \
barecore/toarm/rpi/rpi.pmic.c \
barecore/toarm/rpi/rpi.watchdog.c \
barecore/toarm/rpi/rpi.systmr.c \
barecore/toarm/rpi/rpi.sdhci.c \
barecore/toarm/rpi/rpi.sdhost.c \
barecore/toarm/rpi/rpi.gpio.c \
barecore/toarm/rpi/rpi.i2c.c \
barecore/toarm/rpi/rpi.spi.c \
barecore/toarm/rpi/rpi.uart.c \
barecore/toarm/rpi/rpi.bt.c \
barecore/toarm/rpi/rpi.pcie.c \
barecore/toarm/rpi/rpi.usb.innerxhci.c \
barecore/toarm/rpi/rpi.usb.dwc2.c \
barelib/anyusb/xhci/xhci-driver.c \
barecore/toarm/barepi34.c
barepi64_o = $(patsubst %.c, %.o, $(barepi64_c))

myospi64_c = \
barecore/toarm/cpu64el0/arm64.abi.c \
barecore/toarm/cpu64el0/arm64.syscall.caller.c \
barecore/toarm/barepi34.c
myospi64_o = $(patsubst %.c, %.o, $(myospi64_c))

linuxarm64_c = \
inoscore/inlinux/linux.wire.bt.c \
inoscore/inlinux/linux.wire.gpio.c \
inoscore/inlinux/linux.wire.i2c.c \
inoscore/inlinux/linux.wire.pci.c \
inoscore/inlinux/linux.wire.spi.c \
inoscore/inlinux/linux.wire.usb.c \
inoscore/inlinux/linux.wire.wifi.c \
inoscore/inlinux/hardware.c
linuxarm64_o = $(patsubst %.c, %.o, $(linuxarm64_c))

linuxpi64_c = \
inoscore/inlinux/linux.wire.bt.c \
inoscore/inlinux/linux.wire.gpio.c \
inoscore/inlinux/linux.wire.i2c.c \
inoscore/inlinux/linux.wire.pci.c \
inoscore/inlinux/linux.wire.spi.c \
inoscore/inlinux/linux.wire.usb.c \
inoscore/inlinux/linux.wire.wifi.c \
inoscore/inlinux/hardware.c
linuxpi64_o = $(patsubst %.c, %.o, $(linuxpi64_c))

barepi64:$(arm64_o) $(barepi64_o)
	$(AS) -c barecore/toarm/cpu64el1/arm64.boothelp.s -o $(DIR)/arm64.boothelp.o
	$(AS) -c barecore/toarm/cpu64el1/arm64.exchelp.s -o $(DIR)/arm64.exchelp.o

myospi64:$(arm64_o) $(myospi64_o)

linuxarm64:$(arm64_o) $(linuxarm64_o)

linuxpi64:$(arm64_o) $(linuxpi64_o)




#---------------------mips32-------------------------
mips_c = \
barecore/tomips/cpu32/mips.fp.c \
device.c
mips_o = $(patsubst %.c, %.o, $(mips_c))




#---------------------mips64-------------------------
mips64_c = \
barecore/tomips/cpu64/mips64.fp.c \
device.c
mips64_o = $(patsubst %.c, %.o, $(mips64_c))




#---------------------x86-------------------------
x86_c = \
barecore/tox86/cpu32/x86.endian.c \
barecore/tox86/cpu32/x86.fp.c \
barecore/tox86/cpu32/x86.port.c \
device.c
x86_o = $(patsubst %.c, %.o, $(x86_c))

winx86_c = \
barecore/tox86/cpu32/x86.endian.c \
barecore/tox86/cpu32/x86.fp.c \
barecore/tox86/cpu32/x86.port.c \
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
barecore/tox86/cpu64/x64.endian.c \
barecore/tox86/cpu64/x64.fp.c \
barecore/tox86/cpu64/x64.port.c \
device.c
x64_o = $(patsubst %.c, %.o, $(x64_c))

barex64_c = \
barelib/anyusb/ehci/ehci-driver.c \
barelib/anyusb/xhci/xhci-driver.c \
barelib/anystor/ahci/ahci-driver.c \
barelib/anystor/nvme/nvme-driver.c \
barecore/tox86/cpu64kern/x64.paging.c \
barecore/tox86/cpu64virt/vt/x64.vt.c \
barecore/tox86/cpu64virt/svm/x64.svm.c \
barecore/tox86/bus/input/8042.c \
barecore/tox86/bus/ethernet/e1000.c \
barecore/tox86/bus/pciport/pciport.c \
barecore/tox86/bus/pciport/pciport.ahci.c \
barecore/tox86/bus/pciport/pciport.nvme.c \
barecore/tox86/bus/pciport/pciport.xhci.c \
barecore/tox86/bus/pciport/pciport.usb4hia.c \
barecore/tox86/bus/pcimmio/pcimmio.c \
barecore/tox86/irq/irq.c \
barecore/tox86/irq/8259/8259.c \
barecore/tox86/irq/apic/apic.io.c \
barecore/tox86/irq/apic/apic.local.c \
barecore/tox86/irq/apic/apic.timer.c \
barecore/tox86/pmic/power.c \
barecore/tox86/stor/ide-driver.c \
barecore/tox86/timer/825x.c \
barecore/tox86/timer/rtc.c \
barecore/tox86/wire/bare.wire.bt.c \
barecore/tox86/wire/bare.wire.gpio.c \
barecore/tox86/wire/bare.wire.i2c.c \
barecore/tox86/wire/bare.wire.spi.c \
barecore/tox86/wire/bare.wire.uart.c \
barecore/tox86/wire/bare.wire.wifi.c \
barecore/tox86/vmtool/vmware.c \
barecore/tox86/barepc.c
barex64_o = $(patsubst %.c, %.o, $(barex64_c))

efix64_c = \
inoscore/inefi/efi.wire.bt.c \
inoscore/inefi/efi.wire.gpio.c \
inoscore/inefi/efi.wire.i2c.c \
inoscore/inefi/efi.wire.spi.c \
inoscore/inefi/efi.wire.uart.c \
inoscore/inefi/efi.wire.usb.c \
inoscore/inefi/efi.wire.wifi.c \
inoscore/inefi/hardware.c
efix64_o = $(patsubst %.c, %.o, $(efix64_c))

myosx64_c = \
barecore/tox86/cpu64user/x64.abi.c
myosx64_o = $(patsubst %.c, %.o, $(myosx64_c))

linuxx64_c = \
inoscore/inlinux/linux.wire.bt.c \
inoscore/inlinux/linux.wire.i2c.c \
inoscore/inlinux/linux.wire.gpio.c \
inoscore/inlinux/linux.wire.pci.c \
inoscore/inlinux/linux.wire.spi.c \
inoscore/inlinux/linux.wire.usb.c \
inoscore/inlinux/linux.wire.wifi.c \
inoscore/inlinux/hardware.c
linuxx64_o = $(patsubst %.c, %.o, $(linuxx64_c))

macx64_c = \
inoscore/inmac/mac.wire.bt.c \
inoscore/inmac/mac.wire.i2c.c \
inoscore/inmac/mac.wire.gpio.c \
inoscore/inmac/mac.wire.pci.c \
inoscore/inmac/mac.wire.spi.c \
inoscore/inmac/mac.wire.usb.c \
inoscore/inmac/mac.wire.wifi.c \
inoscore/inmac/hardware.c
macx64_o = $(patsubst %.c, %.o, $(macx64_c))

winx64_c = \
inoscore/inwin/win.wire.bt.c \
inoscore/inwin/win.wire.i2c.c \
inoscore/inwin/win.wire.gpio.c \
inoscore/inwin/win.wire.pci.c \
inoscore/inwin/win.wire.spi.c \
inoscore/inwin/win.wire.usb.c \
inoscore/inwin/win.wire.wifi.c \
inoscore/inwin/hardware.c
winx64_o = $(patsubst %.c, %.o, $(winx64_c))

.PHONY:barex64
barex64:$(x64_o) $(barex64_o)
	$(CC) $(CF) barecore/tox86/cpu64kern/x64.asm.c                             -o $(DIR)/x64.asm.o
	yasm -a x86 -m amd64 -f elf64 barecore/tox86/cpu64kern/x64.asmhelp.asm     -o $(DIR)/x64.asmhelp.o
	$(CC) $(CF) barecore/tox86/cpu64kern/x64.boot.c                            -o $(DIR)/x64.boot.o
	yasm -a x86 -m amd64 -f elf64 barecore/tox86/cpu64kern/x64.boothelp.asm    -o $(DIR)/x64.boothelp.o
	$(CC) $(CF) barecore/tox86/cpu64kern/x64.gdt.c                             -o $(DIR)/x64.gdt.o
	yasm -a x86 -m amd64 -f elf64 barecore/tox86/cpu64kern/x64.gdthelp.asm     -o $(DIR)/x64.gdthelp.o
	$(CC) $(CF) -mgeneral-regs-only barecore/tox86/cpu64kern/x64.idt.c         -o $(DIR)/x64.idt.o
	yasm -a x86 -m amd64 -f elf64 barecore/tox86/cpu64kern/x64.idthelp.asm     -o $(DIR)/x64.idthelp.o
	$(CC) $(CF) -mgeneral-regs-only barecore/tox86/cpu64kern/x64.syscall.handler.c     -o $(DIR)/x64.syscall.handler_c.o
	yasm -a x86 -m amd64 -f elf64 barecore/tox86/cpu64kern/x64.syscall.handler.asm -o $(DIR)/x64.syscall.handler_asm.o
	$(CC) $(CF) -mgeneral-regs-only barecore/tox86/cpu64user/x64.syscall.caller.c     -o $(DIR)/x64.syscall.caller_c.o
	yasm -a x86 -m amd64 -f elf64 barecore/tox86/cpu64user/x64.syscall.caller.asm -o $(DIR)/x64.syscall.caller_asm.o

.PHONY:myosx64
myosx64:$(x64_o) $(myosx64_o)
	$(CC) $(CF) -mgeneral-regs-only barecore/tox86/cpu64user/x64.syscall.caller.c     -o $(DIR)/x64.syscall.caller_c.o
	yasm -a x86 -m amd64 -f elf64 barecore/tox86/cpu64user/x64.syscall.caller.asm -o $(DIR)/x64.syscall.caller_asm.o

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
