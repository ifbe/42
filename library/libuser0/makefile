compiler ?= gcc
cflag ?= -c -O2 -fPIC -I.
SRC = \
r2v-sensor/sensor.ahrs.c \
r2v-sensor/sensor.slam.c \
r2v-sensor/sensor.lidar.c \
r2v-sensor/sensor.radar.c \
v2r-gadget/1602/gadget.1602.c \
v2r-gadget/12864/gadget.12864.c \
supply.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(compiler) $(cflag) $<
opengl:
	$(compiler) $(cflag) v2r-window/lib-gl/gldraw/gl.none.c
	$(compiler) $(cflag) v2r-window/lib-gl/gldraw/gl.easy.c
	$(compiler) $(cflag) v2r-window/lib-gl/gldraw/gl.full.c
	$(compiler) $(cflag) v2r-window/lib-gl/gltar/gl41.target.color.c
	$(compiler) $(cflag) v2r-window/lib-gl/gltar/gl41.target.cube.c
	$(compiler) $(cflag) v2r-window/lib-gl/gltar/gl41.target.depth.c
	$(compiler) $(cflag) v2r-window/lib-gl/gltar/gl41.target.gbuffer.c
	$(compiler) $(cflag) v2r-window/lib-gl/gltar/gl41.target.window.c
	$(compiler) $(cflag) v2r-window/lib-gl/globj/globj.fbo.c
	$(compiler) $(cflag) v2r-window/lib-gl/globj/globj.shader.c
	$(compiler) $(cflag) v2r-window/lib-gl/globj/globj.texture.c
	$(compiler) $(cflag) v2r-window/lib-gl/globj/globj.varray.c
	$(compiler) $(cflag) v2r-window/lib-gl/globj/globj.vertex.c




############################bare#########################
.PHYOY:bare
bare:
	$(compiler) $(cflag) w2w-cmdline/bare/bare.stdio.c
	$(compiler) $(cflag) w2w-gamepad/bare/bare.joy.c
	$(compiler) $(cflag) w2w-systray/bare/bare.tray.c
	$(compiler) $(cflag) r2v-camera/os-bare/bare.cam.c
	$(compiler) $(cflag) r2v-micphone/os-bare/bare.mic.c
	$(compiler) $(cflag) v2r-speaker/os-bare/bare.spk.c
.PHONY:barecli
barecli:target bare
	$(compiler) $(cflag) v2r-window/os-bare/bare.cli.c
.PHONY:baretui
baretui:target bare
	$(compiler) $(cflag) v2r-window/os-bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target bare
	$(compiler) $(cflag) v2r-window/os-bare/bare.8bit.c
.PHONY:barefb
barefb:target bare
	$(compiler) $(cflag) v2r-window/os-bare/bare.fb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target bare
	$(compiler) $(cflag) v2r-window/os-efi/efi.cli.c
.PHONY:efifb
efifb:target bare
	$(compiler) $(cflag) v2r-window/os-efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linux
linux:
	$(compiler) $(cflag) w2w-cmdline/linux/linux.stdio.c
	$(compiler) $(cflag) w2w-systray/linux/linux.tray.c
	$(compiler) $(cflag) r2v-micphone/os-linux/linux.pulsein.c
	$(compiler) $(cflag) v2r-speaker/os-linux/linux.pulseout.c
	$(compiler) $(cflag) r2v-camera/os-linux/linux.v4l2.c

.PHONY:linuxcli
linuxcli:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.cli.c
	$(compiler) $(cflag) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxtui
linuxtui:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.tui.c
	$(compiler) $(cflag) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxfb
linuxfb:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.fb.c
	$(compiler) $(cflag) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxgtk
linuxgtk:target linux
	$(compiler) $(cflag) `pkg-config --cflags gtk+-3.0` v2r-window/os-linux/linux.gtk.c
	$(compiler) $(cflag) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxwayland
linuxwayland:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.wayland.c
	$(compiler) $(cflag) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxxlib
linuxxlib:target linux
	$(compiler) $(cflag) v2r-window/os-linux/linux.xlib.c
	$(compiler) $(cflag) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxglut
linuxglut:target linux
	$(compiler) $(cflag) v2r-window/lib-gl/glut.c
	$(compiler) $(cflag) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxglfw
linuxglfw:target linux opengl
	$(compiler) $(cflag) v2r-window/lib-gl/gllib/glfw.c
	$(compiler) $(cflag) w2w-gamepad/glfw/glfw.gamepad.c

.PHONY:linuxsdl
linuxsdl:target linux
	$(compiler) $(cflag) v2r-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) w2w-gamepad/sdl/sdl.gamepad.c
#########################################################




#############################mac##########################
.PHONY:macos
macos:
	$(compiler) $(cflag) w2w-cmdline/mac/mac.stdio.c
	$(compiler) $(cflag) w2w-systray/mac/mac.tray.c
	$(compiler) $(cflag) r2v-micphone/os-mac/mac.aqin.c
	$(compiler) $(cflag) v2r-speaker/os-mac/mac.aqout.c
	$(compiler) $(cflag) r2v-camera/os-mac/mac.cam.c

.PHONY:maccli
maccli:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.cli.c
	$(compiler) $(cflag) w2w-gamepad/mac/mac.gamepad.c

.PHONY:mactui
mactui:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.tui.c
	$(compiler) $(cflag) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macquartz
macquartz:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.quartz.c
	$(compiler) $(cflag) w2w-gamepad/mac/mac.gamepad.c

.PHONY:maccocoa
maccocoa:target macos
	$(compiler) $(cflag) v2r-window/os-mac/mac.cocoa.m
	$(compiler) $(cflag) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macmetal
macmetal:target macos
	$(compiler) $(cflag) v2r-window/lib-metal/metal.m
	$(compiler) $(cflag) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macsdl
macsdl:target macos
	$(compiler) $(cflag) v2r-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(compiler) $(cflag) w2w-gamepad/sdl/sdl.gamepad.c

.PHONY:macglut
macglut:target macos
	$(compiler) $(cflag) v2r-window/lib-gl/glut.c
	$(compiler) $(cflag) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macglfw
macglfw:target macos opengl
	$(compiler) $(cflag) v2r-window/lib-gl/gllib/glfw.c
	$(compiler) $(cflag) w2w-gamepad/glfw/glfw.gamepad.c
#########################################################




###########################windows#######################
.PHONY:windows
windows:
	$(compiler) $(cflag) w2w-cmdline/win/win.stdio.c
	$(compiler) $(cflag) w2w-systray/win/win.tray.c
	$(compiler) $(cflag) r2v-micphone/os-win/win.wavein.c
	$(compiler) $(cflag) v2r-speaker/os-win/win.waveout.c
	g++ $(cflag) -fno-rtti r2v-camera/os-win/win.dshow.cpp

.PHONY:wincli
wincli:target windows
	$(compiler) $(cflag) v2r-window/os-win/win.cli.c
	$(compiler) $(cflag) w2w-gamepad/win/win.xinput.c

.PHONY:wintui
wintui:target windows
	$(compiler) $(cflag) v2r-window/os-win/win.tui.c
	$(compiler) $(cflag) w2w-gamepad/win/win.xinput.c

.PHONY:winapi
winapi:target windows
	$(compiler) $(cflag) v2r-window/os-win/win.api.c
	$(compiler) $(cflag) w2w-gamepad/win/win.xinput.c

.PHONY:windx11
windx11:target windows
	g++ $(cflag) v2r-window/lib-dx11/dx11.cpp
	g++ $(cflag) v2r-window/lib-dx11/dx11.easy.cpp
	$(compiler) $(cflag) w2w-gamepad/win/win.xinput.c

.PHONY:winglut
winglut:target windows
	$(compiler) $(cflag) v2r-window/lib-gl/glut.c
	$(compiler) $(cflag) w2w-gamepad/win/win.xinput.c

.PHONY:winglfw
winglfw:target windows opengl
	$(compiler) $(cflag) v2r-window/lib-gl/gllib/glfw.c
	$(compiler) $(cflag) w2w-gamepad/glfw/glfw.gamepad.c

.PHONY:winsdl
winsdl:target windows
	$(compiler) $(cflag) v2r-window/lib-sdl/sdl2.c
	$(compiler) $(cflag) w2w-gamepad/sdl/sdl.gamepad.c
##########################################################




clean:
	rm -f *.o *.a
