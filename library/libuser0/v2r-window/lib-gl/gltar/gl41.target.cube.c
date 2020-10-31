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




int gl41fbo6_take(_sup* this,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	//say("@gl41fbo6_read: %llx\n", self->pchip);

	if(0 == this->fbo){
		this->width = this->fbwidth = 1024;
		this->height = this->fbheight = 1024;
		fbocreate(this, '6');
	}
	return 0;
}
int gl41fbo6_give(_sup* this,void* foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	//say("@gl41fbo6_write: %llx\n", self->pchip);

	if(0 == this->fbo){
		this->width = this->fbwidth = 1024;
		this->height = this->fbheight = 1024;
		fbocreate(this, '6');
	}
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	struct supply* wnd = stack[sp-8].pchip;
	struct fstyle* area = stack[sp-1].pfoot;
	fullwindow_upload(this->glfull_camera, this->glfull_light, wnd->glfull_solid, wnd->glfull_opaque);
	fullwindow_render(this->glfull_camera, this->glfull_light, wnd->glfull_solid, wnd->glfull_opaque, this, area);
	return 0;
}
int gl41fbo6_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41fbo6_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41fbo6_delete(struct entity* act)
{
	return 0;
}
int gl41fbo6_create(struct entity* act, void* addr)
{
	act->glfull_camera = memorycreate(0x10000, 0);
	act->glfull_light  = memorycreate(0x10000, 0);
	return 0;
}
