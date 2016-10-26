compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) debug.c
clang:
	make -s target compiler="clang" cflag="-c -fPIC -O2"
gcc:
	make -s target compiler="gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
mingw:
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clean:
	rm -f *.o *.a *.so
