compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:arm
arm:
	rm -f *.o
	$(compiler) $(cflag) cpu/arm/arm.fp.c
	$(compiler) $(cflag) device.c

.PHONY:arm64
arm64:
	rm -f *.o
	$(compiler) $(cflag) cpu/arm64/arm64.fp.c
	$(compiler) $(cflag) device.c

.PHONY:mips
mips:
	rm -f *.o
	$(compiler) $(cflag) cpu/mips/mips.fp.c
	$(compiler) $(cflag) device.c

.PHONY:mips64
mips64:
	rm -f *.o
	$(compiler) $(cflag) cpu/mips64/mips64.fp.c
	$(compiler) $(cflag) device.c

.PHONY:x86
x86:
	rm -f *.o
	$(compiler) $(cflag) cpu/x86/x86.endian.c
	$(compiler) $(cflag) cpu/x86/x86.fp.c
	$(compiler) $(cflag) cpu/x86/x86.port.c
	$(compiler) $(cflag) device.c

.PHONY:x64
x64:
	rm -f *.o
	$(compiler) $(cflag) cpu/x64/x64.endian.c
	$(compiler) $(cflag) cpu/x64/x64.fp.c
	$(compiler) $(cflag) cpu/x64/x64.port.c
	$(compiler) $(cflag) device.c

.PHONY:barex64
barex64:
	rm -f *.o
	$(compiler) $(cflag) cpu/x64/x64.endian.c
	$(compiler) $(cflag) cpu/x64/x64.fp.c
	$(compiler) $(cflag) cpu/x64/x64.port.c
	$(compiler) $(cflag) cpu/x64/x64.asm.c
	$(compiler) $(cflag) cpu/x64/x64.page.c
	$(compiler) $(cflag) cpu/x64/x64.gdt.c
	$(compiler) $(cflag) -mgeneral-regs-only cpu/x64/x64.idt.c
	$(compiler) $(cflag) irq/8259.c
	#$(compiler) $(cflag) irq/apic.c
	$(compiler) $(cflag) bus/input/8042.c
	$(compiler) $(cflag) bus/pci/pci.c
	$(compiler) $(cflag) timer/825x.c
	$(compiler) $(cflag) timer/rtc.c
	$(compiler) $(cflag) bare.c

.PHONY:winx86
winx86:
	rm -f *.o
	$(compiler) $(cflag) cpu/x86/x86.endian.c
	$(compiler) $(cflag) cpu/x86/x86.fp.c
	$(compiler) $(cflag) cpu/x86/x86.port.c
	$(compiler) $(cflag) device.c

.PHONY:winx64
winx64:
	rm -f *.o
	$(compiler) $(cflag) cpu/x64/x64.endian.c
	$(compiler) $(cflag) cpu/x64/x64.fp.c
	$(compiler) $(cflag) cpu/x64/x64.port.c
	$(compiler) $(cflag) device.c

clean:
	rm -f *.o *.a
