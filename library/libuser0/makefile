compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = \
gadget/1602/gadget.1602.c \
gadget/12864/gadget.12864.c \
robot/bionic/robot-arm.c \
robot/bionic/robot-dog.c \
robot/bionic/robot-man.c \
robot/bionic/robot-snake.c \
robot/copter/copter-hexa.c \
robot/copter/copter-octo.c \
robot/copter/copter-quad.c \
robot/factory/3d-printer.c \
robot/factory/3d-router.c \
robot/vehicle/car-bldc.c \
robot/vehicle/car-step.c \
robot/vehicle/car-toy.c \
world/cli/cli.base.c \
world/cli/cli.tool.c \
world/tui/tui.base.c \
world/tui/tui.tool.c \
world/func/func.base.c \
world/func/func.tool.c \
world/html/html.base.c \
world/html/html.tool.c \
world/json/json.base.c \
world/json/json.tool.c \
world/xml/xml.base.c \
world/xml/xml.tool.c \
world/sch/sch.base.c \
world/sch/sch.tool.c \
world/vbo/vbo.base.c \
world/vbo/vbo.tool.c \
world/rgba/rgba.base.c \
world/rgba/rgba.tool.c \
arena.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<




############################bare#########################
.PHONY:barecli
barecli:target
	$(compiler) $(cflag) window/os-bare/bare.cli.c
.PHONY:baretui
baretui:target
	$(compiler) $(cflag) window/os-bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target
	$(compiler) $(cflag) window/os-bare/bare.8bit.c
.PHONY:barefb
barefb:target
	$(compiler) $(cflag) window/os-bare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target
	$(compiler) $(cflag) window/os-efi/efi.cli.c
.PHONY:efifb
efifb:target
	$(compiler) $(cflag) window/os-efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linux
linux:
	$(compiler) $(cflag) cmdline/linux/linux.stdio.c
	$(compiler) $(cflag) systray/linux/linux.tray.c
	$(compiler) $(cflag) camera/linux/linux.v4l2.c
	$(compiler) $(cflag) micphone/linux/linux.pulse.c

.PHONY:linuxcli
linuxcli:target linux
	$(compiler) $(cflag) gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) window/linux/linux.cli.c

.PHONY:linuxtui
linuxtui:target linux
	$(compiler) $(cflag) gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) window/os-linux/linux.tui.c

.PHONY:linuxfb
linuxfb:target linux
	$(compiler) $(cflag) gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) window/os-linux/linux.fb.c

.PHONY:linuxgtk
linuxgtk:target linux
	$(compiler) $(cflag) gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` window/os-linux/linux.gtk.c

.PHONY:linuxwayland
linuxwayland:target linux
	$(compiler) $(cflag) gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) window/os-linux/linux.wayland.c

.PHONY:linuxxlib
linuxxlib:target linux
	$(compiler) $(cflag) gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) window/os-linux/linux.xlib.c

.PHONY:linuxglut
linuxglut:target linux
	$(compiler) $(cflag) gamepad/linux/linux.devjsn.c
	$(compiler) $(cflag) window/lib-gl/glut.c

.PHONY:linuxglfw
linuxglfw:target linux
	$(compiler) $(cflag) gamepad/glfw/glfw.gamepad.c
	$(compiler) $(cflag) window/lib-gl/glfw.c
	$(compiler) $(cflag) window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) window/lib-gl/globject.shader.c
	$(compiler) $(cflag) window/lib-gl/globject.texture.c
	$(compiler) $(cflag) window/lib-gl/globject.vertex.c

.PHONY:linuxsdl
linuxsdl:target linux
	$(compiler) $(cflag) gamepad/sdl/sdl.gamepad.c
	$(compiler) $(cflag) window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




#############################mac##########################
.PHONY:macos
macos:
	$(compiler) $(cflag) cmdline/mac/mac.stdio.c
	$(compiler) $(cflag) systray/mac/mac.tray.c
	$(compiler) $(cflag) micphone/mac/mac.mic.c
	$(compiler) $(cflag) camera/mac/mac.cam.c

.PHONY:maccli
maccli:target macos
	$(compiler) $(cflag) gamepad/mac.joy.c
	$(compiler) $(cflag) os-mac/mac.cli.c

.PHONY:mactui
mactui:target macos
	$(compiler) $(cflag) gamepad/mac.joy.c
	$(compiler) $(cflag) os-mac/mac.tui.c

.PHONY:maccocoa
maccocoa:target macos
	$(compiler) $(cflag) gamepad/mac.joy.c
	$(compiler) $(cflag) os-mac/mac.cocoa.m

.PHONY:macquartz
macquartz:target macos
	$(compiler) $(cflag) gamepad/mac.joy.c
	$(compiler) $(cflag) os-mac/mac.quartz.c

.PHONY:macglut
macglut:target macos
	$(compiler) $(cflag) gamepad/mac.joy.c
	$(compiler) $(cflag) lib-gl/glut.c

.PHONY:macglfw
macglfw:target macos
	$(compiler) $(cflag) gamepad/glfw/glfw.gamepad.c
	$(compiler) $(cflag) window/lib-gl/glfw.c
	$(compiler) $(cflag) window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) window/lib-gl/globject.shader.c
	$(compiler) $(cflag) window/lib-gl/globject.texture.c
	$(compiler) $(cflag) window/lib-gl/globject.vertex.c

.PHONY:macsdl
macsdl:target macos
	$(compiler) $(cflag) gamepad/sdl/sdl.gamepad.c
	$(compiler) $(cflag) lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################




###########################windows#######################
.PHONY:windows
windows:
	$(compiler) $(cflag) cmdline/win/win.stdio.c
	$(compiler) $(cflag) systray/win/win.tray.c
	$(compiler) $(cflag) micphone/win/win.waveio.c
	g++ $(cflag) -fno-rtti camera/win/win.dshow.cpp

.PHONY:wincli
wincli:target windows
	$(compiler) $(cflag) gamepad/win/win.xinput.c
	$(compiler) $(cflag) window/os-win/win.cli.c

.PHONY:wintui
wintui:target windows
	$(compiler) $(cflag) gamepad/win/win.xinput.c
	$(compiler) $(cflag) window/os-win/win.tui.c

.PHONY:winapi
winapi:target windows
	$(compiler) $(cflag) gamepad/win/win.xinput.c
	$(compiler) $(cflag) window/os-win/win.api.c

.PHONY:wind3d
wind3d:target windows
	$(compiler) $(cflag) gamepad/win/win.xinput.c
	$(compiler) $(cflag) window/os-win/win.d3d.c

.PHONY:winglut
winglut:target windows
	$(compiler) $(cflag) gamepad/win/win.xinput.c
	$(compiler) $(cflag) window/lib-gl/glut.c

.PHONY:winglfw
winglfw:target windows
	$(compiler) $(cflag) gamepad/glfw/glfw.gamepad.c
	$(compiler) $(cflag) window/lib-gl/glfw.c
	$(compiler) $(cflag) window/lib-gl/glwindow.test.c
	$(compiler) $(cflag) window/lib-gl/glwindow.host.c
	$(compiler) $(cflag) window/lib-gl/glwindow.coop.c
	$(compiler) $(cflag) window/lib-gl/glwindow.fbo.c
	$(compiler) $(cflag) window/lib-gl/globject.shader.c
	$(compiler) $(cflag) window/lib-gl/globject.texture.c
	$(compiler) $(cflag) window/lib-gl/globject.vertex.c

.PHONY:winsdl
winsdl:target windows
	$(compiler) $(cflag) gamepad/sdl/sdl.gamepad.c
	$(compiler) $(cflag) window/lib-sdl/sdl2.c
##########################################################




clean:
	rm -f *.o *.a
