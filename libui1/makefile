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
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a anscii.o draw.o
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a anscii.o draw.o
win:
	gcc -c -O2 anscii.c
	gcc -c -O2 draw.c
	rm -f libui1.a
	ar crs libui1.a anscii.o draw.o
clean:
	rm -f *.o *.a
