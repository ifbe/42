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



/*
void coopctx_copy(struct datapair* d, struct datapair* s)
{
	int j;
	u8* ss = (void*)(&s->src);
	u8* dd = (void*)(&d->src);
	struct gldst* host = &s->dst;
	struct gldst* coop = &d->dst;
	for(j=0;j<sizeof(struct glsrc);j++)dd[j] = ss[j];

	//shader
	coop->shader = host->shader;

	//texture
	for(j=0;j<4;j++){
		coop->tex[j] = host->tex[j];
	}

	//vertex
	coop->vbo = host->vbo;
	coop->ibo = host->ibo;
}
void coopctx_update_one(struct datapair* pair)
{
	struct glsrc* gs = &pair->src;
	struct gldst* gd = &pair->dst;

	//skip
	if(0 != gd->vao)return;
	if(0 == gd->vbo)return;

	//gen
	glGenVertexArrays(1, &gd->vao);
	glBindVertexArray(gd->vao);
	glBindBuffer(GL_ARRAY_BUFFER, gd->vbo);
	say("%x,%x,%x\n", gd->vao, gd->vbo, gs->vbuf_fmt);
	switch(gs->vbuf_fmt)
	{
		case vbuffmt_3:
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12, (void*)0);
			glEnableVertexAttribArray(0);
			break;
		}
		case vbuffmt_33:
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
			glEnableVertexAttribArray(1);
			break;
		}
		case vbuffmt_333:
		{
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
			glEnableVertexAttribArray(2);
			break;
		}
		case vbuffmt_4:
		{
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 16, (void*)0);
			glEnableVertexAttribArray(0);
			break;
		}
		case vbuffmt_44:
		{
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 32, (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 32, (void*)16);
			glEnableVertexAttribArray(1);
			break;
		}
		case vbuffmt_444:
		{
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 48, (void*)0);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 48, (void*)16);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 48, (void*)32);
			glEnableVertexAttribArray(2);
			break;
		}
	}
}
void coopctx_update(struct supply* win)
{
	int j;
	struct relation* rel;
	struct supply* ctx;
	struct datapair* coopmod;
	struct datapair* hostmod;

	//rel
	rel = win->orel0;
	if(0 == rel)return;

	//tar
	ctx = (void*)(rel->dstchip);
	if(0 == ctx)return;

	coopmod = win->gl_solid;
	hostmod = ctx->gl_solid;
	for(j=0;j<64;j++){
		coopctx_copy(&coopmod[j], &hostmod[j]);
		coopctx_update_one(&coopmod[j]);
	}

	coopmod = win->gl_opaque;
	hostmod = ctx->gl_opaque;
	for(j=0;j<64;j++){
		coopctx_copy(&coopmod[j], &hostmod[j]);
		coopctx_update_one(&coopmod[j]);
	}
}



void coopwindow_render(struct supply* win)
{
}
void coopwindow_create(struct supply* win)
{
	int j;
	u8* buf;

	win->gl_solid = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->gl_opaque = buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;
}*/
