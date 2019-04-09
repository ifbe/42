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




void coopctx_copy(struct datapair* s, struct datapair* d)
{
	int j;
	u8* ss = (void*)(&s->src);
	u8* dd = (void*)(&d->src);
	struct gldst* host = &s->dst;
	struct gldst* coop = &d->dst;
	for(j=0;j<sizeof(struct glsrc);j++)dd[j] = ss[j];

	coop->shader = host->shader;
	//coop->vao = host->vao;	//never !!!!!!

	//arg
	for(j=0;j<4;j++){
		coop->arg[j] = host->arg[j];
	}

	//texture
	for(j=0;j<4;j++){
		coop->tex_len[j] = host->tex_len[j];
		coop->tex[j] = host->tex[j];
	}

	//vertex
	coop->vbo     = host->vbo;
	coop->vbo_len = host->vbo_len;
	coop->ibo     = host->ibo;
	coop->ibo_len = host->ibo_len;
}
void coopctx_update(struct arena* win)
{
	int j;
	struct relation* rel;
	struct arena* tar;
	struct datapair* coopmod;
	struct datapair* hostmod;
	struct glsrc* gs;
	struct gldst* gd;

	//rel
	rel = win->orel0;
	if(0 == rel)return;

	//tar
	tar = (void*)(rel->dstchip);
	if(0 == tar)return;

	//dst: coop
	coopmod = win->mod;
	if(0 == coopmod)return;

	//src: host
	hostmod = tar->mod;
	if(0 == hostmod)return;

	for(j=0;j<64;j++)
	{
		//copy everything, except vao
		coopctx_copy(&hostmod[j], &coopmod[j]);
		gs = &coopmod[j].src;
		gd = &coopmod[j].dst;

		//skip
		if(0 != gd->vao)continue;
		if(0 == gd->vbo)continue;

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
		}
	}
}




void coopctx_create(struct arena* win)
{
	int j;
	u8* buf;

	buf = memorycreate(0x10000);
	for(j=0;j<0x10000;j++)buf[j] = 0;

	win->mod = (void*)buf;
}