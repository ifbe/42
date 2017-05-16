compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) say.c
	$(compiler) $(cflag) event.c
	$(compiler) $(cflag) debug.c
clean:
	rm -f *.o *.a *.so
