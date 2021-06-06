#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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
#define _sens_ hex32('s','e','n','s')
void checkevent();
//
void initobject(void*);
void initshader(void*);
void inittexture(void*);
void initvertex(void*);
void fullwindow_create(void*);
void fullwindow_delete(void*);
void fullwindow_take(void*,void*, void*,int, void*,int, void*,int);
void fullwindow_give(void*,void*, void*,int, void*,int, void*,int);




//
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLContext context = EGL_NO_CONTEXT;
static EGLSurface surface = EGL_NO_SURFACE;
static int width = 0;
static int height = 0;
//
static struct supply* thewnd = 0;
static int candraw = 0;
//
vec3 sensor[3];




int window_take(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(wnd != thewnd)return 0;
	if(candraw){
		fullwindow_take(wnd,foot, stack,sp, arg,key, buf,len);
		eglSwapBuffers(display, surface);
	}
	checkevent();
	return 0;
}
int window_give(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(wnd->fmt){
	default:fullwindow_give(wnd,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct supply* wnd)
{
}
void windowcreate(struct supply* wnd, void* arg)
{
	say("@windowcreate\n");
	switch(wnd->fmt){
	default:{
		thewnd = wnd;
		wnd->fbwidth = wnd->width = width;
		wnd->fbheight= wnd->height= height;
		say("w=%d,h=%d\n", width, height);

		fullwindow_create(wnd);
		break;
	}//default
	}//switch
}




void initwindow()
{
	while(0==height)checkevent();
}
void freewindow()
{
}




void openwindow(struct android_app* theapp)
{
	int x,y,z;
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

	width = ANativeWindow_getWidth(theapp->window);
	height = ANativeWindow_getHeight(theapp->window);
	ANativeWindow_setBuffersGeometry(theapp->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, theapp->window, NULL);

	EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		say("eglCreateContext failed with error 0x%04x\n", eglGetError());
		return;
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		say("eglMakeCurrent failed with error 0x%04x\n", eglGetError());
		return;
	}

	say("GL Version = %s\n", glGetString(GL_VERSION));
	say("GL Vendor = %s\n", glGetString(GL_VENDOR));
	say("GL Renderer = %s\n", glGetString(GL_RENDERER));
	say("GL Extensions = %s\n", glGetString(GL_EXTENSIONS));

	candraw = 1;
}
void closewindow(struct android_app* theapp)
{
	candraw = 0;

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

	int j;
	struct supply* wnd = thewnd;
	if(0 == wnd)return;

	struct gl41data** cam = wnd->glfull_camera;
	for(j=0;j<64;j++){
		if(cam[j])bzero(&cam[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** lit = wnd->glfull_light;
	for(j=0;j<64;j++){
		if(lit[j])bzero(&lit[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** solid = wnd->glfull_solid;
	for(j=0;j<64;j++){
		if(solid[j])bzero(&solid[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** opaque = wnd->glfull_opaque;
	for(j=0;j<64;j++){
		if(opaque[j])bzero(&opaque[j]->dst, sizeof(struct gldst));
	}
}
void sendtowindow(int k, float* v)
{
	//say("%c: %f,%f,%f\n", k, v[0],v[1],v[2]);
	switch(k){
	case 'g':
		sensor[0][0] = v[0];
		sensor[0][1] = v[1];
		sensor[0][2] = v[2];
		break;
	case 'a':
		sensor[1][0] = -v[0];
		sensor[1][1] = -v[1];
		sensor[1][2] = -v[2];
		break;
	case 'm':
		sensor[2][0] = v[0];
		sensor[2][1] = v[1];
		sensor[2][2] = v[2];
		break;
	}

	struct halfrel st[32];
	window_give(thewnd,0, st,0, 0,_sens_, sensor,6);
}