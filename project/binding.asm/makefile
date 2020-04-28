linux:
	nasm dsdc89.s -f elf64 -o dsdc89.o
	gcc dsdc89.o 42.a -lpulse -lpulse-simple -lpthread
macos:
	nasm dsdc89.s -f macho64 -o dsdc89.o
	gcc dsdc89.o 42.a
windows:
	nasm cd89.s -f win64 -o cd89.o
	gcc cd89.o 42.a -lwinmm -lws2_32
42:
	nasm 89abcdef.s -o 89abcdef.o
	gcc 89abcdef.o 42.a
clean:
	rm -f *.o *.out *.exe
