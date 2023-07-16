#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libuser.h"

#ifdef __ANDROID__
	#include <jni.h>
	#include <errno.h>
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES3/gl3.h>
	#include <GLES3/gl3ext.h>
	#include <android/log.h>
	#include <android_native_app_glue.h>
#else
	#include <GL/glew.h>
#endif




void nonewindow_take(_obj* win,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	float w = win->whdf.width;
	float h = win->whdf.height;
	float fbw = win->whdf.fbwidth;
	float fbh = win->whdf.fbheight;

	float x = win->whdf.ix0;
	float y = h-1 - win->whdf.iy0;
	float r = x / w;
	float g = y / h;
	float b = 0.0;
	float a = 0.0;

	//fbw != w, fbh != h, so ...
	x *= fbw / w;
	y *= fbh / h;

	//viewport can not change clearcolor area
	glViewport(0, 0, x, y);

	//but scissor can ...
	glScissor(0, 0, x, y);
	glEnable(GL_SCISSOR_TEST);

	//clear screen
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT);	//GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT
}
void nonewindow_give(_obj* win,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	struct event* ev = buf;
	if(0x2b70 == ev->what){
		short* t = (void*)ev;
		win->whdf.ix0 = t[0];
		win->whdf.iy0 = t[1];
		//say("%d,%d\n",t[0],t[1]);
	}
}
void nonewindow_delete(_obj* win)
{
}
void nonewindow_create(_obj* win)
{
	win->hfmt = _gl41none_;
	win->whdf.ix0 = win->whdf.width / 2;
	win->whdf.iy0 = win->whdf.height / 2;
}
