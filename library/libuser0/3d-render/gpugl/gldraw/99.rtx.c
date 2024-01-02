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




#define _clr_ hex32('c','l','r',0)
void cmdqwindow_execute(_obj* wnd)
{
	int j;
	float w = wnd->whdf.fbwidth;
	float h = wnd->whdf.fbheight;
	u64* data = wnd->gl41cmdq.data;
	u64* code = wnd->gl41cmdq.code;
	for(j=0;j<1;j++){
		say("code[0] = %.4s\n", &code[0]);
		switch(code[0]){
		case _clr_:

			//viewport can not change clearcolor area
			glViewport(0, 0, w, h);

			//but scissor can ...
			glScissor(0, 0, w, h);
			glEnable(GL_SCISSOR_TEST);

			//clear screen
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			break;
		default:
			break;
		}//switch

	}//for
}




void cmdqwindow_take(_obj* wnd,void* foot, _syn* stack,int sp, p64 arg,int cmd, void* buf,int len)
{
	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		entity_takeby(rel->pdstchip,rel->dst, stack,sp+2, arg,cmd, 0,0);

		rel = samesrcnextdst(rel);
	}

	cmdqwindow_execute(wnd);
}
void cmdqwindow_give(_obj* win,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
}
void cmdqwindow_delete(_obj* ogl)
{
}
void cmdqwindow_create(_obj* ogl)
{
	ogl->hfmt = _gl41cmdq_;
	ogl->vfmt = _gl41cmdq_;

	ogl->gl41cmdq.data = memoryalloc(0x100000, 0);
	ogl->gl41cmdq.code = memoryalloc(0x100000, 0);
}
