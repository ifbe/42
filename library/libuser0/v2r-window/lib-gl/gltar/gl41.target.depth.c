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




int gl41fbod_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct supply* wnd;
	struct supply* fbo;
	struct fstyle* sty;
	//say("@gl41fboc: %llx\n", self->pchip);

	fbo = self->pchip;
	sty = self->pfoot;
	if(0 == fbo->fbo){
		fbo->width = fbo->fbwidth = 1024;
		fbo->height = fbo->fbheight = 1024;
		fbocreate(fbo, 'd');
	}

	wnd = stack[rsp-4]->pchip;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo->fbo);
	fullwindow_render(fbo->gl_camera, fbo->gl_light, wnd->gl_solid, wnd->gl_opaque, fbo, sty);
    return 0;
}
int gl41fbod_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
    return 0;
}
int gl41fbod_stop(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}
int gl41fbod_start(struct halfrel* self, struct halfrel* peer)
{
    return 0;
}
int gl41fbod_delete(struct entity* act)
{
    return 0;
}
int gl41fbod_create(struct entity* act, void* addr)
{
	int j;
	u8* buf;

	buf = act->gl_camera = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	buf = act->gl_light = memorycreate(0x10000, 0);
	for(j=0;j<0x10000;j++)buf[j] = 0;

    return 0;
}
