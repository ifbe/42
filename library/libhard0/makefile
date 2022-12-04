DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC -I..




#---------------------arm32-------------------------
arm_c = \
oscore/toarm/cpu32/arm.fp.c \
device.c
arm_o = $(patsubst %.c, %.o, $(arm_c))

linuxarm_c = \
appcore/inlinux/linux.wire.bt.c \
appcore/inlinux/linux.wire.i2c.c \
appcore/inlinux/linux.wire.gpio.c \
appcore/inlinux/linux.wire.pci.c \
appcore/inlinux/linux.wire.spi.c \
appcore/inlinux/linux.wire.usb.c \
appcore/inlinux/linux.wire.wifi.c \
appcore/inlinux/hardware.c
linuxarm_o = $(patsubst %.c, %.o, $(linuxarm_c))

linuxpi32_c = \
applib/libgpio/rpi.wiringpi.c \
appcore/inlinux/linux.wire.bt.c \
appcore/inlinux/linux.wire.i2c.c \
appcore/inlinux/linux.wire.pci.c \
appcore/inlinux/linux.wire.spi.c \
appcore/inlinux/linux.wire.usb.c \
appcore/inlinux/linux.wire.wifi.c \
appcore/inlinux/hardware.c
linuxpi32_o = $(patsubst %.c, %.o, $(linuxpi32_c))

linuxarm:$(arm_o) $(linuxarm_o)

linuxpi32:$(arm_o) $(linuxpi32_o)




#---------------------arm64-------------------------
arm64_c = \
oscore/toarm/cpu64/arm64.fp.c \
oscore/toarm/cpu64/arm64.asm.c \
device.c
arm64_o = $(patsubst %.c, %.o, $(arm64_c))

barepi64_c = \
oslib/anyusb/xhci/xhci-driver.c \
oscore/toarm/cpu64el0/arm64.syscall.caller.c \
oscore/toarm/cpu64el1/arm64.boot.c \
oscore/toarm/cpu64el1/arm64.paging.c \
oscore/toarm/cpu64el1/arm64.exc.c \
oscore/toarm/cpu64el1/arm64.cache.c \
oscore/toarm/cpu64el1/arm64.syscall.handler.c \
oscore/toarm/irq/irq.gic4.c \
oscore/toarm/timer/generictimer.c \
oscore/toarm/rpi/rpi.mbox.c \
oscore/toarm/rpi/rpi.pmic.c \
oscore/toarm/rpi/rpi.watchdog.c \
oscore/toarm/rpi/rpi.systmr.c \
oscore/toarm/rpi/rpi.sdhci.c \
oscore/toarm/rpi/rpi.sdhost.c \
oscore/toarm/rpi/rpi.gpio.c \
oscore/toarm/rpi/rpi.i2c.c \
oscore/toarm/rpi/rpi.spi.c \
oscore/toarm/rpi/rpi.uart.c \
oscore/toarm/rpi/rpi.bt.c \
oscore/toarm/rpi/rpi.pcie.c \
oscore/toarm/rpi/rpi.usb.innerxhci.c \
oscore/toarm/rpi/rpi.usb.dwc2.c \
oscore/toarm/barepi34.c
barepi64_o = $(patsubst %.c, %.o, $(barepi64_c))

myospi64_c = \
oscore/toarm/cpu64el0/arm64.abi.c \
oscore/toarm/cpu64el0/arm64.syscall.caller.c \
oscore/toarm/barepi34.c
myospi64_o = $(patsubst %.c, %.o, $(myospi64_c))

linuxarm64_c = \
appcore/inlinux/linux.wire.bt.c \
appcore/inlinux/linux.wire.gpio.c \
appcore/inlinux/linux.wire.i2c.c \
appcore/inlinux/linux.wire.pci.c \
appcore/inlinux/linux.wire.spi.c \
appcore/inlinux/linux.wire.usb.c \
appcore/inlinux/linux.wire.wifi.c \
appcore/inlinux/hardware.c
linuxarm64_o = $(patsubst %.c, %.o, $(linuxarm64_c))

linuxpi64_c = \
appcore/inlinux/linux.wire.bt.c \
appcore/inlinux/linux.wire.gpio.c \
appcore/inlinux/linux.wire.i2c.c \
appcore/inlinux/linux.wire.pci.c \
appcore/inlinux/linux.wire.spi.c \
appcore/inlinux/linux.wire.usb.c \
appcore/inlinux/linux.wire.wifi.c \
appcore/inlinux/hardware.c
linuxpi64_o = $(patsubst %.c, %.o, $(linuxpi64_c))

barepi64:$(arm64_o) $(barepi64_o)
	$(AS) -c oscore/toarm/cpu64el1/arm64.boothelp.s -o $(DIR)/arm64.boothelp.o
	$(AS) -c oscore/toarm/cpu64el1/arm64.exchelp.s -o $(DIR)/arm64.exchelp.o

myospi64:$(arm64_o) $(myospi64_o)

linuxarm64:$(arm64_o) $(linuxarm64_o)

linuxpi64:$(arm64_o) $(linuxpi64_o)




#---------------------mips32-------------------------
mips_c = \
oscore/tomips/cpu32/mips.fp.c \
device.c
mips_o = $(patsubst %.c, %.o, $(mips_c))




#---------------------mips64-------------------------
mips64_c = \
oscore/tomips/cpu64/mips64.fp.c \
device.c
mips64_o = $(patsubst %.c, %.o, $(mips64_c))




#---------------------x86-------------------------
x86_c = \
oscore/tox86/cpu32/x86.endian.c \
oscore/tox86/cpu32/x86.fp.c \
oscore/tox86/cpu32/x86.port.c \
device.c
x86_o = $(patsubst %.c, %.o, $(x86_c))

winx86_c = \
oscore/tox86/cpu32/x86.endian.c \
oscore/tox86/cpu32/x86.fp.c \
oscore/tox86/cpu32/x86.port.c \
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
oscore/tox86/cpu64/x64.endian.c \
oscore/tox86/cpu64/x64.fp.c \
oscore/tox86/cpu64/x64.port.c \
device.c
x64_o = $(patsubst %.c, %.o, $(x64_c))

barex64_c = \
oslib/anyusb/ehci/ehci-driver.c \
oslib/anyusb/xhci/xhci-driver.c \
oslib/anystor/ahci/ahci-driver.c \
oslib/anystor/nvme/nvme-driver.c \
oscore/tox86/cpu64kern/x64.paging.c \
oscore/tox86/cpu64virt/vt/x64.vt.c \
oscore/tox86/cpu64virt/svm/x64.svm.c \
oscore/tox86/bus/input/8042.c \
oscore/tox86/bus/ethernet/e1000.c \
oscore/tox86/bus/pciport/pciport.c \
oscore/tox86/bus/pciport/pciport.ahci.c \
oscore/tox86/bus/pciport/pciport.nvme.c \
oscore/tox86/bus/pciport/pciport.xhci.c \
oscore/tox86/bus/pciport/pciport.usb4hia.c \
oscore/tox86/bus/pcimmio/pcimmio.c \
oscore/tox86/irq/irq.c \
oscore/tox86/irq/8259/8259.c \
oscore/tox86/irq/apic/apic.io.c \
oscore/tox86/irq/apic/apic.local.c \
oscore/tox86/irq/msi/msi.c \
oscore/tox86/pmic/power.c \
oscore/tox86/stor/ide-driver.c \
oscore/tox86/timer/persoc/timer.hpet.c \
oscore/tox86/timer/persoc/timer.pit.c \
oscore/tox86/timer/persoc/timer.rtc.c \
oscore/tox86/timer/timer.c \
oscore/tox86/wire/bare.wire.bt.c \
oscore/tox86/wire/bare.wire.gpio.c \
oscore/tox86/wire/bare.wire.i2c.c \
oscore/tox86/wire/bare.wire.spi.c \
oscore/tox86/wire/bare.wire.uart.c \
oscore/tox86/wire/bare.wire.wifi.c \
oscore/tox86/vmtool/vmware.c \
oscore/tox86/barepc.c
barex64_o = $(patsubst %.c, %.o, $(barex64_c))

efix64_c = \
appcore/inefi/efi.wire.bt.c \
appcore/inefi/efi.wire.gpio.c \
appcore/inefi/efi.wire.i2c.c \
appcore/inefi/efi.wire.spi.c \
appcore/inefi/efi.wire.uart.c \
appcore/inefi/efi.wire.usb.c \
appcore/inefi/efi.wire.wifi.c \
appcore/inefi/hardware.c
efix64_o = $(patsubst %.c, %.o, $(efix64_c))

myosx64_c = \
oscore/tox86/cpu64user/x64.abi.c
myosx64_o = $(patsubst %.c, %.o, $(myosx64_c))

linuxx64_c = \
appcore/inlinux/linux.wire.bt.c \
appcore/inlinux/linux.wire.i2c.c \
appcore/inlinux/linux.wire.gpio.c \
appcore/inlinux/linux.wire.pci.c \
appcore/inlinux/linux.wire.spi.c \
appcore/inlinux/linux.wire.usb.c \
appcore/inlinux/linux.wire.wifi.c \
appcore/inlinux/hardware.c
linuxx64_o = $(patsubst %.c, %.o, $(linuxx64_c))

macx64_c = \
appcore/inmac/mac.wire.bt.c \
appcore/inmac/mac.wire.i2c.c \
appcore/inmac/mac.wire.gpio.c \
appcore/inmac/mac.wire.pci.c \
appcore/inmac/mac.wire.spi.c \
appcore/inmac/mac.wire.usb.c \
appcore/inmac/mac.wire.wifi.c \
appcore/inmac/hardware.c
macx64_o = $(patsubst %.c, %.o, $(macx64_c))

winx64_c = \
appcore/inwin/win.wire.bt.c \
appcore/inwin/win.wire.i2c.c \
appcore/inwin/win.wire.gpio.c \
appcore/inwin/win.wire.pci.c \
appcore/inwin/win.wire.spi.c \
appcore/inwin/win.wire.usb.c \
appcore/inwin/win.wire.wifi.c \
appcore/inwin/hardware.c
winx64_o = $(patsubst %.c, %.o, $(winx64_c))

.PHONY:barex64
barex64:$(x64_o) $(barex64_o)
	$(CC) $(CF) oscore/tox86/cpu64kern/x64.asm.c                             -o $(DIR)/x64.asm.o
	yasm -a x86 -m amd64 -f elf64 oscore/tox86/cpu64kern/x64.asmhelp.asm     -o $(DIR)/x64.asmhelp.o
	$(CC) $(CF) oscore/tox86/cpu64kern/x64.boot.c                            -o $(DIR)/x64.boot.o
	yasm -a x86 -m amd64 -f elf64 oscore/tox86/cpu64kern/x64.boothelp.asm    -o $(DIR)/x64.boothelp.o
	$(CC) $(CF) oscore/tox86/cpu64kern/x64.gdt.c                             -o $(DIR)/x64.gdt.o
	yasm -a x86 -m amd64 -f elf64 oscore/tox86/cpu64kern/x64.gdthelp.asm     -o $(DIR)/x64.gdthelp.o
	$(CC) $(CF) -mgeneral-regs-only oscore/tox86/cpu64kern/x64.idt.c         -o $(DIR)/x64.idt.o
	yasm -a x86 -m amd64 -f elf64 oscore/tox86/cpu64kern/x64.idthelp.asm     -o $(DIR)/x64.idthelp.o
	$(CC) $(CF) -mgeneral-regs-only oscore/tox86/cpu64kern/x64.syscall.handler.c     -o $(DIR)/x64.syscall.handler_c.o
	yasm -a x86 -m amd64 -f elf64 oscore/tox86/cpu64kern/x64.syscall.handler.asm -o $(DIR)/x64.syscall.handler_asm.o
	$(CC) $(CF) -mgeneral-regs-only oscore/tox86/cpu64user/x64.syscall.caller.c     -o $(DIR)/x64.syscall.caller_c.o
	yasm -a x86 -m amd64 -f elf64 oscore/tox86/cpu64user/x64.syscall.caller.asm -o $(DIR)/x64.syscall.caller_asm.o

.PHONY:myosx64
myosx64:$(x64_o) $(myosx64_o)
	$(CC) $(CF) -mgeneral-regs-only oscore/tox86/cpu64user/x64.syscall.caller.c     -o $(DIR)/x64.syscall.caller_c.o
	yasm -a x86 -m amd64 -f elf64 oscore/tox86/cpu64user/x64.syscall.caller.asm -o $(DIR)/x64.syscall.caller_asm.o

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
