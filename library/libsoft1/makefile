binary:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 memory.c
	gcc -c -fPIC -O2 anscii/anscii.c
	gcc -c -fPIC -O2 anscii/anscii.convert.c
	gcc -c -fPIC -O2 math/math.c
	gcc -c -fPIC -O2 math/math.calc.c
	gcc -c -fPIC -O2 math/math.convert.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	memory.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o \
	anscii.o anscii.convert.o \
	math.o math.calc.o math.convert.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 master.c
	x86_64-elf-gcc -c -fPIC -O2 memory.c
	x86_64-elf-gcc -c -fPIC -O2 anscii/anscii.c
	x86_64-elf-gcc -c -fPIC -O2 anscii/anscii.convert.c
	x86_64-elf-gcc -c -fPIC -O2 math/math.c
	x86_64-elf-gcc -c -fPIC -O2 math/math.calc.c
	x86_64-elf-gcc -c -fPIC -O2 math/math.convert.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/gpt.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/mbr.c
	x86_64-elf-gcc -c -fPIC -O2 ext.c
	x86_64-elf-gcc -c -fPIC -O2 fat.c
	x86_64-elf-gcc -c -fPIC -O2 hfs.c
	x86_64-elf-gcc -c -fPIC -O2 ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	memory.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o \
	anscii.o anscii.convert.o \
	math.o math.calc.o math.convert.o
linux:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 memory.c
	gcc -c -fPIC -O2 anscii/anscii.c
	gcc -c -fPIC -O2 anscii/anscii.convert.c
	gcc -c -fPIC -O2 math/math.c
	gcc -c -fPIC -O2 math/math.calc.c
	gcc -c -fPIC -O2 math/math.convert.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 filesystem/ext.c
	gcc -c -fPIC -O2 filesystem/fat.c
	gcc -c -fPIC -O2 filesystem/hfs.c
	gcc -c -fPIC -O2 filesystem/ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	memory.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o \
	anscii.o anscii.convert.o \
	math.o math.calc.o math.convert.o
mac:
	clang -c -fPIC -O2 master.c
	clang -c -fPIC -O2 memory.c
	clang -c -fPIC -O2 anscii/anscii.c
	clang -c -fPIC -O2 anscii/anscii.convert.c
	clang -c -fPIC -O2 math/math.c
	clang -c -fPIC -O2 math/math.calc.c
	clang -c -fPIC -O2 math/math.convert.c
	clang -c -fPIC -O2 partitiontable/gpt.c
	clang -c -fPIC -O2 partitiontable/mbr.c
	clang -c -fPIC -O2 filesystem/ext.c
	clang -c -fPIC -O2 filesystem/fat.c
	clang -c -fPIC -O2 filesystem/hfs.c
	clang -c -fPIC -O2 filesystem/ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	memory.o
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o \
	anscii.o anscii.convert.o \
	math.o math.calc.o math.convert.o
win:
	gcc -c -O2 master.c
	gcc -c -O2 memory.c
	gcc -c -O2 anscii/anscii.c
	gcc -c -O2 anscii/anscii.convert.c
	gcc -c -O2 math/math.c
	gcc -c -O2 math/math.calc.c
	gcc -c -O2 math/math.convert.c
	gcc -c -O2 partitiontable/gpt.c
	gcc -c -O2 partitiontable/mbr.c
	gcc -c -O2 filesystem/ext.c
	gcc -c -O2 filesystem/fat.c
	gcc -c -O2 filesystem/hfs.c
	gcc -c -O2 filesystem/ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	memory.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o \
	anscii.o anscii.convert.o \
	math.o math.calc.o math.convert.o
clean:
	rm -f *.o *.a
