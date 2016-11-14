compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) draw/draw.ascii.c
	$(compiler) $(cflag) draw/draw.background.c
	$(compiler) $(cflag) draw/draw.shape.c
	$(compiler) $(cflag) draw/draw.unicode.c
	$(compiler) $(cflag) draw/image.blur.c
	$(compiler) $(cflag) draw/image.channel.c
	$(compiler) $(cflag) draw/image.gray.c
	$(compiler) $(cflag) draw/image.mosaic.c
	$(compiler) $(cflag) game/game.2048.c
	$(compiler) $(cflag) game/game.ooxx.c
	$(compiler) $(cflag) game/game.snake.c
	$(compiler) $(cflag) game/game.sudoku.c
	$(compiler) $(cflag) game/game.tetris.c
	$(compiler) $(cflag) game/game.weiqi.c
	$(compiler) $(cflag) game/game.xiangqi.c
	$(compiler) $(cflag) menu/menu.center.c
	$(compiler) $(cflag) menu/menu.virtkbd.c
	$(compiler) $(cflag) menu/menu.roster.c
	$(compiler) $(cflag) test/test.color.c
	$(compiler) $(cflag) test/test.doodle.c
	$(compiler) $(cflag) test/test.font.c
	$(compiler) $(cflag) test/test.pure.c
	$(compiler) $(cflag) tool/tool.calculator.c
	$(compiler) $(cflag) tool/tool.console.c
	$(compiler) $(cflag) tool/tool.control.c
	$(compiler) $(cflag) tool/tool.hex.c
	$(compiler) $(cflag) tool/tool.qrcode.c
	$(compiler) $(cflag) tool/tool.sketchpad.c
	$(compiler) $(cflag) tool/tool.spectrum.c
	$(compiler) $(cflag) tool/tool.tree.c
	$(compiler) $(cflag) character.c
clean:
	rm -f *.o *.a
