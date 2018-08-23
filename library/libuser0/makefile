compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = arena.c \
bydummy/dummy.func.c \
bydummy/dummy.html.c \
bydummy/dummy.json.c \
bydummy/dummy.rgba.c \
bydummy/dummy.tty.c \
bydummy/dummy.vbo.c \
bydummy/dummy.vpin.c \
bydummy/dummy.ws.c \
bydummy/dummy.xml.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<




############################bare#########################
.PHONY:barecli
barecli:target
	$(compiler) $(cflag) osbare/bare.cli.c
.PHONY:baretui
baretui:target
	$(compiler) $(cflag) osbare/bare.tui.c
.PHONY:bare8bit
bare8bit:target
	$(compiler) $(cflag) osbare/bare.8bit.c
.PHONY:barefb
barefb:target
	$(compiler) $(cflag) osbare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target
	$(compiler) $(cflag) osefi/efi.cli.c
.PHONY:efifb
efifb:target
	$(compiler) $(cflag) osefi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:target
	$(compiler) $(cflag) oslinux/linux.cli.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c

.PHONY:linuxtui
linuxtui:target
	$(compiler) $(cflag) oslinux/linux.tui.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c

.PHONY:linuxfb
linuxfb:target
	$(compiler) $(cflag) oslinux/linux.fb.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c

.PHONY:linuxgtk
linuxgtk:target
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` oslinux/linux.gtk.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c

.PHONY:linuxwayland
linuxwayland:target
	$(compiler) $(cflag) oslinux/linux.wayland.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c

.PHONY:linuxxlib
linuxxlib:target
	$(compiler) $(cflag) oslinux/linux.xlib.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c

.PHONY:linuxglfw
linuxglfw:target
	$(compiler) $(cflag) bygl/glfw.c
	$(compiler) $(cflag) bygl/gl.object.c
	$(compiler) $(cflag) bygl/gl.shader.c
	$(compiler) $(cflag) bygl/gl.texture.c
	$(compiler) $(cflag) bygl/gl.vertex.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c

.PHONY:linuxglut
linuxglut:target
	$(compiler) $(cflag) bygl/glut.c
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c

.PHONY:linuxsdl
linuxsdl:target
	$(compiler) $(cflag) bysdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) oslinux/linux.tray.c
	$(compiler) $(cflag) oslinux/linux.pulseaudio.c
	$(compiler) $(cflag) oslinux/linux.v4l2.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:target
	$(compiler) $(cflag) osmac/mac.cli.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c

.PHONY:mactui
mactui:target
	$(compiler) $(cflag) osmac/mac.tui.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c

.PHONY:maccocoa
maccocoa:target
	$(compiler) $(cflag) osmac/mac.cocoa.m
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c

.PHONY:macquartz
macquartz:target
	$(compiler) $(cflag) osmac/mac.quartz.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c

.PHONY:macglfw
macglfw:target
	$(compiler) $(cflag) bygl/glfw.c
	$(compiler) $(cflag) bygl/gl.object.c
	$(compiler) $(cflag) bygl/gl.shader.c
	$(compiler) $(cflag) bygl/gl.texture.c
	$(compiler) $(cflag) bygl/gl.vertex.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c

.PHONY:macglut
macglut:target
	$(compiler) $(cflag) bygl/glut.c
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c

.PHONY:macsdl
macsdl:target
	$(compiler) $(cflag) bysdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) osmac/mac.tray.c
	$(compiler) $(cflag) osmac/mac.cam.c
	$(compiler) $(cflag) osmac/mac.mic.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:target
	$(compiler) $(cflag) oswin/win.cli.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp

.PHONY:wintui
wintui:target
	$(compiler) $(cflag) oswin/win.tui.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp

.PHONY:winapi
winapi:target
	$(compiler) $(cflag) oswin/win.api.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp

.PHONY:wind3d
wind3d:target
	$(compiler) $(cflag) oswin/win.d3d.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp

.PHONY:winglfw
winglfw:target
	$(compiler) $(cflag) bygl/glfw.c
	$(compiler) $(cflag) bygl/gl.object.c
	$(compiler) $(cflag) bygl/gl.shader.c
	$(compiler) $(cflag) bygl/gl.texture.c
	$(compiler) $(cflag) bygl/gl.vertex.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp

.PHONY:winglut
winglut:target
	$(compiler) $(cflag) bygl/glut.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp

.PHONY:winsdl
winsdl:target
	$(compiler) $(cflag) bysdl/sdl2.c
	$(compiler) $(cflag) oswin/win.tray.c
	$(compiler) $(cflag) oswin/win.waveio.c
	g++ $(cflag) -fno-rtti oswin/win.dshow.cpp
##########################################################




clean:
	rm -f *.o *.a
