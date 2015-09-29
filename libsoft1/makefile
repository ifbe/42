binary:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 master.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/gpt.c
	x86_64-elf-gcc -c -fPIC -O2 partitiontable/mbr.c
	x86_64-elf-gcc -c -fPIC -O2 ext.c
	x86_64-elf-gcc -c -fPIC -O2 fat.c
	x86_64-elf-gcc -c -fPIC -O2 hfs.c
	x86_64-elf-gcc -c -fPIC -O2 ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o
linux:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 partitiontable/gpt.c
	gcc -c -fPIC -O2 partitiontable/mbr.c
	gcc -c -fPIC -O2 filesystem/ext.c
	gcc -c -fPIC -O2 filesystem/fat.c
	gcc -c -fPIC -O2 filesystem/hfs.c
	gcc -c -fPIC -O2 filesystem/ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o
mac:
	clang -c -fPIC -O2 master.c
	clang -c -fPIC -O2 partitiontable/gpt.c
	clang -c -fPIC -O2 partitiontable/mbr.c
	clang -c -fPIC -O2 filesystem/ext.c
	clang -c -fPIC -O2 filesystem/fat.c
	clang -c -fPIC -O2 filesystem/hfs.c
	clang -c -fPIC -O2 filesystem/ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o
win:
	gcc -c -O2 master.c
	gcc -c -O2 partitiontable/gpt.c
	gcc -c -O2 partitiontable/mbr.c
	gcc -c -O2 filesystem/ext.c
	gcc -c -O2 filesystem/fat.c
	gcc -c -O2 filesystem/hfs.c
	gcc -c -O2 filesystem/ntfs.c
	rm -f libsoft1.a
	ar crs libsoft1.a \
	master.o \
	gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o
clean:
	rm -f *.o *.a
