compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = arena.c \
in/in.ev.c \
in/in.data.c \
out/out.cli.c \
out/out.func.c \
out/out.html.c \
out/out.json.c \
out/out.pcb.c \
out/out.rgba.c \
out/out.sch.c \
out/out.tui.c \
out/out.vbo.c \
out/out.xml.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<




############################bare#########################
.PHONY:barecli
barecli:target
	rm -f bare.*.o
	$(compiler) $(cflag) os/osbare/bare.cli.c
.PHONY:baretui
baretui:target
	rm -f bare.*.o
	$(compiler) $(cflag) os/osbare/bare.tui.c
.PHONY:bare8bit
bare8bit:target
	rm -f bare.*.o
	$(compiler) $(cflag) os/osbare/bare.8bit.c
.PHONY:barefb
barefb:target
	rm -f bare.*.o
	$(compiler) $(cflag) os/osbare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target
	rm -f efi.*.o
	$(compiler) $(cflag) os/osefi/efi.cli.c
.PHONY:efifb
efifb:target
	rm -f efi.*.o
	$(compiler) $(cflag) os/osefi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) os/oslinux/linux.1.cli.c
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c

.PHONY:linuxtui
linuxtui:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) os/oslinux/linux.1.tui.c
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c

.PHONY:linuxfb
linuxfb:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) os/oslinux/linux.1.fb.c
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c

.PHONY:linuxgtk
linuxgtk:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` os/oslinux/linux.gtk.c
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c

.PHONY:linuxwayland
linuxwayland:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) os/oslinux/linux.1.wayland.c
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c

.PHONY:linuxxlib
linuxxlib:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) os/oslinux/linux.1.xlib.c
	$(compiler) $(cflag) os/oslinux/linux.2.v4l2.c
	$(compiler) $(cflag) os/oslinux/linux.3.pulseaudio.c

.PHONY:linuxglfw
linuxglfw:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) 3rd/bygl/glfw.c
	$(compiler) $(cflag) 3rd/bygl/gl.object.c
	$(compiler) $(cflag) 3rd/bygl/gl.shader.c
	$(compiler) $(cflag) 3rd/bygl/gl.texture.c
	$(compiler) $(cflag) 3rd/bygl/gl.vertex.c
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c

.PHONY:linuxglut
linuxglut:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) 3rd/bygl/glut.c
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c

.PHONY:linuxsdl
linuxsdl:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os/oslinux/linux.0.term.c
	$(compiler) $(cflag) os/oslinux/linux.0.tray.c
	$(compiler) $(cflag) 3rd/bysdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) os/oslinux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os/oslinux/linux.3.v4l2.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os/osmac/mac.0.term.c
	$(compiler) $(cflag) os/osmac/mac.0.tray.c
	$(compiler) $(cflag) os/osmac/mac.1.cli.c
	$(compiler) $(cflag) os/osmac/mac.2.mic.c
	$(compiler) $(cflag) os/osmac/mac.3.cam.c

.PHONY:mactui
mactui:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os/osmac/mac.0.term.c
	$(compiler) $(cflag) os/osmac/mac.0.tray.c
	$(compiler) $(cflag) os/osmac/mac.1.tui.c
	$(compiler) $(cflag) os/osmac/mac.2.mic.c
	$(compiler) $(cflag) os/osmac/mac.3.cam.c

.PHONY:maccocoa
maccocoa:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os/osmac/mac.0.term.c
	$(compiler) $(cflag) os/osmac/mac.0.tray.c
	$(compiler) $(cflag) os/osmac/mac.1.cocoa.m
	$(compiler) $(cflag) os/osmac/mac.2.mic.c
	$(compiler) $(cflag) os/osmac/mac.3.cam.c

.PHONY:macquartz
macquartz:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os/osmac/mac.0.term.c
	$(compiler) $(cflag) os/osmac/mac.0.tray.c
	$(compiler) $(cflag) os/osmac/mac.1.quartz.c
	$(compiler) $(cflag) os/osmac/mac.2.mic.c
	$(compiler) $(cflag) os/osmac/mac.3.cam.c

.PHONY:macglfw
macglfw:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os/osmac/mac.0.term.c
	$(compiler) $(cflag) os/osmac/mac.0.tray.c
	$(compiler) $(cflag) 3rd/bygl/glfw.c
	$(compiler) $(cflag) 3rd/bygl/gl.object.c
	$(compiler) $(cflag) 3rd/bygl/gl.shader.c
	$(compiler) $(cflag) 3rd/bygl/gl.texture.c
	$(compiler) $(cflag) 3rd/bygl/gl.vertex.c
	$(compiler) $(cflag) os/osmac/mac.2.mic.c
	$(compiler) $(cflag) os/osmac/mac.3.cam.c

.PHONY:macglut
macglut:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os/osmac/mac.0.term.c
	$(compiler) $(cflag) os/osmac/mac.0.tray.c
	$(compiler) $(cflag) 3rd/bygl/glut.c
	$(compiler) $(cflag) os/osmac/mac.2.mic.c
	$(compiler) $(cflag) os/osmac/mac.3.cam.c

.PHONY:macsdl
macsdl:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os/osmac/mac.0.term.c
	$(compiler) $(cflag) os/osmac/mac.0.tray.c
	$(compiler) $(cflag) 3rd/bysdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) os/osmac/mac.2.mic.c
	$(compiler) $(cflag) os/osmac/mac.3.cam.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os/oswin/win.0.term.c
	$(compiler) $(cflag) os/oswin/win.0.tray.c
	$(compiler) $(cflag) os/oswin/win.1.cli.c
	$(compiler) $(cflag) os/oswin/win.2.waveio.c
	g++ $(cflag) -fno-rtti os/oswin/win.3.dshow.cpp

.PHONY:wintui
wintui:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os/oswin/win.0.term.c
	$(compiler) $(cflag) os/oswin/win.0.tray.c
	$(compiler) $(cflag) os/oswin/win.1.tui.c
	$(compiler) $(cflag) os/oswin/win.2.waveio.c
	g++ $(cflag) -fno-rtti os/oswin/win.3.dshow.cpp

.PHONY:winapi
winapi:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os/oswin/win.0.term.c
	$(compiler) $(cflag) os/oswin/win.0.tray.c
	$(compiler) $(cflag) os/oswin/win.1.api.c
	$(compiler) $(cflag) os/oswin/win.2.waveio.c
	g++ $(cflag) -fno-rtti os/oswin/win.3.dshow.cpp

.PHONY:wind3d
wind3d:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os/oswin/win.0.term.c
	$(compiler) $(cflag) os/oswin/win.0.tray.c
	$(compiler) $(cflag) os/oswin/win.1.d3d.c
	$(compiler) $(cflag) os/oswin/win.2.waveio.c
	g++ $(cflag) -fno-rtti os/oswin/win.3.dshow.cpp

.PHONY:winglfw
winglfw:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os/oswin/win.0.term.c
	$(compiler) $(cflag) os/oswin/win.0.tray.c
	$(compiler) $(cflag) 3rd/bygl/glfw.c
	$(compiler) $(cflag) 3rd/bygl/gl.object.c
	$(compiler) $(cflag) 3rd/bygl/gl.shader.c
	$(compiler) $(cflag) 3rd/bygl/gl.texture.c
	$(compiler) $(cflag) 3rd/bygl/gl.vertex.c
	$(compiler) $(cflag) os/oswin/win.2.waveio.c
	g++ $(cflag) -fno-rtti os/oswin/win.3.dshow.cpp

.PHONY:winglut
winglut:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os/oswin/win.0.term.c
	$(compiler) $(cflag) os/oswin/win.0.tray.c
	$(compiler) $(cflag) 3rd/bygl/glut.c
	$(compiler) $(cflag) os/oswin/win.waveio.c
	g++ $(cflag) -fno-rtti os/oswin/win.dshow.cpp

.PHONY:winsdl
winsdl:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os/oswin/win.0.term.c
	$(compiler) $(cflag) os/oswin/win.0.tray.c
	$(compiler) $(cflag) 3rd/bysdl/sdl2.c
	$(compiler) $(cflag) os/oswin/win.2.waveio.c
	g++ $(cflag) -fno-rtti os/oswin/win.3.dshow.cpp
##########################################################




clean:
	rm -f *.o *.a
