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
	u32 w,h,len;

	//vao
	if(0 == dst->vao)glGenVertexArrays(1, &dst->vao);
	glBindVertexArray(dst->vao);

	//vbo
	w = dst->vbo_deq;
	h = src->vbuf_enq;
	buf = (void*)(src->vbuf);
	if((w != h) && (0 != buf))
	{
		//say("@2: %d\n", dst->vao);
		w = src->vbuf_w;
		h = src->vbuf_h;
		if(dst->vbo)
		{
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, w*h, buf);
		}
		else
		{
			len = src->vbuf_len;
			glGenBuffers(1, &dst->vbo);
			glBindBuffer(GL_ARRAY_BUFFER, dst->vbo);
			glBufferData(GL_ARRAY_BUFFER, len, buf, GL_STATIC_DRAW);

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
			}//switch
		}//new vertex, remake it
	}

	//idx
	w = dst->ibo_deq;
	h = src->ibuf_enq;
	buf = (void*)(src->ibuf);
	if((w != h) && (0 != buf))
	{
		//say("@1: %d\n", dst->vao);
		w = src->ibuf_w;
		h = src->ibuf_h;
		if(dst->ibo)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, w*h, buf);
		}
		else
		{
			len = src->ibuf_len;
			glGenBuffers(1, &dst->ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, len, buf, GL_STATIC_DRAW);
		}
	}

}