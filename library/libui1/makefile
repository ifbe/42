compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = \
0test/test.clock.c \
0test/test.codeimg.c \
0test/test.color.c \
0test/test.doodle.c \
0test/test.fractal.c \
0test/test.pure.c \
1game/game.2048.c \
1game/game.chess.c \
1game/game.maze.c \
1game/game.ooxx.c \
1game/game.rubikscube.c \
1game/game.snake.c \
1game/game.sudoku.c \
1game/game.tetris.c \
1game/game.weiqi.c \
1game/game.xiangqi.c \
2tool/tool.calculator.c \
2tool/tool.camera.c \
2tool/tool.circuit.c \
2tool/tool.font.c \
2tool/tool.qrcode.c \
2tool/tool.sketchpad.c \
2tool/tool.spectrum.c \
2tool/tool.stl.c \
2tool/tool.terminal.c \
3hack/hack.algo.c \
3hack/hack.browser.c \
3hack/hack.editor.c \
3hack/hack.hex.c \
3hack/hack.tree.c \
3hack/hack.joystick.c \
3hack/hack.keyboard.c \
lib1d/cli/print.cli.c \
lib1d/tui/print.tui.c \
lib1d/html/html.shape.c \
lib1d/json/json.shape.c \
lib1d/latex/latex.shape.c \
lib1d/xaml/xaml.shape.c \
lib1d/lib1d.c \
lib2d/draw/draw.ascii.c \
lib2d/draw/draw.icon.c \
lib2d/draw/draw.shape.c \
lib2d/draw/draw.ttf.c \
lib2d/draw/draw.vt100.c \
lib2d/photo/photo.blur.c \
lib2d/photo/photo.channel.c \
lib2d/photo/photo.convert.c \
lib2d/photo/photo.gray.c \
lib2d/photo/photo.mosaic.c \
lib2d/photo/photo.rotate.c \
lib2d/photo/photo.scale.c \
lib2d/lib2d.c \
lib3d/carve/carve.point.c \
lib3d/carve/carve.line.c \
lib3d/carve/carve.plane.c \
lib3d/carve/carve.shape.c \
lib3d/model/model.magnify.c \
lib3d/model/model.minify.c \
lib3d/model/model.rotate.c \
lib3d/lib3d.c \
lib4d/asset/asset.font.c \
lib4d/asset/asset.image.c \
lib4d/asset/asset.sound.c \
lib4d/asset/asset.texture.c \
lib4d/help/help.bgfg.c \
lib4d/help/help.ctx.c \
lib4d/help/help.login.c \
lib4d/help/help.input.c \
lib4d/help/help.term.c \
lib4d/help/help.win.c \
lib4d/lib4d.c \
actor.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<

clean:
	rm -f *.o *.a
