compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) draw/draw.ascii.c
	$(compiler) $(cflag) draw/draw.background.c
	$(compiler) $(cflag) draw/draw.shape.c
	$(compiler) $(cflag) draw/draw.unicode.c
	$(compiler) $(cflag) game/game.2048.c
	$(compiler) $(cflag) game/game.snake.c
	$(compiler) $(cflag) game/game.tetris.c
	$(compiler) $(cflag) menu/menu.center.c
	$(compiler) $(cflag) menu/menu.upper.c
	$(compiler) $(cflag) menu/menu.nether.c
	$(compiler) $(cflag) menu/menu.left.c
	$(compiler) $(cflag) menu/menu.right.c
	$(compiler) $(cflag) test/test.color.c
	$(compiler) $(cflag) test/test.doodle.c
	$(compiler) $(cflag) test/test.font.c
	$(compiler) $(cflag) tool/tool.console.c
	$(compiler) $(cflag) tool/tool.hex.c
	$(compiler) $(cflag) tool/tool.keyboard.c
	$(compiler) $(cflag) tool/tool.qrcode.c
	$(compiler) $(cflag) tool/tool.sketchpad.c
	$(compiler) $(cflag) tool/tool.spectrum.c
	$(compiler) $(cflag) tool/tool.tree.c
	$(compiler) $(cflag) character.c
binary:
	make -s target compiler="gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
x86_64-elf-gcc:
	make -s target compiler="x86_64-elf-gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
gcc:
	make -s target compiler="gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
mingw:
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clang:
	make -s target compiler="clang" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clean:
	rm -f *.o *.a
