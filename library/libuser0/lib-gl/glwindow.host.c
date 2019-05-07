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
void fixmatrix(float*, void*);




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




void display_eachpass(
	struct gldst* dst, struct glsrc* src,
	struct arena* win, float* cammvp)
{
	int j;
	u32 tmp;
	u32 fmt;
	u32 vbo;
	u32 vao;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	//0.shader
	glUseProgram(dst->shader);

	//1.argument
	glUniformMatrix4fv(glGetUniformLocation(dst->shader, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform3fv(glGetUniformLocation(dst->shader, "camxyz"  ), 1, win->camera.vc);
	//glUniformMatrix4fv(glGetUniformLocation(dst->shader, "sunmvp"), 1, GL_FALSE, cammvp);
	//glUniform3fv(glGetUniformLocation(dst->shader, "sunxyz"  ), 1, win->camera.vc);
	for(j=0;j<4;j++){
		if(0 == src->arg_name[j])continue;
		if(0 == src->arg_data[j])continue;

		tmp = glGetUniformLocation(dst->shader, src->arg_name[j]);
		switch(src->arg_fmt[j]){
			case 'm':{
				glUniformMatrix4fv(tmp, 1, GL_FALSE, src->arg_data[j]);
				break;
			}//mat4

			case 'f':{
				glUniform1fv(tmp, 1, src->arg_data[j]);
				break;
			}
		}//switch
	}//for

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
void hostviewport_render(
	struct arena* ctx, struct style* aa,
	struct arena* win, struct style* st)
{
	int j;
	struct datapair* mod;

	float cammvp[4*4];
	float w,h,x0,y0,x1,y1;

	w = win->fbwidth;
	h = win->fbheight;
	x0 = w * st->vc[0];	//0
	y0 = h * st->vc[1];	//0
	x1 = w * st->vq[0];	//0.5
	y1 = h * st->vq[1];	//1

	//
	glViewport(x0, y0, x1, y1);


	//global arg: camera, light
	fixmatrix(cammvp, ctx);
	mat4_transpose((void*)cammvp);


	//solid
	mod = ctx->gl_solid;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(&mod[j].dst, &mod[j].src, ctx, cammvp);
	}


	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	mod = ctx->gl_opaque;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(&mod[j].dst, &mod[j].src, ctx, cammvp);
	}

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void hostwindow_render(struct arena* win)
{
	struct relation* rel;
	struct arena* ctx;
	struct style* st;
	struct style* s1;

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
			st = (void*)(rel->srcfoot);
			s1 = (void*)(rel->dstfoot);

			ctx = (void*)(rel->dstchip);
			hostviewport_render(ctx, s1, win, st);
		}

		rel = samesrcnextdst(rel);
	}
}