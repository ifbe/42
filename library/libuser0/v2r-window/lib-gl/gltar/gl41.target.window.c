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
int fullwindow_upload(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque);
int fullwindow_render(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque, struct supply* wnd, struct fstyle* area);




//stack[rsp-2] = ogl, 0
//stack[rsp-1] = wnd, 0
int gl41wnd0_read(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	//say("@gl41wnd0_read\n");
	//say("%d,%llx@fullwindow_renderwnd\n", rsp, stack);
	//say("gl41wnd0_read:%llx,%llx,%llx,%x,%llx,%d\n",self,peer,stack,rsp,buf,len);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, wnd->fbwidth, wnd->fbheight);
	glScissor(0, 0, wnd->fbwidth, wnd->fbheight);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//foreach camera
	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		//wnd = rel->psrcchip;		//double check
		struct fstyle* area = rel->psrcfoot;
		if(area){
			//get vertex
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			//stack[sp+0].type = rel->srctype;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			//stack[sp+1].type = rel->dsttype;
			stack[sp+1].flag = rel->dstflag;
			entityread(rel->pdstchip, rel->dstflag, stack,sp+2, 0,'v', 0, 0);

			//upload
			fullwindow_upload(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque);

			//render
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			fullwindow_render(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque, wnd, area);
		}

		rel = samesrcnextdst(rel);
	}
	return 0;
}
int gl41wnd0_write(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	float x,y,x0,y0,xn,yn;
	short* v;
	struct relation* rel;
	struct fstyle* sty;
	//say("@gl41wnd0_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);

	struct event* ev = buf;
	if(0x4070 == ev->what){
		rel = wnd->oreln;
		while(1){
			if(0 == rel)return 0;
			sty = rel->psrcfoot;
			x0 = sty->vc[0] * wnd->width;
			y0 = sty->vc[1] * wnd->height;
			xn = sty->vq[0] * wnd->width + x0;
			yn = sty->vq[1] * wnd->height + y0;

			v = (short*)ev;
			x = v[0];
			y = (wnd->height-1) - v[1];
			if( (x>x0) && (x<xn) && (y>y0) && (y<yn) )goto found;
			rel = samesrcprevdst(rel);
		}
		return 0;
	}
	else{
		rel = wnd->glevto;
		if(0 == rel)rel = wnd->oreln;
		if(0 == rel)return 0;
	}

found:
	wnd->glevto = rel;
	stack[sp+0].pchip = rel->psrcchip;
	stack[sp+0].pfoot = rel->psrcfoot;
	stack[sp+0].flag = rel->srcflag;

	stack[sp+1].pchip = rel->pdstchip;
	stack[sp+1].pfoot = rel->pdstfoot;
	stack[sp+1].flag = rel->dstflag;
	entitywrite(rel->pdstchip, rel->dstflag, stack,sp+2, 0,0, ev,0);
	return 0;
}
int gl41wnd0_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int gl41wnd0_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int gl41wnd0_search(struct supply* act)
{
	return 0;
}
int gl41wnd0_modify(struct supply* act)
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
