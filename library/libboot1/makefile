compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) worker.c
	$(compiler) $(cflag) work/work.compile.c
	$(compiler) $(cflag) work/work.kernel.c
	$(compiler) $(cflag) work/work.myml.c
	$(compiler) $(cflag) work/work.mython.c
	$(compiler) $(cflag) loop/loop.exiter.c
	$(compiler) $(cflag) loop/loop.poller.c
	$(compiler) $(cflag) loop/loop.pulser.c
	$(compiler) $(cflag) loop/loop.realer.c
	$(compiler) $(cflag) loop/loop.waiter.c
	$(compiler) $(cflag) std/stdev.c
	$(compiler) $(cflag) std/stdin.c
	$(compiler) $(cflag) std/stdout.c
	$(compiler) $(cflag) std/stdrel.c
clean:
	rm -f *.o *.a *.so
