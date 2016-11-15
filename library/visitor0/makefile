compiler ?= gcc
cflag ?= -c -O2 -fPIC

.PHONY:target
target:
	rm -f *.o
	$(compiler) $(cflag) app/world.c
	$(compiler) $(cflag) app/event.c

.PHONY:bare
bare:
	rm -f *.o
	$(compiler) $(cflag) bare/start.c
	$(compiler) $(cflag) bare/event.c

clean:
	rm -f *.o
