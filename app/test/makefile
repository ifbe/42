############################text#############################
win_math:
	make -C ../none+x64+win+cli static --no-print-directory
	gcc math/x64.c ../none+x64+win+cli/library.a -lm
linux_math:
	make -C ../none+x64+linux+cli static --no-print-directory
	gcc math/x64.c ../none+x64+linux+cli/library.a -lm

win_unicode:
	make -C ../none+x64+win+cli static --no-print-directory
	gcc text/unicodetable.c ../none+x64+win+cli/library.a
linux_unicode:
	make -C ../none+x64+linux+cli static --no-print-directory
	gcc text/unicodetable.c ../none+x64+linux+cli/library.a

win_utf:
	make -C ../none+x64+win+cli static --no-print-directory
	gcc text/utf.c ../none+x64+win+cli/library.a
linux_utf:
	make -C ../none+x64+linux+cli static --no-print-directory
	gcc text/utf.c ../none+x64+linux+cli/library.a
############################graphical##########################
winapi:
	make -C ../none+x64+win+api static --no-print-directory
	gcc graphic/graphical.c ./library.a -lgdi32
xlib:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc graphic/graphical.c ./library.a -lX11

winapi_hanzi:
	make -C ../none+x64+win+api static --no-print-directory
	gcc graphic/hanzi.c ./library.a -lgdi32
xlib_hanzi:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc graphic/hanzi.c ./library.a -lX11
############################so cool############################
winapi_1:
	make -C ../none+x64+win+api static --no-print-directory
	gcc cool/1.c ../none+x64+win+api/library.a -lgdi32 -lm
xlib_1:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc cool/1.c ../none+x64+linux+xlib/library.a -lX11 -lm

winapi_2:
	make -C ../none+x64+win+api static --no-print-directory
	gcc cool/2.c ../none+x64+win+api/library.a -lgdi32
xlib_2:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc cool/2.c ../none+x64+linux+xlib/library.a -lX11

winapi_3:
	make -C ../none+x64+win+api static --no-print-directory
	gcc cool/3.c ../none+x64+win+api/library.a -lgdi32
xlib_3:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc cool/3.c ../none+x64+linux+xlib/library.a -lX11

winapi_4:
	make -C ../none+x64+win+api static --no-print-directory
	gcc cool/4.c ../none+x64+win+api/library.a -lgdi32
xlib_4:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc cool/4.c ../none+x64+linux+xlib/library.a -lX11

winapi_5:
	make -C ../none+x64+win+api static --no-print-directory
	gcc cool/5.c ../none+x64+win+api/library.a -lgdi32
xlib_5:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc cool/5.c ../none+x64+linux+xlib/library.a -lX11

winapi_6:
	make -C ../none+x64+win+api static --no-print-directory
	gcc cool/6.c ../none+x64+win+api/library.a -lgdi32
xlib_6:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc cool/6.c ../none+x64+linux+xlib/library.a -lX11
###########################image process###########################
winapi_blur:
	make -C ../none+x64+win+api static --no-print-directory
	gcc image/blur.c ../none+x64+win+api/library.a -lgdi32
xlib_blur:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc image/blur.c ../none+x64+linux+xlib/library.a -lX11

winapi_channel:
	make -C ../none+x64+win+api static --no-print-directory
	gcc image/channel.c ../none+x64+win+api/library.a -lgdi32
xlib_channel:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc image/channel.c ../none+x64+linux+xlib/library.a -lX11

winapi_gray:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc image/gray.c ../none+x64+win+api/library.a -lgdi32
xlib_gray:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc image/gray.c ../none+x64+linux+xlib/library.a -lX11

winapi_mosaic:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc image/mosaic.c ../none+x64+win+api/library.a -lgdi32
xlib_mosaic:
	make -C ../none+x64+linux+xlib static --no-print-directory
	gcc image/mosaic.c ../none+x64+linux+xlib/library.a -lX11
