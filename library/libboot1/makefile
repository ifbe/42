compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) main.c
	$(compiler) $(cflag) libev.c
	$(compiler) $(cflag) libnode.c
	$(compiler) $(cflag) librel.c
	$(compiler) $(cflag) libterm.c
	$(compiler) $(cflag) stdin.c
	$(compiler) $(cflag) stdout.c
clean:
	rm -f *.o *.a *.so
