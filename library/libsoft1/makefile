compiler ?= gcc
cflag ?= -c -O2
target:
	$(compiler) $(cflag) artery.c
	$(compiler) $(cflag) interface/interface.c
	$(compiler) $(cflag) interface/interface.bluetooth.c
	$(compiler) $(cflag) interface/interface.gpio.c
	$(compiler) $(cflag) interface/interface.i2c.c
	$(compiler) $(cflag) interface/interface.ir.c
	$(compiler) $(cflag) interface/interface.nfc.c
	$(compiler) $(cflag) interface/interface.pci.c
	$(compiler) $(cflag) interface/interface.spi.c
	$(compiler) $(cflag) interface/interface.uart.c
	$(compiler) $(cflag) interface/interface.usb.c
	$(compiler) $(cflag) interface/interface.wifi.c
	$(compiler) $(cflag) interface/interface.zigbee.c
	$(compiler) $(cflag) memory/memory.c
	$(compiler) $(cflag) memory/bin/bin.c
	$(compiler) $(cflag) memory/bin/bin.elf.c
	$(compiler) $(cflag) memory/bin/bin.macho.c
	$(compiler) $(cflag) memory/bin/bin.pe.c
	$(compiler) $(cflag) memory/encdec/comp.7z.c
	$(compiler) $(cflag) memory/encdec/comp.cpio.c
	$(compiler) $(cflag) memory/encdec/comp.gz.c
	$(compiler) $(cflag) memory/encdec/comp.tar.c
	$(compiler) $(cflag) memory/encdec/comp.zip.c
	$(compiler) $(cflag) memory/encdec/crypt.aes.c
	$(compiler) $(cflag) memory/encdec/crypt.des.c
	$(compiler) $(cflag) memory/encdec/crypt.rsa.c
	$(compiler) $(cflag) memory/encdec/hash.crc8.c
	$(compiler) $(cflag) memory/encdec/hash.crc16.c
	$(compiler) $(cflag) memory/encdec/hash.crc32.c
	$(compiler) $(cflag) memory/encdec/hash.md5.c
	$(compiler) $(cflag) memory/encdec/hash.sha256.c
	$(compiler) $(cflag) memory/encdec/hash.string.c
	$(compiler) $(cflag) memory/encdec/qrcode.c
	$(compiler) $(cflag) memory/fs/fs.c
	$(compiler) $(cflag) memory/fs/fs.ext.c
	$(compiler) $(cflag) memory/fs/fs.fat.c
	$(compiler) $(cflag) memory/fs/fs.hfs.c
	$(compiler) $(cflag) memory/fs/fs.ntfs.c
	$(compiler) $(cflag) memory/fs/pt.c
	$(compiler) $(cflag) memory/fs/pt.apple.c
	$(compiler) $(cflag) memory/fs/pt.gpt.c
	$(compiler) $(cflag) memory/fs/pt.mbr.c
	$(compiler) $(cflag) memory/string/string.compare.c
	$(compiler) $(cflag) memory/string/string.convert.c
	$(compiler) $(cflag) memory/string/string.parse.c
	$(compiler) $(cflag) memory/text/text.ascii.c
	$(compiler) $(cflag) memory/text/text.gbk.c
	$(compiler) $(cflag) memory/text/text.unicode.c
	$(compiler) $(cflag) net/net.c
	$(compiler) $(cflag) net/net.arp.c
	$(compiler) $(cflag) net/net.ethernet.c
	$(compiler) $(cflag) net/net.ftp.c
	$(compiler) $(cflag) net/net.http.c
	$(compiler) $(cflag) net/net.icmp.c
	$(compiler) $(cflag) net/net.ipv4.c
	$(compiler) $(cflag) net/net.ipv6.c
	$(compiler) $(cflag) net/net.ppp.c
	$(compiler) $(cflag) net/net.quic.c
	$(compiler) $(cflag) net/net.slip.c
	$(compiler) $(cflag) net/net.ssh.c
	$(compiler) $(cflag) net/net.tcp.c
	$(compiler) $(cflag) net/net.tftp.c
	$(compiler) $(cflag) net/net.udp.c
	$(compiler) $(cflag) special/special.c
	$(compiler) $(cflag) special/host/host.folder.c
	$(compiler) $(cflag) special/host/host.setting.c
	$(compiler) $(cflag) special/math/math.calc.c
	$(compiler) $(cflag) special/math/math.notation.c
crosscompile:
	make -s target compiler="x86_64-elf-gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
gcc:
	make -s target compiler="gcc" cflag="-c -fPIC -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
mingw:
	make -s target compiler="gcc" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clang:
	make -s target compiler="clang" cflag="-c -O2 -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast"
clean:
	rm -f *.o *.a
