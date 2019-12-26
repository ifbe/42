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
int fbocreate(void*, int);
int fullwindow_upload(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque);
int fullwindow_render(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque, struct supply* wnd, struct fstyle* area);




int gl41fbog_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	u64 tmp[16];
	struct supply* wnd;
	struct fstyle* sty;
	struct relation* rel;
//say("@gl41fbog\n");

	wnd = self->pchip;
	if(0 == wnd->fbo){
		wnd->width = wnd->fbwidth = 1024;
		wnd->height = wnd->fbheight = 1024;
		fbocreate(wnd, 'g');
	}

	glBindFramebuffer(GL_FRAMEBUFFER, wnd->fbo);
	glViewport(0, 0, wnd->fbwidth, wnd->fbheight);
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	stack = (void*)tmp;
	for(rsp=0;rsp<16;rsp++)stack[rsp] = 0;
	rsp = 0;

	//foreach camera:
	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		//wnd = rel->psrcchip;		//double check
		sty = rel->psrcfoot;
		if(sty){
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
//say("@read v\n");
			//get vertex
			entityread(stack[rsp+1], stack[rsp+0], stack, rsp+2, buf, 'v');
//say("@upload\n");
			//upload
			fullwindow_upload(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque);
//say("@read ?\n");
			//get fbo tex, get cam mvp
			entityread(stack[rsp+1], stack[rsp+0], stack, rsp+2, buf, '?');
//say("@render\n");
			//render
			glBindFramebuffer(GL_FRAMEBUFFER, wnd->fbo);
			fullwindow_render(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque, wnd, sty);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int gl41fbog_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gl41fbog_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41fbog_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41fbog_delete(struct entity* act)
{
	return 0;
}
int gl41fbog_create(struct entity* act, void* addr)
{
	act->gl_camera = memorycreate(0x10000, 0);
	act->gl_light  = memorycreate(0x10000, 0);
	act->gl_solid  = memorycreate(0x10000, 0);
	act->gl_opaque = memorycreate(0x10000, 0);
	return 0;
}
