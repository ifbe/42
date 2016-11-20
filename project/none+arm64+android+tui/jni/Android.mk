#cd ____coderoot____
#ln -s ____fullpathofme____ ____linkname____

#source ____build/envsetup.sh____
#lunch ____devicename____
#mmm ____linkname____

#adb root
#adb remount
#adb push coderoot/out/target/product/devicename/system/bin/42 /system/bin/




#0.head
LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)




#1.boot0
LOCAL_SRC_FILES := \
	library/libboot0/none/tx.c \
	library/libboot0/basic.c

#1.boot1
LOCAL_SRC_FILES += \
	library/libboot1/debug.c

#2.hard0
ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/x86/x86.fp.c \
	library/libhard0/cpu/x86/x86.lock.c \
	library/libhard0/cpu/x86/x86.port.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/x64/x64.fp.c \
	library/libhard0/cpu/x64/x64.lock.c \
	library/libhard0/cpu/x64/x64.port.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm/arm.fp.c \
	library/libhard0/cpu/arm/arm.lock.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm64/arm64.fp.c \
	library/libhard0/cpu/arm64/arm64.lock.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips/mips.fp.c \
	library/libhard0/cpu/mips/mips.lock.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips64/mips64.fp.c \
	library/libhard0/cpu/mips64/mips64.lock.c \
	library/libhard0/driver.c
endif

#2.hard1
LOCAL_SRC_FILES += \
	library/libhard1/body.c

#3.soft0
LOCAL_SRC_FILES += \
	library/libsoft0/linux/linux.wire.bt.c \
	library/libsoft0/linux/linux.wire.gpio.c \
	library/libsoft0/linux/linux.wire.i2c.c \
	library/libsoft0/linux/linux.wire.spi.c \
	library/libsoft0/linux/linux.wire.uart.c \
	library/libsoft0/linux/linux.wire.usb.c \
	library/libsoft0/linux/linux.wire.wifi.c \
	library/libsoft0/linux/linux.memory.file.c \
	library/libsoft0/linux/linux.memory.physical.c \
	library/libsoft0/linux/linux.memory.virtual.c \
	library/libsoft0/linux/linux.net.client.c \
	library/libsoft0/linux/linux.net.server.c \
	library/libsoft0/linux/linux.system.folder.c \
	library/libsoft0/linux/linux.system.process.c \
	library/libsoft0/linux/linux.system.random.c \
	library/libsoft0/linux/linux.system.signal.c \
	library/libsoft0/linux/linux.system.time.c \
	library/libsoft0/system.c

#3.soft1
LOCAL_SRC_FILES += \
	library/libsoft1/libmemory/libbio/bio.c \
	library/libsoft1/libmemory/libchem/chem.c \
	library/libsoft1/libmemory/libfile/file.c \
	library/libsoft1/libmemory/libfs/compress/compress.7z.c \
	library/libsoft1/libmemory/libfs/compress/compress.cpio.c \
	library/libsoft1/libmemory/libfs/compress/compress.gz.c \
	library/libsoft1/libmemory/libfs/compress/compress.tar.c \
	library/libsoft1/libmemory/libfs/compress/compress.zip.c \
	library/libsoft1/libmemory/libfs/diskimage/img.raw.c \
	library/libsoft1/libmemory/libfs/diskimage/img.vhd.c \
	library/libsoft1/libmemory/libfs/diskimage/img.vmdk.c \
	library/libsoft1/libmemory/libfs/executable/exec.elf.c \
	library/libsoft1/libmemory/libfs/executable/exec.macho.c \
	library/libsoft1/libmemory/libfs/executable/exec.pe.c \
	library/libsoft1/libmemory/libfs/filesystem/fs.ext.c \
	library/libsoft1/libmemory/libfs/filesystem/fs.fat.c \
	library/libsoft1/libmemory/libfs/filesystem/fs.hfs.c \
	library/libsoft1/libmemory/libfs/filesystem/fs.ntfs.c \
	library/libsoft1/libmemory/libfs/parttable/pt.apple.c \
	library/libsoft1/libmemory/libfs/parttable/pt.gpt.c \
	library/libsoft1/libmemory/libfs/parttable/pt.mbr.c \
	library/libsoft1/libmemory/libfs/fs.c \
	library/libsoft1/libmemory/libmath/math.calc.c \
	library/libsoft1/libmemory/libmath/math.notation.c \
	library/libsoft1/libmemory/libmath/math.prime.c \
	library/libsoft1/libmemory/libmath/math.c \
	library/libsoft1/libmemory/libphys/physics.electrical.c \
	library/libsoft1/libmemory/libphys/physics.mechanics.c \
	library/libsoft1/libmemory/libphys/phys.c \
	library/libsoft1/libmemory/libsec/crypt/crypt.aes.c \
	library/libsoft1/libmemory/libsec/crypt/crypt.base64.c \
	library/libsoft1/libmemory/libsec/crypt/crypt.des.c \
	library/libsoft1/libmemory/libsec/crypt/crypt.rsa.c \
	library/libsoft1/libmemory/libsec/hash/hash.crc8.c \
	library/libsoft1/libmemory/libsec/hash/hash.crc16.c \
	library/libsoft1/libmemory/libsec/hash/hash.crc32.c \
	library/libsoft1/libmemory/libsec/hash/hash.md5.c \
	library/libsoft1/libmemory/libsec/hash/hash.sha1.c \
	library/libsoft1/libmemory/libsec/hash/hash.string.c \
	library/libsoft1/libmemory/libsec/qrcode.c \
	library/libsoft1/libmemory/libsec/sec.c \
	library/libsoft1/libmemory/libtext/compare/compare.simple.c \
	library/libsoft1/libmemory/libtext/compare/compare.wildcard.c \
	library/libsoft1/libmemory/libtext/convert/convert.argument.c \
	library/libsoft1/libmemory/libtext/convert/convert.ascii.c \
	library/libsoft1/libmemory/libtext/convert/convert.copy.c \
	library/libsoft1/libmemory/libtext/convert/convert.encoding.c \
	library/libsoft1/libmemory/libtext/convert/convert.filename.c \
	library/libsoft1/libmemory/libtext/convert/convert.network.c \
	library/libsoft1/libmemory/libtext/convert/convert.number.c \
	library/libsoft1/libmemory/libtext/find/find.byte.c \
	library/libsoft1/libmemory/libtext/find/find.str.c \
	library/libsoft1/libmemory/libtext/text.c \
	library/libsoft1/libmemory/memory.c \
	library/libsoft1/libnet/worker/net.arp.c \
	library/libsoft1/libnet/worker/net.chat.c \
	library/libsoft1/libnet/worker/net.eth.c \
	library/libsoft1/libnet/worker/net.ftp.c \
	library/libsoft1/libnet/worker/net.http.c \
	library/libsoft1/libnet/worker/net.icmp.c \
	library/libsoft1/libnet/worker/net.ipv4.c \
	library/libsoft1/libnet/worker/net.ipv6.c \
	library/libsoft1/libnet/worker/net.ppp.c \
	library/libsoft1/libnet/worker/net.quic.c \
	library/libsoft1/libnet/worker/net.slip.c \
	library/libsoft1/libnet/worker/net.ssh.c \
	library/libsoft1/libnet/worker/net.tftp.c \
	library/libsoft1/libnet/worker/net.ws.c \
	library/libsoft1/libnet/manager/net.client.c \
	library/libsoft1/libnet/manager/net.server.c \
	library/libsoft1/libnet/net.c \
	library/libsoft1/libsystem/system.folder.c \
	library/libsoft1/libsystem/system.process.c \
	library/libsoft1/libsystem/system.setting.c \
	library/libsoft1/libsystem/system.c \
	library/libsoft1/libwire/wire.bt.c \
	library/libsoft1/libwire/wire.i2c.c \
	library/libsoft1/libwire/wire.ir.c \
	library/libsoft1/libwire/wire.nfc.c \
	library/libsoft1/libwire/wire.pci.c \
	library/libsoft1/libwire/wire.spi.c \
	library/libsoft1/libwire/wire.uart.c \
	library/libsoft1/libwire/wire.usb.c \
	library/libsoft1/libwire/wire.wifi.c \
	library/libsoft1/libwire/wire.zigbee.c \
	library/libsoft1/libwire/wire.c \
	library/libsoft1/artery.c

#4.ui0
LOCAL_SRC_FILES += \
	library/libui0/linux/linux.tui.c \
	library/libui0/display.c

#4.ui1
LOCAL_SRC_FILES += \
	library/libui1/draw/draw.background.c \
	library/libui1/draw/draw.shape.c \
	library/libui1/draw/draw.ascii.c \
	library/libui1/draw/draw.unicode.c \
	library/libui1/game/game.2048.c \
	library/libui1/game/game.ooxx.c \
	library/libui1/game/game.snake.c \
	library/libui1/game/game.sudoku.c \
	library/libui1/game/game.tetris.c \
	library/libui1/game/game.weiqi.c \
	library/libui1/game/game.xiangqi.c \
	library/libui1/menu/menu.center.c \
	library/libui1/menu/menu.roster.c \
	library/libui1/menu/menu.virtkbd.c \
	library/libui1/test/test.color.c \
	library/libui1/test/test.doodle.c \
	library/libui1/test/test.font.c \
	library/libui1/test/test.pure.c \
	library/libui1/tool/tool.calculator.c \
	library/libui1/tool/tool.console.c \
	library/libui1/tool/tool.control.c \
	library/libui1/tool/tool.hex.c \
	library/libui1/tool/tool.qrcode.c \
	library/libui1/tool/tool.sketchpad.c \
	library/libui1/tool/tool.spectrum.c \
	library/libui1/tool/tool.tree.c \
	library/libui1/character.c

#1.visitor0
LOCAL_SRC_FILES += \
	library/visitor0/app/world.c \
	library/visitor0/app/event.c

#1.visitor1
LOCAL_SRC_FILES += \
	library/visitor1/gui.c




#2
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := 42
include $(BUILD_EXECUTABLE)
