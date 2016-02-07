LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)



#1.boot1
LOCAL_SRC_FILES := \
        library/libboot0/logcat/logcat.c

#1.boot1
LOCAL_SRC_FILES += \
        library/libboot1/listen.c \
        library/libboot1/say.c \
        library/libboot1/debug.c

#2.hard0
ifeq ($(TARGET_ARCH),x86)
LOCAL_SRC_FILES += \
        library/libhard0/x86/x86.fp.c \
        library/libhard0/x86/x86.port.c \
        library/libhard0/x86/x86.platform.c \
        library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
        library/libhard0/x86/x86.fp.c \
        library/libhard0/x86/x86.port.c \
        library/libhard0/x86/x86.platform.c \
        library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm)
LOCAL_SRC_FILES += \
        library/libhard0/arm/arm.fp.c \
        library/libhard0/arm/arm.platform.c \
        library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += \
        library/libhard0/arm/arm.fp.c \
        library/libhard0/arm/arm.platform.c \
        library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips)
LOCAL_SRC_FILES += \
        library/libhard0/mips/mips.fp.c \
        library/libhard0/mips/mips.platform.c \
        library/libhard0/driver.c
endif
ifeq ($(TARGET_ARCH),mips64)
LOCAL_SRC_FILES += \
        library/libhard0/mips/mips.fp.c \
        library/libhard0/mips/mips.platform.c \
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
        library/libsoft1/compress/7z.c \
        library/libsoft1/compress/cpio.c \
        library/libsoft1/compress/gz.c \
        library/libsoft1/compress/tar.c \
        library/libsoft1/compress/zip.c \
        library/libsoft1/executable/elf.c \
        library/libsoft1/executable/macho.c \
        library/libsoft1/executable/pe.c \
        library/libsoft1/filesystem/fs.ext.c \
        library/libsoft1/filesystem/fs.fat.c \
        library/libsoft1/filesystem/fs.hfs.c \
        library/libsoft1/filesystem/fs.ntfs.c \
        library/libsoft1/manager/logic.c \
        library/libsoft1/manager/real.c \
        library/libsoft1/math/calc.c \
        library/libsoft1/math/scientificnotation.c \
        library/libsoft1/partitiontable/pt.gpt.c \
        library/libsoft1/partitiontable/pt.mbr.c \
        library/libsoft1/text/ascii.c \
        library/libsoft1/text/unicode.c \
        library/libsoft1/master.c \

#4.ui0
LOCAL_SRC_FILES += \
        library/libui0/cli/cli.cli.c \
        library/libui0/window.c

#4.ui1
LOCAL_SRC_FILES += \
        library/libui1/draw/background.c \
        library/libui1/draw/shape.c \
	library/libui1/game/2048.c \
	library/libui1/menu/menu.c \
        library/libui1/text/ascii.c \
	library/libui1/text/unicode.c \
	library/libui1/tool/hex.c \
	library/libui1/tool/keyboard.c \
	library/libui1/tool/tree.c \
	library/libui1/tool/sketchpad.c \
	library/libui1/tool/console.c \
        library/libui1/character.c

#1.visitor
LOCAL_SRC_FILES += \
        library/visitor/sweepingmonk.c \
        library/visitor/plasma.c




#2
LOCAL_CFLAGS := -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
LOCAL_LDLIBS    := -lm -llog -ljnigraphics
LOCAL_MODULE    := plasma
include $(BUILD_SHARED_LIBRARY)
