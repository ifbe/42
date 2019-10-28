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
void fullwindow_upload(struct arena* ogl, struct actor* ctx);
void fullwindow_render(struct arena* ogl, int tmp, struct halfrel* src, struct halfrel* dst);




int gl41wnd0_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//say("@gl41wnd0_read\n");
	struct arena* ogl;
	struct arena* wnd;
	struct actor* ctx;
	struct relation* rel;
	//say("%d,%llx@fullwindow_renderwnd\n", rsp, stack);

	ogl = stack[rsp-2]->pchip;
	wnd = stack[rsp-1]->pchip;

	//
	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		ctx = rel->pdstchip;
		if(_gl41data_ == ctx->type){
			//read ctx
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			actorread(stack[rsp+1], stack[rsp+0], stack, rsp+2, 0, 0);

			//upload ctx
			fullwindow_upload(ogl, ctx);

			//render all
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			fullwindow_render(ogl, 0, stack[rsp+0], stack[rsp+1]);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int gl41wnd0_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	float x,y,x0,y0,xn,yn;
	short* v;
	struct actor* wnd;
	struct fstyle* sty;
	struct relation* rel;
	struct event* ev;

	ev = buf;
	//say("@gl41wnd0_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);

	wnd = self->pchip;
	if(0 == wnd)return 0;

	if('p' == (ev->what&0xff)){
		rel = wnd->oreln;
		if(0 == rel)return 0;

		while(1){
			if(0 == rel)return 0;
			sty = rel->psrcfoot;
			x0 = sty->vc[0] * wnd->width;
			y0 = sty->vc[1] * wnd->height;
			xn = sty->vq[0] * wnd->width + x0;
			yn = sty->vq[1] * wnd->height + y0;

			v = (short*)ev;
			x = v[0];
			y = wnd->height-v[1];
			if( (x>x0) && (x<xn) && (y>y0) && (y<yn) )goto found;
			rel = samesrcprevdst(rel);
		}
		return 0;
	}
	else{
		rel = wnd->buf0;
		if(0 == rel)rel = wnd->orel0;
		if(0 == rel)return 0;
	}

found:
	wnd->buf0 = rel;
	stack[rsp+0] = (void*)(rel->src);
	stack[rsp+1] = (void*)(rel->dst);
	actorwrite(stack[rsp+1], stack[rsp+0], stack, rsp+2, buf, len);
	return 0;
}
int gl41wnd0_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41wnd0_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41wnd0_delete(struct actor* act)
{
	return 0;
}
int gl41wnd0_create(struct actor* act, void* data)
{
	return 0;
}
