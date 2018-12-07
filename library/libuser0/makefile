compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = arena.c \
10-hw/1602.c \
10-hw/12864.c \
10-mem/cli/out.cli.c \
10-mem/func/out.func.c \
10-mem/html/out.html.c \
10-mem/json/out.json.c \
10-mem/pcb/out.pcb.c \
10-mem/rgba/out.rgba.c \
10-mem/sch/out.sch.c \
10-mem/tui/out.tui.c \
10-mem/vbo/out.vbo.c \
10-mem/xml/out.xml.c \
20-bg/back.c \
20-bg/back.gif.c \
20-bg/back.jpg.c \
20-fg/fore.c \
20-fg/menu.list.c \
20-fg/menu.nav.c \
20-fg/menu.tab.c \
20-fg/menu.tag.c \
20-fg/mode0.testland.c \
20-fg/mode1.console.c \
20-fg/mode2.overview.c \
20-fg/mode3.detail.c \
20-fg/mode4.win.c \
20-fg/mode5.rts.c \
20-fg/mode6.editor.c \
20-fg/mode6.editor.camera.c \
20-fg/mode6.editor.target.c \
20-fg/mode7.player.c \
20-menu/popup.c \
20-vkbd/vkbd.c \
20-vkbd/vkbd.joystick.c \
20-vkbd/vkbd.keyboard.c \
30-glass/view.glass.c \
30-light/view.light.c \
30-mirror/view.mirror.c \
30-portal/view.portal.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<




############################bare#########################
.PHONY:barecli
barecli:target
	rm -f bare.*.o
	$(compiler) $(cflag) 00-os/bare/bare.cli.c
.PHONY:baretui
baretui:target
	rm -f bare.*.o
	$(compiler) $(cflag) 00-os/bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target
	rm -f bare.*.o
	$(compiler) $(cflag) 00-os/bare/bare.8bit.c
.PHONY:barefb
barefb:target
	rm -f bare.*.o
	$(compiler) $(cflag) 00-os/bare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target
	rm -f efi.*.o
	$(compiler) $(cflag) 00-os/efi/efi.cli.c
.PHONY:efifb
efifb:target
	rm -f efi.*.o
	$(compiler) $(cflag) 00-os/efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linuxcli
linuxcli:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.1.cli.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c

.PHONY:linuxtui
linuxtui:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.1.tui.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c

.PHONY:linuxfb
linuxfb:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.1.fb.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c

.PHONY:linuxgtk
linuxgtk:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` 00-os/linux/linux.gtk.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c

.PHONY:linuxwayland
linuxwayland:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.1.wayland.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c

.PHONY:linuxxlib
linuxxlib:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.1.xlib.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c

.PHONY:linuxglfw
linuxglfw:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c
	$(compiler) $(cflag) 00-lib/gl/glfw.c
	$(compiler) $(cflag) 00-lib/gl/gl.object.c
	$(compiler) $(cflag) 00-lib/gl/gl.shader.c
	$(compiler) $(cflag) 00-lib/gl/gl.texture.c
	$(compiler) $(cflag) 00-lib/gl/gl.vertex.c

.PHONY:linuxglut
linuxglut:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c
	$(compiler) $(cflag) 00-lib/gl/glut.c

.PHONY:linuxsdl
linuxsdl:target
	rm -f linux.*.o gl*.o
	$(compiler) $(cflag) 00-os/linux/linux.0.joy.c
	$(compiler) $(cflag) 00-os/linux/linux.0.term.c
	$(compiler) $(cflag) 00-os/linux/linux.0.tray.c
	$(compiler) $(cflag) 00-os/linux/linux.2.pulseaudio.c
	$(compiler) $(cflag) 00-os/linux/linux.3.v4l2.c
	$(compiler) $(cflag) 00-lib/sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) 00-os/mac/mac.0.joy.c
	$(compiler) $(cflag) 00-os/mac/mac.0.term.c
	$(compiler) $(cflag) 00-os/mac/mac.0.tray.c
	$(compiler) $(cflag) 00-os/mac/mac.1.cli.c
	$(compiler) $(cflag) 00-os/mac/mac.2.mic.c
	$(compiler) $(cflag) 00-os/mac/mac.3.cam.c

.PHONY:mactui
mactui:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) 00-os/mac/mac.0.joy.c
	$(compiler) $(cflag) 00-os/mac/mac.0.term.c
	$(compiler) $(cflag) 00-os/mac/mac.0.tray.c
	$(compiler) $(cflag) 00-os/mac/mac.1.tui.c
	$(compiler) $(cflag) 00-os/mac/mac.2.mic.c
	$(compiler) $(cflag) 00-os/mac/mac.3.cam.c

.PHONY:maccocoa
maccocoa:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) 00-os/mac/mac.0.joy.c
	$(compiler) $(cflag) 00-os/mac/mac.0.term.c
	$(compiler) $(cflag) 00-os/mac/mac.0.tray.c
	$(compiler) $(cflag) 00-os/mac/mac.1.cocoa.m
	$(compiler) $(cflag) 00-os/mac/mac.2.mic.c
	$(compiler) $(cflag) 00-os/mac/mac.3.cam.c

.PHONY:macquartz
macquartz:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) 00-os/mac/mac.0.joy.c
	$(compiler) $(cflag) 00-os/mac/mac.0.term.c
	$(compiler) $(cflag) 00-os/mac/mac.0.tray.c
	$(compiler) $(cflag) 00-os/mac/mac.1.quartz.c
	$(compiler) $(cflag) 00-os/mac/mac.2.mic.c
	$(compiler) $(cflag) 00-os/mac/mac.3.cam.c

.PHONY:macglfw
macglfw:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) 00-os/mac/mac.0.joy.c
	$(compiler) $(cflag) 00-os/mac/mac.0.term.c
	$(compiler) $(cflag) 00-os/mac/mac.0.tray.c
	$(compiler) $(cflag) 00-os/mac/mac.2.mic.c
	$(compiler) $(cflag) 00-os/mac/mac.3.cam.c
	$(compiler) $(cflag) 00-lib/gl/glfw.c
	$(compiler) $(cflag) 00-lib/gl/gl.object.c
	$(compiler) $(cflag) 00-lib/gl/gl.shader.c
	$(compiler) $(cflag) 00-lib/gl/gl.texture.c
	$(compiler) $(cflag) 00-lib/gl/gl.vertex.c

.PHONY:macglut
macglut:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) 00-os/mac/mac.0.joy.c
	$(compiler) $(cflag) 00-os/mac/mac.0.term.c
	$(compiler) $(cflag) 00-os/mac/mac.0.tray.c
	$(compiler) $(cflag) 00-os/mac/mac.2.mic.c
	$(compiler) $(cflag) 00-os/mac/mac.3.cam.c
	$(compiler) $(cflag) 00-lib/gl/glut.c

.PHONY:macsdl
macsdl:target
	rm -f mac.*.o gl*.o
	$(compiler) $(cflag) 00-os/mac/mac.0.joy.c
	$(compiler) $(cflag) 00-os/mac/mac.0.term.c
	$(compiler) $(cflag) 00-os/mac/mac.0.tray.c
	$(compiler) $(cflag) 00-os/mac/mac.2.mic.c
	$(compiler) $(cflag) 00-os/mac/mac.3.cam.c
	$(compiler) $(cflag) 00-lib/sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) 00-os/win/win.0.joy.c
	$(compiler) $(cflag) 00-os/win/win.0.term.c
	$(compiler) $(cflag) 00-os/win/win.0.tray.c
	$(compiler) $(cflag) 00-os/win/win.1.cli.c
	$(compiler) $(cflag) 00-os/win/win.2.waveio.c
	g++ $(cflag) -fno-rtti 00-os/win/win.3.dshow.cpp

.PHONY:wintui
wintui:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) 00-os/win/win.0.joy.c
	$(compiler) $(cflag) 00-os/win/win.0.term.c
	$(compiler) $(cflag) 00-os/win/win.0.tray.c
	$(compiler) $(cflag) 00-os/win/win.1.tui.c
	$(compiler) $(cflag) 00-os/win/win.2.waveio.c
	g++ $(cflag) -fno-rtti 00-os/win/win.3.dshow.cpp

.PHONY:winapi
winapi:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) 00-os/win/win.0.joy.c
	$(compiler) $(cflag) 00-os/win/win.0.term.c
	$(compiler) $(cflag) 00-os/win/win.0.tray.c
	$(compiler) $(cflag) 00-os/win/win.1.api.c
	$(compiler) $(cflag) 00-os/win/win.2.waveio.c
	g++ $(cflag) -fno-rtti 00-os/win/win.3.dshow.cpp

.PHONY:wind3d
wind3d:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) 00-os/win/win.0.joy.c
	$(compiler) $(cflag) 00-os/win/win.0.term.c
	$(compiler) $(cflag) 00-os/win/win.0.tray.c
	$(compiler) $(cflag) 00-os/win/win.1.d3d.c
	$(compiler) $(cflag) 00-os/win/win.2.waveio.c
	g++ $(cflag) -fno-rtti 00-os/win/win.3.dshow.cpp

.PHONY:winglfw
winglfw:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) 00-os/win/win.0.joy.c
	$(compiler) $(cflag) 00-os/win/win.0.term.c
	$(compiler) $(cflag) 00-os/win/win.0.tray.c
	$(compiler) $(cflag) 00-lib/gl/glfw.c
	$(compiler) $(cflag) 00-lib/gl/gl.object.c
	$(compiler) $(cflag) 00-lib/gl/gl.shader.c
	$(compiler) $(cflag) 00-lib/gl/gl.texture.c
	$(compiler) $(cflag) 00-lib/gl/gl.vertex.c
	$(compiler) $(cflag) 00-os/win/win.2.waveio.c
	g++ $(cflag) -fno-rtti 00-os/win/win.3.dshow.cpp

.PHONY:winglut
winglut:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) 00-os/win/win.0.joy.c
	$(compiler) $(cflag) 00-os/win/win.0.term.c
	$(compiler) $(cflag) 00-os/win/win.0.tray.c
	$(compiler) $(cflag) 00-lib/gl/glut.c
	$(compiler) $(cflag) 00-os/win/win.waveio.c
	g++ $(cflag) -fno-rtti 00-os/win/win.dshow.cpp

.PHONY:winsdl
winsdl:target
	rm -f win.*.o gl*.o
	$(compiler) $(cflag) 00-os/win/win.0.joy.c
	$(compiler) $(cflag) 00-os/win/win.0.term.c
	$(compiler) $(cflag) 00-os/win/win.0.tray.c
	$(compiler) $(cflag) 00-lib/sdl/sdl2.c
	$(compiler) $(cflag) 00-os/win/win.2.waveio.c
	g++ $(cflag) -fno-rtti 00-os/win/win.3.dshow.cpp
##########################################################




clean:
	rm -f *.o *.a
