compiler ?= gcc
cflag ?= -c -O2 -fPIC




############################linux#########################
.PHONY:barecli
barecli:
	rm -f *.o
	$(compiler) $(cflag) bare/bare.cli.c
	$(compiler) $(cflag) display.c
.PHONY:barefb
barefb:
	$(compiler) $(cflag) bare/bare.fb.c
	$(compiler) $(cflag) display.c
	rm -f *.o
##########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.cli.c
	$(compiler) $(cflag) display.c

.PHONY:linuxtui
linuxtui:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.tui.c
	$(compiler) $(cflag) display.c

.PHONY:linuxfb
linuxfb:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.fb.c
	$(compiler) $(cflag) display.c

.PHONY:linuxgtk
linuxgtk:
	rm -f *.o
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` linux/linux.gtk.c
	$(compiler) $(cflag) display.c

.PHONY:linuxwayland
linuxwayland:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.wayland.c
	$(compiler) $(cflag) display.c

.PHONY:linuxxlib
linuxxlib:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.xlib.c
	$(compiler) $(cflag) display.c

.PHONY:linuxopengl
linuxopengl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.opengl.c
	$(compiler) $(cflag) display.c

.PHONY:linuxsdl
linuxsdl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) display.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.cli.c
	$(compiler) $(cflag) display.c

.PHONY:mactui
mactui:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.tui.c
	$(compiler) $(cflag) display.c

.PHONY:maccocoa
maccocoa:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.cocoa.c
	$(compiler) $(cflag) display.c

.PHONY:macquartz
macquartz:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.cocoa.c
	$(compiler) $(cflag) display.c

.PHONY:macopengl
macopengl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.opengl.c
	$(compiler) $(cflag) display.c

.PHONY:macsdl
macsdl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) display.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:
	rm -f *.o
	$(compiler) $(cflag) win/win.cli.c
	$(compiler) $(cflag) display.c

.PHONY:wintui
wintui:
	rm -f *.o
	$(compiler) $(cflag) win/win.tui.c
	$(compiler) $(cflag) display.c

.PHONY:winapi
winapi:
	rm -f *.o
	$(compiler) $(cflag) win/win.api.c
	$(compiler) $(cflag) display.c

.PHONY:winopengl
winopengl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.opengl.c
	$(compiler) $(cflag) display.c

.PHONY:winsdl
winsdl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.sdl2.c
	$(compiler) $(cflag) display.c
##########################################################




clean:
	rm -f *.o *.a
