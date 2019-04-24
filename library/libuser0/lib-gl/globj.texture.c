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
	#define GL_SINGLE GL_ALPHA
	#define GL_BORDER GL_REPEAT
#else
	#include <GL/glew.h>
	#define GL_SINGLE GL_RED
	#define GL_BORDER GL_REPEAT		//GL_CLAMP_TO_BORDER
#endif




int ispowerof2(int x)
{
	return (x & (x-1)) == 0;
}
GLuint uploadtexture(struct gldst* dst, GLuint tex,
	void* buf, int fmt, int w, int h)
{
	if(tex)
	{
		glBindTexture(GL_TEXTURE_2D, tex);

		glTexSubImage2D(GL_TEXTURE_2D, 0,
			0, 0, w, h,
			GL_RGBA, GL_UNSIGNED_BYTE, buf
		);
	}
	else
	{
		glGenTextures(1, &tex);
		glBindTexture(GL_TEXTURE_2D, tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_BORDER);

		switch(fmt){
			case hex32('o','n','e',0):
			{
				glTexImage2D(GL_TEXTURE_2D, 0,
					GL_SINGLE, w, h, 0,
					GL_SINGLE, GL_UNSIGNED_BYTE, buf
				);
				break;
			}
			case hex32('r','g', 0, 0):
			{
				glTexImage2D(GL_TEXTURE_2D, 0,
					GL_RG, w, h, 0,
					GL_RG, GL_UNSIGNED_BYTE, buf
				);
				break;
			}
			case hex32('r','g','b',0):
			{
				glTexImage2D(GL_TEXTURE_2D, 0,
					GL_RGB, w, h, 0,
					GL_RGB, GL_UNSIGNED_BYTE, buf
				);
				break;
			}
			default:
			{
				glTexImage2D(GL_TEXTURE_2D, 0,
					GL_RGBA, w, h, 0,
					GL_RGBA, GL_UNSIGNED_BYTE, buf
				);
			}
		}
	}

	return tex;
}