#############################################################
library.a@linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	cp ../none+x64+linux+cli/library.a .
library.so@linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli shared
	cp ../none+x64+linux+cli/library.so .

library.a@win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	cp ../none+x64+win+cli/library.a .
library.dll@win.cmd:
	make --no-print-directory -C ../none+x64+win+cli shared
	cp ../none+x64+win+cli/library.dll .

library.a@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
library.so@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib shared
	cp ../none+x64+linux+xlib/library.so .

library.a@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
library.dll@win.api:
	make --no-print-directory -C ../none+x64+win+api shared
	cp ../none+x64+win+api/library.dll .
############################text#############################
math.static@linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	cp ../none+x64+linux+cli/library.a .
	gcc math/x64.c library.a -lm
math.shared@linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli shared
	cp ../none+x64+linux+cli/library.so .
	gcc math/x64.c library.so -lm
math.static@win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	cp ../none+x64+win+cli/library.a .
	gcc math/x64.c library.a -lm
math.shared@win.cmd:
	make --no-print-directory -C ../none+x64+win+cli shared
	cp ../none+x64+win+cli/library.dll .
	gcc math/x64.c library.dll -lm

unicode.static@linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	cp ../none+x64+linux+cli/library.a .
	gcc text/unicodetable.c library.a
unicode.static@win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	cp ../none+x64+win+cli/library.a .
	gcc text/unicodetable.c library.a

utf.static@linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	cp ../none+x64+linux+cli/library.a .
	gcc text/utf.c library.a
utf.static@win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	cp ../none+x64+win+cli/library.a .
	gcc text/utf.c library.a
############################graphical##########################
graphic.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc graphic/graphical.c library.a -lX11
graphic.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc graphic/graphical.c library.a -lgdi32

hanzi.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc graphic/hanzi.c library.a -lX11
hanzi.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc graphic/hanzi.c library.a -lgdi32
############################so cool############################
1.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc cool/1.c library.a -lX11 -lm
1.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc cool/1.c library.a -lgdi32 -lm

2.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc cool/2.c library.a -lX11
2.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc cool/2.c library.a -lgdi32

3.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc cool/3.c library.a -lX11
3.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc cool/3.c library.a -lgdi32

4.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc cool/4.c library.a -lX11
4.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc cool/4.c library.a -lgdi32

5.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc cool/5.c library.a -lX11
5.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc cool/5.c library.a -lgdi32

6.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc cool/6.c library.a -lX11
6.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc cool/6.c library.a -lgdi32
###########################image process###########################
blur.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc image/blur.c library.a -lX11
blur.shared@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib shared
	cp ../none+x64+linux+xlib/library.a .
	gcc image/blur.c library.so -lX11
blur.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc image/blur.c library.a -lgdi32
blur.shared@win.api:
	make --no-print-directory -C ../none+x64+win+api shared
	cp ../none+x64+win+api/library.dll .
	gcc image/blur.c library.dll

channel.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc image/channel.c library.a -lX11
channel.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc image/channel.c library.a -lgdi32

gray.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc image/gray.c library.a -lX11
gray.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc image/gray.c library.a -lgdi32

mosaic.static@linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
	gcc image/mosaic.c library.a -lX11
mosaic.static@win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
	gcc image/mosaic.c library.a -lgdi32
