compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) draw/draw.background.c
	$(compiler) $(cflag) draw/draw.shape.c
	$(compiler) $(cflag) game/game.2048.c
	$(compiler) $(cflag) game/game.snake.c
	$(compiler) $(cflag) game/game.tetris.c
	$(compiler) $(cflag) menu/menu.center.c
	$(compiler) $(cflag) print/print.ascii.c
	$(compiler) $(cflag) print/print.unicode.c
	$(compiler) $(cflag) tool/tool.color.c
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
