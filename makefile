all:
	echo "what?"
linux:								#cli
	make -s -C main cli
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C init linux
	gcc -o a.out \
	main/main.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a
mac:								#cli
	make -s -C main cli
	make -s -C libsoft1 mac
	make -s -C libsoft mac
	make -s -C init mac
	clang -o a.out \
	main/main.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a
win:								#cli
	make -s -C main cli
	make -s -C libsoft1 win
	make -s -C libsoft win
	make -s -C init win
	gcc -o a.exe init/uac.res \
	main/main.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a
linux+fb:
	make -s -C main gui
	make -s -C libui1 linux
	make -s -C libui linux
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C init linux+fb
	gcc -o a.out \
	main/main.a \
	libui1/libui1.a \
	libui/libui.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a
linux+xlib:
	make -s -C main gui
	make -s -C libui1 linux
	make -s -C libui linux
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C init linux+xlib
	gcc -o a.out \
	main/main.a \
	libui1/libui1.a \
	libui/libui.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a \
	-lX11
linux+sdl:
	make -s -C main gui
	make -s -C libui1 linux
	make -s -C libui linux
	make -s -C libsoft1 linux
	make -s -C libsoft linux
	make -s -C init linux+sdl
	gcc -o a.out \
	main/main.a \
	libui1/libui1.a \
	libui/libui.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a \
	-lSDL2 -lm
mac+sdl:
	make -s -C main cli
	make -s -C libui1 linux
	make -s -C libui linux
	make -s -C libsoft1 mac
	make -s -C libsoft mac
	make -s -C init mac
	clang -o a.out \
	main/main.a \
	libui1/libui1.a \
	libui/libui.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a \
	-L /usr/local/Cellar/sdl2/2.0.3/lib \
	-lSDL2 -lm
win+api:
	make -s -C main gui
	make -s -C libui1 win
	make -s -C libui win
	make -s -C libsoft1 win
	make -s -C libsoft win
	make -s -C init win+api
	gcc -o a.exe -mwindows init/uac.res \
	main/main.a \
	libui1/libui1.a \
	libui/libui.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a \
	-lgdi32
win+sdl:
	make -s -C main gui
	make -s -C libui1 win
	make -s -C libui win
	make -s -C libsoft1 win
	make -s -C libsoft win
	make -s -C init win+sdl
	gcc -o a.exe \
	init/uac.res \
	main/main.a \
	libui1/libui1.a \
	libui/libui.a \
	libsoft1/libsoft1.a \
	libsoft/libsoft.a \
	init/init.a \
	-Wl,--no-undefined -static -static-libgcc \
	-L /opt/SDL2-2.0.3/x86_64-w64-mingw32/lib \
	-lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 \
	-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 \
	-lshell32 -lversion -luuid
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
	rm -f */*.o */*.a */*.res *.bin *.out *.exe
