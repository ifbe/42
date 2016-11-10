compiler ?= gcc
cflag ?= -c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

.PHONY:linux
linux:
	make -s target compiler="gcc -fPIC"

.PHONY:mac
mac:
	make -s target compiler="clang -fPIC"

.PHONY:win
win:
	make -s target compiler="gcc"

.PHONY:bare
bare:
	rm -f *.o
	gcc -c -O2 -fPIC bare/start.c
	gcc -c -O2 -fPIC bare/event.c

.PHONY:target
target:
	rm -f *.o
	$(compiler) $(cflag) world.c
	$(compiler) $(cflag) event.c

clean:
	rm -f *.o
