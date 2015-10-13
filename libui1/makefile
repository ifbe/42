binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a anscii.o draw.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a anscii.o draw.o
linux:
	gcc -c -O2 anscii.c
	gcc -c -O2 draw.c
	gcc -c -O2 background.c
	gcc -c -O2 esc.c
	gcc -c -O2 f1.c
	gcc -c -O2 f2.c
	gcc -c -O2 f3.c
	gcc -c -O2 f4.c
	rm -f libui1.a
	ar crs libui1.a \
	anscii.o draw.o \
	background.o esc.o f1.o f2.o f3.o f4.o
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a anscii.o draw.o
win:
	gcc -c -O2 anscii.c
	gcc -c -O2 draw.c
	gcc -c -O2 background.c
	gcc -c -O2 esc.c
	gcc -c -O2 f1.c
	gcc -c -O2 f2.c
	gcc -c -O2 f3.c
	gcc -c -O2 f4.c
	rm -f libui1.a
	ar crs libui1.a \
	anscii.o draw.o \
	background.o esc.o f1.o f2.o f3.o f4.o
clean:
	rm -f *.o *.a
