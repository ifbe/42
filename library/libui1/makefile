compiler ?= gcc
cflag ?= -c -O2
extra ?=
target:
	$(compiler) $(cflag) $(extra) character.c
	$(compiler) $(cflag) $(extra) draw/draw.ascii.c
	$(compiler) $(cflag) $(extra) draw/draw.background.c
	$(compiler) $(cflag) $(extra) draw/draw.shape.c
	$(compiler) $(cflag) $(extra) draw/draw.unicode.c
	$(compiler) $(cflag) $(extra) draw/image.blur.c
	$(compiler) $(cflag) $(extra) draw/image.channel.c
	$(compiler) $(cflag) $(extra) draw/image.gray.c
	$(compiler) $(cflag) $(extra) draw/image.mosaic.c
	$(compiler) $(cflag) $(extra) game/game.2048.c
	$(compiler) $(cflag) $(extra) game/game.ooxx.c
	$(compiler) $(cflag) $(extra) game/game.snake.c
	$(compiler) $(cflag) $(extra) game/game.sudoku.c
	$(compiler) $(cflag) $(extra) game/game.tetris.c
	$(compiler) $(cflag) $(extra) game/game.weiqi.c
	$(compiler) $(cflag) $(extra) game/game.xiangqi.c
	$(compiler) $(cflag) $(extra) menu/menu.center.c
	$(compiler) $(cflag) $(extra) menu/menu.virtkbd.c
	$(compiler) $(cflag) $(extra) menu/menu.roster.c
	$(compiler) $(cflag) $(extra) test/test.color.c
	$(compiler) $(cflag) $(extra) test/test.doodle.c
	$(compiler) $(cflag) $(extra) test/test.font.c
	$(compiler) $(cflag) $(extra) test/test.pure.c
	$(compiler) $(cflag) $(extra) tool/tool.calculator.c
	$(compiler) $(cflag) $(extra) tool/tool.console.c
	$(compiler) $(cflag) $(extra) tool/tool.control.c
	$(compiler) $(cflag) $(extra) tool/tool.hex.c
	$(compiler) $(cflag) $(extra) tool/tool.qrcode.c
	$(compiler) $(cflag) $(extra) tool/tool.sketchpad.c
	$(compiler) $(cflag) $(extra) tool/tool.spectrum.c
	$(compiler) $(cflag) $(extra) tool/tool.tree.c
binary:
	make -s target compiler="gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
x86_64-elf-gcc:
	make -s target compiler="x86_64-elf-gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
gcc:
	make -s target compiler="gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
mingw:
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clang:
	make -s target compiler="clang" cflag="-c -O2"
clean:
	rm -f *.o *.a
