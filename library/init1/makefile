compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:target
target:
	rm -f *.o
	$(compiler) $(cflag) connect.c
	$(compiler) $(cflag) event.c
	$(compiler) $(cflag) main.c

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) connect.c
	$(compiler) $(cflag) event.c
	$(compiler) $(cflag) main.c

clean:
	rm -f *.o *.o.cmd
