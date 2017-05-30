compiler ?= gcc
cflag ?= -c -O2 -fPIC

target:
	$(compiler) $(cflag) lib1d/draw.xt100.c
	$(compiler) $(cflag) lib2d/draw/draw.ascii.c
	$(compiler) $(cflag) lib2d/draw/draw.background.c
	$(compiler) $(cflag) lib2d/draw/draw.icon.c
	$(compiler) $(cflag) lib2d/draw/draw.shape.c
	$(compiler) $(cflag) lib2d/draw/draw.ttf.c
	$(compiler) $(cflag) lib2d/photo/photo.blur.c
	$(compiler) $(cflag) lib2d/photo/photo.channel.c
	$(compiler) $(cflag) lib2d/photo/photo.convert.c
	$(compiler) $(cflag) lib2d/photo/photo.gray.c
	$(compiler) $(cflag) lib2d/photo/photo.mosaic.c
	$(compiler) $(cflag) lib2d/photo/photo.rotate.c
	$(compiler) $(cflag) lib2d/photo/photo.scale.c
	$(compiler) $(cflag) lib3d/carve/carve.ground.c
	$(compiler) $(cflag) lib3d/carve/carve.shape.c
	$(compiler) $(cflag) lib3d/carve/carve.thing.c
	$(compiler) $(cflag) lib3d/carve/carve.water.c
	$(compiler) $(cflag) lib3d/model/model.magnify.c
	$(compiler) $(cflag) lib3d/model/model.minify.c
	$(compiler) $(cflag) floor/game/game.2048.c
	$(compiler) $(cflag) floor/game/game.chess.c
	$(compiler) $(cflag) floor/game/game.ooxx.c
	$(compiler) $(cflag) floor/game/game.snake.c
	$(compiler) $(cflag) floor/game/game.sudoku.c
	$(compiler) $(cflag) floor/game/game.tetris.c
	$(compiler) $(cflag) floor/game/game.weiqi.c
	$(compiler) $(cflag) floor/game/game.xiangqi.c
	$(compiler) $(cflag) floor/hack/hack.browse.c
	$(compiler) $(cflag) floor/hack/hack.editor.c
	$(compiler) $(cflag) floor/hack/hack.hex.c
	$(compiler) $(cflag) floor/hack/hack.input.c
	$(compiler) $(cflag) floor/hack/hack.tree.c
	$(compiler) $(cflag) floor/link/link.door.c
	$(compiler) $(cflag) floor/link/link.friend.c
	$(compiler) $(cflag) floor/test/test.color.c
	$(compiler) $(cflag) floor/test/test.doodle.c
	$(compiler) $(cflag) floor/test/test.font.c
	$(compiler) $(cflag) floor/test/test.pure.c
	$(compiler) $(cflag) floor/tool/tool.3dmodel.c
	$(compiler) $(cflag) floor/tool/tool.calculator.c
	$(compiler) $(cflag) floor/tool/tool.camera.c
	$(compiler) $(cflag) floor/tool/tool.circuit.c
	$(compiler) $(cflag) floor/tool/tool.qrcode.c
	$(compiler) $(cflag) floor/tool/tool.sketchpad.c
	$(compiler) $(cflag) floor/tool/tool.spectrum.c
	$(compiler) $(cflag) float/menu/menu.center.c
	$(compiler) $(cflag) float/popup/menu.roster.c
	$(compiler) $(cflag) float/vkbd/menu.virtkbd.c
	$(compiler) $(cflag) character.c
clean:
	rm -f *.o *.a
