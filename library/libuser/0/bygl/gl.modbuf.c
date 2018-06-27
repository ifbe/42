#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "libuser.h"
void* allocifoot();
void fixmatrix(float*, void*);
GLuint shaderprogram(void* v, void* f);
GLuint uploadtexture(void* i, void* o, void* buf, int fmt, int w, int h);
GLuint uploadvertex(void* i, void* o);





void initmodbuf(struct arena* w)
{
	struct texandobj* mod;
	w->mod = malloc(0x10000);
	w->buf = malloc(0x400000);
	mod = w->mod;

//--------------------font3d-------------------
	//[0000,3fff]
	mod[0].vbuf = malloc(0x200000);
	mod[0].vlen = 0;
	mod[0].ibuf = malloc(0x100000);
	mod[0].ilen = 0;

	//[4000,7fff]
	mod[1].vbuf = malloc(0x200000);
	mod[1].vlen = 0;
	mod[1].ibuf = malloc(0x100000);
	mod[1].ilen = 0;

	//[8000,bfff]
	mod[2].vbuf = malloc(0x200000);
	mod[2].vlen = 0;
	mod[2].ibuf = malloc(0x100000);
	mod[2].ilen = 0;

	//[c000,ffff]
	mod[3].vbuf = malloc(0x200000);
	mod[3].vlen = 0;
	mod[3].ibuf = malloc(0x100000);
	mod[3].ilen = 0;

//--------------------font2d-------------------
	//[0000,3fff]
	mod[4].vbuf = malloc(0x200000);
	mod[4].vlen = 0;
	mod[4].ibuf = malloc(0x100000);
	mod[4].ilen = 0;

	//[4000,7fff]
	mod[5].vbuf = malloc(0x200000);
	mod[5].vlen = 0;
	mod[5].ibuf = malloc(0x100000);
	mod[5].ilen = 0;

	//[8000,bfff]
	mod[6].vbuf = malloc(0x200000);
	mod[6].vlen = 0;
	mod[6].ibuf = malloc(0x100000);
	mod[6].ilen = 0;

	//[c000,ffff]
	mod[7].vbuf = malloc(0x200000);
	mod[7].vlen = 0;
	mod[7].ibuf = malloc(0x100000);
	mod[7].ilen = 0;
/*
	//2d screen
	mod[4].vbuf = malloc(0x100000);
	mod[4].vlen = 0;
*/
//--------------------3d-------------------
	//drawarray.point
	mod[0x80].vbuf = malloc(0x100000);
	mod[0x80].vlen = 0;

	//drawelement.line
	mod[0x81].ibuf = malloc(0x100000);
	mod[0x81].ilen = 0;
	mod[0x81].vbuf = malloc(0x100000);
	mod[0x81].vlen = 0;

	//drawarray.trigon
	mod[0x82].vbuf = malloc(0x1000000);
	mod[0x82].vlen = 0;

	//drawelement.trigon
	mod[0x83].ibuf = malloc(0x100000);
	mod[0x83].ilen = 0;
	mod[0x83].vbuf = malloc(0x1000000);
	mod[0x83].vlen = 0;

//----------------------2d--------------------
	//drawarray.point
	mod[0x84].vbuf = malloc(0x100000);
	mod[0x84].vlen = 0;

	//drawelement.line
	mod[0x85].ibuf = malloc(0x100000);
	mod[0x85].ilen = 0;
	mod[0x85].vbuf = malloc(0x100000);
	mod[0x85].vlen = 0;

	//drawelement.trigon
	mod[0x86].ibuf = malloc(0x100000);
	mod[0x86].ilen = 0;
	mod[0x86].vbuf = malloc(0x100000);
	mod[0x86].vlen = 0;
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
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0].ilen, mod[0].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0].vlen,mod[0].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[1].ilen, mod[1].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[1].vlen, mod[1].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[2].ilen,mod[2].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[2].vlen,mod[2].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[3].ilen, mod[3].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[3].vlen, mod[3].vbuf);


//----------------------font2d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[4].ilen, mod[4].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[4].vlen,mod[4].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[5].ilen, mod[5].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[5].vlen, mod[5].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[6].ilen,mod[6].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[6].vlen,mod[6].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[7].ilen, mod[7].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[7].vlen, mod[7].vbuf);


//----------------------3d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x80].vlen, mod[0x80].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x81].ilen, mod[0x81].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x81].vlen, mod[0x81].vbuf);

	//drawarray: trigon
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0x82].vlen, mod[0x82].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x83].ilen, mod[0x83].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 36*mod[0x83].vlen, mod[0x83].vbuf);


//----------------------2d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x84].vlen, mod[0x84].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x85].ilen, mod[0x85].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x85].vlen, mod[0x85].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x86].ilen, mod[0x86].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 24*mod[0x86].vlen, mod[0x86].vbuf);
}




void callback_display_eachpass(struct ifoot* fi, struct ofoot* fo, float* cammvp)
{
	if((fi->shader)&&(fi->tex[0])&&(fi->vao))
	{
		glUseProgram(fi->shader);
		glUniform1i(glGetUniformLocation(fi->shader, "tex0"), 0);
		glUniformMatrix4fv(
			glGetUniformLocation(fi->shader, "cammvp"),
			1, GL_FALSE, cammvp
		);

		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, fi->tex[0]);

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
void callback_display(struct arena* win)
{
	u32 program;
	struct texandobj* mod;
	GLfloat cammvp[4*4];

	fixmatrix(cammvp, win);
	mat4_transpose((void*)cammvp);
	mod = win->mod;


	//set
	glViewport(0, 0, win->width, win->height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


	//
	callback_display_eachactor(win, cammvp);


//--------------------glsl2dprogram------------------
	//point,line
	glUseProgram(mod[0x84].program);

	glBindVertexArray(mod[0x84].vao);
	glDrawArrays(GL_POINTS, 0, mod[0x84].vlen);

	glBindVertexArray(mod[0x85].vao);
	glDrawElements(GL_LINES, 2*mod[0x85].ilen, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(mod[0x86].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x86].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------simpleprogram------------------
	//point,line
	program = mod[0x80].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);

	glBindVertexArray(mod[0x80].vao);
	glDrawArrays(GL_POINTS, 0, mod[0x80].vlen);

	glBindVertexArray(mod[0x81].vao);
	glDrawElements(GL_LINES, 2*mod[0x81].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------prettyprogram------------------
	//stl,triangle
	program = mod[0x82].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);
	fixlight(win, program);

	glBindVertexArray(mod[0x82].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[0x82].vlen);

	glBindVertexArray(mod[0x83].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x83].ilen, GL_UNSIGNED_SHORT, 0);


	//
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//--------------------font3dprogram------------------
	program = mod[0].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform1i(glGetUniformLocation(program, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glBindVertexArray(mod[0].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glBindVertexArray(mod[1].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[1].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glBindVertexArray(mod[2].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[2].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glBindVertexArray(mod[3].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[3].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------font2dprogram------------------
	program = mod[4].program;
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glBindVertexArray(mod[4].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[4].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glBindVertexArray(mod[5].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[5].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glBindVertexArray(mod[6].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[6].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glBindVertexArray(mod[7].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[7].ilen, GL_UNSIGNED_SHORT, 0);

/*
//------------------directprogram-------------------
	glUseProgram(directprogram);
	glUniformMatrix4fv(glGetUniformLocation(font3dprogram, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform1i(glGetUniformLocation(font3dprogram, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glBindVertexArray(mod[4].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[4].vlen);
*/

	//
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}