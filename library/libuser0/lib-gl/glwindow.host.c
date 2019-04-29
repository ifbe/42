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

GLuint shaderprogram(void* v, void* f, void* g, void* tc, void* te, void* c);
GLuint uploadtexture(void* i, u32 t, void* buf, int fmt, int w, int h);
GLuint uploadvertex(void* i, void* o);
//
void* allocstyle();
void* hostviewport_create(void*);
void* vbonode_create(u64, u64);
void hostviewport_render(struct arena*  vp, struct style* aa, struct arena* win, struct style* st);




void update_eachpass(struct gldst* dst, struct glsrc* src)
{
	int j;
	u32 fd;
	int w,h,fmt;
	void* buf0;
	void* buf1;
	void* buf2;
	//say("%llx,%llx\n", dst, src);
//say("@update shader\n");
	//0: shader
	if(dst->shader_deq != src->shader_enq)
	{
		//say("@1\n");
		buf0 = (void*)(src->vs);
		buf1 = (void*)(src->fs);
		buf2 = (void*)(src->gs);
		if((0 != buf0)&&(0 != buf1))
		{
			fd = shaderprogram(buf0, buf1, buf2, 0, 0, 0);

			dst->shader = fd;
			//say("shader:(%llx,%llx,%llx)->%x\n", buf0, buf1, buf2, fd);
		}

		dst->shader_deq = src->shader_enq;
	}

//say("@update vertex\n");
	//2: vertex
	if(	(dst->vbo_deq != src->vbuf_enq) |
		(dst->ibo_deq != src->ibuf_enq) )
	{
		//say("@4\n");
		uploadvertex(dst, src);
		//say("vertex:(%x,%x,%x)\n", dst->vao, dst->vbo, dst->ibo);
		dst->vbo_deq = src->vbuf_enq;
		dst->ibo_deq = src->ibuf_enq;
	}

//say("@update texture\n");
	//3: texture
	for(j=0;j<4;j++){
		if(dst->tex_deq[j] == src->tex_enq[j])continue;

		buf0 = (void*)(src->tex_data[j]);
		if(0 != buf0)
		{
			fmt = src->tex_fmt[j];
			w = src->tex_w[j];
			h = src->tex_h[j];
			fd = uploadtexture(dst, dst->tex[j], buf0, fmt, w, h);

			dst->tex[j] = fd;
			//say("texture:(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, fd);
		}

		dst->tex_deq[j] = src->tex_enq[j];
	}
//say("@update done\n");
}
void hostwindow_update(struct arena* win)
{
	int j;
	struct datapair* mod;
//say("@hostctx_update.start\n");

	//solid
	mod = win->gl_solid;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		//say("%d\n",j);
		update_eachpass(&mod[j].dst, &mod[j].src);
	}

	//opaque
	mod = win->gl_opaque;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		//say("%d\n",j);
		update_eachpass(&mod[j].dst, &mod[j].src);
	}

//say("@hostctx_update.end\n");
}




void hostwindow_render(struct arena* win)
{
	struct relation* rel;
	struct arena* vp;
	struct style* st;

	//glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	//prepare
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glPointSize(2.0 * win->fbwidth / win->width);
	//glLineWidth(8);

	rel = win->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype){
			vp = (void*)(rel->dstchip);
			st = (void*)(rel->srcfoot);
			hostviewport_render(vp, 0, win, st);
		}

		rel = samesrcnextdst(rel);
	}
}




void viewportvertex1(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] =-0.8290;
	win->camera.vb[2] =-0.55919290;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 0.8290;
	win->camera.vu[2] = 0.55919290;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = 0.55919290;
	win->camera.vn[2] =-0.8290;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] =-2000.0 * 0.55919290;	//sin(34)
	win->camera.vc[2] = 2000.0 * 0.8290;		//cos(34)
}
void viewportvertex2(struct arena* win)
{
	//target
	win->target.vc[0] = 0.0;
	win->target.vc[1] = 0.0;
	win->target.vc[2] = 0.0;

	win->target.vr[0] = 500.0;
	win->target.vr[1] = 0.0;
	win->target.vr[2] = 0.0;

	win->target.vf[0] = 0.0;
	win->target.vf[1] = 500.0;
	win->target.vf[2] = 0.0;

	win->target.vu[0] = 0.0;
	win->target.vu[1] = 0.0;
	win->target.vu[2] = 500.0;

	//camera
	win->camera.vl[0] = -1.0;
	win->camera.vl[1] = 0.0;
	win->camera.vl[2] = 0.0;

	win->camera.vr[0] = 1.0;
	win->camera.vr[1] = 0.0;
	win->camera.vr[2] = 0.0;

	win->camera.vb[0] = 0.0;
	win->camera.vb[1] = -1.0;
	win->camera.vb[2] = 0.0;

	win->camera.vu[0] = 0.0;
	win->camera.vu[1] = 1.0;
	win->camera.vu[2] = 0.0;

	win->camera.vn[0] = 0.0;
	win->camera.vn[1] = 0.0;
	win->camera.vn[2] = -1.0;
/*
	win->camera.vf[0] = 0.0;
	win->camera.vf[1] = 0.0;
	win->camera.vf[2] = 0.0;

	win->camera.vq[0] = 0.0;
	win->camera.vq[1] = 0.0;
	win->camera.vq[2] = 0.0;
*/
	win->camera.vc[0] = 0.0;
	win->camera.vc[1] = 0.0;
	win->camera.vc[2] = 100000.0;
}
void hostwindow_create(struct arena* window)
{
	struct arena* viewport1;
	struct style* sty1;
	struct arena* world1;

	viewport1 = hostviewport_create(window);
	if(viewport1)
	{
		sty1 = allocstyle();
		sty1->vc[0] = 0.0;
		sty1->vc[1] = 0.0;
		sty1->vq[0] = 0.5;
		sty1->vq[1] = 1.0;

		viewportvertex1(viewport1);
		relationcreate(viewport1, 0, _win_, window, sty1, _win_);
	}

	world1 = vbonode_create(_vbo_, _3d_);
	if(world1){
		world1->win = window->win;
		relationcreate(world1, 0, _win_, viewport1, 0, _win_);
	}


	struct arena* viewport2;
	struct style* sty2;
	struct arena* world2;

	viewport2 = hostviewport_create(window);
	if(viewport2)
	{
		sty2 = allocstyle();
		sty2->vc[0] = 0.0;
		sty2->vc[1] = 0.0;
		sty2->vq[0] = 1.0;
		sty2->vq[1] = 1.0;

		viewportvertex2(viewport2);
		relationcreate(viewport2, 0, _win_, window, sty2, _win_);
	}

	world2 = vbonode_create(_vbo_, _ui_);
	if(world2){
		world2->win = window->win;
		relationcreate(world2, 0, _win_, viewport2, 0, _win_);
	}
}