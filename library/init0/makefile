compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:target
target:
	rm -f *.o
	$(compiler) $(cflag) startapp.c

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) startbare.c

clean:
	rm -f *.o *.o.cmd
