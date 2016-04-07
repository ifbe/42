LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)



#1.boot1
LOCAL_SRC_FILES := \
        library/libboot0/basic.c \
        library/libboot0/logcat/logcat.c

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
	library/libsoft0/linux/linux.communicate.socket.c \
	library/libsoft0/linux/linux.event.fake.c \
	library/libsoft0/linux/linux.event.net.c \
        library/libsoft0/linux/linux.flatmem.file.c \
        library/libsoft0/linux/linux.flatmem.physical.c \
        library/libsoft0/linux/linux.flatmem.virtual.c \
        library/libsoft0/linux/linux.special.folder.c \
        library/libsoft0/linux/linux.special.random.c \
        library/libsoft0/system.c

#3.soft1
LOCAL_SRC_FILES += \
        library/libsoft1/core/core.compare.c \
        library/libsoft1/core/core.convert.c \
        library/libsoft1/core/core.parse.c \
        library/libsoft1/core/core.prelibation.c \
        library/libsoft1/compress/compress.c \
        library/libsoft1/compress/compress.7z.c \
        library/libsoft1/compress/compress.cpio.c \
        library/libsoft1/compress/compress.gz.c \
        library/libsoft1/compress/compress.tar.c \
        library/libsoft1/compress/compress.zip.c \
        library/libsoft1/default/default.bin.c \
        library/libsoft1/default/default.folder.c \
        library/libsoft1/executable/exec.c \
        library/libsoft1/executable/exec.elf.c \
        library/libsoft1/executable/exec.macho.c \
        library/libsoft1/executable/exec.pe.c \
        library/libsoft1/filesystem/fs.c \
        library/libsoft1/filesystem/fs.ext.c \
        library/libsoft1/filesystem/fs.fat.c \
        library/libsoft1/filesystem/fs.hfs.c \
        library/libsoft1/filesystem/fs.ntfs.c \
        library/libsoft1/math/math.calc.c \
        library/libsoft1/math/math.notation.c \
        library/libsoft1/net/net.c \
        library/libsoft1/net/transport/net.tcp.c \
        library/libsoft1/net/transport/net.udp.c \
        library/libsoft1/parttable/pt.c \
        library/libsoft1/parttable/pt.apple.c \
        library/libsoft1/parttable/pt.gpt.c \
        library/libsoft1/parttable/pt.mbr.c \
        library/libsoft1/text/text.ascii.c \
        library/libsoft1/text/text.unicode.c \
        library/libsoft1/artery.c

#4.ui0
LOCAL_SRC_FILES += \
        library/libui0/cli/cli.cli.c \
        library/libui0/window.c

#4.ui1
LOCAL_SRC_FILES += \
        library/libui1/draw/draw.background.c \
        library/libui1/draw/draw.shape.c \
	library/libui1/game/game.2048.c \
	library/libui1/game/game.snake.c \
	library/libui1/game/game.tetris.c \
	library/libui1/menu/menu.center.c \
        library/libui1/print/print.ascii.c \
	library/libui1/print/print.unicode.c \
	library/libui1/tool/tool.console.c \
	library/libui1/tool/tool.hex.c \
	library/libui1/tool/tool.keyboard.c \
	library/libui1/tool/tool.sketchpad.c \
	library/libui1/tool/tool.tree.c \
        library/libui1/character.c

#1.visitor
LOCAL_SRC_FILES += \
        library/visitor/temple/temple.abbot.c \
        library/visitor/temple/temple.monk.c \
        library/visitor/traveller/plasma.c




#2
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS    := -lm -llog -ljnigraphics
LOCAL_MODULE    := plasma
include $(BUILD_SHARED_LIBRARY)
