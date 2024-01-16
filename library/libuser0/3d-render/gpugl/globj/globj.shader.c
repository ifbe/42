#include <stdio.h>
#include <stdlib.h>
#include "libuser.h"

#ifdef __ANDROID__
	#include <jni.h>
	#include <errno.h>
	#include <EGL/egl.h>
	#include <GLES/gl.h>
	#include <GLES3/gl32.h>
	#include <GLES3/gl3ext.h>
	#include <android/log.h>
	#include <android_native_app_glue.h>
#else
	#include <GL/glew.h>
#endif




GLuint compileShader(GLenum type, const char* source)
{
	GLuint shader = glCreateShader(type);
	if(!shader)return 0;

	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	GLint compileStatus;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	if (GL_TRUE == compileStatus)return shader;

	GLint infoLogLength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
	if(infoLogLength)
	{
		char* infoLog = (char*)malloc(infoLogLength);
		if (infoLog)
		{
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
			logtoall("Could not compile shader %d:\n%s", type, infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
	}
	return 0;
}
GLuint shaderprogram(void* v, void* f, void* g, void* tc, void* te, void* c)
{
	GLuint vShader = compileShader(GL_VERTEX_SHADER, v);
	if(!vShader){
		logtoall("fail@compileShader: %s\n", v);
		return 0;
	}

	GLuint fShader = compileShader(GL_FRAGMENT_SHADER, f);
	if(!fShader){
		logtoall("fail@compileShader: %s\n", f);
		return 0;
	}

	GLuint gShader = 0;
	if(g){
		gShader = compileShader(GL_GEOMETRY_SHADER, g);
		if(!gShader)logtoall("fail@compileShader: %s\n", g);
	}
/*
	GLuint tcShader = 0;
	if(tc){
		compileShader(GL_TESS_CONTROL_SHADER, tc);
		if(!tcShader)logtoall("fail@compileShader: %s\n", tc);
	}

	GLuint teShader = 0;
	if(te){
		compileShader(GL_TESS_EVALUATION_SHADER, te);
		if(!teShader)logtoall("fail@compileShader: %s\n", te);
	}

	GLuint cShader = 0;
	if(c){
		compileShader(GL_COMPUTE_SHADER, c);
		if(!cShader)logtoall("fail@compileShader: %s\n", c);
	}
*/

	//3.glsl program
	GLuint prog = glCreateProgram();
	if(0 == prog)
	{
		printf("ERROR : create program failed");
		exit(1);
	}

	glAttachShader(prog, vShader);
	glAttachShader(prog, fShader);
	if(gShader)glAttachShader(prog, gShader);
	//if(tcShader)glAttachShader(prog, tcShader);
	//if(teShader)glAttachShader(prog, teShader);
	//if( cShader)glAttachShader(prog,  cShader);
	glLinkProgram(prog);

	GLint linkStatus;
	glGetProgramiv(prog, GL_LINK_STATUS, &linkStatus);
	if(GL_TRUE == linkStatus)return prog;

	printf("ERROR : link shader program failed");
	GLint logLen;
	glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &logLen);
	if(logLen > 0)
	{
		char *log = (char*)malloc(logLen);
		GLsizei written;
		glGetProgramInfoLog(prog, logLen, &written, log);
		printf("Program log :%s\n", log);
	}

	glDeleteProgram(prog);
	return 0;
}