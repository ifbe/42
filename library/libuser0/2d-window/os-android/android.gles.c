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
#define _quat_ hex32('q','u','a','t')
#define _sens_ hex32('s','e','n','s')
#define _easyag_  hex64('e','a','s','y','a','g', 0 , 0 )
#define _mahony_  hex64('m','a','h','o','n','y', 0 , 0 )
#define _madgwick_ hex64('m','a','d','g','w','i','c','k')
void checkevent();
//
void* supply_alloc();
void* supply_recycle(void*);
//
void easyag_modify(void*,int, void*,int, void*,int);
void mahony_modify(void*,int, void*,int, void*,int);
void madgwick_modify(void*,int, void*,int, void*,int);
//
void initobject(void*);
void initshader(void*);
void inittexture(void*);
void initvertex(void*);
//
void fullwindow_create(_obj* ogl, void* arg, int argc, char** argv);
void fullwindow_delete(void*);
void fullwindow_take(void*,void*, void*,int, p64,int, void*,int);
void fullwindow_give(void*,void*, void*,int, p64,int, void*,int);




//
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLContext context = EGL_NO_CONTEXT;
static EGLSurface surface = EGL_NO_SURFACE;
static int width = 0;
static int height = 0;
//
static _obj* thewnd = 0;
static int candraw = 0;
//
struct artery* fusion_easyag = 0;
struct artery* fusion_madgwick = 0;
//
int age[3] = {0};
vec3 sensor[3] = {
	{0,0,0},
	{0,0,0},
	{0,0,0}
};




int window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	if(wnd != thewnd)return 0;
	if(candraw){
		fullwindow_take(wnd,foot, stack,sp, arg,cmd, buf,len);
		eglSwapBuffers(display, surface);
	}
	checkevent();
	return 0;
}
int window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	switch(wnd->hfmt){
	default:fullwindow_give(wnd,foot, stack,sp, arg,cmd, buf,len);
	}
	return 0;
}
void window_detach()
{
}
void window_attach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* wnd)
{
}
void window_create(_obj* wnd, void* arg)
{
	logtoall("@windowcreate\n");
	switch(wnd->hfmt){
	default:{
		thewnd = wnd;
		wnd->whdf.fbwidth = wnd->whdf.width = width;
		wnd->whdf.fbheight= wnd->whdf.height= height;
		logtoall("w=%d,h=%d\n", width, height);

		fullwindow_create(wnd, 0, 0, 0);
		break;
	}//default
	}//switch
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
	while(0==height)checkevent();
	fusion_easyag = artery_create(_easyag_, 0, 0, 0);
	//fusion = artery_create(_mahony_, 0, 0, 0);
	fusion_madgwick = artery_create(_madgwick_, 0, 0, 0);
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
		logtoall("eglCreateContext failed with error 0x%04x\n", eglGetError());
		return;
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		logtoall("eglMakeCurrent failed with error 0x%04x\n", eglGetError());
		return;
	}

	logtoall("GL Version = %s\n", glGetString(GL_VERSION));
	logtoall("GL Vendor = %s\n", glGetString(GL_VENDOR));
	logtoall("GL Renderer = %s\n", glGetString(GL_RENDERER));
	logtoall("GL Extensions = %s\n", glGetString(GL_EXTENSIONS));

	if(thewnd)thewnd->gl41list.ctxage += 1;
	candraw = 1;
}
void closewindow(struct android_app* theapp)
{
	//0. set flag
	candraw = 0;

	//1. gpu clear
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
/*
	//2. tell everyone who connected
	int j;
	_obj* wnd = thewnd;
	if(0 == wnd)return;

	struct gl41world* world = wnd->gl41list.world;
	//if(0 == world)return;

	struct gl41data** cam = world->camera;
	for(j=0;j<64;j++){
		if(cam[j])bzero(&cam[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** lit = world->light;
	for(j=0;j<64;j++){
		if(lit[j])bzero(&lit[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** solid = world->solid;
	for(j=0;j<64;j++){
		if(solid[j])bzero(&solid[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** opaque = world->opaque;
	for(j=0;j<64;j++){
		if(opaque[j])bzero(&opaque[j]->dst, sizeof(struct gldst));
	}*/
}




void sendtowindow_sensor(int k, float* v)
{
	//logtoall("%c: %f,%f,%f\n", k, v[0],v[1],v[2]);
	switch(k){
	case 'g':
		sensor[0][0] = v[0];
		sensor[0][1] = v[1];
		sensor[0][2] = v[2];
		age[0] = 1;
		break;		//send packet
	case 'a':
		sensor[1][0] = -v[0];
		sensor[1][1] = -v[1];
		sensor[1][2] = -v[2];
		age[1] = 1;
		return;		//record only
	case 'm':
		//logtoall("%f,%f,%f\n", v[0],v[1],v[2]);
		sensor[2][0] = v[0];
		sensor[2][1] = v[1];
		sensor[2][2] = v[2];
		age[2] = 1;
		return;		//record only
	}
	if(0 == age[0])return;
	if(0 == age[1])return;

	vec4 quaternion;
	if(0 == age[2]){
		if(0 == fusion_easyag)return;
		easyag_modify(fusion_easyag,0, sensor,0, quaternion,0);
	}
	else{
		//mahony_modify(fusion,0, sensor,0, quaternion,0);
		if(0 == fusion_madgwick)return;
		madgwick_modify(fusion_madgwick,0, sensor,0, quaternion,0);
	}
	//logtoall("q=%f,%f,%f,%f\n",quaternion[0],quaternion[1],quaternion[2],quaternion[3]);

	struct halfrel st[32];
	window_give(thewnd,0, st,0, 0,_quat_, quaternion,6);
}
void sendtowindow_touch(void* ev)
{
	struct halfrel st[32];
	window_give(thewnd,0, st,0, 0,0, ev,0);
}