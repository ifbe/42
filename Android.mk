#cd ____androidcode____
#source ____build/envsetup.sh____
#lunch ____devicename____
#ln -s ____fullpathofthis____ ____linkname____
#mm ____linkname____
#adb root
#adb remount
#adb push androidcode/out/target/product/devicename/system/bin/haha /system/bin/

LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)
LOCAL_SRC_FILES := main/cli.c \
	library/libsoft0/stdio/0.listen.c \
	library/libsoft0/stdio/1.say.c \
	library/libsoft0/stdio/2.diary.c \
	library/libsoft0/linux/file.c \
	library/libsoft0/linux/arg.c \
	library/libsoft0/memory.c \
	library/libsoft1/anscii/anscii.c \
	library/libsoft1/anscii/anscii.convert.c \
	library/libsoft1/partitiontable/gpt.c \
	library/libsoft1/partitiontable/mbr.c \
	library/libsoft1/filesystem/ext.c \
	library/libsoft1/filesystem/fat.c \
	library/libsoft1/filesystem/hfs.c \
	library/libsoft1/filesystem/ntfs.c \
	library/libsoft1/master.c \
	library/libui0/console/console.c \
	library/libui1/draw/background.c \
	library/libui1/draw/shape.c \
	library/libui1/menu/menu.c \
	library/libui1/text/anscii.c \
	library/librarian.c
LOCAL_SHARED_LIBRARIES += libc
LOCAL_LDFLAGS := -Wl,--hash-style=sysv
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := haha
include $(BUILD_EXECUTABLE)
