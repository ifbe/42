############################text#############################
linux_math--static:
	make --no-print-directory -C ../none+x64+linux+cli static
	cp ../none+x64+linux+cli/library.a .
	gcc math/x64.c library.a -lm
linux_math--shared:
	make --no-print-directory -C ../none+x64+linux+cli shared
	cp ../none+x64+linux+cli/library.so .
	gcc math/x64.c library.so -lm
win_math--static:
	make --no-print-directory -C ../none+x64+win+cli static
	cp ../none+x64+win+cli/library.a .
	gcc math/x64.c library.a -lm
win_math--shared:
	make --no-print-directory -C ../none+x64+win+cli shared
	cp ../none+x64+win+cli/library.so .
	gcc math/x64.c library.so -lm

win_unicode--static:
	make --no-print-directory -C ../none+x64+win+cli static
	gcc text/unicodetable.c ../none+x64+win+cli/library.a
linux_unicode--static:
	make --no-print-directory -C ../none+x64+linux+cli static
	gcc text/unicodetable.c ../none+x64+linux+cli/library.a

win_utf--static:
	make --no-print-directory -C ../none+x64+win+cli static
	gcc text/utf.c ../none+x64+win+cli/library.a
linux_utf--static:
	make --no-print-directory -C ../none+x64+linux+cli static
	gcc text/utf.c ../none+x64+linux+cli/library.a
############################graphical##########################
winapi--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc graphic/graphical.c ./library.a -lgdi32
xlib--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc graphic/graphical.c ./library.a -lX11

winapi_hanzi--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc graphic/hanzi.c ./library.a -lgdi32
xlib_hanzi--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc graphic/hanzi.c ./library.a -lX11
############################so cool############################
winapi_1--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/1.c ../none+x64+win+api/library.a -lgdi32 -lm
xlib_1--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/1.c ../none+x64+linux+xlib/library.a -lX11 -lm

winapi_2--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/2.c ../none+x64+win+api/library.a -lgdi32
xlib_2--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/2.c ../none+x64+linux+xlib/library.a -lX11

winapi_3--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/3.c ../none+x64+win+api/library.a -lgdi32
xlib_3--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/3.c ../none+x64+linux+xlib/library.a -lX11

winapi_4--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/4.c ../none+x64+win+api/library.a -lgdi32
xlib_4--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/4.c ../none+x64+linux+xlib/library.a -lX11

winapi_5--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/5.c ../none+x64+win+api/library.a -lgdi32
xlib_5--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/5.c ../none+x64+linux+xlib/library.a -lX11

winapi_6--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc cool/6.c ../none+x64+win+api/library.a -lgdi32
xlib_6--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc cool/6.c ../none+x64+linux+xlib/library.a -lX11
###########################image process###########################
winapi_blur--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc image/blur.c ../none+x64+win+api/library.a -lgdi32
xlib_blur--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/blur.c ../none+x64+linux+xlib/library.a -lX11

winapi_channel--static:
	make --no-print-directory -C ../none+x64+win+api static
	gcc image/channel.c ../none+x64+win+api/library.a -lgdi32
xlib_channel--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/channel.c ../none+x64+linux+xlib/library.a -lX11

winapi_gray--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/gray.c ../none+x64+win+api/library.a -lgdi32
xlib_gray--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/gray.c ../none+x64+linux+xlib/library.a -lX11

winapi_mosaic--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/mosaic.c ../none+x64+win+api/library.a -lgdi32
xlib_mosaic--static:
	make --no-print-directory -C ../none+x64+linux+xlib static
	gcc image/mosaic.c ../none+x64+linux+xlib/library.a -lX11
