compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) debug.c
	$(compiler) $(cflag) event.c
	$(compiler) $(cflag) relation.c
	$(compiler) $(cflag) say.c
	$(compiler) $(cflag) main.c
clean:
	rm -f *.o *.a *.so
