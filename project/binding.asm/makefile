linux:
	nasm dsdc89.s -f elf64 -o dsdc89.o
	gcc dsdc89.o 42.a -lpulse -lpulse-simple -lpthread
macos:
	nasm dsdc89.s -f macho64 -o dsdc89.o
	gcc dsdc89.o 42.a
windows:
	nasm cd89.s -f win64 -o cd89.o
	gcc cd89.o 42.a -lwinmm -lws2_32
clean:
	rm -f *.o *.a *.exe
