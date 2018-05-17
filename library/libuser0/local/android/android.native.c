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

static void windowthread(struct arena* win)
{
	ANativeWindow_Buffer buffer;
	while(1)
	{
		//wait or not
		if(win->deq == win->enq)
		{
			sleep_us(1000);
			continue;
		}
		win->deq = win->enq;

		//get addr and size
		ANativeWindow_lock(native, &buffer, NULL);

		//set addr and size
		win->buf = buffer.bits;
		win->width = buffer.width;
		win->height = buffer.height;
		win->stride = buffer.stride;

		//read data
		actorread_all(win);

		//write screen
		ANativeWindow_unlockAndPost(native);
	}
}




void windowread(void* dc,void* df,void* sc,void* sf)
{
}
void windowwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
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
	threadcreate(windowthread, win);
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
void inittray()
{
}
void freetray()
{
}