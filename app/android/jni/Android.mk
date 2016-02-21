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
	library/libboot0/none/serial.rx.c \
	library/libboot0/none/serial.tx.c \
	library/libboot0/none/basic.c

#1.boot1
LOCAL_SRC_FILES += \
	library/libboot1/listen.c \
	library/libboot1/say.c \
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
	library/libsoft0/linux/linux.event.c \
	library/libsoft0/linux/linux.file.c \
	library/libsoft0/linux/linux.random.c \
	library/libsoft0/memory.c

#3.soft1
LOCAL_SRC_FILES += \
	library/libsoft1/compress/compress.7z.c \
	library/libsoft1/compress/compress.cpio.c \
	library/libsoft1/compress/compress.gz.c \
	library/libsoft1/compress/compress.tar.c \
	library/libsoft1/compress/compress.zip.c \
	library/libsoft1/executable/exe.elf.c \
	library/libsoft1/executable/exe.macho.c \
	library/libsoft1/executable/exe.pe.c \
	library/libsoft1/filesystem/fs.ext.c \
	library/libsoft1/filesystem/fs.fat.c \
	library/libsoft1/filesystem/fs.hfs.c \
	library/libsoft1/filesystem/fs.ntfs.c \
        library/libsoft1/math/math.calc.c \
        library/libsoft1/math/math.notation.c \
	library/libsoft1/partitiontable/pt.gpt.c \
	library/libsoft1/partitiontable/pt.mbr.c \
	library/libsoft1/text/text.ascii.c \
	library/libsoft1/text/text.unicode.c \
	library/libsoft1/manager/manager.master.c \
	library/libsoft1/manager/manager.logic.c \
	library/libsoft1/manager/manager.prelibation.c

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
	library/visitor/temple/abbot.c \
	library/visitor/temple/monk.c \
	library/visitor/traveller/cli.c




#2
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast

LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := 42
include $(BUILD_EXECUTABLE)
