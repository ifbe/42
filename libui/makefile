binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 draw.c
	rm -f libui.a
	ar crs libui.a anscii.o draw.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 draw.c
	rm -f libui.a
	ar crs libui.a anscii.o draw.o
linux:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 draw.c
	rm -f libui.a
	ar crs libui.a anscii.o draw.o
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 draw.c
	rm -f libui.a
	ar crs libui.a anscii.o draw.o
win:
	gcc -c -O2 anscii.c
	gcc -c -O2 draw.c
	rm -f libui.a
	ar crs libui.a anscii.o draw.o
clean:
	rm -f *.o *.a
