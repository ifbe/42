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




void nonewindow_write(struct arena* win, struct event* ev)
{
	short* t;
	if(0x2b70 == ev->what){
		t = (void*)ev;
		win->x0 = t[0];
		win->y0 = t[1];
		//say("%d,%d\n",t[0],t[1]);
	}
}
void nonewindow_read(struct arena* win)
{
	float w = win->width;
	float h = win->height;
	float fbw = win->fbwidth;
	float fbh = win->fbheight;

	float x = win->x0;
	float y = h-1 - win->y0;
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
void nonewindow_delete(struct arena* win)
{
}
void nonewindow_create(struct arena* win)
{
	win->fmt = _none_;
	win->x0 = win->width / 2;
	win->y0 = win->height / 2;
}
