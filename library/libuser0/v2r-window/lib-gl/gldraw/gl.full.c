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



void update_onedraw(struct gldst* dst, struct glsrc* src)
{
	int j;
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
		//say("shader:%llx,%llx,%llx\n", buf0, buf1, buf2);
		if((0 != buf0)&&(0 != buf1))
		{
			dst->shader = shaderprogram(buf0, buf1, buf2, 0, 0, 0);
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
			dst->tex[j] = uploadtexture(dst, dst->tex[j], buf0, fmt, w, h);
			//say("texture:(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, fd);
		}

		dst->tex_deq[j] = src->tex_enq[j];
	}
//say("@update done\n");
}
void fullwindow_upload(struct arena* ogl, struct actor* ctx)
{
	int j;
	struct datapair* mod;
	//say("@fullwindow_upload: %llx,%llx,%.8s\n", ogl, ctx, &ctx->type);

	//solid
	mod = ctx->gl_solid;
	if(0 == mod)return;

	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		//say("%d\n",j);
		update_onedraw(&mod[j].dst, &mod[j].src);
	}

	//opaque
	mod = ctx->gl_opaque;
	if(0 == mod)return;

	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		//say("%d\n",j);
		update_onedraw(&mod[j].dst, &mod[j].src);
	}
}
void update_frustum(struct arena* ogl, struct fstyle* area, struct fstyle* frus)
{
	float w, h, x0, y0, dx, dy;
	w = ogl->fbwidth;
	h = ogl->fbheight;

	if(0 == area){
		x0 = 0;
		y0 = 0;
		dx = w;
		dy = h;
	}
	else{
		x0 = w * area->vc[0];
		y0 = h * area->vc[1];
		dx = w * area->vq[0];
		dy = h * area->vq[1];
	}
	frus->vb[3] =-dy / dx;
	frus->vt[3] = dy / dx;

	glViewport(x0, y0, dx, dy);
}




void updatearg(u32 shader, struct glsrc* src)
{
	int j;
	u32 tmp;

	for(j=0;j<4;j++){
		if(0 == src->arg_name[j])break;
		if(0 == src->arg_data[j])break;
//say("%d,%d,%llx,%s\n", j, shader, src, src->arg_name[j]);
		tmp = glGetUniformLocation(shader, src->arg_name[j]);
		switch(src->arg_fmt[j]){
			case 'm':{
				glUniformMatrix4fv(tmp, 1, GL_FALSE, src->arg_data[j]);
				break;
			}//mat4
			case 'v':{
				glUniform3fv(tmp, 1, src->arg_data[j]);
				break;
			}//vertex
			case 'f':{
				glUniform1fv(tmp, 1, src->arg_data[j]);
				break;
			}//float
		}//switch
	}//for
}
void render_onedraw(struct gldst* dst, struct glsrc* src, struct glsrc* cam)
{
	int j;
	u32 fmt;
	u32 vbo;
	u32 vao;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	//0.shader
	glUseProgram(dst->shader);

	//1.argument
	updatearg(dst->shader, src);
	updatearg(dst->shader, cam);

	//2.texture
	for(j=0;j<4;j++)
	{
		if(0 == dst->tex[j])continue;
		if(0 == src->tex_name[j])continue;

		glUniform1i(glGetUniformLocation(dst->shader, src->tex_name[j]), j);
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, dst->tex[j]);
	}

	//3.vertex
	fmt = src->vbuf_fmt;
	vbo = dst->vbo;
	vao = dst->vao;
	glBindVertexArray(vao);
//say("%d,%d,%d,%d,%d,%d\n", dst->shader, dst->tex[0], vao, vbo, src->ibuf_h, src->vbuf_h);

//say("8888@method=%x, geom=%x, vbuf_h=%x, ibuf_h=%x\n", src->method, src->geometry, src->vbuf_h, src->ibuf_h);
	if('i' == src->method)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
		if(1 == src->geometry)glDrawElements(GL_POINTS, src->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else if(2 == src->geometry)glDrawElements(GL_LINES, 2*src->ibuf_h, GL_UNSIGNED_SHORT, 0);
		else glDrawElements(GL_TRIANGLES, 3*src->ibuf_h, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		if(1 == src->geometry)glDrawArrays(GL_POINTS, 0, src->vbuf_h);
		else if(2 == src->geometry)glDrawArrays(GL_LINES, 0, src->vbuf_h);
		else glDrawArrays(GL_TRIANGLES, 0, src->vbuf_h);
	}
}
void fullwindow_eachpass(struct arena* ogl, struct actor* view)
{
	int j;
	struct datapair* mod;
	struct datapair* cam = view->gl_camera;
	struct datapair* lit = view->gl_light;
	struct datapair* solid = view->gl_solid;
	struct datapair* opaque = view->gl_opaque;
	//say("@fullwindow_render: %llx,%llx,%llx,%llx\n", view->gl_camera, view->gl_light, view->gl_solid, view->gl_opaque);


	//solid
	for(j=0;j<64;j++)
	{
		if(0 == solid[j].src.vbuf)continue;
		render_onedraw(&solid[j].dst, &solid[j].src, &cam[0].src);
	}


	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(j=0;j<64;j++)
	{
		if(0 == opaque[j].src.vbuf)continue;
		render_onedraw(&opaque[j].dst, &opaque[j].src, &cam[0].src);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void fullwindow_viewport(struct arena* ogl, struct actor* view)
{
	struct actor* tmp;
	struct fstyle* area;
	struct fstyle* frus;

	struct relation* rel;
	struct halfrel* self;
	struct halfrel* peer;

	//say("@fullwindow_viewport: %llx,%llx,%.8s\n", ogl, view, &view->type);
	if(0 == view)return;

	rel = view->orel0;
	while(1){
		if(0 == rel)break;

		tmp = (void*)(rel->dstchip);
		//say("=>%.8s\n", &tmp->type);
		switch(tmp->type){
			case _gl41data_:
			case _gl41coop_:{
				view->gl_camera = tmp->gl_camera;
				view->gl_light = tmp->gl_light;
				view->gl_solid = tmp->gl_solid;
				view->gl_opaque = tmp->gl_opaque;
				break;
			}
			default:{
				area = (void*)(rel->srcfoot);
				frus = (void*)(rel->dstfoot);
				update_frustum(ogl, area, frus);

				//get mvp
				self = (void*)(&rel->dstchip);
				peer = (void*)(&rel->srcchip);
				actorread(self, peer, 0, 0, 0, 0);

				//render
				fullwindow_eachpass(ogl, view);
				break;
			}
		}

		rel = samesrcnextdst(rel);
	}
}
void fullwindow_renderfboc(struct arena* ogl, struct actor* wnd)
{
	say("@gl41fboc\n");
}
void fullwindow_renderfbod(struct arena* ogl, struct actor* wnd)
{
	say("@gl41fbod\n");
}
void fullwindow_renderfbog(struct arena* ogl, struct actor* wnd)
{
	say("@gl41fbog\n");
}
void fullwindow_renderwnd(struct arena* ogl, struct actor* wnd)
{
	int cnt;
	struct relation* rel;
	struct actor* act;
	//say("@fullwindow_render: %llx,%llx,%.8s\n", ogl, wnd, &wnd->type);
	if(0 == wnd)return;

	cnt = 0;
	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		act = (void*)(rel->dstchip);
		//say("=>%.8s\n", &act->type);
		switch(act->type){
			case _gl41view_:cnt++;fullwindow_viewport(ogl, act);break;
		}

		rel = samesrcnextdst(rel);
	}

	if(0 == cnt)fullwindow_viewport(ogl, wnd);
}




void fullwindow_write(struct arena* ogl, struct event* ev)
{
	say("@fullwindow_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);
	relationwrite(ogl, _wnd_, 0, 0, ev, 0);
}
void fullwindow_read(struct arena* ogl)
{
	float w, h;
	struct relation* rel;
	struct actor* act;
	//say("@fullwindow_read\n");

	//clear screen
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glenable
	glEnable(GL_DEPTH_TEST);
	glPointSize(4.0);

	//0: upload data to gpu
	rel = ogl->orel0;
	while(1){
		if(0 == rel)break;

		act = (void*)(rel->dstchip);
		switch(act->type){
			case _gl41data_:fullwindow_upload(ogl, act);break;
			case _gl41fboc_:fullwindow_renderfboc(ogl, act);break;
			case _gl41fbod_:fullwindow_renderfboc(ogl, act);break;
			case _gl41fbog_:fullwindow_renderfboc(ogl, act);break;
			case _gl41wnd0_:fullwindow_renderwnd(ogl, act);break;
		}

		rel = samesrcnextdst(rel);
	}
}
void fullwindow_delete(struct arena* ogl)
{
}
void fullwindow_create(struct arena* ogl)
{
	ogl->fmt = _full_;
}
