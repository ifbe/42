#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "arena.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
void* getandroidapp();
void setandroidapp(void*);




static struct android_app* app;
static ANativeWindow* native;
static ANativeWindow_Buffer buffer;




void windowread()
{
}
void windowwrite(struct arena* win)
{
	ANativeWindow_unlockAndPost(native);

	ANativeWindow_lock(native, &buffer, NULL);
	win->buf = buffer.bits;
	win->width = buffer.width;
	win->height = buffer.height;
	win->stride = buffer.stride;
}
void windowstart(struct arena* win)
{
	setandroidapp(win);
	native = app->window;
	LOGI("android window:%llx\n", (u64)native);

	int w = ANativeWindow_getWidth(native);
	int h = ANativeWindow_getHeight(native);
	ANativeWindow_setBuffersGeometry(native, w, h, WINDOW_FORMAT_RGBA_8888);
	LOGI("w=%d,h=%d\n", w, h);

	win->type = hex32('w','i','n',0);
	win->fmt = hex64('r','g','b','a','8','8','8','8');

	ANativeWindow_lock(native, &buffer, NULL);
	win->buf = buffer.bits;
	win->width = buffer.width;
	win->height = buffer.height;
	win->stride = buffer.stride;
}
void windowstop()
{
}
void windowcreate()
{
}
void windowdelete()
{
}




void initwindow()
{
	app = getandroidapp();
}
void freewindow()
{
}
