compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) body.c
	$(compiler) $(cflag) pci.c
	$(compiler) $(cflag) usb.c
	$(compiler) $(cflag) i2c.c
clean:
	rm -f *.o
