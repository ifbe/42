compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = arena.c \
root-hw/1602.c \
root-hw/12864.c \
root-mem/cli/out.cli.c \
root-mem/func/out.func.c \
root-mem/html/out.html.c \
root-mem/json/out.json.c \
root-mem/pcb/out.pcb.c \
root-mem/rgba/out.rgba.c \
root-mem/sch/out.sch.c \
root-mem/tui/out.tui.c \
root-mem/vbo/out.vbo.c \
root-mem/xml/out.xml.c \
twig-bg/back.c \
twig-bg/back.gif.c \
twig-bg/back.jpg.c \
twig-fg/fore.c \
twig-fg/menu.list.c \
twig-fg/menu.nav.c \
twig-fg/menu.tab.c \
twig-fg/menu.tag.c \
twig-fg/mode0.testland.c \
twig-fg/mode1.console.c \
twig-fg/mode2.overview.c \
twig-fg/mode3.detail.c \
twig-fg/mode4.win.c \
twig-fg/mode5.rts.c \
twig-fg/mode6.editor.c \
twig-fg/mode6.editor.camera.c \
twig-fg/mode6.editor.target.c \
twig-fg/mode7.player.c \
twig-menu/popup.c \
twig-vkbd/vkbd.c \
twig-vkbd/vkbd.joystick.c \
twig-vkbd/vkbd.keyboard.c \
twig-glass/view.glass.c \
twig-light/view.light.c \
twig-mirror/view.mirror.c \
twig-portal/view.portal.c
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
