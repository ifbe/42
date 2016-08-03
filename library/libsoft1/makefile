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
	$(compiler) $(cflag) memory/bin/exec/exec.elf.c
	$(compiler) $(cflag) memory/bin/exec/exec.macho.c
	$(compiler) $(cflag) memory/bin/exec/exec.pe.c
	$(compiler) $(cflag) memory/bin/fs/fs.c
	$(compiler) $(cflag) memory/bin/fs/comp.7z.c
	$(compiler) $(cflag) memory/bin/fs/comp.cpio.c
	$(compiler) $(cflag) memory/bin/fs/comp.gz.c
	$(compiler) $(cflag) memory/bin/fs/comp.tar.c
	$(compiler) $(cflag) memory/bin/fs/comp.zip.c
	$(compiler) $(cflag) memory/bin/fs/fs.ext.c
	$(compiler) $(cflag) memory/bin/fs/fs.fat.c
	$(compiler) $(cflag) memory/bin/fs/fs.hfs.c
	$(compiler) $(cflag) memory/bin/fs/fs.ntfs.c
	$(compiler) $(cflag) memory/bin/fs/pt.c
	$(compiler) $(cflag) memory/bin/fs/pt.apple.c
	$(compiler) $(cflag) memory/bin/fs/pt.gpt.c
	$(compiler) $(cflag) memory/bin/fs/pt.mbr.c
	$(compiler) $(cflag) memory/bin/image/image.blur.c
	$(compiler) $(cflag) memory/bin/image/image.channel.c
	$(compiler) $(cflag) memory/bin/image/image.gray.c
	$(compiler) $(cflag) memory/bin/image/image.mosaic.c
	$(compiler) $(cflag) memory/encdec/crypt/crypt.aes.c
	$(compiler) $(cflag) memory/encdec/crypt/crypt.des.c
	$(compiler) $(cflag) memory/encdec/crypt/crypt.rsa.c
	$(compiler) $(cflag) memory/encdec/hash/hash.crc8.c
	$(compiler) $(cflag) memory/encdec/hash/hash.crc16.c
	$(compiler) $(cflag) memory/encdec/hash/hash.crc32.c
	$(compiler) $(cflag) memory/encdec/hash/hash.md5.c
	$(compiler) $(cflag) memory/encdec/hash/hash.sha256.c
	$(compiler) $(cflag) memory/encdec/hash/hash.string.c
	$(compiler) $(cflag) memory/encdec/qrcode.c
	$(compiler) $(cflag) memory/string/string.compare.c
	$(compiler) $(cflag) memory/string/string.convert.c
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
	$(compiler) $(cflag) special/math/math.calc.c
	$(compiler) $(cflag) special/math/math.notation.c
	$(compiler) $(cflag) special/physics/physics.electrical.c
	$(compiler) $(cflag) special/physics/physics.mechanics.c
	$(compiler) $(cflag) special/system/system.folder.c
	$(compiler) $(cflag) special/system/system.process.c
	$(compiler) $(cflag) special/system/system.thread.c
	$(compiler) $(cflag) special/system/system.setting.c
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
