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





void uploadvertex(struct gldst* dst, struct mysrc* src)
{
	u32 w,h;
	struct vertex* vtx = src->vtx;

	//vao
	if(0 == dst->vao)glGenVertexArrays(1, &dst->vao);
	glBindVertexArray(dst->vao);

	//vbo
	if((dst->vbo_deq != src->vbuf_enq) && (0 != vtx->vbuf))
	{
		//say("@2: %d\n", dst->vao);
		w = vtx->vbuf_w;
		h = vtx->vbuf_h;
		if(dst->vbo)
		{
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, w*h, vtx->vbuf);
		}
		else
		{
			glGenBuffers(1, &dst->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferData(GL_ARRAY_BUFFER, vtx->vbuf_len, vtx->vbuf, GL_STATIC_DRAW);

			switch(vtx->vbuf_fmt)
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
			}//switch
		}//new vertex, remake it
	}

	//idx
	if((dst->vbo_deq != src->vbuf_enq) && (0 != vtx->ibuf))
	{
		//say("@1: %d\n", dst->vao);
		w = vtx->ibuf_w;
		h = vtx->ibuf_h;
		if(dst->ibo)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, w*h, vtx->ibuf);
		}
		else
		{
			glGenBuffers(1, &dst->ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, vtx->ibuf_len, vtx->ibuf, GL_STATIC_DRAW);
		}
	}

}
