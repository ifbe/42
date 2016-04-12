compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) interface/interface.bluetooth.c
	$(compiler) $(cflag) interface/interface.i2c.c
	$(compiler) $(cflag) interface/interface.ir.c
	$(compiler) $(cflag) interface/interface.nfc.c
	$(compiler) $(cflag) interface/interface.pci.c
	$(compiler) $(cflag) interface/interface.spi.c
	$(compiler) $(cflag) interface/interface.uart.c
	$(compiler) $(cflag) interface/interface.usb.c
	$(compiler) $(cflag) interface/interface.wifi.c
	$(compiler) $(cflag) interface/interface.zigbee.c
	$(compiler) $(cflag) memory/bin.c
	$(compiler) $(cflag) memory/executable/exec.c
	$(compiler) $(cflag) memory/executable/exec.elf.c
	$(compiler) $(cflag) memory/executable/exec.macho.c
	$(compiler) $(cflag) memory/executable/exec.pe.c
	$(compiler) $(cflag) memory/filesystem/fs.c
	$(compiler) $(cflag) memory/filesystem/fs.ext.c
	$(compiler) $(cflag) memory/filesystem/fs.fat.c
	$(compiler) $(cflag) memory/filesystem/fs.hfs.c
	$(compiler) $(cflag) memory/filesystem/fs.ntfs.c
	$(compiler) $(cflag) memory/parttable/pt.c
	$(compiler) $(cflag) memory/parttable/pt.apple.c
	$(compiler) $(cflag) memory/parttable/pt.gpt.c
	$(compiler) $(cflag) memory/parttable/pt.mbr.c
	$(compiler) $(cflag) memory/text/text.c
	$(compiler) $(cflag) memory/text/text.ascii.c
	$(compiler) $(cflag) memory/text/text.gbk.c
	$(compiler) $(cflag) memory/text/text.unicode.c
	$(compiler) $(cflag) net/net.c
	$(compiler) $(cflag) net/application/net.http.c
	$(compiler) $(cflag) net/transport/net.tcp.c
	$(compiler) $(cflag) net/transport/net.udp.c
	$(compiler) $(cflag) special/compress/compress.c
	$(compiler) $(cflag) special/compress/compress.7z.c
	$(compiler) $(cflag) special/compress/compress.cpio.c
	$(compiler) $(cflag) special/compress/compress.gz.c
	$(compiler) $(cflag) special/compress/compress.tar.c
	$(compiler) $(cflag) special/compress/compress.zip.c
	$(compiler) $(cflag) special/crypt/crypt.aes.c
	$(compiler) $(cflag) special/crypt/crypt.des.c
	$(compiler) $(cflag) special/crypt/crypt.rsa.c
	$(compiler) $(cflag) special/hash/hash.c
	$(compiler) $(cflag) special/hash/hash.crc8.c
	$(compiler) $(cflag) special/hash/hash.crc16.c
	$(compiler) $(cflag) special/hash/hash.crc32.c
	$(compiler) $(cflag) special/hash/hash.md5.c
	$(compiler) $(cflag) special/hash/hash.sha256.c
	$(compiler) $(cflag) special/hash/hash.string.c
	$(compiler) $(cflag) special/math/math.calc.c
	$(compiler) $(cflag) special/math/math.notation.c
	$(compiler) $(cflag) special/parasite/parasite.folder.c
	$(compiler) $(cflag) special/parasite/parasite.setting.c
	$(compiler) $(cflag) special/string/string.compare.c
	$(compiler) $(cflag) special/string/string.convert.c
	$(compiler) $(cflag) special/string/string.parse.c
	$(compiler) $(cflag) special/string/string.prelibation.c
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
