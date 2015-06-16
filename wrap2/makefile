binary:
	gcc -c -fPIC -O2 master.c
	gcc -c -fPIC -O2 parttable.c
	gcc -c -fPIC -O2 ext.c
	gcc -c -fPIC -O2 fat.c
	gcc -c -fPIC -O2 hfs.c
	gcc -c -fPIC -O2 ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a master.o parttable.o ext.o fat.o hfs.o ntfs.o
crosscompile:
	x86_64-elf-gcc -c -fPIC -O2 master.c
	x86_64-elf-gcc -c -fPIC -O2 parttable.c
	x86_64-elf-gcc -c -fPIC -O2 ext.c
	x86_64-elf-gcc -c -fPIC -O2 fat.c
	x86_64-elf-gcc -c -fPIC -O2 hfs.c
	x86_64-elf-gcc -c -fPIC -O2 ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a master.o parttable.o ext.o fat.o hfs.o ntfs.o
linux:
	gcc -c -fPIC -O2 master.c
	gcc -c -O2 partitiontable/parttable.c
	gcc -c -O2 partitiontable/gpt.c
	gcc -c -O2 partitiontable/mbr.c
	gcc -c -O2 filesystem/ext.c
	gcc -c -O2 filesystem/fat.c
	gcc -c -O2 filesystem/hfs.c
	gcc -c -O2 filesystem/ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a master.o parttable.o gpt.o mbr.o ext.o fat.o hfs.o ntfs.o
mac:
	clang -c -fPIC -O2 master.c
	clang -c -O2 partitiontable/parttable.c
	clang -c -O2 partitiontable/gpt.c
	clang -c -O2 partitiontable/mbr.c
	clang -c -O2 filesystem/ext.c
	clang -c -O2 filesystem/fat.c
	clang -c -O2 filesystem/hfs.c
	clang -c -O2 filesystem/ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a master.o parttable.o ext.o fat.o hfs.o ntfs.o
win:
	gcc -c -O2 master.c
	gcc -c -O2 partitiontable/parttable.c
	gcc -c -O2 partitiontable/gpt.c
	gcc -c -O2 partitiontable/mbr.c
	gcc -c -O2 filesystem/ext.c
	gcc -c -O2 filesystem/fat.c
	gcc -c -O2 filesystem/hfs.c
	gcc -c -O2 filesystem/ntfs.c
	rm -f wrap2.a
	ar crs wrap2.a \
	master.o \
	parttable.o gpt.o mbr.o \
	ext.o fat.o hfs.o ntfs.o
clean:
	rm -f *.o *.a
