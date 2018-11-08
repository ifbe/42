compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) main.c
	$(compiler) $(cflag) loop.c
	$(compiler) $(cflag) libev.c
	$(compiler) $(cflag) libin.c
	$(compiler) $(cflag) libnode.c
	$(compiler) $(cflag) libout.c
	$(compiler) $(cflag) librel.c
	$(compiler) $(cflag) libterm.c
clean:
	rm -f *.o *.a *.so
