flag := -fPIC -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast




############################linux#########################
.PHONY:barecli
barecli:
	rm -f *.o

.PHONY:barefb
barefb:
	rm -f *.o
##########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:
	rm -f *.o
	gcc -c -O2 $(flag) linux/linux.cli.c
	gcc -c -O2 $(flag) display.c

.PHONY:linuxtui
linuxtui:
	rm -f *.o
	gcc -c -O2 $(flag) linux/linux.tui.c
	gcc -c -O2 $(flag) display.c

.PHONY:linuxfb
linuxfb:
	rm -f *.o
	gcc -c -O2 $(flag) linux/linux.fb.c
	gcc -c -O2 $(flag) display.c

.PHONY:linuxgtk
linuxgtk:
	rm -f *.o
	gcc -c -O2 `pkg-config --cflags gtk+-3.0` $(flag) linux/linux.gtk.c
	gcc -c -O2 $(flag) display.c

.PHONY:linuxwayland
linuxwayland:
	rm -f *.o
	gcc -c -O2 $(flag) linux/linux.wayland.c
	gcc -c -O2 $(flag) display.c

.PHONY:linuxxlib
linuxxlib:
	rm -f *.o
	gcc -c -O2 $(flag) linux/linux.xlib.c
	gcc -c -O2 $(flag) display.c

.PHONY:linuxopengl
linuxopengl:
	rm -f *.o
	gcc -c -O2 $(flag) cross/cross.opengl.c
	gcc -c -O2 $(flag) display.c

.PHONY:linuxsdl
linuxsdl:
	rm -f *.o
	gcc -c -O2 $(flag) cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	gcc -c -O2 $(flag) display.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:
	rm -f *.o
	gcc -c -fPIC -O2 mac/mac.cli.c
	gcc -c -fPIC -O2 display.c

.PHONY:mactui
mactui:
	rm -f *.o
	gcc -c -fPIC -O2 mac/mac.tui.c
	gcc -c -fPIC -O2 display.c

.PHONY:maccocoa
maccocoa:
	rm -f *.o
	clang -c -fPIC -O2 mac/mac.cocoa.c
	clang -c -fPIC -O2 display.c

.PHONY:macquartz
macquartz:
	rm -f *.o
	clang -c -fPIC -O2 mac/mac.cocoa.c
	clang -c -fPIC -O2 display.c

.PHONY:macopengl
macopengl:
	rm -f *.o
	clang -c -O2 cross/cross.opengl.c
	gcc -c -O2 display.c

.PHONY:macsdl
macsdl:
	rm -f *.o
	clang -c -O2 cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	gcc -c -O2 display.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:
	rm -f *.o
	gcc -c -O2 win/win.cli.c
	gcc -c -O2 display.c

.PHONY:wintui
wintui:
	rm -f *.o
	gcc -c -O2 win/win.tui.c
	gcc -c -O2 display.c

.PHONY:winapi
winapi:
	rm -f *.o
	gcc -c -O2 win/win.api.c
	gcc -c -O2 display.c

.PHONY:winopengl
winopengl:
	rm -f *.o
	gcc -c -O2 cross/cross.opengl.c
	gcc -c -O2 display.c

.PHONY:winsdl
winsdl:
	rm -f *.o
	gcc -c -O2 cross/cross.sdl2.c
	gcc -c -O2 display.c
##########################################################




clean:
	rm -f *.o *.a
