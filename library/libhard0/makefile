extra ?=

.PHONY:arm
arm:
	rm -f arm64.*.o mips.*.o mips64.*.o x86.*.o x64.*.o
	gcc -c -O2 -fPIC $(extra) cpu/arm/arm.fp.c
	gcc -c -O2 -fPIC $(extra) cpu/arm/arm.lock.c
	gcc -c -O2 -fPIC $(extra) driver.c

.PHONY:arm64
arm64:
	rm -f arm.*.o mips.*.o mips64.*.o x86.*.o x64.*.o
	gcc -c -O2 -fPIC $(extra) cpu/arm64/arm64.fp.c
	gcc -c -O2 -fPIC $(extra) cpu/arm64/arm64.lock.c
	gcc -c -O2 -fPIC $(extra) driver.c

.PHONY:mips
mips:
	rm -f arm.*.o arm64.*.o mips64.*.o x86.*.o x64.*.o
	gcc -c -O2 -fPIC $(extra) cpu/mips/mips.fp.c
	gcc -c -O2 -fPIC $(extra) cpu/mips/mips.lock.c
	gcc -c -O2 -fPIC $(extra) driver.c

.PHONY:mips64
mips64:
	rm -f arm.*.o arm64.*.o mips.*.o x86.*.o x64.*.o
	gcc -c -O2 -fPIC $(extra) cpu/mips64/mips64.fp.c
	gcc -c -O2 -fPIC $(extra) cpu/mips64/mips64.lock.c
	gcc -c -O2 -fPIC $(extra)driver.c

.PHONY:x86
x86:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x64.*.o
	gcc -c -O2 -fPIC $(extra) cpu/x86/x86.endian.c
	gcc -c -O2 -fPIC $(extra) cpu/x86/x86.fp.c
	gcc -c -O2 -fPIC $(extra) cpu/x86/x86.lock.c
	gcc -c -O2 -fPIC $(extra) cpu/x86/x86.port.c
	gcc -c -O2 -fPIC $(extra) driver.c

.PHONY:x64
x64:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x86.*.o
	gcc -c -O2 -fPIC $(extra) cpu/x64/x64.endian.c
	gcc -c -O2 -fPIC $(extra) cpu/x64/x64.fp.c
	gcc -c -O2 -fPIC $(extra) cpu/x64/x64.lock.c
	gcc -c -O2 -fPIC $(extra) cpu/x64/x64.port.c
	gcc -c -O2 -fPIC $(extra) driver.c

.PHONY:winx86
winx86:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x64.*.o
	gcc -c -O2 cpu/x86/x86.endian.c
	gcc -c -O2 cpu/x86/x86.fp.c
	gcc -c -O2 cpu/x86/x86.lock.c
	gcc -c -O2 cpu/x86/x86.port.c
	gcc -c -O2 driver.c

.PHONY:winx64
winx64:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x86.*.o
	gcc -c -O2 cpu/x64/x64.endian.c
	gcc -c -O2 cpu/x64/x64.fp.c
	gcc -c -O2 cpu/x64/x64.lock.c
	gcc -c -O2 cpu/x64/x64.port.c
	gcc -c -O2 driver.c

clean:
	rm -f *.o *.a
