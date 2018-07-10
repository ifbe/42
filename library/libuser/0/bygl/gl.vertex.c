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

//---------------------0--------------------------
	//vao
	glGenVertexArrays(1, &mod[0].vao);
	glBindVertexArray(mod[0].vao);

	//ibo
	glGenBuffers(1, &mod[0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[0].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[0].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------1--------------------------
	//vao
	glGenVertexArrays(1, &mod[1].vao);
	glBindVertexArray(mod[1].vao);

	//ibo
	glGenBuffers(1, &mod[1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[1].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[1].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------2--------------------------
	//vao
	glGenVertexArrays(1, &mod[2].vao);
	glBindVertexArray(mod[2].vao);

	//ibo
	glGenBuffers(1, &mod[2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[2].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[2].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------3--------------------------
	//vao
	glGenVertexArrays(1, &mod[3].vao);
	glBindVertexArray(mod[3].vao);

	//ibo
	glGenBuffers(1, &mod[3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[3].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[3].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//---------------------4--------------------------
	//vao
	glGenVertexArrays(1, &mod[4].vao);
	glBindVertexArray(mod[4].vao);

	//ibo
	glGenBuffers(1, &mod[4].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[4].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[4].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[4].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------5--------------------------
	//vao
	glGenVertexArrays(1, &mod[5].vao);
	glBindVertexArray(mod[5].vao);

	//ibo
	glGenBuffers(1, &mod[5].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[5].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[5].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[5].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------6--------------------------
	//vao
	glGenVertexArrays(1, &mod[6].vao);
	glBindVertexArray(mod[6].vao);

	//ibo
	glGenBuffers(1, &mod[6].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[6].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[6].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[6].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------7--------------------------
	//vao
	glGenVertexArrays(1, &mod[7].vao);
	glBindVertexArray(mod[7].vao);

	//ibo
	glGenBuffers(1, &mod[7].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[7].ibuf, GL_STATIC_DRAW);

	//vbo
	glGenBuffers(1, &mod[7].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000, mod[7].vbuf, GL_STATIC_DRAW);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------0x80--------------------------
//drawarray.point(vertex,colour)
	glGenVertexArrays(1, &mod[0x80].vao);
	glBindVertexArray(mod[0x80].vao);

	glGenBuffers(1, &mod[0x80].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[0x80].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x81--------------------------
	//drawelement.line(vertex,colour)
	glGenVertexArrays(1, &mod[0x81].vao);
	glBindVertexArray(mod[0x81].vao);

	glGenBuffers(1, &mod[0x81].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[0x81].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[0x81].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[0x81].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x82---------------------------
	//drawarray.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x82].vao);
	glBindVertexArray(mod[0x82].vao);

	glGenBuffers(1, &mod[0x82].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, mod[0x82].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------0x83---------------------------
	//drawelement.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x83].vao);
	glBindVertexArray(mod[0x83].vao);

	glGenBuffers(1, &mod[0x83].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[0x83].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[0x83].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, mod[0x83].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//------------------------0x84--------------------------
	glGenVertexArrays(1, &mod[0x84].vao);
	glBindVertexArray(mod[0x84].vao);

	glGenBuffers(1, &mod[0x84].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[0x84].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x85-------------------------
	glGenVertexArrays(1, &mod[0x85].vao);
	glBindVertexArray(mod[0x85].vao);

	glGenBuffers(1, &mod[0x85].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[0x85].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[0x85].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[0x85].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x86-------------------------
	glGenVertexArrays(1, &mod[0x86].vao);
	glBindVertexArray(mod[0x86].vao);

	glGenBuffers(1, &mod[0x86].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, mod[0x86].ibuf, GL_STATIC_DRAW);

	glGenBuffers(1, &mod[0x86].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000, mod[0x86].vbuf, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);
}