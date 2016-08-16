############################linux#########################
.PHONY:linuxcli
linuxcli:
	rm -f *.o
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast terminal/terminal.cli.c
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast display.c
.PHONY:linuxsdl
linuxsdl:
	rm -f *.o
	gcc -c -O2 cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	gcc -c -O2 display.c
.PHONY:linuxtext
linuxtext:
	rm -f *.o
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast linux/linux.text.c
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast display.c
.PHONY:linuxfb
linuxfb:
	rm -f *.o
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast linux/linux.fb.c
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast display.c
.PHONY:linuxwayland
linuxwayland:
	rm -f *.o
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast linux/linux.wayland.c
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast display.c
.PHONY:linuxxlib
linuxxlib:
	rm -f *.o
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast linux/linux.xlib.c
	gcc -c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast display.c
#########################################################




#############################mac##########################
.PHONY:maccli
maccli:
	rm -f *.o
	gcc -c -fPIC -O2 terminal/terminal.cli.c
	gcc -c -fPIC -O2 display.c
.PHONY:macsdl
macsdl:
	rm -f *.o
	clang -c -O2 cross/cross.sdl2.c -I /usr/local/Cellar/sdl2/2.0.3/include
	gcc -c -O2 display.c
.PHONY:maccocoa
maccocoa:
	rm -f *.o
	clang -c -fPIC -O2 mac/mac.cocoa.c
	clang -c -fPIC -O2 display.c
#########################################################




###########################windows#######################
.PHONY:wincli
wincli:
	rm -f *.o
	gcc -c -O2 terminal/terminal.cli.c
	gcc -c -O2 display.c
.PHONY:winsdl
winsdl:
	rm -f *.o
	gcc -c -O2 cross/cross.sdl2.c -I /opt/SDL2-2.0.3/x86_64-w64-mingw32/include
	gcc -c -O2 display.c
.PHONY:wintext
wintext:
	rm -f *.o
	gcc -c -O2 win/win.text.c
	gcc -c -O2 display.c
.PHONY:winapi
winapi:
	rm -f *.o
	gcc -c -O2 win/win.api.c
	gcc -c -O2 display.c
##########################################################

clean:
	rm -f *.o *.a
