compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) body.c
	$(compiler) $(cflag) pci.c
	$(compiler) $(cflag) usb.c
	$(compiler) $(cflag) i2c.c
gcc:
	make -s compiler="gcc" cflag="-c -fPIC -O2"
mingw:
	make -s compiler="gcc" cflag="-c -O2"
clean:
	rm -f *.o
