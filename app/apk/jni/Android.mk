LOCAL_PATH:=$(call my-dir)/../../..
include $(CLEAR_VARS)

LOCAL_MODULE    := plasma
LOCAL_SRC_FILES := library/user/plasma.c
LOCAL_LDLIBS    := -lm -llog -ljnigraphics

include $(BUILD_SHARED_LIBRARY)
