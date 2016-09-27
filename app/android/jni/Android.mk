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
	library/libhard0/cpu/x86/x86.port.c \
	library/libhard0/cpu/x86/x86.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/x64/x64.fp.c \
	library/libhard0/cpu/x64/x64.port.c \
	library/libhard0/cpu/x64/x64.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm/arm.fp.c \
	library/libhard0/cpu/arm/arm.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/arm64/arm64.fp.c \
	library/libhard0/cpu/arm64/arm64.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips/mips.fp.c \
	library/libhard0/cpu/mips/mips.platform.c \
	library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
	library/libhard0/cpu/mips64/mips64.fp.c \
	library/libhard0/cpu/mips64/mips64.platform.c \
	library/libhard0/driver.c
endif

#2.hard1
LOCAL_SRC_FILES += \
	library/libhard1/body.c

#3.soft0
LOCAL_SRC_FILES += \
	library/libsoft0/linux/linux.interface.bluetooth.c \
	library/libsoft0/linux/linux.interface.gpio.c \
	library/libsoft0/linux/linux.interface.i2c.c \
	library/libsoft0/linux/linux.interface.spi.c \
	library/libsoft0/linux/linux.interface.uart.c \
	library/libsoft0/linux/linux.interface.usb.c \
	library/libsoft0/linux/linux.interface.wifi.c \
	library/libsoft0/linux/linux.memory.file.c \
	library/libsoft0/linux/linux.memory.physical.c \
	library/libsoft0/linux/linux.memory.virtual.c \
	library/libsoft0/linux/linux.net.epoll.c \
	library/libsoft0/linux/linux.net.socket.c \
	library/libsoft0/linux/linux.special.folder.c \
	library/libsoft0/linux/linux.special.random.c \
	library/libsoft0/linux/linux.special.signal.c \
	library/libsoft0/linux/linux.special.time.c \
	library/libsoft0/system.c

#3.soft1
LOCAL_SRC_FILES += \
	library/libsoft1/artery.c \
	library/libsoft1/interface/interface.c \
	library/libsoft1/interface/interface.bluetooth.c \
	library/libsoft1/interface/interface.i2c.c \
	library/libsoft1/interface/interface.ir.c \
	library/libsoft1/interface/interface.nfc.c \
	library/libsoft1/interface/interface.pci.c \
	library/libsoft1/interface/interface.spi.c \
	library/libsoft1/interface/interface.uart.c \
	library/libsoft1/interface/interface.usb.c \
	library/libsoft1/interface/interface.wifi.c \
	library/libsoft1/interface/interface.zigbee.c \
	library/libsoft1/memory/memory.c \
	library/libsoft1/memory/datasystem/compress/comp.7z.c \
	library/libsoft1/memory/datasystem/compress/comp.cpio.c \
	library/libsoft1/memory/datasystem/compress/comp.gz.c \
	library/libsoft1/memory/datasystem/compress/comp.tar.c \
	library/libsoft1/memory/datasystem/compress/comp.zip.c \
	library/libsoft1/memory/datasystem/executable/exec.elf.c \
	library/libsoft1/memory/datasystem/executable/exec.macho.c \
	library/libsoft1/memory/datasystem/executable/exec.pe.c \
	library/libsoft1/memory/datasystem/filesystem/fs.c \
	library/libsoft1/memory/datasystem/filesystem/fs.ext.c \
	library/libsoft1/memory/datasystem/filesystem/fs.fat.c \
	library/libsoft1/memory/datasystem/filesystem/fs.hfs.c \
	library/libsoft1/memory/datasystem/filesystem/fs.ntfs.c \
	library/libsoft1/memory/datasystem/parttable/pt.c \
	library/libsoft1/memory/datasystem/parttable/pt.apple.c \
	library/libsoft1/memory/datasystem/parttable/pt.gpt.c \
	library/libsoft1/memory/datasystem/parttable/pt.mbr.c \
	library/libsoft1/memory/encdec/qrcode.c \
	library/libsoft1/memory/encdec/crypt/crypt.aes.c \
	library/libsoft1/memory/encdec/crypt/crypt.base64.c \
	library/libsoft1/memory/encdec/crypt/crypt.des.c \
	library/libsoft1/memory/encdec/crypt/crypt.rsa.c \
	library/libsoft1/memory/encdec/hash/hash.crc8.c \
	library/libsoft1/memory/encdec/hash/hash.crc16.c \
	library/libsoft1/memory/encdec/hash/hash.crc32.c \
	library/libsoft1/memory/encdec/hash/hash.md5.c \
	library/libsoft1/memory/encdec/hash/hash.sha1.c \
	library/libsoft1/memory/encdec/hash/hash.string.c \
	library/libsoft1/memory/text/code/code.c \
	library/libsoft1/memory/text/compare/compare.simple.c \
	library/libsoft1/memory/text/compare/compare.wildcard.c \
	library/libsoft1/memory/text/convert/convert.argument.c \
	library/libsoft1/memory/text/convert/convert.ascii.c \
	library/libsoft1/memory/text/convert/convert.encoding.c \
	library/libsoft1/memory/text/convert/convert.filename.c \
	library/libsoft1/memory/text/convert/convert.network.c \
	library/libsoft1/memory/text/convert/convert.number.c \
	library/libsoft1/net/net.c \
	library/libsoft1/net/net.arp.c \
	library/libsoft1/net/net.ethernet.c \
	library/libsoft1/net/net.ftp.c \
	library/libsoft1/net/net.http.c \
	library/libsoft1/net/net.icmp.c \
	library/libsoft1/net/net.ipv4.c \
	library/libsoft1/net/net.ipv6.c \
	library/libsoft1/net/net.ppp.c \
	library/libsoft1/net/net.quic.c \
	library/libsoft1/net/net.slip.c \
	library/libsoft1/net/net.ssh.c \
	library/libsoft1/net/net.tcp.c \
	library/libsoft1/net/net.tftp.c \
	library/libsoft1/net/net.udp.c \
	library/libsoft1/special/special.c \
	library/libsoft1/special/math/math.calc.c \
	library/libsoft1/special/math/math.notation.c \
	library/libsoft1/special/physics/physics.electrical.c \
	library/libsoft1/special/physics/physics.mechanics.c \
	library/libsoft1/special/system/system.folder.c \
	library/libsoft1/special/system/system.process.c \
	library/libsoft1/special/system/system.setting.c

#4.ui0
LOCAL_SRC_FILES += \
	library/libui0/terminal/terminal.cli.c \
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

#1.visitor
LOCAL_SRC_FILES += \
	library/visitor0/command.c \
	library/visitor0/event.c \
	library/visitor0/universe.c \
	library/visitor1/cli.c




#2
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := 42
include $(BUILD_EXECUTABLE)
