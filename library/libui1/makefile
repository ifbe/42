compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = content/game/game.2048.c \
content/game/game.chess.c \
content/game/game.ooxx.c \
content/game/game.rubikscube.c \
content/game/game.snake.c \
content/game/game.sudoku.c \
content/game/game.tetris.c \
content/game/game.weiqi.c \
content/game/game.xiangqi.c \
content/hack/hack.algo.c \
content/hack/hack.browser.c \
content/hack/hack.editor.c \
content/hack/hack.hex.c \
content/hack/hack.tree.c \
content/hack/hack.joystick.c \
content/hack/hack.keyboard.c \
content/test/test.codeimg.c \
content/test/test.color.c \
content/test/test.doodle.c \
content/test/test.fractal.c \
content/test/test.pure.c \
content/tool/tool.calculator.c \
content/tool/tool.camera.c \
content/tool/tool.circuit.c \
content/tool/tool.font.c \
content/tool/tool.qrcode.c \
content/tool/tool.sketchpad.c \
content/tool/tool.spectrum.c \
content/tool/tool.stl.c \
content/tool/tool.terminal.c \
content/content.c \
helper/helper.bgfg.c \
helper/helper.input.c \
helper/helper.login.c \
helper/helper.term.c \
helper/helper.win.c \
helper/helper.c \
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
lib3d/carve/carve.shape.c \
lib3d/model/model.magnify.c \
lib3d/model/model.minify.c \
lib3d/model/model.rotate.c \
lib3d/lib3d.c \
actor.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<

clean:
	rm -f *.o *.a
