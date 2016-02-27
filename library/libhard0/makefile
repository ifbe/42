.PHONY:arm
arm:
	rm -f arm64.*.o mips.*.o mips64.*.o x86.*.o x64.*.o
	gcc -c -fPIC -O2 cpu/arm/arm.fp.c
	gcc -c -fPIC -O2 cpu/arm/arm.platform.c
	gcc -c -fPIC -O2 driver.c
.PHONY:arm64
arm64:
	rm -f arm.*.o mips.*.o mips64.*.o x86.*.o x64.*.o
	gcc -c -fPIC -O2 cpu/arm64/arm64.fp.c
	gcc -c -fPIC -O2 cpu/arm64/arm64.platform.c
	gcc -c -fPIC -O2 driver.c
.PHONY:mips
mips:
	rm -f arm.*.o arm64.*.o mips64.*.o x86.*.o x64.*.o
	gcc -c -fPIC -O2 cpu/mips/mips.fp.c
	gcc -c -fPIC -O2 cpu/mips/mips.platform.c
	gcc -c -fPIC -O2 driver.c
.PHONY:mips64
mips64:
	rm -f arm.*.o arm64.*.o mips.*.o x86.*.o x64.*.o
	gcc -c -fPIC -O2 cpu/mips64/mips64.fp.c
	gcc -c -fPIC -O2 cpu/mips64/mips64.platform.c
	gcc -c -fPIC -O2 driver.c
.PHONY:x86
x86:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x64.*.o
	gcc -c -fPIC -O2 cpu/x86/x86.endian.c
	gcc -c -fPIC -O2 cpu/x86/x86.fp.c
	gcc -c -fPIC -O2 cpu/x86/x86.platform.c
	gcc -c -fPIC -O2 cpu/x86/x86.port.c
	gcc -c -fPIC -O2 driver.c
.PHONY:x64
x64:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x86.*.o
	gcc -c -fPIC -O2 cpu/x64/x64.endian.c
	gcc -c -fPIC -O2 cpu/x64/x64.fp.c
	gcc -c -fPIC -O2 cpu/x64/x64.platform.c
	gcc -c -fPIC -O2 cpu/x64/x64.port.c
	gcc -c -fPIC -O2 driver.c
.PHONY:winx64
winx64:
	rm -f arm.*.o arm64.*.o mips.*.o mips64.*.o x86.*.o
	gcc -c -O2 cpu/x64/x64.endian.c
	gcc -c -O2 cpu/x64/x64.fp.c
	gcc -c -O2 cpu/x64/x64.platform.c
	gcc -c -O2 cpu/x64/x64.port.c
	gcc -c -O2 driver.c
clean:
	rm -f *.o *.a
