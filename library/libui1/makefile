binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 background.c
	gcc -c -fPIC -O2 draw.c
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 background.c
	x86_64-elf-gcc -c -fPIC -O2 draw.c
gcc:
	gcc -c -O2 palette.c
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/anscii.c
clang:
	clang -c -O2 palette.c
	clang -c -O2 draw/background.c
	clang -c -O2 draw/shape.c
	clang -c -O2 menu/menu.c
	clang -c -O2 text/anscii.c
clean:
	rm -f *.o *.a
