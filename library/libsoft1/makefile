compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) core/core.compare.c
	$(compiler) $(cflag) core/core.convert.c
	$(compiler) $(cflag) core/core.parse.c
	$(compiler) $(cflag) core/core.prelibation.c
	$(compiler) $(cflag) compress/compress.c
	$(compiler) $(cflag) compress/compress.7z.c
	$(compiler) $(cflag) compress/compress.cpio.c
	$(compiler) $(cflag) compress/compress.gz.c
	$(compiler) $(cflag) compress/compress.tar.c
	$(compiler) $(cflag) compress/compress.zip.c
	$(compiler) $(cflag) default/default.bin.c
	$(compiler) $(cflag) default/default.folder.c
	$(compiler) $(cflag) executable/exec.c
	$(compiler) $(cflag) executable/exec.elf.c
	$(compiler) $(cflag) executable/exec.macho.c
	$(compiler) $(cflag) executable/exec.pe.c
	$(compiler) $(cflag) filesystem/fs.c
	$(compiler) $(cflag) filesystem/fs.ext.c
	$(compiler) $(cflag) filesystem/fs.fat.c
	$(compiler) $(cflag) filesystem/fs.hfs.c
	$(compiler) $(cflag) filesystem/fs.ntfs.c
	$(compiler) $(cflag) math/math.calc.c
	$(compiler) $(cflag) math/math.notation.c
	$(compiler) $(cflag) partitiontable/pt.c
	$(compiler) $(cflag) partitiontable/pt.apple.c
	$(compiler) $(cflag) partitiontable/pt.gpt.c
	$(compiler) $(cflag) partitiontable/pt.mbr.c
	$(compiler) $(cflag) text/text.c
	$(compiler) $(cflag) text/text.ascii.c
	$(compiler) $(cflag) text/text.gbk.c
	$(compiler) $(cflag) text/text.unicode.c
	$(compiler) $(cflag) artery.c
crosscompile:
	make -s target compiler="x86_64-elf-gcc" cflag="-c -fPIC -O2"
gcc:
	make -s target compiler="gcc" cflag="-c -fPIC -O2"
mingw:
	make -s target compiler="gcc" cflag="-c -O2"
clang:
	make -s target compiler="clang" cflag="-c -O2"
clean:
	rm -f *.o *.a
