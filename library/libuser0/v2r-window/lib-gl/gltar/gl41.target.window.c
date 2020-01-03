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
int fullwindow_upload(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque);
int fullwindow_render(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque, struct supply* wnd, struct fstyle* area);




//stack[rsp-2] = ogl, 0
//stack[rsp-1] = wnd, 0
int gl41wnd0_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct supply* wnd;
	struct fstyle* sty;
	struct relation* rel;
	//say("@gl41wnd0_read\n");
	//say("%d,%llx@fullwindow_renderwnd\n", rsp, stack);

	wnd = self->pchip;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, wnd->fbwidth, wnd->fbheight);
	glScissor(0, 0, wnd->fbwidth, wnd->fbheight);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			fullwindow_render(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque, wnd, sty);
		}

		rel = samesrcnextdst(rel);
	}
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
	act->gl_camera = memorycreate(0x10000, 0);
	act->gl_light  = memorycreate(0x10000, 0);
	act->gl_solid  = memorycreate(0x10000, 0);
	act->gl_opaque = memorycreate(0x10000, 0);
	return 0;
}
