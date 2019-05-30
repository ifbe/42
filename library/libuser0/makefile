compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = \
v2r-gadget/1602/gadget.1602.c \
v2r-gadget/12864/gadget.12864.c \
v2r-robot/bionic/robot-arm.c \
v2r-robot/bionic/robot-dog.c \
v2r-robot/bionic/robot-man.c \
v2r-robot/bionic/robot-snake.c \
v2r-robot/copter/copter-hexa.c \
v2r-robot/copter/copter-octo.c \
v2r-robot/copter/copter-quad.c \
v2r-robot/factory/3d-printer.c \
v2r-robot/factory/3d-router.c \
v2r-robot/vehicle/car-bldc.c \
v2r-robot/vehicle/car-step.c \
v2r-robot/vehicle/car-toy.c \
arena.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<




############################bare#########################
.PHONY:barecli
barecli:target
	$(compiler) $(cflag) v2r-window/os-bare/bare.cli.c
.PHONY:baretui
baretui:target
	$(compiler) $(cflag) v2r-window/os-bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target
	$(compiler) $(cflag) v2r-window/os-bare/bare.8bit.c
.PHONY:barefb
barefb:target
	$(compiler) $(cflag) v2r-window/os-bare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target
	$(compiler) $(cflag) v2r-window/os-efi/efi.cli.c
.PHONY:efifb
efifb:target
	$(compiler) $(cflag) v2r-window/os-efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linux
linux:
	$(compiler) $(cflag) r2v-camera/linux/linux.v4l2.c
	$(compiler) $(cflag) r2v-micphone/linux/linux.pulse.c
	$(compiler) $(cflag) ctl-cmdline/linux/linux.stdio.c
	$(compiler) $(cflag) ctl-systray/linux/linux.tray.c

.PHONY:linuxcli
linuxcli:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.cli.c
	$(compiler) $(cflag) ctl-gamepad/linux/linux.devjsn.c

.PHONY:linuxtui
linuxtui:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.tui.c
	$(compiler) $(cflag) ctl-gamepad/linux/linux.devjsn.c

.PHONY:linuxfb
linuxfb:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.fb.c
	$(compiler) $(cflag) ctl-gamepad/linux/linux.devjsn.c

.PHONY:linuxgtk
linuxgtk:target linux
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` v2r-window/os-linux/linux.gtk.c
	$(compiler) $(cflag) ctl-gamepad/linux/linux.devjsn.c

.PHONY:linuxwayland
linuxwayland:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.wayland.c
	$(compiler) $(cflag) ctl-gamepad/linux/linux.devjsn.c

.PHONY:linuxxlib
linuxxlib:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.xlib.c
	$(compiler) $(cflag) ctl-gamepad/linux/linux.devjsn.c

.PHONY:linuxglut
linuxglut:target linux
	$(compiler) $(cflag) v2r-window/lib-gl/glut.c
	$(compiler) $(cflag) ctl-gamepad/linux/linux.devjsn.c

.PHONY:linuxglfw
linuxglfw:target linux
	$(compiler) $(cflag) v2r-window/lib-gl/glfw.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.vertex.c
	$(compiler) $(cflag) ctl-gamepad/glfw/glfw.gamepad.c

.PHONY:linuxsdl
linuxsdl:target linux
	$(compiler) $(cflag) v2r-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) ctl-gamepad/sdl/sdl.gamepad.c
#########################################################




#############################mac##########################
.PHONY:macos
macos:
	$(compiler) $(cflag) r2v-camera/mac/mac.cam.c
	$(compiler) $(cflag) r2v-micphone/mac/mac.mic.c
	$(compiler) $(cflag) ctl-cmdline/mac/mac.stdio.c
	$(compiler) $(cflag) ctl-systray/mac/mac.tray.c

.PHONY:maccli
maccli:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.cli.c
	$(compiler) $(cflag) ctl-gamepad/mac/mac.gamepad.c

.PHONY:mactui
mactui:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.tui.c
	$(compiler) $(cflag) ctl-gamepad/mac/mac.gamepad.c

.PHONY:maccocoa
maccocoa:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.cocoa.m
	$(compiler) $(cflag) ctl-gamepad/mac/mac.gamepad.c

.PHONY:macquartz
macquartz:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.quartz.c
	$(compiler) $(cflag) ctl-gamepad/mac/mac.gamepad.c

.PHONY:macglut
macglut:target macos
	$(compiler) $(cflag) v2r-window/lib-gl/glut.c
	$(compiler) $(cflag) ctl-gamepad/mac/mac.gamepad.c

.PHONY:macglfw
macglfw:target macos
	$(compiler) $(cflag) v2r-window/lib-gl/glfw.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.vertex.c
	$(compiler) $(cflag) ctl-gamepad/glfw/glfw.gamepad.c

.PHONY:macsdl
macsdl:target macos
	$(compiler) $(cflag) v2r-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) ctl-gamepad/sdl/sdl.gamepad.c
#########################################################




###########################windows#######################
.PHONY:windows
windows:
	g++ $(cflag) -fno-rtti r2v-camera/win/win.dshow.cpp
	$(compiler) $(cflag) r2v-micphone/win/win.waveio.c
	$(compiler) $(cflag) ctl-cmdline/win/win.stdio.c
	$(compiler) $(cflag) ctl-systray/win/win.tray.c

.PHONY:wincli
wincli:target windows
	$(compiler) $(cflag) v2r-window/os-win/win.cli.c
	$(compiler) $(cflag) ctl-gamepad/win/win.xinput.c

.PHONY:wintui
wintui:target windows
	$(compiler) $(cflag) v2r-window/os-win/win.tui.c
	$(compiler) $(cflag) ctl-gamepad/win/win.xinput.c

.PHONY:winapi
winapi:target windows
	$(compiler) $(cflag) v2r-window/os-win/win.api.c
	$(compiler) $(cflag) ctl-gamepad/win/win.xinput.c

.PHONY:wind3d
wind3d:target windows
	$(compiler) $(cflag) v2r-window/os-win/win.d3d.c
	$(compiler) $(cflag) ctl-gamepad/win/win.xinput.c

.PHONY:winglut
winglut:target windows
	$(compiler) $(cflag) v2r-window/lib-gl/glut.c
	$(compiler) $(cflag) ctl-gamepad/win/win.xinput.c

.PHONY:winglfw
winglfw:target windows
	$(compiler) $(cflag) v2r-window/lib-gl/glfw.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) v2r-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) v2r-window/lib-gl/globject.vertex.c
	$(compiler) $(cflag) ctl-gamepad/glfw/glfw.gamepad.c

.PHONY:winsdl
winsdl:target windows
	$(compiler) $(cflag) v2r-window/lib-sdl/sdl2.c
	$(compiler) $(cflag) ctl-gamepad/sdl/sdl.gamepad.c
##########################################################




clean:
	rm -f *.o *.a
