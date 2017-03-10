compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) game/game.2048.c
	$(compiler) $(cflag) game/game.ooxx.c
	$(compiler) $(cflag) game/game.snake.c
	$(compiler) $(cflag) game/game.sudoku.c
	$(compiler) $(cflag) game/game.tetris.c
	$(compiler) $(cflag) game/game.weiqi.c
	$(compiler) $(cflag) game/game.xiangqi.c
	$(compiler) $(cflag) hack/hack.browse.c
	$(compiler) $(cflag) hack/hack.console.c
	$(compiler) $(cflag) hack/hack.editor.c
	$(compiler) $(cflag) hack/hack.hex.c
	$(compiler) $(cflag) hack/hack.input.c
	$(compiler) $(cflag) hack/hack.tree.c
	$(compiler) $(cflag) link/link.door.c
	$(compiler) $(cflag) link/link.friend.c
	$(compiler) $(cflag) menu/menu.center.c
	$(compiler) $(cflag) menu/menu.virtkbd.c
	$(compiler) $(cflag) menu/menu.roster.c
	$(compiler) $(cflag) test/test.color.c
	$(compiler) $(cflag) test/test.doodle.c
	$(compiler) $(cflag) test/test.font.c
	$(compiler) $(cflag) test/test.pure.c
	$(compiler) $(cflag) tool/tool.3dmodel.c
	$(compiler) $(cflag) tool/tool.calculator.c
	$(compiler) $(cflag) tool/tool.camera.c
	$(compiler) $(cflag) tool/tool.circuit.c
	$(compiler) $(cflag) tool/tool.qrcode.c
	$(compiler) $(cflag) tool/tool.sketchpad.c
	$(compiler) $(cflag) tool/tool.spectrum.c
	$(compiler) $(cflag) character.c
clean:
	rm -f *.o *.a
