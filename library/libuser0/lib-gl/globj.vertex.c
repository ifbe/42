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





void uploadvertex(struct gldst* dst, struct glsrc* src)
{
	void* buf;
	u32 w,h;

	//vao
	if(0 == dst->vao)glGenVertexArrays(1, &dst->vao);
	glBindVertexArray(dst->vao);

	//idx
	w = dst->ibo_deq;
	h = src->ibuf_enq;
	buf = (void*)(src->ibuf);
	if((w != h) && (0 != buf))
	{
		//say("@1: %d\n", dst->vao);
		w = src->ibuf_w;
		h = src->ibuf_h;
		if(0 == dst->ibo)
		{
			glGenBuffers(1, &dst->ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, w*h, buf, GL_STATIC_DRAW);
		}
		else
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, w*h, buf);
		}
	}

	//vbo
	w = dst->vbo_deq;
	h = src->vbuf_enq;
	buf = (void*)(src->vbuf);
	if((w != h) && (0 != buf))
	{
		//say("@2: %d\n", dst->vao);
		w = src->vbuf_w;
		h = src->vbuf_h;
		if(0 == dst->vbo)
		{
			glGenBuffers(1, &dst->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferData(GL_ARRAY_BUFFER, w*h, buf, GL_STATIC_DRAW);

			switch(src->vbuf_fmt)
			{
				case vbuffmt_3:
				{
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, w, (void*)0);
					glEnableVertexAttribArray(0);
					break;
				}
				case vbuffmt_33:
				{
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, w, (void*)0);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, w, (void*)12);
					glEnableVertexAttribArray(1);
					break;
				}
				case vbuffmt_333:
				{
					glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, w, (void*)0);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, w, (void*)12);
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, w, (void*)24);
					glEnableVertexAttribArray(2);
					break;
				}
				case vbuffmt_4:
				{
					glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, w, (void*)0);
					glEnableVertexAttribArray(0);
					break;
				}
				case vbuffmt_44:
				{
					glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, w, (void*)0);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, w, (void*)16);
					glEnableVertexAttribArray(1);
					break;
				}
				case vbuffmt_444:
				{
					glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, w, (void*)0);
					glEnableVertexAttribArray(0);
					glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, w, (void*)16);
					glEnableVertexAttribArray(1);
					glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, w, (void*)32);
					glEnableVertexAttribArray(2);
					break;
				}
			}
		}
		else
		{
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, w*h, buf);
		}
	}
}
void initvertex(struct arena* win)
{
	struct datapair* mod = win->mod;


//--------------------font3d-------------------
	//[0000,3fff]
	uploadvertex(&mod[font3d0].dst, &mod[font3d0].src);

	//[4000,7fff]
	uploadvertex(&mod[font3d1].dst, &mod[font3d1].src);

	//[8000,bfff]
	uploadvertex(&mod[font3d2].dst, &mod[font3d2].src);

	//[c000,ffff]
	uploadvertex(&mod[font3d3].dst, &mod[font3d3].src);


//--------------------font2d-------------------
	//[0000,3fff]
	uploadvertex(&mod[font2d0].dst, &mod[font2d0].src);

	//[4000,7fff]
	uploadvertex(&mod[font2d1].dst, &mod[font2d1].src);

	//[8000,bfff]
	uploadvertex(&mod[font2d2].dst, &mod[font2d2].src);

	//[c000,ffff]
	uploadvertex(&mod[font2d3].dst, &mod[font2d3].src);


//--------------------3d-------------------
	//drawarray.point3d
	uploadvertex(&mod[point3d].dst, &mod[point3d].src);

	//drawelement.line3d
	uploadvertex(&mod[line3d].dst, &mod[line3d].src);

	//drawelement.trigon3d
	uploadvertex(&mod[trigon3d].dst, &mod[trigon3d].src);

	//opaque3d
	uploadvertex(&mod[opaque3d].dst, &mod[opaque3d].src);


//----------------------2d--------------------
	//drawarray.point2d
	uploadvertex(&mod[point2d].dst, &mod[point2d].src);

	//drawelement.line2d
	uploadvertex(&mod[line2d].dst, &mod[line2d].src);

	//drawelement.trigon2d
	uploadvertex(&mod[trigon2d].dst, &mod[trigon2d].src);

	//opaque2d
	uploadvertex(&mod[opaque2d].dst, &mod[opaque2d].src);
}