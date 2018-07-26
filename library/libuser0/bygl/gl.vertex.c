#include <stdio.h>
#include <stdlib.h>
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
#elif __APPLE__
	#include <OpenGL/gl3.h>
#else
	#include <GL/glew.h>
#endif





void uploadvertex(struct ifoot* fi, struct ofoot* fo)
{
	void* buf;
	u32 w,h,fmt;

	//vao
	if(0 == fi->vao)glGenVertexArrays(1, &fi->vao);
	glBindVertexArray(fi->vao);

	//idx
	w = fi->ibo_deq;
	h = fo->ibuf_enq;
	buf = (void*)(fo->ibuf);
	if((w != h) && (0 != buf))
	{
		w = fo->ibuf_w;
		h = fo->ibuf_h;
		if(0 == fi->ibo)
		{
			glGenBuffers(1, &fi->ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fi->ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, w*h, buf, GL_STATIC_DRAW);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fi->ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, w*h, buf);
		}
	}

	//vbo
	w = fi->vbo_deq;
	h = fo->vbuf_enq;
	buf = (void*)(fo->vbuf);
	if((w != h) && (0 != buf))
	{
		w = fo->vbuf_w;
		h = fo->vbuf_h;
		if(0 == fi->vbo)
		{
			glGenBuffers(1, &fi->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, fi->vbo);
			glBufferData(GL_ARRAY_BUFFER, w*h, buf, GL_STATIC_DRAW);

			fmt = fo->vbuf_fmt;
			if(vbuffmt_33 == fmt)
			{
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, w, (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, w, (void*)12);
				glEnableVertexAttribArray(1);
			}
			else if(vbuffmt_333 == fmt)
			{
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, w, (void*)0);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, w, (void*)12);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, w, (void*)24);
				glEnableVertexAttribArray(2);
			}
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, fi->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, w*h, buf);
		}
	}
}
void initvertex(struct arena* w)  
{
	struct texandobj* mod = w->mod;

//---------------------font3d0--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d0].vao);
	glBindVertexArray(mod[font3d0].vao);

	//ibo
	glGenBuffers(1, &mod[font3d0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d0].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d0].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------font3d1--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d1].vao);
	glBindVertexArray(mod[font3d1].vao);

	//ibo
	glGenBuffers(1, &mod[font3d1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d1].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d1].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------font3d2--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d2].vao);
	glBindVertexArray(mod[font3d2].vao);

	//ibo
	glGenBuffers(1, &mod[font3d2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d2].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d2].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------font3d3--------------------------
	//vao
	glGenVertexArrays(1, &mod[font3d3].vao);
	glBindVertexArray(mod[font3d3].vao);

	//ibo
	glGenBuffers(1, &mod[font3d3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font3d3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font3d3].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font3d3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font3d3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font3d3].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//---------------------font2d0--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d0].vao);
	glBindVertexArray(mod[font2d0].vao);

	//ibo
	glGenBuffers(1, &mod[font2d0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d0].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d0].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------font2d1--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d1].vao);
	glBindVertexArray(mod[font2d1].vao);

	//ibo
	glGenBuffers(1, &mod[font2d1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d1].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d1].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------font2d2--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d2].vao);
	glBindVertexArray(mod[font2d2].vao);

	//ibo
	glGenBuffers(1, &mod[font2d2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d2].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d2].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------font2d3--------------------------
	//vao
	glGenVertexArrays(1, &mod[font2d3].vao);
	glBindVertexArray(mod[font2d3].vao);

	//ibo
	glGenBuffers(1, &mod[font2d3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[font2d3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[font2d3].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[font2d3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[font2d3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[font2d3].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------vert3da--------------------------
//drawarray.point(vertex,colour)
	glGenVertexArrays(1, &mod[vert3da].vao);
	glBindVertexArray(mod[vert3da].vao);

	glGenBuffers(1, &mod[vert3da].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3da].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert3da].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------vert3db--------------------------
	//drawelement.line(vertex,colour)
	glGenVertexArrays(1, &mod[vert3db].vao);
	glBindVertexArray(mod[vert3db].vao);

	glGenBuffers(1, &mod[vert3db].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3db].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert3db].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert3db].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3db].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert3db].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------vert3dc---------------------------
	//drawelement.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[vert3dc].vao);
	glBindVertexArray(mod[vert3dc].vao);

	glGenBuffers(1, &mod[vert3dc].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert3dc].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert3dc].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert3dc].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert3dc].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, mod[vert3dc].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//------------------------vert2da--------------------------
	glGenVertexArrays(1, &mod[vert2da].vao);
	glBindVertexArray(mod[vert2da].vao);

	glGenBuffers(1, &mod[vert2da].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2da].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert2da].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------vert2db-------------------------
	glGenVertexArrays(1, &mod[vert2db].vao);
	glBindVertexArray(mod[vert2db].vao);

	glGenBuffers(1, &mod[vert2db].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2db].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert2db].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert2db].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2db].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert2db].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------vert2dc-------------------------
	glGenVertexArrays(1, &mod[vert2dc].vao);
	glBindVertexArray(mod[vert2dc].vao);

	glGenBuffers(1, &mod[vert2dc].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[vert2dc].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[vert2dc].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[vert2dc].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[vert2dc].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[vert2dc].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);
}
