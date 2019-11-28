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
int gl41data_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len);
int gl41data_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len);
//
int fullwindow_upload(struct datapair* cam, struct datapair* lit, struct datapair* solid, struct datapair* opaque);
int fullwindow_render(struct datapair* cam, struct datapair* lit, struct datapair* solid, struct datapair* opaque, struct supply* wnd, struct fstyle* area);




//stack[rsp-2] = ogl, 0
//stack[rsp-1] = wnd, 0
int gl41wnd0_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct supply* wnd;
	struct fstyle* sty;
	struct relation* rel;
	//say("@gl41wnd0_read\n");
	//say("%d,%llx@fullwindow_renderwnd\n", rsp, stack);

	//foreach camera: 
	wnd = self->pchip;
	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		//wnd = rel->psrcchip;		//double check
		sty = rel->psrcfoot;
		if(sty){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);

			//get vertex
			gl41data_read(stack[rsp-1], stack[rsp-2], stack, rsp+2, buf, 'v');

			//upload
			fullwindow_upload(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque);

			//get fbo tex, get cam mvp
			gl41data_read(stack[rsp-1], stack[rsp-2], stack, rsp+2, buf, '?');

			//render
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			fullwindow_render(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque, wnd, sty);
		}

		rel = samesrcnextdst(rel);
	}
/*
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
			entityread(stack[rsp+1], stack[rsp+0], stack, rsp+2, 0, 0);

			//upload ctx
			fullwindow_upload(ogl, ctx);

			//render all
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			fullwindow_render(ogl, 0, stack[rsp+0], stack[rsp+1]);
		}

		rel = samesrcnextdst(rel);
	}
*/
	return 0;
}
int gl41wnd0_write(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	gl41data_write(self, peer, stack, rsp, buf, len);
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
int gl41wnd0_delete(struct supply* act)
{
	return 0;
}
int gl41wnd0_create(struct supply* act, void* data)
{
	int j;
	u8* buf;

	buf = act->gl_camera = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_light = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_solid = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_opaque = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	return 0;
}
