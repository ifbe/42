compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = arena.c \
root-hw/1602.c \
root-hw/12864.c \
root-robot/3d-printer.c \
root-robot/3d-router.c \
root-robot/car-bldc.c \
root-robot/car-step.c \
root-robot/car-toy.c \
root-robot/copter-hexa.c \
root-robot/copter-octo.c \
root-robot/copter-quad.c \
root-robot/robot-arm.c \
root-robot/robot-dog.c \
root-robot/robot-man.c \
root-robot/robot-snake.c \
twig-cli/out.cli.c \
twig-func/out.func.c \
twig-html/out.html.c \
twig-json/out.json.c \
twig-pcb/out.pcb.c \
twig-rgba/out.rgba.c \
twig-tui/out.tui.c \
twig-xml/out.xml.c \
twig-sch/out.sch.c \
twig-sch/sch.pin.c \
twig-sch/sch.chip.c \
twig-gles/out.vbo.c \
twig-gles/00-light.c \
twig-gles/01-mirror.c \
twig-gles/02-glass.c \
twig-gles/03-portal.c \
twig-gles/10-bg3d.c \
twig-gles/11-fg3d.c \
twig-gles/12-bg2d.c \
twig-gles/13-fg2d.c \
twig-gles/20-menu.c \
twig-gles/21-vkbd.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<




############################bare#########################
.PHONY:barecli
barecli:target
	rm -f bare.*.o
	$(compiler) $(cflag) os-bare/bare.cli.c
.PHONY:baretui
baretui:target
	rm -f bare.*.o
	$(compiler) $(cflag) os-bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target
	rm -f bare.*.o
	$(compiler) $(cflag) os-bare/bare.8bit.c
.PHONY:barefb
barefb:target
	rm -f bare.*.o
	$(compiler) $(cflag) os-bare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target
	rm -f efi.*.o
	$(compiler) $(cflag) os-efi/efi.cli.c
.PHONY:efifb
efifb:target
	rm -f efi.*.o
	$(compiler) $(cflag) os-efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.1.cli.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c

.PHONY:linuxtui
linuxtui:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.1.tui.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c

.PHONY:linuxfb
linuxfb:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.1.fb.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c

.PHONY:linuxgtk
linuxgtk:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` os-linux/linux.gtk.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c

.PHONY:linuxwayland
linuxwayland:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.1.wayland.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c

.PHONY:linuxxlib
linuxxlib:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.1.xlib.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c

.PHONY:linuxglfw
linuxglfw:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c
	$(compiler) $(cflag) root-lib/gl/glfw.c
	$(compiler) $(cflag) root-lib/gl/gl.object.c
	$(compiler) $(cflag) root-lib/gl/gl.shader.c
	$(compiler) $(cflag) root-lib/gl/gl.texture.c
	$(compiler) $(cflag) root-lib/gl/gl.vertex.c

.PHONY:linuxglut
linuxglut:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c
	$(compiler) $(cflag) root-lib/gl/glut.c

.PHONY:linuxsdl
linuxsdl:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) os-linux/linux.0.joy.c
	$(compiler) $(cflag) os-linux/linux.0.std.c
	$(compiler) $(cflag) os-linux/linux.0.tray.c
	$(compiler) $(cflag) os-linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) os-linux/linux.3.v4l2.c
	$(compiler) $(cflag) root-lib/sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os-mac/mac.0.joy.c
	$(compiler) $(cflag) os-mac/mac.0.std.c
	$(compiler) $(cflag) os-mac/mac.0.tray.c
	$(compiler) $(cflag) os-mac/mac.1.cli.c
	$(compiler) $(cflag) os-mac/mac.2.mic.c
	$(compiler) $(cflag) os-mac/mac.3.cam.c

.PHONY:mactui
mactui:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os-mac/mac.0.joy.c
	$(compiler) $(cflag) os-mac/mac.0.std.c
	$(compiler) $(cflag) os-mac/mac.0.tray.c
	$(compiler) $(cflag) os-mac/mac.1.tui.c
	$(compiler) $(cflag) os-mac/mac.2.mic.c
	$(compiler) $(cflag) os-mac/mac.3.cam.c

.PHONY:maccocoa
maccocoa:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os-mac/mac.0.joy.c
	$(compiler) $(cflag) os-mac/mac.0.std.c
	$(compiler) $(cflag) os-mac/mac.0.tray.c
	$(compiler) $(cflag) os-mac/mac.1.cocoa.m
	$(compiler) $(cflag) os-mac/mac.2.mic.c
	$(compiler) $(cflag) os-mac/mac.3.cam.c

.PHONY:macquartz
macquartz:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os-mac/mac.0.joy.c
	$(compiler) $(cflag) os-mac/mac.0.std.c
	$(compiler) $(cflag) os-mac/mac.0.tray.c
	$(compiler) $(cflag) os-mac/mac.1.quartz.c
	$(compiler) $(cflag) os-mac/mac.2.mic.c
	$(compiler) $(cflag) os-mac/mac.3.cam.c

.PHONY:macglfw
macglfw:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os-mac/mac.0.joy.c
	$(compiler) $(cflag) os-mac/mac.0.std.c
	$(compiler) $(cflag) os-mac/mac.0.tray.c
	$(compiler) $(cflag) os-mac/mac.2.mic.c
	$(compiler) $(cflag) os-mac/mac.3.cam.c
	$(compiler) $(cflag) root-lib/gl/glfw.c
	$(compiler) $(cflag) root-lib/gl/gl.object.c
	$(compiler) $(cflag) root-lib/gl/gl.shader.c
	$(compiler) $(cflag) root-lib/gl/gl.texture.c
	$(compiler) $(cflag) root-lib/gl/gl.vertex.c

.PHONY:macglut
macglut:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os-mac/mac.0.joy.c
	$(compiler) $(cflag) os-mac/mac.0.std.c
	$(compiler) $(cflag) os-mac/mac.0.tray.c
	$(compiler) $(cflag) os-mac/mac.2.mic.c
	$(compiler) $(cflag) os-mac/mac.3.cam.c
	$(compiler) $(cflag) root-lib/gl/glut.c

.PHONY:macsdl
macsdl:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) os-mac/mac.0.joy.c
	$(compiler) $(cflag) os-mac/mac.0.std.c
	$(compiler) $(cflag) os-mac/mac.0.tray.c
	$(compiler) $(cflag) os-mac/mac.2.mic.c
	$(compiler) $(cflag) os-mac/mac.3.cam.c
	$(compiler) $(cflag) root-lib/sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os-win/win.0.joy.c
	$(compiler) $(cflag) os-win/win.0.std.c
	$(compiler) $(cflag) os-win/win.0.tray.c
	$(compiler) $(cflag) os-win/win.1.cli.c
	$(compiler) $(cflag) os-win/win.2.waveio.c
	g++ $(cflag) -fno-rtti os-win/win.3.dshow.cpp

.PHONY:wintui
wintui:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os-win/win.0.joy.c
	$(compiler) $(cflag) os-win/win.0.std.c
	$(compiler) $(cflag) os-win/win.0.tray.c
	$(compiler) $(cflag) os-win/win.1.tui.c
	$(compiler) $(cflag) os-win/win.2.waveio.c
	g++ $(cflag) -fno-rtti os-win/win.3.dshow.cpp

.PHONY:winapi
winapi:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os-win/win.0.joy.c
	$(compiler) $(cflag) os-win/win.0.std.c
	$(compiler) $(cflag) os-win/win.0.tray.c
	$(compiler) $(cflag) os-win/win.1.api.c
	$(compiler) $(cflag) os-win/win.2.waveio.c
	g++ $(cflag) -fno-rtti os-win/win.3.dshow.cpp

.PHONY:wind3d
wind3d:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os-win/win.0.joy.c
	$(compiler) $(cflag) os-win/win.0.std.c
	$(compiler) $(cflag) os-win/win.0.tray.c
	$(compiler) $(cflag) os-win/win.1.d3d.c
	$(compiler) $(cflag) os-win/win.2.waveio.c
	g++ $(cflag) -fno-rtti os-win/win.3.dshow.cpp

.PHONY:winglfw
winglfw:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os-win/win.0.joy.c
	$(compiler) $(cflag) os-win/win.0.std.c
	$(compiler) $(cflag) os-win/win.0.tray.c
	$(compiler) $(cflag) root-lib/gl/glfw.c
	$(compiler) $(cflag) root-lib/gl/gl.object.c
	$(compiler) $(cflag) root-lib/gl/gl.shader.c
	$(compiler) $(cflag) root-lib/gl/gl.texture.c
	$(compiler) $(cflag) root-lib/gl/gl.vertex.c
	$(compiler) $(cflag) os-win/win.2.waveio.c
	g++ $(cflag) -fno-rtti os-win/win.3.dshow.cpp

.PHONY:winglut
winglut:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os-win/win.0.joy.c
	$(compiler) $(cflag) os-win/win.0.std.c
	$(compiler) $(cflag) os-win/win.0.tray.c
	$(compiler) $(cflag) root-lib/gl/glut.c
	$(compiler) $(cflag) os-win/win.waveio.c
	g++ $(cflag) -fno-rtti os-win/win.dshow.cpp

.PHONY:winsdl
winsdl:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) os-win/win.0.joy.c
	$(compiler) $(cflag) os-win/win.0.std.c
	$(compiler) $(cflag) os-win/win.0.tray.c
	$(compiler) $(cflag) root-lib/sdl/sdl2.c
	$(compiler) $(cflag) os-win/win.2.waveio.c
	g++ $(cflag) -fno-rtti os-win/win.3.dshow.cpp
##########################################################




clean:
	rm -f *.o *.a
