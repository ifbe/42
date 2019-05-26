compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = \
o-gadget/1602/gadget.1602.c \
o-gadget/12864/gadget.12864.c \
h-robot/bionic/robot-arm.c \
h-robot/bionic/robot-dog.c \
h-robot/bionic/robot-man.c \
h-robot/bionic/robot-snake.c \
h-robot/copter/copter-hexa.c \
h-robot/copter/copter-octo.c \
h-robot/copter/copter-quad.c \
h-robot/factory/3d-printer.c \
h-robot/factory/3d-router.c \
h-robot/vehicle/car-bldc.c \
h-robot/vehicle/car-step.c \
h-robot/vehicle/car-toy.c \
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
	$(compiler) $(cflag) h-cmdline/linux/linux.stdio.c
	$(compiler) $(cflag) h-systray/linux/linux.tray.c
	$(compiler) $(cflag) i-camera/linux/linux.v4l2.c
	$(compiler) $(cflag) i-micphone/linux/linux.pulse.c

.PHONY:linuxcli
linuxcli:target linux
	$(compiler) $(cflag) h-gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) o-window/linux/linux.cli.c

.PHONY:linuxtui
linuxtui:target linux
	$(compiler) $(cflag) h-gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) o-window/os-linux/linux.tui.c

.PHONY:linuxfb
linuxfb:target linux
	$(compiler) $(cflag) h-gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) o-window/os-linux/linux.fb.c

.PHONY:linuxgtk
linuxgtk:target linux
	$(compiler) $(cflag) h-gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` o-window/os-linux/linux.gtk.c

.PHONY:linuxwayland
linuxwayland:target linux
	$(compiler) $(cflag) h-gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) o-window/os-linux/linux.wayland.c

.PHONY:linuxxlib
linuxxlib:target linux
	$(compiler) $(cflag) h-gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) o-window/os-linux/linux.xlib.c

.PHONY:linuxglut
linuxglut:target linux
	$(compiler) $(cflag) h-gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) o-window/lib-gl/glut.c

.PHONY:linuxglfw
linuxglfw:target linux
	$(compiler) $(cflag) h-gamepad/glfw/glfw.gamepad.c
	$(compiler) $(cflag) o-window/lib-gl/glfw.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) o-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) o-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) o-window/lib-gl/globject.vertex.c

.PHONY:linuxsdl
linuxsdl:target linux
	$(compiler) $(cflag) h-gamepad/sdl/sdl.gamepad.c
	$(compiler) $(cflag) o-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




#############################mac##########################
.PHONY:macos
macos:
	$(compiler) $(cflag) h-cmdline/mac/mac.stdio.c
	$(compiler) $(cflag) h-systray/mac/mac.tray.c
	$(compiler) $(cflag) i-camera/mac/mac.cam.c
	$(compiler) $(cflag) i-micphone/mac/mac.mic.c

.PHONY:maccli
maccli:target macos
	$(compiler) $(cflag) h-gamepad/mac.joy.c
	$(compiler) $(cflag) o-window/os-mac/mac.cli.c

.PHONY:mactui
mactui:target macos
	$(compiler) $(cflag) h-gamepad/mac.joy.c
	$(compiler) $(cflag) o-window/os-mac/mac.tui.c

.PHONY:maccocoa
maccocoa:target macos
	$(compiler) $(cflag) h-gamepad/mac.joy.c
	$(compiler) $(cflag) o-window/os-mac/mac.cocoa.m

.PHONY:macquartz
macquartz:target macos
	$(compiler) $(cflag) h-gamepad/mac.joy.c
	$(compiler) $(cflag) o-window/os-mac/mac.quartz.c

.PHONY:macglut
macglut:target macos
	$(compiler) $(cflag) h-gamepad/mac.joy.c
	$(compiler) $(cflag) o-window/lib-gl/glut.c

.PHONY:macglfw
macglfw:target macos
	$(compiler) $(cflag) h-gamepad/glfw/glfw.gamepad.c
	$(compiler) $(cflag) o-window/lib-gl/glfw.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) o-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) o-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) o-window/lib-gl/globject.vertex.c

.PHONY:macsdl
macsdl:target macos
	$(compiler) $(cflag) h-gamepad/sdl/sdl.gamepad.c
	$(compiler) $(cflag) o-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




###########################windows#######################
.PHONY:windows
windows:
	$(compiler) $(cflag) h-cmdline/win/win.stdio.c
	$(compiler) $(cflag) h-systray/win/win.tray.c
	g++ $(cflag) -fno-rtti i-camera/win/win.dshow.cpp
	$(compiler) $(cflag) i-micphone/win/win.waveio.c

.PHONY:wincli
wincli:target windows
	$(compiler) $(cflag) h-gamepad/win/win.xinput.c
	$(compiler) $(cflag) o-window/os-win/win.cli.c

.PHONY:wintui
wintui:target windows
	$(compiler) $(cflag) h-gamepad/win/win.xinput.c
	$(compiler) $(cflag) o-window/os-win/win.tui.c

.PHONY:winapi
winapi:target windows
	$(compiler) $(cflag) h-gamepad/win/win.xinput.c
	$(compiler) $(cflag) o-window/os-win/win.api.c

.PHONY:wind3d
wind3d:target windows
	$(compiler) $(cflag) h-gamepad/win/win.xinput.c
	$(compiler) $(cflag) o-window/os-win/win.d3d.c

.PHONY:winglut
winglut:target windows
	$(compiler) $(cflag) h-gamepad/win/win.xinput.c
	$(compiler) $(cflag) o-window/lib-gl/glut.c

.PHONY:winglfw
winglfw:target windows
	$(compiler) $(cflag) h-gamepad/glfw/glfw.gamepad.c
	$(compiler) $(cflag) o-window/lib-gl/glfw.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) o-window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) o-window/lib-gl/globject.shader.c
	$(compiler) $(cflag) o-window/lib-gl/globject.texture.c
	$(compiler) $(cflag) o-window/lib-gl/globject.vertex.c

.PHONY:winsdl
winsdl:target windows
	$(compiler) $(cflag) h-gamepad/sdl/sdl.gamepad.c
	$(compiler) $(cflag) o-window/lib-sdl/sdl2.c
##########################################################




clean:
	rm -f *.o *.a
