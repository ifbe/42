#############################################################
library.a--linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	cp ../none+x64+linux+cli/library.a .
library.so--linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli shared
	cp ../none+x64+linux+cli/library.so .

library.a--win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	cp ../none+x64+win+cli/library.a .
library.dll--win.cmd:
	make --no-print-directory -C ../none+x64+win+cli shared
	cp ../none+x64+win+cli/library.dll .

library.a--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	cp ../none+x64+linux+xlib/library.a .
library.so--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib shared
	cp ../none+x64+linux+xlib/library.so .

library.a--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	cp ../none+x64+win+api/library.a .
library.dll--win.api:
	make --no-print-directory -C ../none+x64+win+api shared
	cp ../none+x64+win+api/library.dll .
############################text#############################
math.static--linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	cp ../none+x64+linux+cli/library.a .
	gcc math/x64.c library.a -lm
math.shared--linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli shared
	cp ../none+x64+linux+cli/library.so .
	gcc math/x64.c library.so -lm
math.static--win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	cp ../none+x64+win+cli/library.a .
	gcc math/x64.c library.a -lm
math.shared--win.cmd:
	make --no-print-directory -C ../none+x64+win+cli shared
	cp ../none+x64+win+cli/library.dll .
	gcc math/x64.c library.dll -lm

unicode.static--linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	gcc text/unicodetable.c ../none+x64+linux+cli/library.a
unicode.static--win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	gcc text/unicodetable.c ../none+x64+win+cli/library.a

utf.static--linux.cli:
	make --no-print-directory -C ../none+x64+linux+cli static
	gcc text/utf.c ../none+x64+linux+cli/library.a
utf.static--win.cmd:
	make --no-print-directory -C ../none+x64+win+cli static
	gcc text/utf.c ../none+x64+win+cli/library.a
############################graphical##########################
graphic.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc graphic/graphical.c ./library.a -lgdi32
graphic.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc graphic/graphical.c ./library.a -lX11

hanzi.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc graphic/hanzi.c ./library.a -lgdi32
hanzi.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc graphic/hanzi.c ./library.a -lX11
############################so cool############################
1.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/1.c ../none+x64+win+api/library.a -lgdi32 -lm
1.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/1.c ../none+x64+linux+xlib/library.a -lX11 -lm

2.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/2.c ../none+x64+win+api/library.a -lgdi32
2.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/2.c ../none+x64+linux+xlib/library.a -lX11

3.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/3.c ../none+x64+win+api/library.a -lgdi32
3.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/3.c ../none+x64+linux+xlib/library.a -lX11

4.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/4.c ../none+x64+win+api/library.a -lgdi32
4.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/4.c ../none+x64+linux+xlib/library.a -lX11

5.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/5.c ../none+x64+win+api/library.a -lgdi32
5.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/5.c ../none+x64+linux+xlib/library.a -lX11

6.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/6.c ../none+x64+win+api/library.a -lgdi32
6.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/6.c ../none+x64+linux+xlib/library.a -lX11
###########################image process###########################
blur.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc image/blur.c ../none+x64+win+api/library.a -lgdi32
blur.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/blur.c ../none+x64+linux+xlib/library.a -lX11

channel.static--win.api:
	make --no-print-directory -C ../none+x64+win+api static
	gcc image/channel.c ../none+x64+win+api/library.a -lgdi32
channel.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/channel.c ../none+x64+linux+xlib/library.a -lX11

gray.static--win.api:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/gray.c ../none+x64+win+api/library.a -lgdi32
gray.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/gray.c ../none+x64+linux+xlib/library.a -lX11

mosaic.static--winapi:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/mosaic.c ../none+x64+win+api/library.a -lgdi32
mosaic.static--linux.xlib:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/mosaic.c ../none+x64+linux+xlib/library.a -lX11
