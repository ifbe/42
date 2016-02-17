.PHONY:arm
arm:
	rm -f x86.*.o
	gcc -c -O2 arm/arm.fp.c
	gcc -c -O2 arm/arm.platform.c
	gcc -c -O2 driver.c
.PHONY:mips
mips:
	rm -f x86.*.o
	gcc -c -O2 arm/arm.fp.c
	gcc -c -O2 arm/arm.platform.c
	gcc -c -O2 driver.c
.PHONY:x86
x86:
	rm -f arm.*.o
	gcc -c -O2 x86/x86.endian.c
	gcc -c -O2 x86/x86.fp.c
	gcc -c -O2 x86/x86.platform.c
	gcc -c -O2 x86/x86.port.c
	gcc -c -O2 driver.c
clean:
	rm -f *.o *.a
