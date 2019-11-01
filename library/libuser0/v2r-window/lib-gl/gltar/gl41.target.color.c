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
void fullwindow_upload(struct arena* ogl, struct actor* ctx);
void fullwindow_render(struct arena* ogl, int tmp, struct halfrel* src, struct halfrel* dst);




int gl41fboc_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	struct arena* ogl;
	struct arena* wnd;
	struct actor* data;
	struct relation* rel;
	say("@gl41fboc: %llx\n", self->pchip);

	ogl = stack[rsp-2]->pchip;
	wnd = stack[rsp-1]->pchip;
	if(0 == wnd->fbo){
		wnd->width = wnd->fbwidth = 1024;
		wnd->height = wnd->fbheight = 1024;
		fbocreate(wnd, 'c');
	}

	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		data = (void*)(rel->dstchip);
		if(_gl41data_ == data->type){
			glBindFramebuffer(GL_FRAMEBUFFER, wnd->fbo);
			fullwindow_render(ogl, 0, (void*)(rel->src), (void*)(rel->dst));
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int gl41fboc_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	return 0;
}
int gl41fboc_stop(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41fboc_start(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41fboc_delete(struct actor* act)
{
	return 0;
}
int gl41fboc_create(struct actor* act, void* addr)
{
	return 0;
}
