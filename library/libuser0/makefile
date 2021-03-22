DIR:=$(shell pwd)
CC:=gcc
CF:=-c -O2 -fPIC -I..
CPPC:g++
CPPF:-c -O2 -fPIC -I..


else_c = \
gadget-sensor/sensor.ahrs.c \
gadget-sensor/sensor.slam.c \
gadget-sensor/sensor.lidar.c \
gadget-sensor/sensor.radar.c \
gadget-display/1602/gadget.1602.c \
gadget-display/12864/gadget.12864.c \
supply.c
else_o = $(patsubst %.c, %.o, $(else_c))


ogl_c = \
2d-window/lib-gl/gldraw/gl.none.c \
2d-window/lib-gl/gldraw/gl.easy.c \
2d-window/lib-gl/gldraw/gl.full.c \
2d-window/lib-gl/gltar/gl41.target.color.c \
2d-window/lib-gl/gltar/gl41.target.cube.c \
2d-window/lib-gl/gltar/gl41.target.depth.c \
2d-window/lib-gl/gltar/gl41.target.gbuffer.c \
2d-window/lib-gl/globj/globj.fbo.c \
2d-window/lib-gl/globj/globj.shader.c \
2d-window/lib-gl/globj/globj.texture.c \
2d-window/lib-gl/globj/globj.varray.c \
2d-window/lib-gl/globj/globj.vertex.c
ogl_o = $(patsubst %.c, %.o, $(ogl_c))


############################bare#########################
bare_c = \
cmdline/bare/bare.stdio.c \
gamepad/bare/bare.joy.c \
systray/bare/bare.tray.c \
2d-camera/os-bare/bare.cam.c \
sound-micphone/os-bare/bare.mic.c \
sound-speaker/os-bare/bare.spk.c
bare_o = $(patsubst %.c, %.o, $(bare_c))

barecli_c = \
2d-window/os-bare/bare.cli.c
barecli_o = $(patsubst %.c, %.o, $(barecli_c))

baretui_c = \
2d-window/os-bare/bare.tui.c
baretui_o = $(patsubst %.c, %.o, $(baretui_c))

bare8bit_c = \
2d-window/os-bare/bare.8bit.c
bare8bit_o = $(patsubst %.c, %.o, $(bare8bit_c))

barefb_c = \
2d-window/os-bare/bare.fb.c
barefb_o = $(patsubst %.c, %.o, $(barefb_c))

rpifb_c = \
2d-window/os-bare/bare.rpifb.c
rpifb_o = $(patsubst %.c, %.o, $(rpifb_c))

.PHONY:barecli
barecli:$(else_o) $(bare_o) $(barecli_o)

.PHONY:baretui
baretui:$(else_o) $(bare_o) $(baretui_o)

.PHONY:bare8bit
bare8bit:$(else_o) $(bare_o) $(bare8bit_o)

.PHONY:barefb
barefb:$(else_o) $(bare_o) $(barefb_o)

.PHONY:rpifb
rpifb:$(else_o) $(bare_o) $(rpifb_o)
##########################################################


############################efi#########################
rpicli_c = \
2d-window/os-efi/efi.cli.c
rpicli_o = $(patsubst %.c, %.o, $(rpicli_c))

efifb_c = \
2d-window/os-efi/efi.fb.c
efifb_o = $(patsubst %.c, %.o, $(efifb_c))

.PHONY:eficli
eficli:$(else_o) $(bare_o) $(rpicli_o)

.PHONY:efifb
efifb:$(else_o) $(bare_o) $(efifb_o)
#########################################################


############################linux#########################
linux_c = \
cmdline/linux/linux.stdio.c \
systray/linux/linux.tray.c \
sound-micphone/os-linux/linux.pulsein.c \
sound-speaker/os-linux/linux.pulseout.c \
2d-camera/os-linux/linux.v4l2.c
linux_o = $(patsubst %.c, %.o, $(linux_c))

linuxcli_c = \
2d-window/os-linux/linux.cli.c \
gamepad/linux/linux.devjsn.c
linuxcli_o = $(patsubst %.c, %.o, $(linuxcli_c))

linuxtui_c = \
2d-window/os-linux/linux.tui.c \
gamepad/linux/linux.devjsn.c
linuxtui_o = $(patsubst %.c, %.o, $(linuxtui_c))

linuxfb_c = \
2d-window/os-linux/linux.fb.c \
gamepad/linux/linux.devjsn.c
linuxfb_o = $(patsubst %.c, %.o, $(linuxfb_c))

linuxgtk_c = \
gamepad/linux/linux.devjsn.c
linuxgtk_o = $(patsubst %.c, %.o, $(linuxgtk_c))

linuxwayland_c = \
2d-window/os-linux/linux.wayland.c \
gamepad/linux/linux.devjsn.c
linuxwayland_o = $(patsubst %.c, %.o, $(linuxwayland_c))

linuxxlib_c = \
2d-window/os-linux/linux.xlib.c \
gamepad/linux/linux.devjsn.c
linuxxlib_o = $(patsubst %.c, %.o, $(linuxxlib_c))

linuxglut_c = \
2d-window/lib-gl/glut.c \
gamepad/linux/linux.devjsn.c
linuxglut_o = $(patsubst %.c, %.o, $(linuxglut_c))

linuxglfw_c = \
2d-window/lib-gl/gllib/glfw.c \
gamepad/glfw/glfw.gamepad.c
linuxglfw_o = $(patsubst %.c, %.o, $(linuxglfw_c))

linuxsdl_c = \
gamepad/sdl/sdl.gamepad.c
linuxsdl_o = $(patsubst %.c, %.o, $(linuxsdl_c))

.PHONY:linuxcli
linuxcli:$(else_o) $(linux_o) $(linuxcli_o)

.PHONY:linuxtui
linuxtui:$(else_o) $(linux_o) $(linuxtui_o)

.PHONY:linuxfb
linuxfb:$(else_o) $(linux_o) $(linuxfb_o)

.PHONY:linuxgtk
linuxgtk:$(else_o) $(linux_o) $(linuxgtk_o)
	$(CC) $(CF)  -o $(DIR)/linux.gtk.o `pkg-config --CFs gtk+-3.0` 2d-window/os-linux/linux.gtk.c

.PHONY:linuxwayland
linuxwayland:$(else_o) $(linux_o) $(linuxwayland_o)

.PHONY:linuxxlib
linuxxlib:$(else_o) $(linux_o) $(linuxxlib_o)

.PHONY:linuxglut
linuxglut:$(else_o) $(linux_o) $(linuxglut_o)

.PHONY:linuxglfw
linuxglfw:$(else_o) $(linux_o) $(linuxglfw_o) $(ogl_o)

.PHONY:linuxsdl
linuxsdl:$(else_o) $(linux_o) $(linuxsdl_o)
	$(CC) $(CF)  -o $(DIR)/sdl2.o 2d-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
#########################################################


#############################mac##########################
macos_c = \
cmdline/mac/mac.stdio.c \
systray/mac/mac.tray.c \
sound-micphone/os-mac/mac.aqin.c \
sound-speaker/os-mac/mac.aqout.c
macos_o = $(patsubst %.c, %.o, $(macos_c))

macos_m = \
2d-camera/os-mac/mac.avfoundation.m
macos_m_o = $(patsubst %.m, %.m.o, $(macos_m))

maccli_c = \
2d-window/os-mac/mac.cli.c \
gamepad/mac/mac.gamepad.c
maccli_o = $(patsubst %.c, %.o, $(maccli_c))

mactui_c = \
2d-window/os-mac/mac.tui.c \
gamepad/mac/mac.gamepad.c
mactui_o = $(patsubst %.c, %.o, $(mactui_c))

macquartz_c = \
2d-window/os-mac/mac.quartz.c \
gamepad/mac/mac.gamepad.c
macquartz_o = $(patsubst %.c, %.o, $(macquartz_c))

maccocoa_c = \
gamepad/mac/mac.gamepad.c
maccocoa_o = $(patsubst %.c, %.o, $(maccocoa_c))

maccocoa_m = \
2d-window/os-mac/mac.cocoa.m
maccocoa_m_o = $(patsubst %.m, %.m.o, $(maccocoa_m))

macmetal_c = \
gamepad/mac/mac.gamepad.c
macmetal_o = $(patsubst %.c, %.o, $(macmetal_c))

macmetal_m = \
2d-window/lib-metal/metal.m
macmetal_m_o = $(patsubst %.m, %.m.o, $(macmetal_m))

macsdl_c = \
gamepad/sdl/sdl.gamepad.c
macsdl_o = $(patsubst %.c, %.o, $(macsdl_c))

macglut_c = \
2d-window/lib-gl/glut.c \
gamepad/mac/mac.gamepad.c
macglut_o = $(patsubst %.c, %.o, $(macglut_c))

macglfw_c = \
2d-window/lib-gl/gllib/glfw.c \
gamepad/glfw/glfw.gamepad.c
macglfw_o = $(patsubst %.c, %.o, $(macglfw_c))

.PHONY:maccli
maccli:$(else_o) $(macos_o) $(macos_m_o) $(maccli_o)

.PHONY:mactui
mactui:$(else_o) $(macos_o) $(macos_m_o) $(mactui_o)

.PHONY:macquartz
macquartz:$(else_o) $(macos_o) $(macos_m_o) $(macquartz_o)

.PHONY:maccocoa
maccocoa:$(else_o) $(macos_o) $(macos_m_o) $(maccocoa_o) $(maccocoa_m_o)

.PHONY:macmetal
macmetal:$(else_o) $(macos_o) $(macos_m_o) $(macmetal_o) $(macmetal_m_o)

.PHONY:macsdl
macsdl:$(else_o) $(macos_o) $(macos_m_o) $(macsdl_o)
	$(CC) $(CF) -o $(DIR)/sdl2.o 2d-window/lib-sdl/sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include

.PHONY:macglut
macglut:$(else_o) $(macos_o) $(macos_m_o) $(macglut_o)

.PHONY:macglfw
macglfw:$(else_o) $(macos_o) $(macos_m_o) $(macglfw_o) $(ogl_o)
#########################################################


###########################windows#######################
win_c = \
cmdline/win/win.stdio.c \
systray/win/win.tray.c \
sound-micphone/os-win/win.wavein.c \
sound-speaker/os-win/win.waveout.c
win_o = $(patsubst %.c, %.o, $(win_c))

win_cpp = \
2d-camera/os-win/win.dshow.cpp
win_cpp_o = $(patsubst %.cpp, %.cpp.o, $(win_cpp))

wincli_c = \
2d-window/os-win/win.cli.c \
gamepad/win/win.xinput.c
win_cli_o = $(patsubst %.c, %.o, $(wincli_c))

wintui_c = \
2d-window/os-win/win.tui.c \
gamepad/win/win.xinput.c
wintui_o = $(patsubst %.c, %.o, $(wintui_c))

winapi_c = \
2d-window/os-win/win.api.c \
gamepad/win/win.xinput.c
winapi_o = $(patsubst %.c, %.o, $(winapi_c))

windx11_c = \
gamepad/win/win.xinput.c
windx11_o = $(patsubst %.c, %.o, $(windx11_c))

windx11_cpp = \
2d-window/lib-dx11/dx11.cpp \
2d-window/lib-dx11/dx11.easy.cpp
windx11_cpp_o = $(patsubst %.cpp, %.cpp.o, $(windx11_cpp))

winglut_c = \
2d-window/lib-gl/glut.c \
gamepad/win/win.xinput.c
winglut_o = $(patsubst %.c, %.o, $(winglut_c))

winglfw_c = \
2d-window/lib-gl/gllib/glfw.c \
gamepad/glfw/glfw.gamepad.c
winglfw_o = $(patsubst %.c, %.o, $(winglfw_c))

winsdl_c = \
2d-window/lib-sdl/sdl2.c \
gamepad/sdl/sdl.gamepad.c
winsdl_o = $(patsubst %.c, %.o, $(winsdl_c))

.PHONY:wincli
wincli:$(else_o) $(win_o) $(win_cpp_o) $(wincli_o)

.PHONY:wintui
wintui:$(else_o) $(win_o) $(win_cpp_o) $(wintui_o)

.PHONY:winapi
winapi:$(else_o) $(win_o) $(win_cpp_o) $(winapi_o)

.PHONY:windx11
windx11:$(else_o) $(win_o) $(win_cpp_o) $(windx11_o) $(windx11_cpp_o)

.PHONY:winglut
winglut:$(else_o) $(win_o) $(win_cpp_o) $(winglut_o)

.PHONY:winglfw
winglfw:$(else_o) $(win_o) $(win_cpp_o) $(winglfw_o) $(ogl_o)

.PHONY:winsdl
winsdl:$(else_o) $(win_o) $(win_cpp_o) $(winsdl_o)
##########################################################


%.o: %.c
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<
%.m.o: %.m
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<
%.cpp.o: %.cpp
	$(CC) $(CF) -o $(DIR)/$(notdir $@) $<


clean:
	rm -f *.o *.a
