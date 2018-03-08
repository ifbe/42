compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = \
test/test.clock.c \
test/test.codeimg.c \
test/test.doodle.c \
test/test.example.c \
test/test.fractal.c \
test/test.palette.c \
game/game.2048.c \
game/game.chess.c \
game/game.klotski.c \
game/game.maze.c \
game/game.ooxx.c \
game/game.pegged.c \
game/game.poker.c \
game/game.rubik.c \
game/game.snake.c \
game/game.sudoku.c \
game/game.tetris.c \
game/game.weiqi.c \
game/game.xiangqi.c \
tool/tool.browser.c \
tool/tool.calculator.c \
tool/tool.camera.c \
tool/tool.circuit.c \
tool/tool.editor.c \
tool/tool.font.c \
tool/tool.qrcode.c \
tool/tool.sketchpad.c \
tool/tool.spectrum.c \
tool/tool.stl.c \
tool/tool.terminal.c \
hack/hack.algo.c \
hack/hack.bintree.c \
hack/hack.bplus.c \
hack/hack.fs.c \
hack/hack.graph.c \
hack/hack.hex.c \
hack/hack.input.c \
lib1d/cli/print.cli.c \
lib1d/tui/print.tui.c \
lib1d/html/html.css.c \
lib1d/json/json.shape.c \
lib1d/latex/latex.shape.c \
lib1d/xaml/xaml.shape.c \
lib1d/lib1d.c \
lib2d/draw/draw.ascii.c \
lib2d/draw/draw.icon.c \
lib2d/draw/draw.shape.c \
lib2d/draw/draw.ttf.c \
lib2d/draw/draw.utf8.c \
lib2d/draw/draw.vt100.c \
lib2d/photo/photo.blur.c \
lib2d/photo/photo.channel.c \
lib2d/photo/photo.convert.c \
lib2d/photo/photo.gray.c \
lib2d/photo/photo.mosaic.c \
lib2d/photo/photo.rotate.c \
lib2d/photo/photo.scale.c \
lib2d/lib2d.c \
lib3d/carve/carve.ascii.c \
lib3d/carve/carve.point.c \
lib3d/carve/carve.line.c \
lib3d/carve/carve.trigon.c \
lib3d/carve/carve.other.c \
lib3d/model/model.magnify.c \
lib3d/model/model.minify.c \
lib3d/model/model.rotate.c \
lib3d/lib3d.c \
lib4d/help.asset.c \
lib4d/help.bgfg.c \
lib4d/help.ctx.c \
lib4d/help.empty.c \
lib4d/help.input.c \
lib4d/help.output.c \
lib4d/lib4d.c \
actor.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<

clean:
	rm -f *.o *.a
