compiler ?= gcc
cflag ?= -c -O2
all:
	make -s target compiler="gcc" cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
target:
	$(compiler) $(cflag) command.c
	$(compiler) $(cflag) event.c
	$(compiler) $(cflag) universe.c
clean:
	rm -f *.o
