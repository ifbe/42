binary:
	rm -f *.o
	gcc -c -fPIC -O2 draw/background.c
	gcc -c -fPIC -O2 draw/shape.c
	gcc -c -fPIC -O2 text/ascii.c
	gcc -c -fPIC -O2 text/unicode.c
	gcc -c -fPIC -O2 character.c
x86_64-elf-gcc:
	rm -f *.o
	x86_64-elf-gcc -c -fPIC -O2 draw/background.c
	x86_64-elf-gcc -c -fPIC -O2 draw/shape.c
	x86_64-elf-gcc -c -fPIC -O2 text/ascii.c
	x86_64-elf-gcc -c -fPIC -O2 text/unicode.c
	x86_64-elf-gcc -c -fPIC -O2 character.c
mingw64:
	rm -f *.o
	#objcopy -I binary -O pei-x86-64 -B i386 unicode.unicode unicodetable.o
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 text/ascii.c
	gcc -c -O2 text/unicode.c
	gcc -c -O2 character.c
gcc:
	rm -f *.o
	#objcopy -I binary -O elf64-x86-64 -B i386 unicode.unicode unicodetable.o
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 text/ascii.c
	gcc -c -O2 text/unicode.c
	gcc -c -O2 character.c
clang:
	rm -f *.o
	clang -c -O2 draw/background.c
	clang -c -O2 draw/shape.c
	clang -c -O2 text/ascii.c
	clang -c -O2 text/unicode.c
	clang -c -O2 character.c
clean:
	rm -f *.o *.a
