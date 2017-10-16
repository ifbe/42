compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) debug.c
	$(compiler) $(cflag) say.c
	$(compiler) $(cflag) connect.c
	$(compiler) $(cflag) event.c
	$(compiler) $(cflag) main.c
clean:
	rm -f *.o *.a *.so
