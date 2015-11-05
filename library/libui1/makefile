binary:
	gcc -c -fPIC -O2 anscii.c
	gcc -c -fPIC -O2 background.c
	gcc -c -fPIC -O2 draw.c
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 anscii.c
	x86_64-elf-gcc -c -fPIC -O2 background.c
	x86_64-elf-gcc -c -fPIC -O2 draw.c
linux:
	gcc -c -O2 1234/1.hex.c
	gcc -c -O2 1234/2.tree.c
	gcc -c -O2 1234/3.sketchpad.c
	gcc -c -O2 1234/4.console.c
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/anscii.c
mac:
	clang -c -fPIC -O2 anscii.c
	clang -c -fPIC -O2 background.c
	clang -c -fPIC -O2 draw.c
win:
	gcc -c -O2 draw/background.c
	gcc -c -O2 draw/shape.c
	gcc -c -O2 1234/1.hex.c
	gcc -c -O2 1234/2.tree.c
	gcc -c -O2 1234/3.sketchpad.c
	gcc -c -O2 1234/4.console.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/anscii.c
clean:
	rm -f *.o *.a
