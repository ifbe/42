binary:
	gcc -c -fPIC -O2 draw/draw.background.c
	gcc -c -fPIC -O2 draw/draw.shape.c
	gcc -c -fPIC -O2 menu/menu.c
	gcc -c -fPIC -O2 text/text.ascii.c
	gcc -c -fPIC -O2 text/text.unicode.c
	gcc -c -fPIC -O2 character.c
x86_64-elf-gcc:
	x86_64-elf-gcc -c -fPIC -O2 draw/draw.background.c
	x86_64-elf-gcc -c -fPIC -O2 draw/draw.shape.c
	x86_64-elf-gcc -c -fPIC -O2 menu/menu.c
	x86_64-elf-gcc -c -fPIC -O2 text/text.ascii.c
	x86_64-elf-gcc -c -fPIC -O2 text/text.unicode.c
	x86_64-elf-gcc -c -fPIC -O2 character.c
mingw64:
	gcc -c -O2 draw/draw.background.c
	gcc -c -O2 draw/draw.shape.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/text.ascii.c
	gcc -c -O2 text/text.unicode.c
	gcc -c -O2 character.c
gcc:
	gcc -c -O2 draw/draw.background.c
	gcc -c -O2 draw/draw.shape.c
	gcc -c -O2 game/game.2048.c
	gcc -c -O2 game/game.snake.c
	gcc -c -O2 game/game.tetris.c
	gcc -c -O2 image/image.fuzzy.c
	gcc -c -O2 image/image.gray.c
	gcc -c -O2 menu/menu.c
	gcc -c -O2 text/text.ascii.c
	gcc -c -O2 text/text.unicode.c
	gcc -c -O2 tool/tool.console.c
	gcc -c -O2 tool/tool.hex.c
	gcc -c -O2 tool/tool.keyboard.c
	gcc -c -O2 tool/tool.sketchpad.c
	gcc -c -O2 tool/tool.tree.c
	gcc -c -O2 character.c

clang:
	clang -c -O2 draw/draw.background.c
	clang -c -O2 draw/draw.shape.c
	clang -c -O2 menu/menu.c
	clang -c -O2 text/text.ascii.c
	clang -c -O2 text/text.unicode.c
	clang -c -O2 character.c
clean:
	rm -f *.o *.a
