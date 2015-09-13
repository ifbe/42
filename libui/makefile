binary:
	gcc -c -fPIC -O2 anscii.c
	rm -f libui.a
	ar crs libui.a anscii.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	rm -f libui.a
	ar crs libui.a anscii.o
linux:
	gcc -c -fPIC -O2 anscii.c
	rm -f libui.a
	ar crs libui.a anscii.o
mac:
	clang -c -fPIC -O2 anscii.c
	rm -f libui.a
	ar crs libui.a anscii.o
win:
	gcc -c -O2 anscii.c
	rm -f libui.a
	ar crs libui.a anscii.o
clean:
	rm -f *.o *.a
