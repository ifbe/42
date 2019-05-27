compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = \
o-gadget/1602/gadget.1602.c \
o-gadget/12864/gadget.12864.c \
x-robot/bionic/robot-arm.c \
x-robot/bionic/robot-dog.c \
x-robot/bionic/robot-man.c \
x-robot/bionic/robot-snake.c \
x-robot/copter/copter-hexa.c \
x-robot/copter/copter-octo.c \
x-robot/copter/copter-quad.c \
x-robot/factory/3d-printer.c \
x-robot/factory/3d-router.c \
x-robot/vehicle/car-bldc.c \
x-robot/vehicle/car-step.c \
x-robot/vehicle/car-toy.c \
arena.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<




############################bare#########################
.PHONY:barecli
barecli:target
	$(compiler) $(cflag) o-window/os-bare/bare.cli.c
.PHONY:baretui
baretui:target
	$(compiler) $(cflag) o-window/os-bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target
	$(compiler) $(cflag) o-window/os-bare/bare.8bit.c
.PHONY:barefb
barefb:target
	$(compiler) $(cflag) o-window/os-bare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target
	$(compiler) $(cflag) o-window/os-efi/efi.cli.c
.PHONY:efifb
efifb:target
	$(compiler) $(cflag) o-window/os-efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linux
linux:
	$(compiler) $(cflag) i-camera/linux/linux.v4l2.c
	$(compiler) $(cflag) i-micphone/linux/linux.pulse.c
	$(compiler) $(cflag) x-cmdline/linux/linux.stdio.c
	$(compiler) $(cflag) x-systray/linux/linux.tray.c

.PHONY:linuxcli
linuxcli:target linux
	$(compiler) $(cflag) o-window/linux/linux.cli.c
	$(compiler) $(cflag) x-gamepad/linux/linux.devjsn.c

.PHONY:linuxtui
linuxtui:target linux
	$(compiler) $(cflag) o-window/os-linux/linux.tui.c
	$(compiler) $(cflag) x-gamepad/linux/linux.devjsn.c

.PHONY:linuxfb
linuxfb:target linux
	$(compiler) $(cflag) o-window/os-linux/linux.fb.c
	$(compiler) $(cflag) x-gamepad/linux/linux.devjsn.c

.PHONY:linuxgtk
linuxgtk:target linux
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` o-window/os-linux/linux.gtk.c
	$(compiler) $(cflag) x-gamepad/linux/linux.devjsn.c

.PHONY:linuxwayland
linuxwayland:target linux
	$(compiler) $(cflag) o-window/os-linux/linux.wayland.c
	$(compiler) $(cflag) x-gamepad/linux/linux.devjsn.c

.PHONY:linuxxlib
linuxxlib:target linux
	$(compiler) $(cflag) o-window/os-linux/linux.xlib.c
	$(compiler) $(cflag) x-gamepad/linux/linux.devjsn.c

.PHONY:linuxglut
linuxglut:target linux
	$(compiler) $(cflag) o-window/lib-gl/glut.c
	$(compiler) $(cflag) x-gamepad/linux/linux.devjsn.c

.PHONY:linuxglfw
linuxglfw:target linux
	$(compiler) $(cflag) o-window/lib-gl/glfw.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) o-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) o-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) o-window/lib-gl/globject.vertex.c
	$(compiler) $(cflag) x-gamepad/glfw/glfw.gamepad.c

.PHONY:linuxsdl
linuxsdl:target linux
	$(compiler) $(cflag) o-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) x-gamepad/sdl/sdl.gamepad.c
#########################################################




#############################mac##########################
.PHONY:macos
macos:
	$(compiler) $(cflag) i-camera/mac/mac.cam.c
	$(compiler) $(cflag) i-micphone/mac/mac.mic.c
	$(compiler) $(cflag) x-cmdline/mac/mac.stdio.c
	$(compiler) $(cflag) x-systray/mac/mac.tray.c

.PHONY:maccli
maccli:target macos
	$(compiler) $(cflag) o-window/os-mac/mac.cli.c
	$(compiler) $(cflag) x-gamepad/mac.joy.c

.PHONY:mactui
mactui:target macos
	$(compiler) $(cflag) o-window/os-mac/mac.tui.c
	$(compiler) $(cflag) x-gamepad/mac.joy.c

.PHONY:maccocoa
maccocoa:target macos
	$(compiler) $(cflag) o-window/os-mac/mac.cocoa.m
	$(compiler) $(cflag) x-gamepad/mac.joy.c

.PHONY:macquartz
macquartz:target macos
	$(compiler) $(cflag) o-window/os-mac/mac.quartz.c
	$(compiler) $(cflag) x-gamepad/mac.joy.c

.PHONY:macglut
macglut:target macos
	$(compiler) $(cflag) o-window/lib-gl/glut.c
	$(compiler) $(cflag) x-gamepad/mac.joy.c

.PHONY:macglfw
macglfw:target macos
	$(compiler) $(cflag) o-window/lib-gl/glfw.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) o-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) o-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) o-window/lib-gl/globject.vertex.c
	$(compiler) $(cflag) x-gamepad/glfw/glfw.gamepad.c

.PHONY:macsdl
macsdl:target macos
	$(compiler) $(cflag) o-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) x-gamepad/sdl/sdl.gamepad.c
#########################################################




###########################windows#######################
.PHONY:windows
windows:
	g++ $(cflag) -fno-rtti i-camera/win/win.dshow.cpp
	$(compiler) $(cflag) i-micphone/win/win.waveio.c
	$(compiler) $(cflag) x-cmdline/win/win.stdio.c
	$(compiler) $(cflag) x-systray/win/win.tray.c

.PHONY:wincli
wincli:target windows
	$(compiler) $(cflag) o-window/os-win/win.cli.c
	$(compiler) $(cflag) x-gamepad/win/win.xinput.c

.PHONY:wintui
wintui:target windows
	$(compiler) $(cflag) o-window/os-win/win.tui.c
	$(compiler) $(cflag) x-gamepad/win/win.xinput.c

.PHONY:winapi
winapi:target windows
	$(compiler) $(cflag) o-window/os-win/win.api.c
	$(compiler) $(cflag) x-gamepad/win/win.xinput.c

.PHONY:wind3d
wind3d:target windows
	$(compiler) $(cflag) o-window/os-win/win.d3d.c
	$(compiler) $(cflag) x-gamepad/win/win.xinput.c

.PHONY:winglut
winglut:target windows
	$(compiler) $(cflag) o-window/lib-gl/glut.c
	$(compiler) $(cflag) x-gamepad/win/win.xinput.c

.PHONY:winglfw
winglfw:target windows
	$(compiler) $(cflag) o-window/lib-gl/glfw.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) o-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) o-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) o-window/lib-gl/globject.vertex.c
	$(compiler) $(cflag) x-gamepad/glfw/glfw.gamepad.c

.PHONY:winsdl
winsdl:target windows
	$(compiler) $(cflag) o-window/lib-sdl/sdl2.c
	$(compiler) $(cflag) x-gamepad/sdl/sdl.gamepad.c
##########################################################




clean:
	rm -f *.o *.a
