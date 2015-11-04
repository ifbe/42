binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 background.c
	gcc -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a \
	background.o anscii.o draw.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 background.c
	x86_64-elf-gcc -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a \
	background.o anscii.o draw.o
linux:
	gcc -c -O2 1234/1.hex.c
	gcc -c -O2 1234/2.tree.c
	gcc -c -O2 1234/3.sketchpad.c
	gcc -c -O2 1234/4.console.c
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/anscii.c
	rm -f libui1.a
	ar crs libui1.a \
	1.hex.o 2.tree.o 3.sketchpad.o 4.console.o \
	background.o shape.o \
	menu.o \
	anscii.o
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 background.c
	clang -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a \
	anscii.o background.o draw.o
win:
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 1234/1.hex.c
	gcc -c -O2 1234/2.tree.c
	gcc -c -O2 1234/3.sketchpad.c
	gcc -c -O2 1234/4.console.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/anscii.c
	rm -f libui1.a
	ar crs libui1.a \
	1.hex.o 2.tree.o 3.sketchpad.o 4.console.o \
	background.o shape.o \
	menu.o \
	anscii.o
clean:
	rm -f *.o *.a
