CC:=gcc
CF:=-c -O2 -fPIC -I.
SRC= \
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
	$(CC) $(CF) $<
opengl:
	$(CC) $(CF) v2r-window/lib-gl/gldraw/gl.none.c
	$(CC) $(CF) v2r-window/lib-gl/gldraw/gl.easy.c
	$(CC) $(CF) v2r-window/lib-gl/gldraw/gl.full.c
	$(CC) $(CF) v2r-window/lib-gl/gltar/gl41.target.color.c
	$(CC) $(CF) v2r-window/lib-gl/gltar/gl41.target.cube.c
	$(CC) $(CF) v2r-window/lib-gl/gltar/gl41.target.depth.c
	$(CC) $(CF) v2r-window/lib-gl/gltar/gl41.target.gbuffer.c
	$(CC) $(CF) v2r-window/lib-gl/globj/globj.fbo.c
	$(CC) $(CF) v2r-window/lib-gl/globj/globj.shader.c
	$(CC) $(CF) v2r-window/lib-gl/globj/globj.texture.c
	$(CC) $(CF) v2r-window/lib-gl/globj/globj.varray.c
	$(CC) $(CF) v2r-window/lib-gl/globj/globj.vertex.c




############################bare#########################
.PHYOY:bare
bare:
	$(CC) $(CF) w2w-cmdline/bare/bare.stdio.c
	$(CC) $(CF) w2w-gamepad/bare/bare.joy.c
	$(CC) $(CF) w2w-systray/bare/bare.tray.c
	$(CC) $(CF) r2v-camera/os-bare/bare.cam.c
	$(CC) $(CF) r2v-micphone/os-bare/bare.mic.c
	$(CC) $(CF) v2r-speaker/os-bare/bare.spk.c
.PHONY:barecli
barecli:target bare
	$(CC) $(CF) v2r-window/os-bare/bare.cli.c
.PHONY:baretui
baretui:target bare
	$(CC) $(CF) v2r-window/os-bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target bare
	$(CC) $(CF) v2r-window/os-bare/bare.8bit.c
.PHONY:barefb
barefb:target bare
	$(CC) $(CF) v2r-window/os-bare/bare.fb.c
.PHONY:rpifb
rpifb:target bare
	$(CC) $(CF) v2r-window/os-bare/bare.rpifb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target bare
	$(CC) $(CF) v2r-window/os-efi/efi.cli.c
.PHONY:efifb
efifb:target bare
	$(CC) $(CF) v2r-window/os-efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linux
linux:
	$(CC) $(CF) w2w-cmdline/linux/linux.stdio.c
	$(CC) $(CF) w2w-systray/linux/linux.tray.c
	$(CC) $(CF) r2v-micphone/os-linux/linux.pulsein.c
	$(CC) $(CF) v2r-speaker/os-linux/linux.pulseout.c
	$(CC) $(CF) r2v-camera/os-linux/linux.v4l2.c

.PHONY:linuxcli
linuxcli:target linux
	$(CC) $(CF) v2r-window/os-linux/linux.cli.c
	$(CC) $(CF) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxtui
linuxtui:target linux
	$(CC) $(CF) v2r-window/os-linux/linux.tui.c
	$(CC) $(CF) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxfb
linuxfb:target linux
	$(CC) $(CF) v2r-window/os-linux/linux.fb.c
	$(CC) $(CF) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxgtk
linuxgtk:target linux
	$(CC) $(CF) `pkg-config --CFs gtk+-3.0` v2r-window/os-linux/linux.gtk.c
	$(CC) $(CF) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxwayland
linuxwayland:target linux
	$(CC) $(CF) v2r-window/os-linux/linux.wayland.c
	$(CC) $(CF) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxxlib
linuxxlib:target linux
	$(CC) $(CF) v2r-window/os-linux/linux.xlib.c
	$(CC) $(CF) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxglut
linuxglut:target linux
	$(CC) $(CF) v2r-window/lib-gl/glut.c
	$(CC) $(CF) w2w-gamepad/linux/linux.devjsn.c

.PHONY:linuxglfw
linuxglfw:target linux opengl
	$(CC) $(CF) v2r-window/lib-gl/gllib/glfw.c
	$(CC) $(CF) w2w-gamepad/glfw/glfw.gamepad.c

.PHONY:linuxsdl
linuxsdl:target linux
	$(CC) $(CF) v2r-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(CC) $(CF) w2w-gamepad/sdl/sdl.gamepad.c
#########################################################




#############################mac##########################
.PHONY:macos
macos:
	$(CC) $(CF) w2w-cmdline/mac/mac.stdio.c
	$(CC) $(CF) w2w-systray/mac/mac.tray.c
	$(CC) $(CF) r2v-micphone/os-mac/mac.aqin.c
	$(CC) $(CF) v2r-speaker/os-mac/mac.aqout.c
	$(CC) $(CF) r2v-camera/os-mac/mac.cam.c

.PHONY:maccli
maccli:target macos
	$(CC) $(CF) v2r-window/os-mac/mac.cli.c
	$(CC) $(CF) w2w-gamepad/mac/mac.gamepad.c

.PHONY:mactui
mactui:target macos
	$(CC) $(CF) v2r-window/os-mac/mac.tui.c
	$(CC) $(CF) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macquartz
macquartz:target macos
	$(CC) $(CF) v2r-window/os-mac/mac.quartz.c
	$(CC) $(CF) w2w-gamepad/mac/mac.gamepad.c

.PHONY:maccocoa
maccocoa:target macos
	$(CC) $(CF) v2r-window/os-mac/mac.cocoa.m
	$(CC) $(CF) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macmetal
macmetal:target macos
	$(CC) $(CF) v2r-window/lib-metal/metal.m
	$(CC) $(CF) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macsdl
macsdl:target macos
	$(CC) $(CF) v2r-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(CC) $(CF) w2w-gamepad/sdl/sdl.gamepad.c

.PHONY:macglut
macglut:target macos
	$(CC) $(CF) v2r-window/lib-gl/glut.c
	$(CC) $(CF) w2w-gamepad/mac/mac.gamepad.c

.PHONY:macglfw
macglfw:target macos opengl
	$(CC) $(CF) v2r-window/lib-gl/gllib/glfw.c
	$(CC) $(CF) w2w-gamepad/glfw/glfw.gamepad.c
#########################################################




###########################windows#######################
.PHONY:windows
windows:
	$(CC) $(CF) w2w-cmdline/win/win.stdio.c
	$(CC) $(CF) w2w-systray/win/win.tray.c
	$(CC) $(CF) r2v-micphone/os-win/win.wavein.c
	$(CC) $(CF) v2r-speaker/os-win/win.waveout.c
	g++ $(CF) -fno-rtti r2v-camera/os-win/win.dshow.cpp

.PHONY:wincli
wincli:target windows
	$(CC) $(CF) v2r-window/os-win/win.cli.c
	$(CC) $(CF) w2w-gamepad/win/win.xinput.c

.PHONY:wintui
wintui:target windows
	$(CC) $(CF) v2r-window/os-win/win.tui.c
	$(CC) $(CF) w2w-gamepad/win/win.xinput.c

.PHONY:winapi
winapi:target windows
	$(CC) $(CF) v2r-window/os-win/win.api.c
	$(CC) $(CF) w2w-gamepad/win/win.xinput.c

.PHONY:windx11
windx11:target windows
	g++ $(CF) v2r-window/lib-dx11/dx11.cpp
	g++ $(CF) v2r-window/lib-dx11/dx11.easy.cpp
	$(CC) $(CF) w2w-gamepad/win/win.xinput.c

.PHONY:winglut
winglut:target windows
	$(CC) $(CF) v2r-window/lib-gl/glut.c
	$(CC) $(CF) w2w-gamepad/win/win.xinput.c

.PHONY:winglfw
winglfw:target windows opengl
	$(CC) $(CF) v2r-window/lib-gl/gllib/glfw.c
	$(CC) $(CF) w2w-gamepad/glfw/glfw.gamepad.c

.PHONY:winsdl
winsdl:target windows
	$(CC) $(CF) v2r-window/lib-sdl/sdl2.c
	$(CC) $(CF) w2w-gamepad/sdl/sdl.gamepad.c
##########################################################




clean:
	rm -f *.o *.a
