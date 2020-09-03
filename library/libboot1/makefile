CC:=gcc
CF:=-c -O2 -fPIC

target:
	$(CC) $(CF) worker.c
	$(CC) $(CF) work/work.compile.c
	$(CC) $(CF) work/work.kernel.c
	$(CC) $(CF) work/work.myml.c
	$(CC) $(CF) work/work.mython.c
	$(CC) $(CF) loop/loop.exiter.c
	$(CC) $(CF) loop/loop.poller.c
	$(CC) $(CF) loop/loop.pulser.c
	$(CC) $(CF) loop/loop.realer.c
	$(CC) $(CF) loop/loop.waiter.c
	$(CC) $(CF) std/stdev.c
	$(CC) $(CF) std/stdin.c
	$(CC) $(CF) std/stdout.c
	$(CC) $(CF) std/stdrel.c
clean:
	rm -f *.o *.a *.so
