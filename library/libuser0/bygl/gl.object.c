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

void* allocifoot();
void fixmatrix(float*, void*);
GLuint shaderprogram(void* v, void* f);
GLuint uploadtexture(void* i, void* o, void* buf, int fmt, int w, int h);
GLuint uploadvertex(void* i, void* o);





void initobject(struct arena* w)
{
	struct texandobj* mod;
	w->mod = malloc(0x10000);
	w->buf = malloc(0x400000);
	mod = w->mod;

//--------------------font3d-------------------
	//[0000,3fff]
	mod[font3d0].vbuf = malloc(0x200000);
	mod[font3d0].vlen = 0;
	mod[font3d0].ibuf = malloc(0x100000);
	mod[font3d0].ilen = 0;

	//[4000,7fff]
	mod[font3d1].vbuf = malloc(0x200000);
	mod[font3d1].vlen = 0;
	mod[font3d1].ibuf = malloc(0x100000);
	mod[font3d1].ilen = 0;

	//[8000,bfff]
	mod[font3d2].vbuf = malloc(0x200000);
	mod[font3d2].vlen = 0;
	mod[font3d2].ibuf = malloc(0x100000);
	mod[font3d2].ilen = 0;

	//[c000,ffff]
	mod[font3d3].vbuf = malloc(0x200000);
	mod[font3d3].vlen = 0;
	mod[font3d3].ibuf = malloc(0x100000);
	mod[font3d3].ilen = 0;

//--------------------font2d-------------------
	//[0000,3fff]
	mod[font2d0].vbuf = malloc(0x200000);
	mod[font2d0].vlen = 0;
	mod[font2d0].ibuf = malloc(0x100000);
	mod[font2d0].ilen = 0;

	//[4000,7fff]
	mod[font2d1].vbuf = malloc(0x200000);
	mod[font2d1].vlen = 0;
	mod[font2d1].ibuf = malloc(0x100000);
	mod[font2d1].ilen = 0;

	//[8000,bfff]
	mod[font2d2].vbuf = malloc(0x200000);
	mod[font2d2].vlen = 0;
	mod[font2d2].ibuf = malloc(0x100000);
	mod[font2d2].ilen = 0;

	//[c000,ffff]
	mod[font2d3].vbuf = malloc(0x200000);
	mod[font2d3].vlen = 0;
	mod[font2d3].ibuf = malloc(0x100000);
	mod[font2d3].ilen = 0;

//--------------------3d-------------------
	//drawarray.point
	mod[vert3da].vbuf = malloc(0x100000);
	mod[vert3da].vlen = 0;

	//drawelement.line
	mod[vert3db].ibuf = malloc(0x100000);
	mod[vert3db].ilen = 0;
	mod[vert3db].vbuf = malloc(0x100000);
	mod[vert3db].vlen = 0;

	//drawelement.trigon
	mod[vert3dc].ibuf = malloc(0x100000);
	mod[vert3dc].ilen = 0;
	mod[vert3dc].vbuf = malloc(0x1000000);
	mod[vert3dc].vlen = 0;

//----------------------2d--------------------
	//drawarray.point
	mod[vert2da].vbuf = malloc(0x100000);
	mod[vert2da].vlen = 0;

	//drawelement.line
	mod[vert2db].ibuf = malloc(0x100000);
	mod[vert2db].ilen = 0;
	mod[vert2db].vbuf = malloc(0x100000);
	mod[vert2db].vlen = 0;

	//drawelement.trigon
	mod[vert2dc].ibuf = malloc(0x100000);
	mod[vert2dc].ilen = 0;
	mod[vert2dc].vbuf = malloc(0x100000);
	mod[vert2dc].vlen = 0;
}




void callback_update_eachpass(struct ifoot* fi, struct ofoot* fo)
{
	u32 fd;
	int w,h,fmt;
	void* buf0;
	void* buf1;
	//say("%llx,%llx\n", fi, fo);

	//0: shader
	if(fi->shader_deq != fo->shader_enq[0])
	{
		buf0 = (void*)(fo->vs);
		buf1 = (void*)(fo->fs);
		if((0 != buf0)&&(0 != buf1))
		{
			fd = shaderprogram(buf0, buf1);

			fi->shader = fd;
			say("(%llx,%llx)->%x\n", buf0, buf1, fd);
		}

		fi->shader_deq = fo->shader_enq[0];
	}

	//1: argument
	if(fi->arg_deq[0] != fo->arg_enq[0])
	{
		say("arg=%x\n", fo->arg[0]);
		fi->arg_deq[0] = fo->arg_enq[0];
	}

	//2: texture
	if(fi->tex_deq[0] != fo->tex_enq[0])
	{
		buf0 = (void*)(fo->tex[0]);
		if(0 != buf0)
		{
			fmt = fo->tex_fmt[0];
			w = fo->tex_w[0];
			h = fo->tex_h[0];
			fd = uploadtexture(fi, fo, buf0, fmt, w, h);

			fi->tex[0] = fd;
			say("(%llx,%x,%x,%x)->%x\n", buf0, fmt, w, h, fd);
		}

		fi->tex_deq[0] = fo->tex_enq[0];
	}

	//3: vertex
	if(	(fi->vbo_deq != fo->vbuf_enq) |
		(fi->ibo_deq != fo->ibuf_enq) )
	{
		uploadvertex(fi, fo);
		//say("(%x,%x,%x)\n", fi->vao, fi->vbo, fi->ibo);
		fi->vbo_deq = fo->vbuf_enq;
		fi->ibo_deq = fo->ibuf_enq;
	}
}
void callback_update_eachactor(struct arena* w)
{
	int j;
	u64* pi;
	u64* po;
	struct relation* rel;

	w->fmt = hex32('v','b','o',0);
	actorread_all(w);

	rel = w->irel0;
	while(1)
	{
		if(0 == rel)break;

		pi = (void*)(rel->dstfoot) + 0x80;
		po = (void*)(rel->srcfoot) + 0x80;
		for(j=0;j<16;j++)
		{
			if(0 == po[j])break;
			if(0 == pi[j])pi[j] = (u64)allocifoot();
			callback_update_eachpass((void*)pi[j], (void*)po[j]);
		}

		rel = samedstnextsrc(rel);
	}
}
void callback_update(struct arena* w)
{
	struct texandobj* mod = w->mod;
	callback_update_eachactor(w);

//----------------------font3d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d0].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font3d0].ilen, mod[font3d0].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d0].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font3d0].vlen,mod[font3d0].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d1].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font3d1].ilen, mod[font3d1].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d1].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font3d1].vlen, mod[font3d1].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d2].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font3d2].ilen,mod[font3d2].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d2].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font3d2].vlen,mod[font3d2].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d3].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font3d3].ilen, mod[font3d3].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d3].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font3d3].vlen, mod[font3d3].vbuf);


//----------------------font2d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d0].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font2d0].ilen, mod[font2d0].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d0].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font2d0].vlen,mod[font2d0].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d1].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font2d1].ilen, mod[font2d1].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d1].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font2d1].vlen, mod[font2d1].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d2].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font2d2].ilen,mod[font2d2].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d2].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font2d2].vlen,mod[font2d2].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d3].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[font2d3].ilen, mod[font2d3].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d3].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[font2d3].vlen, mod[font2d3].vbuf);


//----------------------3d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3da].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[vert3da].vlen, mod[vert3da].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3db].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[vert3db].ilen, mod[vert3db].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3db].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[vert3db].vlen, mod[vert3db].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3dc].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[vert3dc].ilen, mod[vert3dc].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3dc].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 36*mod[vert3dc].vlen, mod[vert3dc].vbuf);


//----------------------2d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2da].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[vert2da].vlen, mod[vert2da].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2db].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[vert2db].ilen, mod[vert2db].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2db].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[vert2db].vlen, mod[vert2db].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2dc].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[vert2dc].ilen, mod[vert2dc].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2dc].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 24*mod[vert2dc].vlen, mod[vert2dc].vbuf);
}




void callback_display_eachpass(struct ifoot* fi, struct ofoot* fo, float* cammvp)
{
	if(0 == fi->shader)return;
	if(0 == fi->vao)return;

	glUseProgram(fi->shader);
	glUniformMatrix4fv(
		glGetUniformLocation(fi->shader, "cammvp"),
		1, GL_FALSE, cammvp
	);

	if(fi->tex[0])
	{
		glUniform1i(glGetUniformLocation(fi->shader, "tex0"), 0);
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, fi->tex[0]);
	}

	glBindVertexArray(fi->vao);
	if('i' == fo->method)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fi->ibo);
		glDrawElements(GL_TRIANGLES, 3*fo->ibuf_h, GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, fo->vbuf_h);
	}
}
void callback_display_eachactor(struct arena* w, float* cammvp)
{
	int j;
	u64* pi;
	u64* po;
	struct relation* rel;

	rel = w->irel0;
	while(1)
	{
		if(0 == rel)break;

		pi = (void*)(rel->dstfoot) + 0x80;
		po = (void*)(rel->srcfoot) + 0x80;
		for(j=0;j<16;j++)
		{
			if(0 == po[j])break;
			if(0 == pi[j])pi[j] = (u64)allocifoot();
			callback_display_eachpass((void*)pi[j], (void*)po[j], cammvp);
		}

		rel = samedstnextsrc(rel);
	}
}
void fixlight(struct arena* win, u32 program)
{
	GLfloat light0[4] = {0.0f, 0.0f, 1000.0f};
	GLfloat ambientcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat lightcolor[3] = {0.5f, 0.5f, 0.5f};

	GLint ac = glGetUniformLocation(program, "ambientcolor");
	glUniform3fv(ac, 1, ambientcolor);

	GLint dc = glGetUniformLocation(program, "lightcolor");
	glUniform3fv(dc, 1, lightcolor);

	GLint dp = glGetUniformLocation(program, "lightposition");
	glUniform3fv(dp, 1, light0);

	GLint ep = glGetUniformLocation(program, "eyeposition");
	glUniform3fv(ep, 1, win->camera.vc);
}
void callback_display(struct arena* win, struct arena* coop)
{
	u32 program;
	struct texandobj* mod;
	GLfloat cammvp[4*4];

	fixmatrix(cammvp, win);
	mat4_transpose((void*)cammvp);
	mod = win->mod;


	//setup
	glViewport(0, 0, win->width, win->height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//
	callback_display_eachactor(win, cammvp);

//--------------------glsl2dprogram------------------
	//point,line
	glUseProgram(mod[vert2da].program);

	glBindVertexArray(mod[vert2da].vao);
	glDrawArrays(GL_POINTS, 0, mod[vert2da].vlen);

	glBindVertexArray(mod[vert2db].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2db].ibo);
	glDrawElements(GL_LINES, 2*mod[vert2db].ilen, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(mod[vert2dc].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2dc].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[vert2dc].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------simpleprogram------------------
	//point,line
	program = mod[vert3da].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);

	glBindVertexArray(mod[vert3da].vao);
	glDrawArrays(GL_POINTS, 0, mod[vert3da].vlen);

	glBindVertexArray(mod[vert3db].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3db].ibo);
	glDrawElements(GL_LINES, 2*mod[vert3db].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------prettyprogram------------------
	//triangle
	program = mod[vert3dc].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);
	fixlight(win, program);

	glBindVertexArray(mod[vert3dc].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3dc].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[vert3dc].ilen, GL_UNSIGNED_SHORT, 0);


	//opaque
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//--------------------font3dprogram------------------
	program = mod[font3d0].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform1i(glGetUniformLocation(program, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d0].tex);
	glBindVertexArray(mod[font3d0].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d0].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font3d0].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d1].tex);
	glBindVertexArray(mod[font3d1].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d1].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font3d1].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d2].tex);
	glBindVertexArray(mod[font3d2].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d2].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font3d2].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d3].tex);
	glBindVertexArray(mod[font3d3].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d3].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font3d3].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------font2dprogram------------------
	program = mod[font2d0].program;
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d0].tex);
	glBindVertexArray(mod[font2d0].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d0].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font2d0].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d1].tex);
	glBindVertexArray(mod[font2d1].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d1].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font2d1].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d2].tex);
	glBindVertexArray(mod[font2d2].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d2].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font2d2].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d3].tex);
	glBindVertexArray(mod[font2d3].vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d3].ibo);
	glDrawElements(GL_TRIANGLES, 3*mod[font2d3].ilen, GL_UNSIGNED_SHORT, 0);

	//
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}