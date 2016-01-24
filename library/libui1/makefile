binary:
	gcc -c -fPIC -O2 draw/background.c
	gcc -c -fPIC -O2 draw/shape.c
	gcc -c -fPIC -O2 menu/menu.c
	gcc -c -fPIC -O2 text/ascii.c
	gcc -c -fPIC -O2 text/unicode.c
	gcc -c -fPIC -O2 character.c
x86_64-elf-gcc:
	x86_64-elf-gcc -c -fPIC -O2 draw/background.c
	x86_64-elf-gcc -c -fPIC -O2 draw/shape.c
	x86_64-elf-gcc -c -fPIC -O2 menu/menu.c
	x86_64-elf-gcc -c -fPIC -O2 text/ascii.c
	x86_64-elf-gcc -c -fPIC -O2 text/unicode.c
	x86_64-elf-gcc -c -fPIC -O2 character.c
mingw64:
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/ascii.c
	gcc -c -O2 text/unicode.c
	gcc -c -O2 character.c
gcc:
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 game/2048.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/ascii.c
	gcc -c -O2 text/unicode.c
	gcc -c -O2 tool/hex.c
	gcc -c -O2 tool/keyboard.c
	gcc -c -O2 tool/tree.c
	gcc -c -O2 tool/sketchpad.c
	gcc -c -O2 tool/console.c
	gcc -c -O2 character.c

clang:
	clang -c -O2 draw/background.c
	clang -c -O2 draw/shape.c
	clang -c -O2 menu/menu.c
	clang -c -O2 text/ascii.c
	clang -c -O2 text/unicode.c
	clang -c -O2 character.c
clean:
	rm -f *.o *.a
