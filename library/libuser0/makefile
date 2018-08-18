compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.




############################bare#########################
.PHONY:barecli
barecli:
	rm -f *.o
	$(compiler) $(cflag) osbare/bare.cli.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:baretui
baretui:
	rm -f *.o
	$(compiler) $(cflag) osbare/bare.tui.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:bare8bit
bare8bit:
	rm -f *.o
	$(compiler) $(cflag) osbare/bare.8bit.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:barefb
barefb:
	rm -f *.o
	$(compiler) $(cflag) osbare/bare.fb.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:
	rm -f *.o
	$(compiler) $(cflag) osefi/efi.cli.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c
.PHONY:efifb
efifb:
	rm -f *.o
	$(compiler) $(cflag) osefi/efi.fb.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c
#########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.cli.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxtui
linuxtui:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.tui.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxfb
linuxfb:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.fb.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxgtk
linuxgtk:
	rm -f *.o
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` oslinux/linux.gtk.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxwayland
linuxwayland:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.wayland.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxxlib
linuxxlib:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.xlib.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxglfw
linuxglfw:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bygl/gl.object.c
	$(compiler) $(cflag) bygl/gl.shader.c
	$(compiler) $(cflag) bygl/gl.texture.c
	$(compiler) $(cflag) bygl/gl.vertex.c
	$(compiler) $(cflag) bygl/glfw.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxglut
linuxglut:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bygl/glut.c
	$(compiler) $(cflag) arena.c

.PHONY:linuxsdl
linuxsdl:
	rm -f *.o
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bysdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) arena.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:
	rm -f *.o
	$(compiler) $(cflag) osmac/mac.cli.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:mactui
mactui:
	rm -f *.o
	$(compiler) $(cflag) osmac/mac.tui.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:maccocoa
maccocoa:
	rm -f *.o
	$(compiler) $(cflag) osmac/mac.cocoa.m
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:macquartz
macquartz:
	rm -f *.o
	$(compiler) $(cflag) osmac/mac.quartz.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:macglfw
macglfw:
	rm -f *.o
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bygl/gl.object.c
	$(compiler) $(cflag) bygl/gl.shader.c
	$(compiler) $(cflag) bygl/gl.texture.c
	$(compiler) $(cflag) bygl/gl.vertex.c
	$(compiler) $(cflag) bygl/glfw.c
	$(compiler) $(cflag) arena.c

.PHONY:macglut
macglut:
	rm -f *.o
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bygl/glut.c
	$(compiler) $(cflag) arena.c

.PHONY:macsdl
macsdl:
	rm -f *.o
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bysdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) arena.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:
	rm -f *.o
	$(compiler) $(cflag) oswin/win.cli.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:wintui
wintui:
	rm -f *.o
	$(compiler) $(cflag) oswin/win.tui.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:winapi
winapi:
	rm -f *.o
	$(compiler) $(cflag) oswin/win.api.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:wind3d
wind3d:
	rm -f *.o
	$(compiler) $(cflag) oswin/win.d3d.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) arena.c

.PHONY:winglfw
winglfw:
	rm -f *.o
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bygl/gl.object.c
	$(compiler) $(cflag) bygl/gl.shader.c
	$(compiler) $(cflag) bygl/gl.texture.c
	$(compiler) $(cflag) bygl/gl.vertex.c
	$(compiler) $(cflag) bygl/glfw.c
	$(compiler) $(cflag) arena.c

.PHONY:winglut
winglut:
	rm -f *.o
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bygl/glut.c
	$(compiler) $(cflag) arena.c

.PHONY:winsdl
winsdl:
	rm -f *.o
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
	$(compiler) $(cflag) bydummy/dummy.http.c
	$(compiler) $(cflag) bydummy/dummy.rgba.c
	$(compiler) $(cflag) bydummy/dummy.tty.c
	$(compiler) $(cflag) bydummy/dummy.vbo.c
	$(compiler) $(cflag) bydummy/dummy.ws.c
	$(compiler) $(cflag) bysdl/sdl2.c
	$(compiler) $(cflag) arena.c
##########################################################




clean:
	rm -f *.o *.a
