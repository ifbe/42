binary:
	rm -f *.o
	gcc -c -fPIC -O2 draw/background.c
	gcc -c -fPIC -O2 draw/shape.c
	gcc -c -fPIC -O2 text/anscii.c
	gcc -c -fPIC -O2 text/utf.c
	gcc -c -fPIC -O2 pixel.c
crosscompile:
	rm -f *.o
	x86_64-elf-gcc -c -fPIC -O2 draw/background.c
	x86_64-elf-gcc -c -fPIC -O2 draw/shape.c
	x86_64-elf-gcc -c -fPIC -O2 text/anscii.c
	x86_64-elf-gcc -c -fPIC -O2 text/utf.c
	x86_64-elf-gcc -c -fPIC -O2 pixel.c
mingw64:
	rm -f *.o
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 text/anscii.c
	gcc -c -O2 text/utf.c
	gcc -c -O2 pixel.c
	objcopy -I binary -O pei-x86-64 -B i386 unicode.unicode unicodetable.o
gcc:
	rm -f *.o
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 text/anscii.c
	gcc -c -O2 text/utf.c
	gcc -c -O2 pixel.c
	objcopy -I binary -O elf64-x86-64 -B i386 unicode.unicode unicodetable.o
clang:
	rm -f *.o
	clang -c -O2 draw/background.c
	clang -c -O2 draw/shape.c
	clang -c -O2 text/anscii.c
	clang -c -O2 text/utf.c
	clang -c -O2 pixel.c
clean:
	rm -f *.o *.a
