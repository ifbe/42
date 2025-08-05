DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC -I..




#---------------------arm32-------------------------
arm_c = \
device.c
arm_o = $(patsubst %.c, %.o, $(arm_c))


#---------------------arm32 bare-------------------------
barepi32_c = \
0cpu/arm/cpu32/arm.fp.c
barepi32_o = $(patsubst %.c, %.o, $(barepi32_c))


#---------------------arm32 linux-------------------------
linuxarm_c = \
2soc/inlinux/linux.wire.bt.c \
2soc/inlinux/linux.wire.i2c.c \
2soc/inlinux/linux.wire.gpio.c \
2soc/inlinux/linux.wire.pci.c \
2soc/inlinux/linux.wire.spi.c \
2soc/inlinux/linux.wire.usb.c \
2soc/inlinux/linux.wire.wifi.c \
2soc/inlinux/hardware.c
linuxarm_o = $(patsubst %.c, %.o, $(linuxarm_c))

linuxpi32_c = \
2soc/inlinux/linux.wire.bt.c \
2soc/inlinux/linux.wire.i2c.c \
2soc/inlinux/linux.wire.pci.c \
2soc/inlinux/linux.wire.spi.c \
2soc/inlinux/linux.wire.usb.c \
2soc/inlinux/linux.wire.wifi.c \
2soc/inlinux/linux.math.c \
2soc/inlinux/hardware.c
linuxpi32_o = $(patsubst %.c, %.o, $(linuxpi32_c))




#---------------------arm64-------------------------
arm64_c = \
device.c
arm64_o = $(patsubst %.c, %.o, $(arm64_c))


#---------------------arm64 bare-------------------------
barepi64_c = \
0cpu/arm/cpu64/arm64.fp.c \
0cpu/arm/cpu64/arm64.asm.c \
0cpu/arm/cpu64el0/arm64.syscall.caller.c \
0cpu/arm/cpu64el1/arm64.boot.c \
0cpu/arm/cpu64el1/arm64.paging.c \
0cpu/arm/cpu64el1/arm64.exc.c \
0cpu/arm/cpu64el1/arm64.cache.c \
0cpu/arm/cpu64el1/arm64.syscall.handler.c \
1arch/arm/irq/irq.gic4.c \
1arch/arm/timer/generictimer.c \
2soc/chipbcm/rpi/rpi.mbox.c \
2soc/chipbcm/rpi/rpi.pmic.c \
2soc/chipbcm/rpi/rpi.watchdog.c \
2soc/chipbcm/rpi/rpi.systmr.c \
2soc/chipbcm/rpi/rpi.sdhci.c \
2soc/chipbcm/rpi/rpi.sdhost.c \
2soc/chipbcm/rpi/rpi.gpio.c \
2soc/chipbcm/rpi/rpi.i2c.c \
2soc/chipbcm/rpi/rpi.spi.c \
2soc/chipbcm/rpi/rpi.uart.c \
2soc/chipbcm/rpi/rpi.bt.c \
2soc/chipbcm/rpi/rpi.pcie.c \
2soc/chipbcm/rpi/rpi.usb.innerxhci.c \
2soc/chipbcm/rpi/rpi.usb.dwc2.c \
2soc/bareusb/xhci/xhci-driver.c \
3board/pi/barepi34.c
barepi64_o = $(patsubst %.c, %.o, $(barepi64_c))

pi64bare:$(arm64_o) $(barepi64_o)
	$(AS) -c 0cpu/arm/cpu64el1/arm64.boothelp.s -o $(DIR)/arm64.boothelp.o
	$(AS) -c 0cpu/arm/cpu64el1/arm64.exchelp.s -o $(DIR)/arm64.exchelp.o

pi64myos:$(arm64_o) $(myospi64_o)


#---------------------arm64 myos-------------------------
myospi64_c = \
0cpu/arm/cpu64/arm64.fp.c \
0cpu/arm/cpu64/arm64.asm.c \
0cpu/arm/cpu64el0/arm64.syscall.caller.c
myospi64_o = $(patsubst %.c, %.o, $(myospi64_c))


#---------------------arm64 linux-------------------------
linuxarm64_c = \
2soc/inlinux/linux.wire.bt.c \
2soc/inlinux/linux.wire.i2c.c \
2soc/inlinux/linux.wire.pci.c \
2soc/inlinux/linux.wire.spi.c \
2soc/inlinux/linux.wire.usb.c \
2soc/inlinux/linux.wire.wifi.c \
2soc/inlinux/linux.math.c \
2soc/inlinux/hardware.c
linuxarm64_o = $(patsubst %.c, %.o, $(linuxarm64_c))

linuxpi64_c = \
2soc/inlinux/linux.wire.bt.c \
2soc/inlinux/linux.wire.i2c.c \
2soc/inlinux/linux.wire.pci.c \
2soc/inlinux/linux.wire.spi.c \
2soc/inlinux/linux.wire.usb.c \
2soc/inlinux/linux.wire.wifi.c \
2soc/inlinux/linux.math.c \
2soc/inlinux/hardware.c
linuxpi64_o = $(patsubst %.c, %.o, $(linuxpi64_c))

dummygpio_c = \
2soc/inlinux/linux.wire.gpio.c
dummygpio_o = $(patsubst %.c, %.o, $(dummygpio_c))

bcm2835gpio_c = \
2soc/chipbcm/bcm2835.gpio.c \
bcm2835gpio_o = $(patsubst %.c, %.o, $(bcm2835gpio_c))

wiringpi_c = \
2soc/libwiringpi/rpi.wiringpi.c
wiringpi_o = $(patsubst %.c, %.o, $(wiringpi_c))

gpiod_c = \
2soc/libgpiod/gpiod.c
gpiod_o = $(patsubst %.c, %.o, $(gpiod_c))

armlinux:$(arm_o) $(linuxarm_o) $(dummygpio_o)

pi32linux:$(arm_o) $(linuxpi32_o) $(bcm2835gpio_o)

pi32linux_gpiod:$(arm_o) $(linuxpi32_o) $(gpiod_o)

arm64linux:$(arm64_o) $(linuxarm64_o) $(dummygpio_o)

pi64linux:$(arm64_o) $(linuxpi64_o) $(bcm2835gpio_o)

pi64linux_wiringpi:$(arm64_o) $(linuxpi64_o) $(wiringpi_o)

pi64linux_gpiod:$(arm64_o) $(linuxpi64_o) $(gpiod_o)




#---------------------arm64 mac-------------------------
macarm64_c = \
2soc/inmac/mac.wire.bt.c \
2soc/inmac/mac.wire.i2c.c \
2soc/inmac/mac.wire.gpio.c \
2soc/inmac/mac.wire.pci.c \
2soc/inmac/mac.wire.spi.c \
2soc/inmac/mac.wire.usb.c \
2soc/inmac/mac.wire.wifi.c \
2soc/inmac/mac.math.c \
2soc/inmac/hardware.c
macarm64_o = $(patsubst %.c, %.o, $(macarm64_c))

macarm64:$(arm64_o) $(macarm64_o)



#---------------------mips32-------------------------
mips_c = \
1arch/mips/cpu32/mips.fp.c \
device.c
mips_o = $(patsubst %.c, %.o, $(mips_c))




#---------------------mips64-------------------------
mips64_c = \
1arch/mips/cpu64/mips64.fp.c \
device.c
mips64_o = $(patsubst %.c, %.o, $(mips64_c))




#---------------------x86-------------------------
x86_c = \
0cpu/x86/cpu32/x86.endian.c \
0cpu/x86/cpu32/x86.fp.c \
0cpu/x86/cpu32/x86.port.c \
device.c
x86_o = $(patsubst %.c, %.o, $(x86_c))


#---------------------x86 linux-------------------------
.PHONY:linuxx86
linuxx86:$(x86_o)


#---------------------x86 mac-------------------------
.PHONY:macx86
macx86:$(x86_o)


#---------------------x86 win-------------------------
winx86_c = \
0cpu/x86/cpu32/x86.endian.c \
0cpu/x86/cpu32/x86.fp.c \
0cpu/x86/cpu32/x86.port.c \
device.c
winx86_o = $(patsubst %.c, %.o, $(winx86_c))

.PHONY:winx86
winx86:$(x86_o) $(winx86_o)




#---------------------x64-------------------------
x64_c = \
0cpu/x86/cpu64/x64.endian.c \
0cpu/x86/cpu64/x64.fp.c \
0cpu/x86/cpu64/x64.port.c \
device.c
x64_o = $(patsubst %.c, %.o, $(x64_c))


#---------------------x64 bare-------------------------
barex64_c = \
0cpu/x86/cpu64kern/x64.paging.c \
0cpu/x86/cpu64virt/vt/x64.vt.c \
0cpu/x86/cpu64virt/svm/x64.svm.c \
1arch/x86/bus/ethernet/e1000.c \
1arch/x86/bus/pciport/pciport.c \
1arch/x86/bus/pciport/pciport.ahci.c \
1arch/x86/bus/pciport/pciport.nvme.c \
1arch/x86/bus/pciport/pciport.xhci.c \
1arch/x86/bus/pciport/pciport.usb4hia.c \
1arch/x86/bus/pcimmio/pcimmio.c \
1arch/x86/bus/serial/8250.c \
1arch/x86/irq/irq.c \
1arch/x86/irq/8259/8259.c \
1arch/x86/irq/apic/apic.io.c \
1arch/x86/irq/apic/apic.local.c \
1arch/x86/irq/msi/msi.c \
1arch/x86/pmic/power.c \
1arch/x86/stor/ide-driver.c \
1arch/x86/timer/persoc/timer.hpet.c \
1arch/x86/timer/persoc/timer.pit.c \
1arch/x86/timer/persoc/timer.rtc.c \
1arch/x86/timer/timer.c \
1arch/x86/wire/bare.wire.bt.c \
1arch/x86/wire/bare.wire.gpio.c \
1arch/x86/wire/bare.wire.i2c.c \
1arch/x86/wire/bare.wire.spi.c \
1arch/x86/wire/bare.wire.uart.c \
1arch/x86/wire/bare.wire.wifi.c \
2soc/bareusb/ehci/ehci-driver.c \
2soc/bareusb/xhci/xhci-driver.c \
2soc/barestor/ahci/ahci-driver.c \
2soc/barestor/nvme/nvme-driver.c \
2soc/barestor/sata/sata.c \
3board/pc/barepc.c
barex64_o = $(patsubst %.c, %.o, $(barex64_c))

.PHONY:barex64
barex64:$(x64_o) $(barex64_o)
	$(CC) $(CF) 0cpu/x86/cpu64kern/x64.asm.c                             -o $(DIR)/x64.asm.o
	yasm -a x86 -m amd64 -f elf64 0cpu/x86/cpu64kern/x64.asmhelp.asm     -o $(DIR)/x64.asmhelp.o
	$(CC) $(CF) 0cpu/x86/cpu64kern/x64.boot.c                            -o $(DIR)/x64.boot.o
	yasm -a x86 -m amd64 -f elf64 0cpu/x86/cpu64kern/x64.boothelp.asm    -o $(DIR)/x64.boothelp.o
	$(CC) $(CF) -mgeneral-regs-only 0cpu/x86/cpu64kern/x64.gdt.c         -o $(DIR)/x64.gdt.o
	yasm -a x86 -m amd64 -f elf64 0cpu/x86/cpu64kern/x64.gdthelp.asm     -o $(DIR)/x64.gdthelp.o
	$(CC) $(CF) -mgeneral-regs-only 0cpu/x86/cpu64kern/x64.idt.c         -o $(DIR)/x64.idt.o
	yasm -a x86 -m amd64 -f elf64 0cpu/x86/cpu64kern/x64.idthelp.asm     -o $(DIR)/x64.idthelp.o
	$(CC) $(CF) -mgeneral-regs-only 0cpu/x86/cpu64kern/x64.syscall.handler.c -o $(DIR)/x64.syscall.handler_c.o
	yasm -a x86 -m amd64 -f elf64 0cpu/x86/cpu64kern/x64.syscall.handler.asm -o $(DIR)/x64.syscall.handler_asm.o
	$(CC) $(CF) -mgeneral-regs-only 0cpu/x86/cpu64user/x64.syscall.caller.c  -o $(DIR)/x64.syscall.caller_c.o
	yasm -a x86 -m amd64 -f elf64 0cpu/x86/cpu64user/x64.syscall.caller.asm  -o $(DIR)/x64.syscall.caller_asm.o
	$(CC) $(CF) -mgeneral-regs-only 1arch/x86/bus/input/8042.c               -o $(DIR)/8042.o
	$(CC) $(CF) -mgeneral-regs-only 1arch/x86/vmtool/vmware.c                -o $(DIR)/vmware.o


#---------------------x64 myos-------------------------
myosx64_c = 
myosx64_o = $(patsubst %.c, %.o, $(myosx64_c))

.PHONY:myosx64
myosx64:$(x64_o) $(myosx64_o)
	$(CC) $(CF) -mgeneral-regs-only 0cpu/x86/cpu64user/x64.syscall.caller.c     -o $(DIR)/x64.syscall.caller_c.o
	yasm -a x86 -m amd64 -f elf64 0cpu/x86/cpu64user/x64.syscall.caller.asm -o $(DIR)/x64.syscall.caller_asm.o


#---------------------x64 efi-------------------------
efix64_c = \
2soc/inefi/efi.wire.bt.c \
2soc/inefi/efi.wire.gpio.c \
2soc/inefi/efi.wire.i2c.c \
2soc/inefi/efi.wire.spi.c \
2soc/inefi/efi.wire.uart.c \
2soc/inefi/efi.wire.usb.c \
2soc/inefi/efi.wire.wifi.c \
2soc/inefi/hardware.c
efix64_o = $(patsubst %.c, %.o, $(efix64_c))

.PHONY:efix64
efix64:$(x64_o) $(efix64_o)


#---------------------x64 linux-------------------------
linuxx64_c = \
2soc/inlinux/linux.wire.bt.c \
2soc/inlinux/linux.wire.i2c.c \
2soc/inlinux/linux.wire.gpio.c \
2soc/inlinux/linux.wire.pci.c \
2soc/inlinux/linux.wire.spi.c \
2soc/inlinux/linux.wire.usb.c \
2soc/inlinux/linux.wire.wifi.c \
2soc/inlinux/hardware.c
linuxx64_o = $(patsubst %.c, %.o, $(linuxx64_c))

.PHONY:linuxx64
linuxx64:$(x64_o) $(linuxx64_o)


#---------------------x64 mac-------------------------
macx64_c = \
2soc/inmac/mac.wire.bt.c \
2soc/inmac/mac.wire.i2c.c \
2soc/inmac/mac.wire.gpio.c \
2soc/inmac/mac.wire.pci.c \
2soc/inmac/mac.wire.spi.c \
2soc/inmac/mac.wire.usb.c \
2soc/inmac/mac.wire.wifi.c \
2soc/inmac/hardware.c
macx64_o = $(patsubst %.c, %.o, $(macx64_c))

.PHONY:macx64
macx64:$(x64_o) $(macx64_o)


#---------------------x64 win-------------------------
winx64_c = \
2soc/inwin/win.wire.bt.c \
2soc/inwin/win.wire.i2c.c \
2soc/inwin/win.wire.gpio.c \
2soc/inwin/win.wire.pci.c \
2soc/inwin/win.wire.spi.c \
2soc/inwin/win.wire.usb.c \
2soc/inwin/win.wire.wifi.c \
2soc/inwin/hardware.c
winx64_o = $(patsubst %.c, %.o, $(winx64_c))

.PHONY:winx64
winx64:$(x64_o) $(winx64_o)


#---------------------common-------------------------
%.o: %.c
	if [ $(DIR)/$(notdir $@) -nt $< ]; then \
		true;\
	else \
		echo libhard0/$<;\
		$(CC) $(CF) -o $(DIR)/$(notdir $@) $<;\
	fi


#---------------------clean-------------------------
clean:
	rm -f *.o *.a
