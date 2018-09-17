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

void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);




int ispowerof2(int x)
{
	return (x & (x-1)) == 0;
}
GLuint uploadtexture(struct gldst* dst, struct glsrc* src,
	void* buf, int fmt, int w, int h)
{
	if(dst->tex[0])
	{
		glBindTexture(GL_TEXTURE_2D, dst->tex[0]);

		glTexSubImage2D(GL_TEXTURE_2D, 0,
			0, 0, w, h,
			GL_RGBA, GL_UNSIGNED_BYTE, buf
		);
	}
	else
	{
		glGenTextures(1, &dst->tex[0]);
		glBindTexture(GL_TEXTURE_2D, dst->tex[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_BORDER);

		if(hex32('r','g',0,0) == fmt)
		{
			glTexImage2D(GL_TEXTURE_2D, 0,
				GL_RG, w, h, 0,
				GL_RG, GL_UNSIGNED_BYTE, buf
			);
		}
		else if(hex32('r','g','b',0) == fmt)
		{
			glTexImage2D(GL_TEXTURE_2D, 0,
				GL_RGB, w, h, 0,
				GL_RGB, GL_UNSIGNED_BYTE, buf
			);
		}
		else
		{
			glTexImage2D(GL_TEXTURE_2D, 0,
				GL_RGBA, w, h, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, buf
			);
		}
	}

	return dst->tex[0];
}
void inittexture(struct arena* win)  
{
	int j;
	GLuint tex;
	struct datapair* mod;

	//prepare
	mod = win->mod;
	u8* buf = malloc(0x400000);


//---------------------[0000,3fff]------------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0x80;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048, 
			(j&0x7f)<<4, (j&0xff80)>>3, j
		);
	}
	for(j=0x20;j<0x80;j++)
	{
		drawascii_alpha(buf, 2048, 2048,
			j<<4, 0, j
		);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_SINGLE, 2048, 2048, 0,
		GL_SINGLE, GL_UNSIGNED_BYTE, buf
	);

	mod[font3d0].dst.tex[0] = tex;
	mod[font2d0].dst.tex[0] = tex;


//---------------------[4000,7fff]----------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x4000
		);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_SINGLE, 2048, 2048, 0,
		GL_SINGLE, GL_UNSIGNED_BYTE, buf
	);

	mod[font3d1].dst.tex[0] = tex;
	mod[font2d1].dst.tex[0] = tex;


//------------------[8000,bfff]---------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x8000
		);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_SINGLE, 2048, 2048, 0,
		GL_SINGLE, GL_UNSIGNED_BYTE, buf
	);

	mod[font3d2].dst.tex[0] = tex;
	mod[font2d2].dst.tex[0] = tex;


//----------------[c000,ffff]--------------------
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0xc000
		);
	}

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_BORDER);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_SINGLE, 2048, 2048, 0,
		GL_SINGLE, GL_UNSIGNED_BYTE, buf
	);

	mod[font3d3].dst.tex[0] = tex;
	mod[font2d3].dst.tex[0] = tex;

/*
//------------------2d screen--------------------
	glGenTextures(1, &(mod[4].tex));
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);	//GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, buf
	);
*/
	/*
	glGenFramebuffers(1, &shadowfb);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowfb);

	glGenTextures(1, &shadowtexture);
	glBindTexture(GL_TEXTURE_2D, shadowtexture);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_DEPTH_COMPONENT, 1024, 1024, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL
	);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture(
		GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		shadowtexture, 0
	);
	glDrawBuffer(GL_NONE);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("error@fbo\n");
	}
*/
}
