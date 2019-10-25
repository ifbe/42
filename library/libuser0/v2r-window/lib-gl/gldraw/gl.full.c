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
int fbocreate(void*, int);



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
		if(dst->tex_deq[j] == src->tex[j].enq)continue;

		buf0 = (void*)(src->tex[j].data);
		if(0 != buf0)
		{
			fmt = src->tex[j].fmt;
			if('!' == fmt){
				dst->tex[j] = src->tex[j].glfd;
			}
			else{
				w = src->tex[j].w;
				h = src->tex[j].h;
				dst->tex[j] = uploadtexture(dst, dst->tex[j], buf0, fmt, w, h);
			}
			//say("texture:(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, fd);
		}

		dst->tex_deq[j] = src->tex[j].enq;
	}
//say("@update done\n");
}
void fullwindow_upload(struct arena* ogl, struct actor* ctx)
{
	int j;
	struct datapair* mod;
	//say("@fullwindow_upload: %llx,%llx,%.8s\n", ogl, ctx, &ctx->type);

	//light
	mod = ctx->gl_light;
	update_onedraw(&mod[0].dst, &mod[0].src);

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




void updatearg(u32 shader, struct glsrc* src)
{
	int j;
	int iii;
	u32 uuu;

	if((src->routine_name) && (src->routine_detail)){
		//iii = glGetSubroutineUniformLocation(shader, GL_FRAGMENT_SHADER, src->routine_name);
		uuu = glGetSubroutineIndex(shader, GL_FRAGMENT_SHADER, src->routine_detail);
		if(uuu < 256)glUniformSubroutinesuiv(GL_FRAGMENT_SHADER, 1, &uuu);
	}

	for(j=0;j<4;j++){
		if(0 == src->arg[j].name)break;
		if(0 == src->arg[j].data)break;
//say("%d,%d,%llx,%s\n", j, shader, src, src->arg[j].name);

		iii = glGetUniformLocation(shader, src->arg[j].name);
		if(iii < 0)continue;

		switch(src->arg[j].fmt){
			case 'm':{
				glUniformMatrix4fv(iii, 1, GL_FALSE, src->arg[j].data);
				break;
			}//mat4
			case 'v':{
				glUniform3fv(iii, 1, src->arg[j].data);
				break;
			}//vertex
			case 'f':{
				glUniform1fv(iii, 1, src->arg[j].data);
				break;
			}//float
		}//switch
	}//for
}
void render_onedraw(struct datapair* cam, struct datapair* lit, struct datapair* data)
{
	int j,k;
	u32 fmt;
	u32 vbo;
	u32 vao;
	struct gldst* dst = &data->dst;
	struct glsrc* src = &data->src;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	//0.shader
	glUseProgram(dst->shader);

	//1.argument
	updatearg(dst->shader, src);
	updatearg(dst->shader, &lit->src);
	updatearg(dst->shader, &cam->src);

	//2.texture
	k = 0;
	for(j=0;j<4;j++){
		if(0 == lit->src.tex[j].name)continue;
		if(0 == lit->src.tex[j].data)continue;
		glActiveTexture(GL_TEXTURE0 + k);
		glBindTexture(GL_TEXTURE_2D, lit->src.tex[j].glfd);
		glUniform1i(glGetUniformLocation(dst->shader, lit->src.tex[j].name), k);
		k++;
	}
	for(j=0;j<4;j++)
	{
		if(0 == dst->tex[j])continue;
		if(0 == src->tex[j].name)continue;
//say("tex=%x\n", dst->tex[j]);
		glActiveTexture(GL_TEXTURE0 + k);
		glBindTexture(GL_TEXTURE_2D, dst->tex[j]);
		glUniform1i(glGetUniformLocation(dst->shader, src->tex[j].name), k);
		k++;
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
void fullwindow_render(struct arena* ogl, int tmp, struct halfrel* src, struct halfrel* dst)
{
	int j;
	int x0,y0,ww,hh;
	struct actor* wnd = src->pchip;
	struct fstyle* area = src->pfoot;
	struct actor* ctx = dst->pchip;
	struct fstyle* frus = dst->pfoot;

	struct datapair* cam = ctx->gl_camera;
	struct datapair* lit = ctx->gl_light;
	struct datapair* solid = ctx->gl_solid;
	struct datapair* opaque = ctx->gl_opaque;
	//say("@fullwindow_render: %.4s, %.4s\n", &src->flag, &dst->flag);
	//say("@fullwindow_render: %llx,%llx,%llx,%llx\n", ctx->gl_camera, ctx->gl_light, ctx->gl_solid, ctx->gl_opaque);


	//
	x0 = area->vc[0] * wnd->fbwidth;
	y0 = area->vc[1] * wnd->fbheight;
	ww = area->vq[0] * wnd->fbwidth;
	hh = area->vq[1] * wnd->fbheight;
	//say("%d,%d,%d,%d\n", x0, y0, ww, hh);
	glViewport(x0, y0, ww, hh);
	glScissor(x0, y0, ww, hh);

	//
	glEnable(GL_SCISSOR_TEST);
	glEnable(GL_DEPTH_TEST);
	glPointSize(4.0);
	glClearColor(0.1, 0.1, 0.1, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//solid
	for(j=0;j<64;j++){
		if(0 == solid[j].src.vbuf)continue;
		render_onedraw(cam, lit, &solid[j]);
	}

	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(j=0;j<64;j++){
		if(0 == opaque[j].src.vbuf)continue;
		render_onedraw(cam, lit, &opaque[j]);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}




void fullwindow_renderfbod(struct arena* opengl, struct actor* target)
{
	struct actor* data;
	struct relation* rel;
	//say("@gl41fbod\n");

	if(0 == target->fbo){
		target->width = target->fbwidth = 1024;
		target->height = target->fbheight = 1024;
		fbocreate(target, 'd');
	}
	glBindFramebuffer(GL_FRAMEBUFFER, target->fbo);

	rel = target->orel0;
	while(1){
		if(0 == rel)break;

		data = (void*)(rel->dstchip);
		if(_gl41data_ == data->type){
			fullwindow_render(opengl, 0, (void*)(rel->src), (void*)(rel->dst));
		}

		rel = samesrcnextdst(rel);
	}
}
void fullwindow_renderfboc(struct arena* opengl, struct actor* target)
{
	struct actor* data;
	struct relation* rel;
	//say("@gl41fboc\n");

	if(0 == target->fbo){
		target->width = target->fbwidth = 1024;
		target->height = target->fbheight = 1024;
		fbocreate(target, 'c');
	}
	glBindFramebuffer(GL_FRAMEBUFFER, target->fbo);

	rel = target->orel0;
	while(1){
		if(0 == rel)break;

		data = (void*)(rel->dstchip);
		if(_gl41data_ == data->type){
			fullwindow_render(opengl, 0, (void*)(rel->src), (void*)(rel->dst));
		}

		rel = samesrcnextdst(rel);
	}
}
void fullwindow_renderfbog(struct arena* opengl, struct actor* target)
{
	struct actor* data;
	struct relation* rel;
	//say("@gl41fbog\n");

	if(0 == target->fbo){
		target->width = target->fbwidth = 1024;
		target->height = target->fbheight = 1024;
		fbocreate(target, 'g');
	}
	glBindFramebuffer(GL_FRAMEBUFFER, target->fbo);

	rel = target->orel0;
	while(1){
		if(0 == rel)break;

		data = (void*)(rel->dstchip);
		if(_gl41data_ == data->type){
			fullwindow_render(opengl, 0, (void*)(rel->src), (void*)(rel->dst));
		}

		rel = samesrcnextdst(rel);
	}
}
void fullwindow_renderwnd(struct halfrel** stack, int rsp)
{
	struct arena* ogl;
	struct actor* wnd;
	struct actor* ctx;
	struct relation* rel;
	//say("%d,%llx@fullwindow_renderwnd\n", rsp, stack);

	ogl = stack[rsp-2]->pchip;
	wnd = stack[rsp-1]->pchip;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//
	rel = wnd->orel0;
	while(1){
		if(0 == rel)break;

		ctx = rel->pdstchip;
		if(_gl41data_ == ctx->type){
			//read ctx
			stack[rsp+0] = (void*)(rel->src);
			stack[rsp+1] = (void*)(rel->dst);
			actorread(stack[rsp+1], stack[rsp+0], stack, rsp+2, 0, 0);

			//upload ctx
			fullwindow_upload(ogl, ctx);

			//render all
			fullwindow_render(ogl, 0, stack[rsp+0], stack[rsp+1]);
		}

		rel = samesrcnextdst(rel);
	}
}




void fullwindow_write(struct arena* ogl, struct event* ev)
{
	//say("@fullwindow_write:%llx,%llx,%llx,%llx\n", ev->why, ev->what, ev->where, ev->when);
	relationwrite(ogl, _wnd_, 0, 0, ev, 0);
}
void fullwindow_read(struct arena* ogl)
{
	struct halfrel* stack[16];
	struct relation* rel;
	struct actor* act;
	//say("@fullwindow_read\n");

	rel = ogl->orel0;
	while(1){
		if(0 == rel)break;

		act = rel->pdstchip;
		//say("%.8s\n", &act->type);
		switch(act->type){
			case _gl41fboc_:fullwindow_renderfboc(ogl, act);break;
			case _gl41fbod_:fullwindow_renderfbod(ogl, act);break;
			case _gl41fbog_:fullwindow_renderfbog(ogl, act);break;
			case _gl41wnd0_:{
				act->width = ogl->width;
				act->height = ogl->height;
				act->fbwidth = ogl->fbwidth;
				act->fbheight = ogl->fbheight;

				stack[0+0] = (void*)(rel->src);
				stack[0+1] = (void*)(rel->dst);
				fullwindow_renderwnd(stack, 0+2);
				break;
			}
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
