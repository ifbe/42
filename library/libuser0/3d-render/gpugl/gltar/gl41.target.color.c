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
int fullwindow_render(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque, _obj* wnd, struct fstyle* area);




int gl41fboc_take(_obj* wnd,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	//logtoall("@gl41fboc_read: %llx\n", self->pchip);
/*
	if(0 == wnd->fbo){
		wnd->width = wnd->fbwidth = 1024;
		wnd->height = wnd->fbheight = 1024;
		fbocreate(wnd, 'c');
	}
	glBindFramebuffer(GL_FRAMEBUFFER, wnd->fbo);
	glViewport(0, 0, wnd->fbwidth, wnd->fbheight);
	glScissor(0, 0, wnd->fbwidth, wnd->fbheight);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//foreach camera:
	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		//wnd = rel->psrcchip;		//double check
		struct fstyle* area = rel->psrcfoot;
		if(area){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			//stack[sp+0].type = rel->srctype;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			//stack[sp+1].type = rel->dsttype;
			stack[sp+1].flag = rel->dstflag;
			entity_takeby(rel->pdstchip, rel->pdstfoot, stack,sp+2, 0,'v', 0, 0);

			//upload
			fullwindow_upload(wnd->gl41list.camera, wnd->gl41list.light, wnd->gl41list.solid, wnd->gl41list.opaque);

			//render
			glBindFramebuffer(GL_FRAMEBUFFER, wnd->fbo);
			fullwindow_render(wnd->gl41list.camera, wnd->gl41list.light, wnd->gl41list.solid, wnd->gl41list.opaque, wnd, area);
		}

		rel = samesrcnextdst(rel);
	}*/
	return 0;
}
int gl41fboc_give(_obj* this,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	//logtoall("@gl41fboc_write: %llx\n", self->pchip);
/*
	if(0 == this->fbo){
		this->width = this->fbwidth = 1024;
		this->height = this->fbheight = 1024;
		fbocreate(this, 'c');
	}
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	_obj* wnd = stack[sp-8].pchip;
	struct fstyle* area = stack[sp-1].pfoot;
	fullwindow_upload(this->gl41list.camera, this->gl41list.light, wnd->gl41list.solid, wnd->gl41list.opaque);
	fullwindow_render(this->gl41list.camera, this->gl41list.light, wnd->gl41list.solid, wnd->gl41list.opaque, this, area);*/
	return 0;
}
