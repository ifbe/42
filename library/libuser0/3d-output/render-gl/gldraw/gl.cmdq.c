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




void cmdqwindow_take(_sup* wnd,void* foot, _syn* stack,int sp, void* arg,int cmd, void* buf,int len)
{
	float w = wnd->width;
	float h = wnd->height;
	float fbw = wnd->fbwidth;
	float fbh = wnd->fbheight;

	float x = wnd->ix0;
	float y = h-1 - wnd->iy0;
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

	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		entity_take(rel->pdstchip,rel->dst, stack,sp+2, arg,cmd, 0,0);

		rel = samesrcnextdst(rel);
	}
}
void cmdqwindow_give(_sup* win,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	struct event* ev = buf;
	if(0x2b70 == ev->what){
		short* t = (void*)ev;
		win->ix0 = t[0];
		win->iy0 = t[1];
		//say("%d,%d\n",t[0],t[1]);
	}
}
void cmdqwindow_delete(struct supply* win)
{
}
void cmdqwindow_create(struct supply* win)
{
	win->fmt = _gl41cmdq_;
	win->ix0 = win->width / 2;
	win->iy0 = win->height / 2;
}
