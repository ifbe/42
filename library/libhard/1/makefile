compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) driver.c
	$(compiler) $(cflag) info/acpi.c
	$(compiler) $(cflag) info/dts.c
clean:
	rm -f *.o
