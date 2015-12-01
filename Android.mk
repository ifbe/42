#cd ____coderoot____
#ln -s ____fullpathofme____ ____linkname____

#source ____build/envsetup.sh____
#lunch ____devicename____
#mmm ____linkname____

#adb root
#adb remount
#adb push coderoot/out/target/product/devicename/system/bin/haha /system/bin/




#0.just head
LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

#1.source file
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES := \
	library/libsoft0/x86/fp.c \
	library/libsoft0/x86/port.c \
	library/libsoft0/x86/platform.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES := library/libsoft0/arm/platform.c
endif

#1.1 source file
LOCAL_SRC_FILES += \
	library/libsoft0/linux/file.c \
	library/libsoft0/linux/arg.c \
	library/libsoft0/stdio/0.listen.c \
	library/libsoft0/stdio/1.say.c \
	library/libsoft0/stdio/2.diary.c \
	library/libsoft0/stdio/3.history.c \
	library/libsoft0/memory.c \
	library/libsoft1/anscii/anscii.c \
	library/libsoft1/filesystem/ext.c \
	library/libsoft1/filesystem/fat.c \
	library/libsoft1/filesystem/hfs.c \
	library/libsoft1/filesystem/ntfs.c \
	library/libsoft1/partitiontable/gpt.c \
	library/libsoft1/partitiontable/mbr.c \
	library/libsoft1/master.c \
	library/libui0/console/console.c \
	library/libui1/draw/background.c \
	library/libui1/draw/shape.c \
	library/libui1/menu/menu.c \
	library/libui1/text/anscii.c \
	library/librarian.c \
	main/cli.c

#2.so
LOCAL_SHARED_LIBRARIES += libc

#3.link argument
LOCAL_LDFLAGS := -Wl,--hash-style=sysv

#4.tag
LOCAL_MODULE_TAGS := optional

#5.target name
LOCAL_MODULE := haha

#6.don't know
include $(BUILD_EXECUTABLE)
