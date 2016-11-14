compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:arm
arm:
	rm -f arm64.*.o mips.*.o mips64.*.o x86.*.o x64.*.o
	$(compiler) $(cflag) cpu/arm/arm.fp.c
	$(compiler) $(cflag) cpu/arm/arm.lock.c
	$(compiler) $(cflag) driver.c

.PHONY:arm64
arm64:
	rm -f arm.*.o mips.*.o mips64.*.o x86.*.o x64.*.o
	$(compiler) $(cflag) cpu/arm64/arm64.fp.c
	$(compiler) $(cflag) cpu/arm64/arm64.lock.c
	$(compiler) $(cflag) driver.c

.PHONY:mips
mips:
	rm -f arm.*.o arm64.*.o mips64.*.o x86.*.o x64.*.o
	$(compiler) $(cflag) cpu/mips/mips.fp.c
	$(compiler) $(cflag) cpu/mips/mips.lock.c
	$(compiler) $(cflag) driver.c

.PHONY:mips64
mips64:
	rm -f arm.*.o arm64.*.o mips.*.o x86.*.o x64.*.o
	$(compiler) $(cflag) cpu/mips64/mips64.fp.c
	$(compiler) $(cflag) cpu/mips64/mips64.lock.c
	$(compiler) $(cflag) driver.c

.PHONY:x86
x86:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x64.*.o
	$(compiler) $(cflag) cpu/x86/x86.endian.c
	$(compiler) $(cflag) cpu/x86/x86.fp.c
	$(compiler) $(cflag) cpu/x86/x86.lock.c
	$(compiler) $(cflag) cpu/x86/x86.port.c
	$(compiler) $(cflag) driver.c

.PHONY:x64
x64:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x86.*.o
	$(compiler) $(cflag) cpu/x64/x64.endian.c
	$(compiler) $(cflag) cpu/x64/x64.fp.c
	$(compiler) $(cflag) cpu/x64/x64.lock.c
	$(compiler) $(cflag) cpu/x64/x64.port.c
	$(compiler) $(cflag) driver.c

.PHONY:winx86
winx86:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x64.*.o
	$(compiler) $(cflag) cpu/x86/x86.endian.c
	$(compiler) $(cflag) cpu/x86/x86.fp.c
	$(compiler) $(cflag) cpu/x86/x86.lock.c
	$(compiler) $(cflag) cpu/x86/x86.port.c
	$(compiler) $(cflag) driver.c

.PHONY:winx64
winx64:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x86.*.o
	$(compiler) $(cflag) cpu/x64/x64.endian.c
	$(compiler) $(cflag) cpu/x64/x64.fp.c
	$(compiler) $(cflag) cpu/x64/x64.lock.c
	$(compiler) $(cflag) cpu/x64/x64.port.c
	$(compiler) $(cflag) driver.c

clean:
	rm -f *.o *.a
