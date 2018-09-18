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




GLfloat light0[4] = {0.0f, 0.0f, 1000.0f};
GLfloat ambientcolor[3] = {0.5f, 0.5f, 0.5f};
GLfloat lightcolor[3] = {0.5f, 0.5f, 0.5f};




void initobject(struct arena* win)
{
	int j;
	u8* buf = malloc(0x10000);

	for(j=0;j<0x10000;j++)buf[j] = 0;
	win->mod = buf;
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

	//1: argument
	if(dst->arg_deq[0] != src->arg_enq[0])
	{
		say("arg=%x\n", src->arg[0]);
		dst->arg_deq[0] = src->arg_enq[0];
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
	u64* recver;
	u64* sender;
	struct relation* orel;
	struct datapair* mod;

	//test
	//

	//local
	mod = win->mod;
	update_eachpass(&mod[16].dst, &mod[16].src);
	for(j=0;j<16;j++)update_eachpass(&mod[j].dst, &mod[j].src);

	//actor
	orel = win->orel0;
	while(1)
	{
		if(0 == orel)break;

		sender = (void*)(orel->dstfoot) + 0x80;
		recver = (void*)(orel->srcfoot) + 0x80;
		for(j=0;j<16;j++)
		{
			if(0 == sender[j])break;
			if(0 == recver[j])recver[j] = (u64)allocifoot();
			update_eachpass((void*)recver[j], (void*)sender[j]);
		}

		orel = samesrcnextdst(orel);
	}
}
/*
void callback_update_eachactor(struct arena* w)
{
	int j;
	u64* recver;
	u64* sender;
	struct relation* orel = w->orel0;
	while(1)
	{
		if(0 == orel)break;

		sender = (void*)(orel->dstfoot) + 0x80;
		recver = (void*)(orel->srcfoot) + 0x80;
		for(j=0;j<16;j++)
		{
			if(0 == sender[j])break;
			if(0 == recver[j])recver[j] = (u64)allocifoot();
			callback_update_eachpass((void*)recver[j], (void*)sender[j]);
		}

		orel = samesrcnextdst(orel);
	}
}
void callback_update_eachdata(struct arena* w)
{
	struct datapair* mod = w->mod;

//----------------------font3d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d0].dst.ibo);
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
*/




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
void display_eachpass(struct gldst* dst, struct glsrc* src, float* cammvp, struct arena* coop)
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
	glUniformMatrix4fv(glGetUniformLocation(dst->shader, "cammvp"), 1, GL_FALSE, cammvp);
	if(0)
	{
		//glUniform3fv(glGetUniformLocation(program, "ambientcolor" ), 1, ambientcolor);
		//glUniform3fv(glGetUniformLocation(program, "lightcolor"   ), 1, lightcolor);
		//glUniform3fv(glGetUniformLocation(program, "lightposition"), 1, light0);
		//glUniform3fv(glGetUniformLocation(program, "eyeposition"  ), 1, win->camera.vc);
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
	
}
void callback_display(struct arena* win, struct arena* coop)
{
	int j;
	u64* sender;
	u64* recver;
	struct relation* orel;
	struct datapair* mod;
	GLfloat cammvp[4*4];

	//matrix
	if(0 == coop)
	{
		fixmatrix(cammvp, win);
		glViewport(0, 0, win->fbwidth, win->fbheight);
	}
	else
	{
		fixmatrix(cammvp, coop);
		glViewport(0, 0, coop->fbwidth, coop->fbheight);
	}
	mat4_transpose((void*)cammvp);

	//prepare
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	mod = win->mod;

	//sky
	display_eachpass(&mod[16].dst, &mod[16].src, cammvp, coop);

	//geom
	for(j=8;j<15;j++)display_eachpass(&mod[j].dst, &mod[j].src, cammvp, coop);

	//font
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	for(j=0;j<8;j++)display_eachpass(&mod[j].dst, &mod[j].src, cammvp, coop);
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	//actor
	orel = win->orel0;
	while(1)
	{
		if(0 == orel)break;

		sender = (void*)(orel->dstfoot) + 0x80;
		recver = (void*)(orel->srcfoot) + 0x80;
		for(j=0;j<16;j++)
		{
			if(0 == sender[j])break;
			if(0 == recver[j])recver[j] = (u64)allocifoot();
			display_eachpass((void*)recver[j], (void*)sender[j], cammvp, coop);
		}

		orel = samesrcnextdst(orel);
	}
}
/*
void callback_display_eachactor(struct arena* win, struct arena* coop, float* cammvp)
{
	int j;
	u64* sender;
	u64* recver;
	struct relation* orel;

	orel = win->orel0;
	while(1)
	{
		if(0 == orel)break;

		sender = (void*)(orel->dstfoot) + 0x80;
		recver = (void*)(orel->srcfoot) + 0x80;
		for(j=0;j<16;j++)
		{
			if(0 == sender[j])break;
			if(0 == recver[j])recver[j] = (u64)allocifoot();
			callback_display_eachpass((void*)recver[j], (void*)sender[j], cammvp, coop);
		}

		orel = samesrcnextdst(orel);
	}
}
void callback_display_eachdata(struct arena* win, struct arena* coop, float* cammvp)
{
	u32 program;
	u32 len;
	u32 vao;
	u32 vbo;
	u32 ibo;
	struct texandobj* mod = win->mod;

//--------------------glsl2dprogram------------------
	//point,line
	glUseProgram(mod[vert2da].program);


	len = mod[vert2da].vlen;
	vbo = mod[vert2da].vbo;
	vao = mod[vert2da].vao;
	vao = fixvao(coop, vbuffmt_33, vao, vbo);
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, len);


	len = 2*mod[vert2db].ilen;
	ibo = mod[vert2db].ibo;
	vbo = mod[vert2db].vbo;
	vao = mod[vert2db].vao;
	vao = fixvao(coop, vbuffmt_33, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_LINES, len, GL_UNSIGNED_SHORT, 0);


	len = 3*mod[vert2dc].ilen;
	ibo = mod[vert2dc].ibo;
	vbo = mod[vert2dc].vbo;
	vao = mod[vert2dc].vao;
	vao = fixvao(coop, vbuffmt_33, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


//--------------------simpleprogram------------------
	//point,line
	program = mod[vert3da].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);


	len = mod[vert3da].vlen;
	vbo = mod[vert3da].vbo;
	vao = mod[vert3da].vao;
	vao = fixvao(coop, vbuffmt_33, vao, vbo);
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 0, len);


	len = 2*mod[vert3db].ilen;
	ibo = mod[vert3db].ibo;
	vbo = mod[vert3db].vbo;
	vao = mod[vert3db].vao;
	vao = fixvao(coop, vbuffmt_33, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_LINES, len, GL_UNSIGNED_SHORT, 0);


//--------------------prettyprogram------------------
	//triangle
	program = mod[vert3dc].program;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "cammvp"), 1, GL_FALSE, cammvp);
	glUniform3fv(glGetUniformLocation(program, "ambientcolor" ), 1, ambientcolor);
	glUniform3fv(glGetUniformLocation(program, "lightcolor"   ), 1, lightcolor);
	glUniform3fv(glGetUniformLocation(program, "lightposition"), 1, light0);
	glUniform3fv(glGetUniformLocation(program, "eyeposition"  ), 1, win->camera.vc);


	len = 3*mod[vert3dc].ilen;
	ibo = mod[vert3dc].ibo;
	vbo = mod[vert3dc].vbo;
	vao = mod[vert3dc].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


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
	len = 3*mod[font3d0].ilen;
	ibo = mod[font3d0].ibo;
	vao = mod[font3d0].vao;
	vbo = mod[font3d0].vbo;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d1].tex);
	len = 3*mod[font3d1].ilen;
	ibo = mod[font3d1].ibo;
	vbo = mod[font3d1].vbo;
	vao = mod[font3d1].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d2].tex);
	len = 3*mod[font3d2].ilen;
	ibo = mod[font3d2].ibo;
	vbo = mod[font3d2].vbo;
	vao = mod[font3d2].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d3].tex);
	len = 3*mod[font3d3].ilen;
	ibo = mod[font3d3].ibo;
	vbo = mod[font3d3].vbo;
	vao = mod[font3d3].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


//--------------------font2dprogram------------------
	program = mod[font2d0].program;
	glUseProgram(program);
	glUniform1i(glGetUniformLocation(program, "tex2d"), 0);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d0].tex);
	len = 3*mod[font2d0].ilen;
	ibo = mod[font2d0].ibo;
	vbo = mod[font2d0].vbo;
	vao = mod[font2d0].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d1].tex);
	len = 3*mod[font2d1].ilen;
	ibo = mod[font2d1].ibo;
	vbo = mod[font2d1].vbo;
	vao = mod[font2d1].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d2].tex);
	len = 3*mod[font2d2].ilen;
	ibo = mod[font2d2].ibo;
	vbo = mod[font2d2].vbo;
	vao = mod[font2d2].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[font3d3].tex);
	len = 3*mod[font2d3].ilen;
	ibo = mod[font2d3].ibo;
	vbo = mod[font2d3].vbo;
	vao = mod[font2d3].vao;
	vao = fixvao(coop, vbuffmt_333, vao, vbo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, len, GL_UNSIGNED_SHORT, 0);


	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
*/