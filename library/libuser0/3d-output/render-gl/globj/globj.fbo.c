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




int gl41fbod_create(struct gl41data* tar)
{
	int w = tar->src.tex[0].w;
	int h = tar->src.tex[0].h;
	int fmt = tar->src.tex[0].fmt;

	//fbo
	glGenFramebuffers(1, &tar->dst.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, tar->dst.fbo);

	//depth buffer
	glGenTextures(1, &tar->dst.tex[0]);
	glBindTexture(GL_TEXTURE_2D, tar->dst.tex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tar->dst.tex[0], 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tar->dst.tex[0], 0);
	glDrawBuffer(GL_NONE);
#endif

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fbod!!!\n");
	return 0;
}//d
int gl41fboc_create(struct gl41data* tar)
{
	int w = tar->src.tex[0].w;
	int h = tar->src.tex[0].h;
	int fmt = tar->src.tex[0].fmt;

	//fbo
	glGenFramebuffers(1, &tar->dst.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, tar->dst.fbo);

	//render buffer: without this, depth wrong
	glGenRenderbuffers(1, &tar->dst.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, tar->dst.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, w, h);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tar->dst.rbo);

	//color buffer
	glGenTextures(1, &tar->dst.tex[0]);
	glBindTexture(GL_TEXTURE_2D, tar->dst.tex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tar->dst.tex[0], 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tar->dst.tex[0], 0);
#endif

	//1 drawbuffer
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fboc!!!\n");
	return 0;
}//c
int gl41fbo_create(struct gl41data* tar)
{
	switch(tar->src.type){
	case 'c':return gl41fboc_create(tar);
	case 'd':return gl41fbod_create(tar);
	}
	return 0;
}




int fbocreate_d(_obj* tar, int arg)
{
	//depth buffer
	glGenTextures(1, &tar->gl41list.tex[0]);
	glBindTexture(GL_TEXTURE_2D, tar->gl41list.tex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tar->gl41list.tex[0], 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tar->gl41list.tex[0], 0);
	glDrawBuffer(GL_NONE);
#endif

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fbod!!!\n");
	return 0;
}//d

int fbocreate_c(_obj* tar, int arg)
{
	//render buffer: without this, depth wrong
	glGenRenderbuffers(1, &tar->gl41list.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, tar->gl41list.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tar->gl41list.rbo);

	//color buffer
	glGenTextures(1, &tar->gl41list.tex[0]);
	glBindTexture(GL_TEXTURE_2D, tar->gl41list.tex[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tar->gl41list.tex[0], 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tar->gl41list.tex[0], 0);
#endif

	//1 drawbuffer
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fboc!!!\n");
	return 0;
}//c

int fbocreate_6(_obj* tar, int arg)
{
	int j;

	//render buffer: without this, depth wrong
	glGenRenderbuffers(1, &tar->gl41list.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, tar->gl41list.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tar->gl41list.rbo);

	//color buffer
	glGenTextures(1, &tar->gl41list.tex[0]);
	glBindTexture(GL_TEXTURE_2D, tar->gl41list.tex[0]);
	for(j=0;j<6;j++){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP, tar->gl41list.tex[0], 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tar->gl41list.tex[0], 0);
	glDrawBuffer(GL_NONE);
#endif

	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fbo6!!!\n");
	return 0;
}//c

int fbocreate_g(_obj* tar, int arg)
{
	int j;
	GLenum gbuffer[4];

	//depth buffer
	glGenTextures(1, &tar->gl41list.dep);
	glBindTexture(GL_TEXTURE_2D, tar->gl41list.dep);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tar->gl41list.dep, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tar->gl41list.dep, 0);
#endif
/*
	//render buffer: without this, depth wrong
	glGenRenderbuffers(1, &tar->gl41list.rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, tar->gl41list.rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tar->gl41list.rbo);
*/
	for(j=0;j<4;j++){
		//geometry buffer
		glGenTextures(1, &tar->gl41list.tex[j]);
		glBindTexture(GL_TEXTURE_2D, tar->gl41list.tex[j]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+j, GL_TEXTURE_2D, tar->gl41list.tex[j], 0);
#else
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+j, tar->gl41list.tex[j], 0);
#endif

		gbuffer[j] = GL_COLOR_ATTACHMENT0+j;
	}
	glDrawBuffers(4, gbuffer);

	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fbog!!!\n");
	return 0;
}
int fbocreate(_obj* tar, int arg)
{
	//frame buffer
	glGenFramebuffers(1, &tar->gl41list.fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, tar->gl41list.fbo);

	switch(arg){
	case '6':fbocreate_6(tar, arg);break;
	case 'c':fbocreate_c(tar, arg);break;
	case 'd':fbocreate_d(tar, arg);break;
	case 'g':fbocreate_g(tar, arg);break;
	}
	return 0;
}
int fbodelete(_obj* tar)
{
	int j;
	for(j=0;j<4;j++){
		if(tar->gl41list.tex[j])glDeleteTextures(1, &tar->gl41list.tex[j]);
	}
	if(tar->gl41list.rbo)glDeleteRenderbuffers(1, &tar->gl41list.rbo);
	if(tar->gl41list.fbo)glDeleteFramebuffers(1, &tar->gl41list.fbo);
	return 0;
}
