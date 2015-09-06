binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 background.c
	rm -f libui.a
	ar crs libui.a anscii.o background.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 background.c
	rm -f libui.a
	ar crs libui.a anscii.o background.o
linux:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 background.c
	rm -f libui.a
	ar crs libui.a anscii.o background.o
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 background.c
	rm -f libui.a
	ar crs libui.a anscii.o background.o
win:
	gcc -c -O2 anscii.c
	gcc -c -O2 background.c
	rm -f libui.a
	ar crs libui.a anscii.o background.o
clean:
	rm -f *.o *.a
