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
int fullwindow_render(struct datapair* cam, struct datapair* lit, struct datapair* solid, struct datapair* opaque, struct supply* wnd, struct fstyle* area);




int gl41fbog_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct supply* wnd;
	struct fstyle* sty;
	struct relation* rel;
	//say("@gl41fbog\n");

	wnd = stack[rsp-1]->pchip;
	if(0 == wnd->fbo){
		wnd->width = wnd->fbwidth = 1024;
		wnd->height = wnd->fbheight = 1024;
		fbocreate(wnd, 'g');
	}

	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		sty = rel->psrcfoot;
		glBindFramebuffer(GL_FRAMEBUFFER, wnd->fbo);
		fullwindow_render(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque, wnd, sty);

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
	return 0;
}
