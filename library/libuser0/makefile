compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.




############################bare#########################
.PHONY:barecli
barecli:
	rm -f *.o
	$(compiler) $(cflag) bare/bare.cli.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
.PHONY:baretui
baretui:
	rm -f *.o
	$(compiler) $(cflag) bare/bare.tui.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
.PHONY:bare8bit
bare8bit:
	rm -f *.o
	$(compiler) $(cflag) bare/bare.8bit.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
.PHONY:barefb
barefb:
	rm -f *.o
	$(compiler) $(cflag) bare/bare.fb.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:
	rm -f *.o
	$(compiler) $(cflag) efi/efi.cli.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
.PHONY:efifb
efifb:
	rm -f *.o
	$(compiler) $(cflag) efi/efi.fb.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
#########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.cli.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxtui
linuxtui:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.tui.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxfb
linuxfb:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.fb.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxgtk
linuxgtk:
	rm -f *.o
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` linux/linux.gtk.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxwayland
linuxwayland:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.wayland.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxxlib
linuxxlib:
	rm -f *.o
	$(compiler) $(cflag) linux/linux.xlib.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxgles
linuxgles:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.gles.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxglut
linuxglut:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.glut.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxsdl
linuxsdl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.cli.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:mactui
mactui:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.tui.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:maccocoa
maccocoa:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.cocoa.m
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:macquartz
macquartz:
	rm -f *.o
	$(compiler) $(cflag) mac/mac.quartz.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:macgles
macgles:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.gles.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:macglut
macglut:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.glut.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:macsdl
macsdl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:
	rm -f *.o
	$(compiler) $(cflag) win/win.cli.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:wintui
wintui:
	rm -f *.o
	$(compiler) $(cflag) win/win.tui.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:winapi
winapi:
	rm -f *.o
	$(compiler) $(cflag) win/win.api.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:wind3d
wind3d:
	rm -f *.o
	$(compiler) $(cflag) win/win.d3d.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:wingles
wingles:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.gles.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:winglut
winglut:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.glut.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c

.PHONY:winsdl
winsdl:
	rm -f *.o
	$(compiler) $(cflag) cross/cross.sdl2.c
	$(compiler) $(cflag) cross/cross.net.c
	$(compiler) $(cflag) cross/cross.voice.c
	$(compiler) $(cflag) arena.c
##########################################################




clean:
	rm -f *.o *.a