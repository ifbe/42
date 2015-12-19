binary:
	gcc -c -fPIC -O2 draw/background.c
	gcc -c -fPIC -O2 draw/shape.c
	gcc -c -fPIC -O2 text/anscii.c
	gcc -c -fPIC -O2 palette.c
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 draw/background.c
	x86_64-elf-gcc -c -fPIC -O2 draw/shape.c
	x86_64-elf-gcc -c -fPIC -O2 text/anscii.c
	x86_64-elf-gcc -c -fPIC -O2 palette.c
gcc:
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 text/anscii.c
	gcc -c -O2 palette.c
	objcopy -I binary -O elf64-x86-64 -B i386 unicode.unicode unicode.o
clang:
	clang -c -O2 draw/background.c
	clang -c -O2 draw/shape.c
	clang -c -O2 text/anscii.c
	clang -c -O2 palette.c
clean:
	rm -f *.o *.a
