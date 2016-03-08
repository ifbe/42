target:
	$(compiler) $(cflag) debug.c
	$(compiler) $(cflag) listen.c
	$(compiler) $(cflag) say.c
gcc:
	make -s target compiler="gcc" cflag="-c -fPIC -O2"
mingw:
	make -s target compiler="gcc" cflag="-c -O2"
clean:
	rm -f *.o *.a *.so
