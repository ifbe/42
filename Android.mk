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




#1.boot1
LOCAL_SRC_FILES := \
	library/libboot1/0.listen.c \
	library/libboot1/1.say.c \
	library/libboot1/2.diary.c \
	library/libboot1/3.history.c \
	library/libboot1/std.c
#1.hard0
ifeq ($(TARGET_ARCH),x86_64)
LOCAL_SRC_FILES += \
	library/libhard0/x86/x86.fp.c \
	library/libhard0/x86/x86.port.c \
	library/libhard0/x86/x86.platform.c
endif
ifeq ($(TARGET_ARCH),arm64)
LOCAL_SRC_FILES += library/libhard0/arm/arm.platform.c
endif

#1.soft0
LOCAL_SRC_FILES += \
	library/libsoft0/linux/linux.file.c \
	library/libsoft0/linux/linux.arg.c \
	library/libsoft0/memory.c
#1.soft1
LOCAL_SRC_FILES += \
	library/libsoft1/anscii/anscii.c \
	library/libsoft1/filesystem/fs.ext.c \
	library/libsoft1/filesystem/fs.fat.c \
	library/libsoft1/filesystem/fs.hfs.c \
	library/libsoft1/filesystem/fs.ntfs.c \
	library/libsoft1/partitiontable/pt.gpt.c \
	library/libsoft1/partitiontable/pt.mbr.c \
	library/libsoft1/master.c \
	library/libsoft1/servent.c
#1.ui0
LOCAL_SRC_FILES += \
	library/libui0/console/console.c
#1.ui1
LOCAL_SRC_FILES += \
	library/libui1/draw/background.c \
	library/libui1/draw/shape.c \
	library/libui1/text/anscii.c
#1.librarian
LOCAL_SRC_FILES += \
	library/librarian.c
#1.main
LOCAL_SRC_FILES += \
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
