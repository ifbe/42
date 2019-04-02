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

void fixmatrix(float*, void*);
GLuint shaderprogram(void* v, void* f);
GLuint uploadtexture(void* i, void* o, void* buf, int fmt, int w, int h);
GLuint uploadvertex(void* i, void* o);




void initobject(struct arena* win)
{
	int j;
	u8* buf = malloc(0x10000);

	for(j=0;j<0x10000;j++)buf[j] = 0;
	win->mod = buf;
}




int fbodelete(struct arena* win)
{
	if(win->tex_color)glDeleteTextures(1, &win->tex_color);
	if(win->tex_depth)glDeleteTextures(1, &win->tex_depth);
	if(win->rbo)glDeleteRenderbuffers(1, &win->rbo);
	if(win->fbo)glDeleteFramebuffers(1, &win->fbo);
	return 0;
}
int fbocreate(struct arena* win, char* arg)
{
	win->fbwidth = win->width = 1024;
	win->fbheight = win->height = 1024;
	win->fbdepth = win->depth = 1024;
	win->fbstride = win->stride = 1024;


	//frame buffer
	glGenFramebuffers(1, &win->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, win->fbo);

/*
	//render buffer?
	glGenRenderbuffers(1, &w->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, w->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, w->rbo);
*/

	//depth buffer
	glGenTextures(1, &win->tex_depth);
	glBindTexture(GL_TEXTURE_2D, win->tex_depth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, win->tex_depth, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, win->tex_depth, 0);
#endif



	//color buffer
	glGenTextures(1, &win->tex_color);
	glBindTexture(GL_TEXTURE_2D, win->tex_color);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, win->tex_color, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, win->tex_color, 0);
#endif

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers


	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		say("error@framebuffer!!!\n");
	}
	return 0;
}




void update_eachpass(struct gldst* dst, struct glsrc* src)
{
	u32 fd;
	int w,h,fmt;
	void* buf0;
	void* buf1;
	//say("%llx,%llx\n", dst, src);

	//0: shader
	if(dst->shader_deq != src->shader_enq[0])
	{
		//say("@1\n");
		buf0 = (void*)(src->vs);
		buf1 = (void*)(src->fs);
		if((0 != buf0)&&(0 != buf1))
		{
			fd = shaderprogram(buf0, buf1);

			dst->shader = fd;
			say("(%llx,%llx)->%x\n", buf0, buf1, fd);
		}

		dst->shader_deq = src->shader_enq[0];
	}

	//2: texture
	if(dst->tex_deq[0] != src->tex_enq[0])
	{
		//say("@3\n");
		buf0 = (void*)(src->tex[0]);
		if(0 != buf0)
		{
			fmt = src->tex_fmt[0];
			w = src->tex_w[0];
			h = src->tex_h[0];
			fd = uploadtexture(dst, src, buf0, fmt, w, h);

			dst->tex[0] = fd;
			say("(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, fd);
		}

		dst->tex_deq[0] = src->tex_enq[0];
	}

	//3: vertex
	if(	(dst->vbo_deq != src->vbuf_enq) |
		(dst->ibo_deq != src->ibuf_enq) )
	{
		//say("@4\n");
		uploadvertex(dst, src);
		//say("(%x,%x,%x)\n", dst->vao, dst->vbo, dst->ibo);
		dst->vbo_deq = src->vbuf_enq;
		dst->ibo_deq = src->ibuf_enq;
	}
}
void callback_update(struct arena* win)
{
	int j;
	struct datapair* mod;

	//local
	mod = win->mod;
	for(j=0;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		update_eachpass(&mod[j].dst, &mod[j].src);
	}
}




u32 fixvao(struct arena* win, u32 fmt, u32 vao, u32 vbo)
{
	int j;
	u32 (*map)[2];


	//if master window, return
	if(0 == win)return vao;

	map = win->map;
	if(0 == map)return vao;


	//if found it, return
	for(j=0;j<256;j++)
	{
		if(map[j][0] == 0)break;
		if(map[j][0] == vao)return map[j][1];
	}

	//if unregistered, register first, return
	map[j][0] = vao;
	glGenVertexArrays(1, &vao);
	map[j][1] = vao;

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	if(vbuffmt_333 == fmt)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
		glEnableVertexAttribArray(2);
	}
	else if(vbuffmt_33 == fmt)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
		glEnableVertexAttribArray(1);
	}
	return vao;
}
void display_eachpass(
	struct arena* win, struct arena* coop, 
	struct gldst* dst, struct glsrc* src,
	float* cammvp)
{
	int j;
	u32 fmt;
	u32 vbo;
	u32 vao;
	if(0 == dst->shader)return;
	if(0 == dst->vao)return;

	if(src->opaque)
	{
		glDepthMask(GL_FALSE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	//0.shader
	glUseProgram(dst->shader);

	//1.argument
	glUniformMatrix4fv(glGetUniformLocation(dst->shader, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform3fv(glGetUniformLocation(dst->shader, "camxyz"  ), 1, win->camera.vc);
	//glUniformMatrix4fv(glGetUniformLocation(dst->shader, "sunmvp"), 1, GL_FALSE, cammvp);
	//glUniform3fv(glGetUniformLocation(dst->shader, "sunxyz"  ), 1, win->camera.vc);
	if(dst->arg_deq[0] != src->arg_enq[0])
	{
		glUniformMatrix4fv(glGetUniformLocation(dst->shader, src->arg[0]), 1, GL_FALSE, (void*)src->arg_data[0]);
	}

	//2.texture
	for(j=0;j<1;j++)
	{
		if(0 == dst->tex[j])continue;
		//say("tex:%d\n", dst->tex[j]);
		glUniform1i(glGetUniformLocation(dst->shader, "tex0"), j);
		glActiveTexture(GL_TEXTURE0 + j);
		glBindTexture(GL_TEXTURE_2D, dst->tex[j]);
	}

	//3.vertex
	fmt = src->vbuf_fmt;
	vbo = dst->vbo;
	vao = dst->vao;
	vao = fixvao(coop, fmt, vao, vbo);
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

	if(src->opaque)
	{
		glDisable(GL_BLEND);
		glDepthMask(GL_TRUE);
	}
}
static struct arena* saved;
void callback_display(struct arena* this, struct arena* coop)
{
	int j;
	struct arena* win;
	struct datapair* mod;
	GLfloat cammvp[4*4];

	if(_fbo_ == this->fmt)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
		win = saved;
		if(0 == win)return;

		glViewport(0, 0, 1024, 1024);
		fixmatrix(cammvp, this);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		saved = win = this;

		glViewport(0, 0, win->fbwidth, win->fbheight);
		fixmatrix(cammvp, win);
	}
	mat4_transpose((void*)cammvp);

	//prepare
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//else
	mod = win->mod;
	for(j=16;j<64;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(win, coop, &mod[j].dst, &mod[j].src, cammvp);
	}
	if(_fbo_ == this->fmt)
	{
		display_eachpass(win, coop, &mod[trigon3d].dst, &mod[trigon3d].src, cammvp);
		return;
	}

	//geom
	for(j=8;j<16;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(win, coop, &mod[j].dst, &mod[j].src, cammvp);
	}

	//font
	for(j=0;j<8;j++)
	{
		if(0 == mod[j].src.vbuf)continue;
		display_eachpass(win, coop, &mod[j].dst, &mod[j].src, cammvp);
	}
}
