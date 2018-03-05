compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.




############################bare#########################
.PHONY:barecli
barecli:
	rm -f *.o
	$(compiler) $(cflag) local/bare/local.bare.cli.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:baretui
baretui:
	rm -f *.o
	$(compiler) $(cflag) local/bare/local.bare.tui.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:bare8bit
bare8bit:
	rm -f *.o
	$(compiler) $(cflag) local/bare/local.bare.8bit.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:barefb
barefb:
	rm -f *.o
	$(compiler) $(cflag) local/bare/local.bare.fb.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:
	rm -f *.o
	$(compiler) $(cflag) local/efi/local.efi.cli.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:efifb
efifb:
	rm -f *.o
	$(compiler) $(cflag) local/efi/local.efi.fb.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
#########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:
	rm -f *.o
	$(compiler) $(cflag) local/linux/local.linux.cli.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxtui
linuxtui:
	rm -f *.o
	$(compiler) $(cflag) local/linux/local.linux.tui.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxfb
linuxfb:
	rm -f *.o
	$(compiler) $(cflag) local/linux/local.linux.fb.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxgtk
linuxgtk:
	rm -f *.o
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` local/linux/local.linux.gtk.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxwayland
linuxwayland:
	rm -f *.o
	$(compiler) $(cflag) local/linux/local.linux.wayland.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxxlib
linuxxlib:
	rm -f *.o
	$(compiler) $(cflag) local/linux/local.linux.xlib.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxglfw
linuxglfw:
	rm -f *.o
	$(compiler) $(cflag) local/local.glfw.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxglut
linuxglut:
	rm -f *.o
	$(compiler) $(cflag) local/local.glut.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxsdl
linuxsdl:
	rm -f *.o
	$(compiler) $(cflag) local/local.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:
	rm -f *.o
	$(compiler) $(cflag) local/mac/mac.cli.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:mactui
mactui:
	rm -f *.o
	$(compiler) $(cflag) local/mac/mac.tui.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:maccocoa
maccocoa:
	rm -f *.o
	$(compiler) $(cflag) local/mac/local.mac.cocoa.m
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:macquartz
macquartz:
	rm -f *.o
	$(compiler) $(cflag) local/mac/local.mac.quartz.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:macglfw
macglfw:
	rm -f *.o
	$(compiler) $(cflag) local/local.glfw.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:macglut
macglut:
	rm -f *.o
	$(compiler) $(cflag) local/local.glut.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:macsdl
macsdl:
	rm -f *.o
	$(compiler) $(cflag) local/local.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:
	rm -f *.o
	g++ $(cflag) -fno-rtti video/win/video.win.dshow.cpp
	$(compiler) $(cflag) sound/win/sound.win.waveio.c
	$(compiler) $(cflag) local/win/local.win.cli.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:wintui
wintui:
	rm -f *.o
	g++ $(cflag) -fno-rtti video/win/video.win.dshow.cpp
	$(compiler) $(cflag) sound/win/sound.win.waveio.c
	$(compiler) $(cflag) local/win/local.win.tui.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:winapi
winapi:
	rm -f *.o
	g++ $(cflag) -fno-rtti video/win/video.win.dshow.cpp
	$(compiler) $(cflag) sound/win/sound.win.waveio.c
	$(compiler) $(cflag) local/win/local.win.api.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:wind3d
wind3d:
	rm -f *.o
	g++ $(cflag) -fno-rtti video/win/video.win.dshow.cpp
	$(compiler) $(cflag) sound/win/sound.win.waveio.c
	$(compiler) $(cflag) local/win/local.win.d3d.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:winglfw
winglfw:
	rm -f *.o
	g++ $(cflag) -fno-rtti video/win/video.win.dshow.cpp
	$(compiler) $(cflag) sound/win/sound.win.waveio.c
	$(compiler) $(cflag) local/local.glfw.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:winglut
winglut:
	rm -f *.o
	g++ $(cflag) -fno-rtti video/win/video.win.dshow.cpp
	$(compiler) $(cflag) sound/win/sound.win.waveio.c
	$(compiler) $(cflag) local/local.glut.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:winsdl
winsdl:
	rm -f *.o
	g++ $(cflag) -fno-rtti video/win/video.win.dshow.cpp
	$(compiler) $(cflag) sound/win/sound.win.waveio.c
	$(compiler) $(cflag) local/local.sdl2.c
	$(compiler) $(cflag) remote/remote.ws.c
	$(compiler) $(cflag) arena.c
##########################################################




clean:
	rm -f *.o *.a
