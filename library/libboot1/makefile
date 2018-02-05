compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) main.c
	$(compiler) $(cflag) stdin.c
	$(compiler) $(cflag) stdout.c
	$(compiler) $(cflag) stdev.c
	$(compiler) $(cflag) stdrel.c
clean:
	rm -f *.o *.a *.so
