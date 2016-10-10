compiler ?= gcc
cflag ?= -c -O2
linux:
	make -s target compiler="gcc" cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
mac:
	make -s target compiler="clang" cflag="-c -O2 -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
win:
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
target:
	$(compiler) $(cflag) world.c
	$(compiler) $(cflag) event.c
	$(compiler) $(cflag) command.c
	$(compiler) $(cflag) showoff.c
clean:
	rm -f *.o
