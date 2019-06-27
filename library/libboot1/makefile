compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) main.c
	$(compiler) $(cflag) prep/prep.c
	$(compiler) $(cflag) prep/prep.default.c
	$(compiler) $(cflag) prep/prep.myml.c
	$(compiler) $(cflag) prep/prep.mython.c
	$(compiler) $(cflag) loop/loop.c
	$(compiler) $(cflag) loop/loop.test.c
	$(compiler) $(cflag) std/stdev.c
	$(compiler) $(cflag) std/stdin.c
	$(compiler) $(cflag) std/stdout.c
	$(compiler) $(cflag) std/stdrel.c
	$(compiler) $(cflag) term/cmdev.c
	$(compiler) $(cflag) term/cmdnode.c
	$(compiler) $(cflag) term/cmdrel.c
	$(compiler) $(cflag) term/cmdscene.c
	$(compiler) $(cflag) term/term.c
clean:
	rm -f *.o *.a *.so
