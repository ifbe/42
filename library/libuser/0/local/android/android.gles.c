#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libuser.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
void* getandroidapp();
void setandroidapp(void*);
void asset_create();
//
void initmodbuf(void*);
void initshader(void*);
void inittexture(void*);
void initobject(void*);
void callback_update(void*);
void callback_display(void*);




//
static struct android_app* app = 0;
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLContext context = EGL_NO_CONTEXT;
static EGLSurface surface = EGL_NO_SURFACE;




static void windowthread(struct arena* win)
{
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);

	const EGLint configAttribs[] = {
		//		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		//		EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};
	EGLConfig config;
	EGLint numConfigs;
	eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);

	EGLint format;
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	win->width = win->stride = ANativeWindow_getWidth(app->window);
	win->height = ANativeWindow_getHeight(app->window);
	win->depth = (win->width + win->height)/2;
	ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, app->window, NULL);

	EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		LOGI("eglCreateContext failed with error 0x%04x", eglGetError());
		return;
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGI("eglMakeCurrent failed with error 0x%04x", eglGetError());
		return;
	}

	LOGI("GL Version = %s\n", glGetString(GL_VERSION));
	LOGI("GL Vendor = %s\n", glGetString(GL_VENDOR));
	LOGI("GL Renderer = %s\n", glGetString(GL_RENDERER));
	LOGI("GL Extensions = %s\n", glGetString(GL_EXTENSIONS));

	//
	initmodbuf(win);
	initshader(win);
	initobject(win);
	inittexture(win);

	while(1)
	{
		if(win->deq == win->enq)
		{
			sleep_us(1000);
			continue;
		}
		win->deq = win->enq;

		callback_update(win);
		callback_display(win);
		eglSwapBuffers(display, surface);
	}
}




void windowread(void* dc,void* df,void* sc,void* sf)
{
}
void windowwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
}
void windowstop()
{
	if (EGL_NO_DISPLAY != display)
	{
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT)eglDestroyContext(display, context);
		if (surface != EGL_NO_SURFACE)eglDestroySurface(display, surface);
		eglTerminate(display);
	}
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}
void windowstart(struct arena* win)
{
	setandroidapp(win);

	win->type = hex32('w','i','n',0);
	win->fmt = hex32('v','b','o',0);
	win->irel = 0;
	win->orel = 0;

	win->width = 1024;
	win->height = 1024;
	win->depth = 1024;

	asset_create();
	threadcreate(windowthread, win);
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