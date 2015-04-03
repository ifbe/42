linux:
	make -s -C wrap linux
	make -s -C main linux
	gcc main/main.a wrap/wrap.a \
	-lSDL2 -lm
mac:
	make -s -C wrap mac
	make -s -C main mac
	gcc main/color.o main/anscii.o \
	-L /usr/local/Cellar/sdl2/2.0.3/lib \
	wrap/wrap.a -lSDL2 -lm
win:
	make -s -C wrap win
	make -s -C fs win
	make -s -C main win
	gcc main/main.a fs/fs.a wrap/wrap.a wrap/uac.res \
	-Wl,--no-undefined -static -static-libgcc \
	-L d:\msys\lib\SDL2-2.0.3\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid
win-graphic:
	make -s -C wrap win-graphic
	make -s -C fs win-graphic
	make -s -C main win-graphic
	gcc main/main.a fs/fs.a wrap/wrap.a wrap/uac.res \
	-Wl,--no-undefined -static -static-libgcc \
	-L d:\msys\lib\SDL2-2.0.3\x86_64-w64-mingw32\lib -lmingw32 -lSDL2main -lSDL2 \
	-lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid
binary:
	make -s -C wrap binary
	make -s -C main binary
	ld -T link.ld -o color.bin wrap/start.o \
	main/color.o main/anscii.o \
	wrap/wrap
crosscompile:
	make -s -C wrap crosscompile
	make -s -C main crosscompile
	x86_64-elf-ld -T link.ld -o color.bin wrap/start.o \
	main/color.o main/anscii.o \
	wrap/wrap.a
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
