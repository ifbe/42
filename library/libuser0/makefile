CC:=gcc
CF:=-c -O2 -fPIC -I.
SRC= \
gadget-sensor/sensor.ahrs.c \
gadget-sensor/sensor.slam.c \
gadget-sensor/sensor.lidar.c \
gadget-sensor/sensor.radar.c \
gadget-display/1602/gadget.1602.c \
gadget-display/12864/gadget.12864.c \
supply.c
OBJ = $(patsubst %.c,%.o,$(SRC))

target: $(OBJ)
%.o: %.c
	$(CC) $(CF) $<
opengl:
	$(CC) $(CF) 2d-window/lib-gl/gldraw/gl.none.c
	$(CC) $(CF) 2d-window/lib-gl/gldraw/gl.easy.c
	$(CC) $(CF) 2d-window/lib-gl/gldraw/gl.full.c
	$(CC) $(CF) 2d-window/lib-gl/gltar/gl41.target.color.c
	$(CC) $(CF) 2d-window/lib-gl/gltar/gl41.target.cube.c
	$(CC) $(CF) 2d-window/lib-gl/gltar/gl41.target.depth.c
	$(CC) $(CF) 2d-window/lib-gl/gltar/gl41.target.gbuffer.c
	$(CC) $(CF) 2d-window/lib-gl/globj/globj.fbo.c
	$(CC) $(CF) 2d-window/lib-gl/globj/globj.shader.c
	$(CC) $(CF) 2d-window/lib-gl/globj/globj.texture.c
	$(CC) $(CF) 2d-window/lib-gl/globj/globj.varray.c
	$(CC) $(CF) 2d-window/lib-gl/globj/globj.vertex.c




############################bare#########################
.PHYOY:bare
bare:
	$(CC) $(CF) cmdline/bare/bare.stdio.c
	$(CC) $(CF) gamepad/bare/bare.joy.c
	$(CC) $(CF) systray/bare/bare.tray.c
	$(CC) $(CF) 2d-camera/os-bare/bare.cam.c
	$(CC) $(CF) sound-micphone/os-bare/bare.mic.c
	$(CC) $(CF) sound-speaker/os-bare/bare.spk.c
.PHONY:barecli
barecli:target bare
	$(CC) $(CF) 2d-window/os-bare/bare.cli.c
.PHONY:baretui
baretui:target bare
	$(CC) $(CF) 2d-window/os-bare/bare.tui.c
.PHONY:bare8bit
bare8bit:target bare
	$(CC) $(CF) 2d-window/os-bare/bare.8bit.c
.PHONY:barefb
barefb:target bare
	$(CC) $(CF) 2d-window/os-bare/bare.fb.c
.PHONY:rpifb
rpifb:target bare
	$(CC) $(CF) 2d-window/os-bare/bare.rpifb.c
##########################################################




############################efi#########################
.PHONY:eficli
eficli:target bare
	$(CC) $(CF) 2d-window/os-efi/efi.cli.c
.PHONY:efifb
efifb:target bare
	$(CC) $(CF) 2d-window/os-efi/efi.fb.c
#########################################################




############################linux#########################
.PHONY:linux
linux:
	$(CC) $(CF) cmdline/linux/linux.stdio.c
	$(CC) $(CF) systray/linux/linux.tray.c
	$(CC) $(CF) sound-micphone/os-linux/linux.pulsein.c
	$(CC) $(CF) sound-speaker/os-linux/linux.pulseout.c
	$(CC) $(CF) 2d-camera/os-linux/linux.v4l2.c

.PHONY:linuxcli
linuxcli:target linux
	$(CC) $(CF) 2d-window/os-linux/linux.cli.c
	$(CC) $(CF) gamepad/linux/linux.devjsn.c

.PHONY:linuxtui
linuxtui:target linux
	$(CC) $(CF) 2d-window/os-linux/linux.tui.c
	$(CC) $(CF) gamepad/linux/linux.devjsn.c

.PHONY:linuxfb
linuxfb:target linux
	$(CC) $(CF) 2d-window/os-linux/linux.fb.c
	$(CC) $(CF) gamepad/linux/linux.devjsn.c

.PHONY:linuxgtk
linuxgtk:target linux
	$(CC) $(CF) `pkg-config --CFs gtk+-3.0` 2d-window/os-linux/linux.gtk.c
	$(CC) $(CF) gamepad/linux/linux.devjsn.c

.PHONY:linuxwayland
linuxwayland:target linux
	$(CC) $(CF) 2d-window/os-linux/linux.wayland.c
	$(CC) $(CF) gamepad/linux/linux.devjsn.c

.PHONY:linuxxlib
linuxxlib:target linux
	$(CC) $(CF) 2d-window/os-linux/linux.xlib.c
	$(CC) $(CF) gamepad/linux/linux.devjsn.c

.PHONY:linuxglut
linuxglut:target linux
	$(CC) $(CF) 2d-window/lib-gl/glut.c
	$(CC) $(CF) gamepad/linux/linux.devjsn.c

.PHONY:linuxglfw
linuxglfw:target linux opengl
	$(CC) $(CF) 2d-window/lib-gl/gllib/glfw.c
	$(CC) $(CF) gamepad/glfw/glfw.gamepad.c

.PHONY:linuxsdl
linuxsdl:target linux
	$(CC) $(CF) 2d-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(CC) $(CF) gamepad/sdl/sdl.gamepad.c
#########################################################




#############################mac##########################
.PHONY:macos
macos:
	$(CC) $(CF) cmdline/mac/mac.stdio.c
	$(CC) $(CF) systray/mac/mac.tray.c
	$(CC) $(CF) sound-micphone/os-mac/mac.aqin.c
	$(CC) $(CF) sound-speaker/os-mac/mac.aqout.c
	$(CC) $(CF) 2d-camera/os-mac/mac.cam.c

.PHONY:maccli
maccli:target macos
	$(CC) $(CF) 2d-window/os-mac/mac.cli.c
	$(CC) $(CF) gamepad/mac/mac.gamepad.c

.PHONY:mactui
mactui:target macos
	$(CC) $(CF) 2d-window/os-mac/mac.tui.c
	$(CC) $(CF) gamepad/mac/mac.gamepad.c

.PHONY:macquartz
macquartz:target macos
	$(CC) $(CF) 2d-window/os-mac/mac.quartz.c
	$(CC) $(CF) gamepad/mac/mac.gamepad.c

.PHONY:maccocoa
maccocoa:target macos
	$(CC) $(CF) 2d-window/os-mac/mac.cocoa.m
	$(CC) $(CF) gamepad/mac/mac.gamepad.c

.PHONY:macmetal
macmetal:target macos
	$(CC) $(CF) 2d-window/lib-metal/metal.m
	$(CC) $(CF) gamepad/mac/mac.gamepad.c

.PHONY:macsdl
macsdl:target macos
	$(CC) $(CF) 2d-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	$(CC) $(CF) gamepad/sdl/sdl.gamepad.c

.PHONY:macglut
macglut:target macos
	$(CC) $(CF) 2d-window/lib-gl/glut.c
	$(CC) $(CF) gamepad/mac/mac.gamepad.c

.PHONY:macglfw
macglfw:target macos opengl
	$(CC) $(CF) 2d-window/lib-gl/gllib/glfw.c
	$(CC) $(CF) gamepad/glfw/glfw.gamepad.c
#########################################################




###########################windows#######################
.PHONY:windows
windows:
	$(CC) $(CF) cmdline/win/win.stdio.c
	$(CC) $(CF) systray/win/win.tray.c
	$(CC) $(CF) sound-micphone/os-win/win.wavein.c
	$(CC) $(CF) sound-speaker/os-win/win.waveout.c
	g++ $(CF) -fno-rtti 2d-camera/os-win/win.dshow.cpp

.PHONY:wincli
wincli:target windows
	$(CC) $(CF) 2d-window/os-win/win.cli.c
	$(CC) $(CF) gamepad/win/win.xinput.c

.PHONY:wintui
wintui:target windows
	$(CC) $(CF) 2d-window/os-win/win.tui.c
	$(CC) $(CF) gamepad/win/win.xinput.c

.PHONY:winapi
winapi:target windows
	$(CC) $(CF) 2d-window/os-win/win.api.c
	$(CC) $(CF) gamepad/win/win.xinput.c

.PHONY:windx11
windx11:target windows
	g++ $(CF) 2d-window/lib-dx11/dx11.cpp
	g++ $(CF) 2d-window/lib-dx11/dx11.easy.cpp
	$(CC) $(CF) gamepad/win/win.xinput.c

.PHONY:winglut
winglut:target windows
	$(CC) $(CF) 2d-window/lib-gl/glut.c
	$(CC) $(CF) gamepad/win/win.xinput.c

.PHONY:winglfw
winglfw:target windows opengl
	$(CC) $(CF) 2d-window/lib-gl/gllib/glfw.c
	$(CC) $(CF) gamepad/glfw/glfw.gamepad.c

.PHONY:winsdl
winsdl:target windows
	$(CC) $(CF) 2d-window/lib-sdl/sdl2.c
	$(CC) $(CF) gamepad/sdl/sdl.gamepad.c
##########################################################




clean:
	rm -f *.o *.a
