compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) worker.c
	$(compiler) $(cflag) prep/prep.c
	$(compiler) $(cflag) prep/term.c
	$(compiler) $(cflag) prep/prep.default.c
	$(compiler) $(cflag) prep/prep.myml.c
	$(compiler) $(cflag) prep/prep.mython.c
	$(compiler) $(cflag) loop/loop.c
	$(compiler) $(cflag) loop/loop.test.c
	$(compiler) $(cflag) std/stdev.c
	$(compiler) $(cflag) std/stdin.c
	$(compiler) $(cflag) std/stdout.c
	$(compiler) $(cflag) std/stdrel.c
clean:
	rm -f *.o *.a *.so
