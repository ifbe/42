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




int fbocreate_d(struct supply* tar, int arg)
{
	//depth buffer
	glGenTextures(1, &tar->tex0);
	glBindTexture(GL_TEXTURE_2D, tar->tex0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tar->tex0, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tar->tex0, 0);
	glDrawBuffer(GL_NONE);
#endif

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fbod!!!\n");
	return 0;
}//d

int fbocreate_c(struct supply* tar, int arg)
{
	//render buffer: without this, depth wrong
	glGenRenderbuffers(1, &tar->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, tar->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tar->rbo);

	//color buffer
	glGenTextures(1, &tar->tex0);
	glBindTexture(GL_TEXTURE_2D, tar->tex0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tar->tex0, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tar->tex0, 0);
#endif

	//1 drawbuffer
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers);

	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fboc!!!\n");
	return 0;
}//c

int fbocreate_6(struct supply* tar, int arg)
{
	int j;

	//render buffer: without this, depth wrong
	glGenRenderbuffers(1, &tar->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, tar->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tar->rbo);

	//color buffer
	glGenTextures(1, &tar->tex0);
	glBindTexture(GL_TEXTURE_2D, tar->tex0);
	for(j=0;j<6;j++){
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + j, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_CUBE_MAP, tar->tex0, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tar->tex0, 0);
	glDrawBuffer(GL_NONE);
#endif

	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fbo6!!!\n");
	return 0;
}//c

int fbocreate_g(struct supply* tar, int arg)
{
	int j;
	GLenum gbuffer[4];
	tar->tex = memorycreate(0x100, 0);

	//depth buffer
	glGenTextures(1, &tar->dep);
	glBindTexture(GL_TEXTURE_2D, tar->dep);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, tar->dep, 0);
#else
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tar->dep, 0);
#endif
/*
	//render buffer: without this, depth wrong
	glGenRenderbuffers(1, &tar->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, tar->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, tar->rbo);
*/
	for(j=0;j<4;j++){
		//geometry buffer
		glGenTextures(1, &tar->tex[j]);
		glBindTexture(GL_TEXTURE_2D, tar->tex[j]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

#ifdef __ANDROID__
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+j, GL_TEXTURE_2D, tar->tex[j], 0);
#else
		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+j, tar->tex[j], 0);
#endif

		gbuffer[j] = GL_COLOR_ATTACHMENT0+j;
	}
	glDrawBuffers(4, gbuffer);

	//check
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)say("err@fbog!!!\n");
	return 0;
}
int fbocreate(struct supply* tar, int arg)
{
	//frame buffer
	glGenFramebuffers(1, &tar->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, tar->fbo);

	switch(arg){
	case '6':fbocreate_6(tar, arg);break;
	case 'c':fbocreate_c(tar, arg);break;
	case 'd':fbocreate_d(tar, arg);break;
	case 'g':fbocreate_g(tar, arg);break;
	}
	return 0;
}
int fbodelete(struct supply* tar)
{
	int j;
	if(tar->tex){
		for(j=0;j<4;j++){
			if(tar->tex[j])glDeleteTextures(1, &tar->tex[j]);
		}
		free(tar->tex);
	}
	if(tar->tex0)glDeleteTextures(1, &tar->tex0);
	if(tar->rbo)glDeleteRenderbuffers(1, &tar->rbo);
	if(tar->fbo)glDeleteFramebuffers(1, &tar->fbo);
	return 0;
}
