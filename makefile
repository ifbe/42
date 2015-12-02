what:
	@echo "----------------please choose one example below----------------"
	@echo "make linux:    run in terminal"
	@echo "make windows    run in cmd"
	@echo "make linux+fb:    run in framebuffer(ctrl+alt+f?)"
	@echo "make linux+xlib    run in xwindow:"
	@echo "make windows+api    run in windows xp+"
	@echo "make kvm:    test with kvm"
	@echo "make qemu:    test with qemu"
	@echo "make clean:    clean"
	@echo "----------------please choose one example above----------------"
x86+linux+cli:
	make -s -C main cli
	make -s -C library x86+linux
	gcc main/main.a library/library.a \
	-o a.out
x86+mac+cli:
	make -s -C main cli
	make -s -C library x86+mac
	clang main/main.a library/library.a \
	-o a.out
x86+win+cli:
	make -s -C main cli
	make -s -C library x86+win
	gcc main/main.a library/library.a library/libsoft0/uac.res \
	-o a.exe




x86+linux+fb:
	make -s -C main gui
	make -s -C library linux+fb
	gcc main/main.a library/library.a \
	-o a.out
x86+linux+xlib:
	make -s -C main gui
	make -s -C library linux+xlib
	gcc main/main.a library/library.a \
	-lX11 \
	-o a.out
x86+linux+sdl:
	make -s -C main gui
	make -s -C library linux+sdl
	gcc main/main.a library/library.a \
	-lSDL2 -lm \
	-o a.out
x86+linux+web:
	make -s -C main gui
	make -s -C library linux+web
	gcc \
	main/main.a library/library.a \
	-o a.out




x86+mac+sdl:
	make -s -C main cli
	make -s -C library mac+sdl
	clang main/main.a library/library.a \
	-L /usr/local/Cellar/sdl2/2.0.3/lib \
	-lSDL2 -lm \
	-o a.out




x86+win+api:
	make -s -C main gui
	make -s -C library win+api
	#gcc -o a.exe libsoft0/uac.res \
	#gcc -o a.exe \
	gcc -mwindows \
	main/main.a library/library.a library/libsoft0/uac.res \
	-lgdi32 \
	-o a.exe
x86+win+sdl:
	make -s -C main gui
	make -s -C library win+sdl
	gcc -mwindows \
	main/main.a library/library.a library/libsoft0/uac.res \
	-Wl,--no-undefined -static -static-libgcc \
	-L /opt/SDL2-2.0.3/x86_64-w64-mingw32/lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid \
	-o a.exe
x86+win+web:
	make -s -C main gui
	make -s -C library win+web
	gcc -mwindows \
	main/main.a library/library.a library/libsoft0/uac.res \
	-o a.exe




qemu:
	cp ../../../core/core.bin test.img    #first64k
	dd if=color.bin of=test.img bs=65536 seek=1 conv=notrunc    #real bin
	echo -n 9 | dd of=test.img bs=1 seek=1048575 conv=notrunc    #fill 0
	echo "test" | dd of=test.img bs=1 seek=448 conv=notrunc    #test sign
	../../../tool/qemu $(shell pwd)/test.img
kvm:
	cp ../../../core/core.bin test.img    #first64k
	dd if=color.bin of=test.img bs=65536 seek=1 conv=notrunc    #real bin
	echo -n 9 | dd of=test.img bs=1 seek=1048575 conv=notrunc    #fill 0
	echo "test" | dd of=test.img bs=1 seek=448 conv=notrunc    #test sign
	../../../tool/kvm $(shell pwd)/test.img




clean:
	rm -f *.bin *.out *.exe
	rm -f */*.o
	rm -f */*.a
	rm -f */*.res
	make -s -C library clean
	make -s -C main clean
