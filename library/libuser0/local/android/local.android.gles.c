#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "arena.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
void* getandroidapp();
void setandroidapp(void*);
//
void asset_create();
void fixmatrix(float*, void*);
void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);




//
static struct android_app* app = 0;
static struct arena* win;
static int32_t width = 512;
static int32_t height = 512;
//
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLContext context = EGL_NO_CONTEXT;
static EGLSurface surface = EGL_NO_SURFACE;
//
static GLuint font3dprogram;
static GLuint font2dprogram;
static GLuint simpleprogram;
static GLuint prettyprogram;
static GLuint directprogram;
static GLuint glsl2dprogram;
//
struct texandobj
{
	GLuint program;
	GLuint vao;

	GLuint tex;
	GLuint tlen;
	void* tbuf;

	GLuint ibo;
	GLuint ilen;
	void* ibuf;

	GLuint vbo;
	GLuint vlen;
	void* vbuf;
};
static struct texandobj mod[256];




char glsl2dvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"vcolor = colour;\n"
		"gl_Position = vec4(vertex, 1.0);\n"
	"}\n"
};
char glsl2dfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char simplevert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"uniform mat4 simplemvp;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"vcolor = colour;\n"
		"gl_Position = simplemvp * vec4(vertex,1.0);\n"
	"}\n"
};
char simplefrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char prettyvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec3 normal;\n"
	"uniform mat4 prettymvp;\n"
	"uniform mediump vec3 ambientcolor;\n"
	"uniform mediump vec3 lightcolor;\n"
	"uniform mediump vec3 lightposition;\n"
	"uniform mediump vec3 eyeposition;\n"
	"out mediump vec3 vcolor;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 N = normalize(normal);\n"
		"mediump vec3 L = normalize(vec3(lightposition - vertex));\n"
		"mediump vec3 E = normalize(eyeposition-vertex);\n"
		"mediump vec3 R = reflect(-L, N);\n"
		"mediump float SN = max(dot(N, L), 0.0);\n"
		"mediump float RV = max(dot(R, E), 0.0);\n"
		"mediump vec3 ambient = ambientcolor;\n"
		"mediump vec3 diffuse = lightcolor * SN;\n"
		"mediump vec3 specular = vec3(0.0, 0.0, 0.0);\n"
		"if(SN>0.0)specular = lightcolor * pow(RV, 4.0);\n"
		"vcolor = colour*(ambient + diffuse + specular);\n"
		"gl_Position = prettymvp * vec4(vertex,1.0);\n"
	"}\n"
};
char prettyfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vcolor,1.0);\n"
	"}\n"
};
char font3dvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoord;\n"
	"uniform mat4 prettymvp;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = prettymvp * vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoord;\n"
	"}\n"
};
char font3dfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D texdata;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(texdata, texuv).aaaa;\n"
	"}\n"
};
char font2dvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoo;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoo;\n"
	"}\n"
};
char font2dfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D tex2d;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(tex2d, texuv).aaaa;\n"
	"}\n"
};
char directvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 vertex;\n"
	"layout(location = 1)in mediump vec3 colour;\n"
	"layout(location = 2)in mediump vec2 texcoo;\n"
	"uniform mat4 prettymvp;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = prettymvp * vec4(vertex,1.0);\n"
		"origcolor = colour;\n"
		"texuv = texcoo;\n"
	"}\n"
};
char directfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D tex2d;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(tex2d, texuv);\n"
	"}\n"
};




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
	if (infoLogLength)
	{
		char* infoLog = (char*)malloc(infoLogLength);
		if (infoLog)
		{
			glGetShaderInfoLog(shader, infoLogLength, NULL, infoLog);
			LOGI("Could not compile shader %d:\n%s", type, infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
	}
	return 0;
}
GLuint createProgram(const char* v, const char* f)
{
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, v);
	if (!vertexShader)return 0;

	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, f);
	if (!fragmentShader)return 0;

	GLuint program = glCreateProgram();
	if (!program)return 0;

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	GLint linkStatus;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if(GL_TRUE == linkStatus)return program;

	GLint infoLogLength = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
	if (infoLogLength)
	{
		char* infoLog = (char*)malloc(infoLogLength);
		if(infoLog)
		{
			glGetProgramInfoLog(program, infoLogLength, NULL, infoLog);
			LOGI("Could not link program:\n%s", infoLog);
			free(infoLog);
		}
	}
	glDeleteProgram(program);
	return 0;
}
void initshader()
{
	font3dprogram = createProgram(font3dvert, font3dfrag);
	if (!font3dprogram) {
		LOGI("Could not create program");
		return;
	}
	mod[0].program = font3dprogram;
	mod[1].program = font3dprogram;
	mod[2].program = font3dprogram;
	mod[3].program = font3dprogram;

	font2dprogram = createProgram(font2dvert, font2dfrag);
	if (!font3dprogram) {
		LOGI("Could not create program");
		return;
	}
	mod[4].program = font3dprogram;
	mod[5].program = font3dprogram;
	mod[6].program = font3dprogram;
	mod[7].program = font3dprogram;

	simpleprogram = createProgram(simplevert, simplefrag);
	if (!simpleprogram) {
		LOGI("Could not create program");
		return;
	}
	mod[0x80].program = simpleprogram;
	mod[0x81].program = simpleprogram;

	prettyprogram = createProgram(prettyvert, prettyfrag);
	if (!prettyprogram) {
		LOGI("Could not create program");
		return;
	}
	mod[0x82].program = prettyprogram;
	mod[0x83].program = prettyprogram;

	glsl2dprogram = createProgram(glsl2dvert, glsl2dfrag);
	if (!glsl2dprogram) {
		LOGI("Could not create program");
		return;
	}
	mod[0x84].program = glsl2dprogram;
	mod[0x85].program = glsl2dprogram;
	mod[0x86].program = glsl2dprogram;
	mod[0x87].program = glsl2dprogram;
}
void initobject()  
{
//---------------------0--------------------------
	//vao
	glGenVertexArrays(1, &mod[0].vao);
	glBindVertexArray(mod[0].vao);

	//ibo
	glGenBuffers(1, &mod[0].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[0].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[0].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------1--------------------------
	//vao
	glGenVertexArrays(1, &mod[1].vao);
	glBindVertexArray(mod[1].vao);

	//ibo
	glGenBuffers(1, &mod[1].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[1].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[1].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[1].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------2--------------------------
	//vao
	glGenVertexArrays(1, &mod[2].vao);
	glBindVertexArray(mod[2].vao);

	//ibo
	glGenBuffers(1, &mod[2].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[2].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[2].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[2].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------3--------------------------
	//vao
	glGenVertexArrays(1, &mod[3].vao);
	glBindVertexArray(mod[3].vao);

	//ibo
	glGenBuffers(1, &mod[3].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[3].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[3].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[3].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);

/*
//-------------------------4--------------------------
	//vao
	glGenVertexArrays(1, &mod[4].vao);
	glBindVertexArray(mod[4].vao);

	//vbo
	glGenBuffers(1, &mod[4].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[4].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);
*/


//---------------------4--------------------------
	//vao
	glGenVertexArrays(1, &mod[4].vao);
	glBindVertexArray(mod[4].vao);

	//ibo
	glGenBuffers(1, &mod[4].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[4].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[4].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[4].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//----------------------5--------------------------
	//vao
	glGenVertexArrays(1, &mod[5].vao);
	glBindVertexArray(mod[5].vao);

	//ibo
	glGenBuffers(1, &mod[5].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[5].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[5].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[5].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-----------------------6--------------------------
	//vao
	glGenVertexArrays(1, &mod[6].vao);
	glBindVertexArray(mod[6].vao);

	//ibo
	glGenBuffers(1, &mod[6].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[6].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[6].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[6].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------7--------------------------
	//vao
	glGenVertexArrays(1, &mod[7].vao);
	glBindVertexArray(mod[7].vao);

	//ibo
	glGenBuffers(1, &mod[7].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[7].ibuf, GL_STATIC_DRAW
	);

	//vbo
	glGenBuffers(1, &mod[7].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x200000,
		mod[7].vbuf, GL_STATIC_DRAW
	);

	//attr
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);

//-------------------------0x80--------------------------
//drawarray.point(vertex,colour)
	glGenVertexArrays(1, &mod[0x80].vao);
	glBindVertexArray(mod[0x80].vao);

	glGenBuffers(1, &mod[0x80].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x80].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x81--------------------------
	//drawelement.line(vertex,colour)
	glGenVertexArrays(1, &mod[0x81].vao);
	glBindVertexArray(mod[0x81].vao);

	glGenBuffers(1, &mod[0x81].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x81].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x81].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x81].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//-------------------------0x82---------------------------
	//drawarray.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x82].vao);
	glBindVertexArray(mod[0x82].vao);

	glGenBuffers(1, &mod[0x82].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000,
		mod[0x82].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//-------------------------0x83---------------------------
	//drawelement.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x83].vao);
	glBindVertexArray(mod[0x83].vao);

	glGenBuffers(1, &mod[0x83].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x83].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x83].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000,
		mod[0x83].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);


//------------------------0x84--------------------------
	glGenVertexArrays(1, &mod[0x84].vao);
	glBindVertexArray(mod[0x84].vao);

	glGenBuffers(1, &mod[0x84].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x84].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x85-------------------------
	glGenVertexArrays(1, &mod[0x85].vao);
	glBindVertexArray(mod[0x85].vao);

	glGenBuffers(1, &mod[0x85].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x85].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x85].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x85].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


//------------------------0x86-------------------------
	glGenVertexArrays(1, &mod[0x86].vao);
	glBindVertexArray(mod[0x86].vao);

	glGenBuffers(1, &mod[0x86].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x86].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x86].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x86].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);
}
void inittexture()
{
	int j;
	u8* buf = win->buf;

	//[0000,3fff]
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

	glGenTextures(1, &(mod[0].tex));
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_ALPHA, 2048, 2048, 0,
		GL_ALPHA, GL_UNSIGNED_BYTE, buf
	);


	//[4000,7fff]
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x4000
		);
	}

	glGenTextures(1, &(mod[1].tex));
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_ALPHA, 2048, 2048, 0,
		GL_ALPHA, GL_UNSIGNED_BYTE, buf
	);


	//[8000,bfff]
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0x8000
		);
	}

	glGenTextures(1, &(mod[2].tex));
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_ALPHA, 2048, 2048, 0,
		GL_ALPHA, GL_UNSIGNED_BYTE, buf
	);


	//[c000,ffff]
	for(j=0;j<0x400000;j++)buf[j] = 0;
	for(j=0;j<0x4000;j++)
	{
		drawunicode_alpha(buf, 2048, 2048,
			(j&0x7f)<<4, (j&0xff80)>>3, j+0xc000
		);
	}

	glGenTextures(1, &(mod[3].tex));
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_ALPHA, 2048, 2048, 0,
		GL_ALPHA, GL_UNSIGNED_BYTE, buf
	);

//------------------2d screen--------------------
	glGenTextures(1, &(mod[4].tex));
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, buf
	);

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




void fixlight()
{
	GLfloat light0[4] = {0.0f, 0.0f, 1000.0f};
	GLfloat ambientcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat lightcolor[3] = {0.5f, 0.5f, 0.5f};

	GLint ac = glGetUniformLocation(prettyprogram, "ambientcolor");
	glUniform3fv(ac, 1, ambientcolor);

	GLint dc = glGetUniformLocation(prettyprogram, "lightcolor");
	glUniform3fv(dc, 1, lightcolor);

	GLint dp = glGetUniformLocation(prettyprogram, "lightposition");
	glUniform3fv(dp, 1, light0);

	GLint ep = glGetUniformLocation(prettyprogram, "eyeposition");
	glUniform3fv(ep, 1, win->camera.vc);
}
void callback_display()
{
	GLfloat cameramvp[4*4];
	fixmatrix(cameramvp, win);
	glViewport(0, 0, width, height);

	//set
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


//--------------------glsl2dprogram------------------
	//point,line
	glUseProgram(glsl2dprogram);

	glBindVertexArray(mod[0x84].vao);
	glDrawArrays(GL_POINTS, 0, mod[0x84].vlen);

	glBindVertexArray(mod[0x85].vao);
	glDrawElements(GL_LINES, 2*mod[0x85].ilen, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(mod[0x86].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x86].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------simpleprogram------------------
	//point,line
	glUseProgram(simpleprogram);
	glUniformMatrix4fv(glGetUniformLocation(simpleprogram, "simplemvp"), 1, GL_FALSE, cameramvp);

	glBindVertexArray(mod[0x80].vao);
	glDrawArrays(GL_POINTS, 0, mod[0x80].vlen);

	glBindVertexArray(mod[0x81].vao);
	glDrawElements(GL_LINES, 2*mod[0x81].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------prettyprogram------------------
	//stl,triangle
	glUseProgram(prettyprogram);
	glUniformMatrix4fv(glGetUniformLocation(prettyprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	fixlight();

	glBindVertexArray(mod[0x82].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[0x82].vlen);

	glBindVertexArray(mod[0x83].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x83].ilen, GL_UNSIGNED_SHORT, 0);


	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//--------------------font3dprogram------------------
	//font0000,4000,8000,c000
	glUseProgram(font3dprogram);
	glUniformMatrix4fv(glGetUniformLocation(font3dprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	glUniform1i(glGetUniformLocation(font3dprogram, "texdata"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glBindVertexArray(mod[0].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glBindVertexArray(mod[1].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[1].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glBindVertexArray(mod[2].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[2].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glBindVertexArray(mod[3].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[3].ilen, GL_UNSIGNED_SHORT, 0);


//--------------------font2dprogram------------------
	glUseProgram(font2dprogram);
	glUniform1i(glGetUniformLocation(font2dprogram, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[0].tex);
	glBindVertexArray(mod[4].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[4].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[1].tex);
	glBindVertexArray(mod[5].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[5].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[2].tex);
	glBindVertexArray(mod[6].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[6].ilen, GL_UNSIGNED_SHORT, 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[3].tex);
	glBindVertexArray(mod[7].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[7].ilen, GL_UNSIGNED_SHORT, 0);

/*
	glUseProgram(directprogram);
	glUniformMatrix4fv(glGetUniformLocation(font3dprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	glUniform1i(glGetUniformLocation(font3dprogram, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glBindVertexArray(mod[4].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[4].vlen);
*/
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void callback_update()
{
//--------------------------------
/*
	win->fmt = hex64('r','g','b','a','8','8','8','8');
	win->height = 1024;
	win->width = win->stride = 1024;
	actorread(win, 0, 0, 0);

	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_RGBA, 1024, 1024, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, win->buf
	);
*/
//--------------------------------
	win->fmt = hex32('v','b','o',0);
	win->height = height;
	win->width = win->stride = width;
	actorread(win, 0, 0, 0);

	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0].ilen, mod[0].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0].vlen,mod[0].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[1].ilen, mod[1].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[1].vlen, mod[1].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[2].ilen,mod[2].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[2].vlen,mod[2].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[3].ilen, mod[3].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[3].vlen, mod[3].vbuf);


//----------------------font2d---------------------
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[4].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[4].ilen, mod[4].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[4].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[4].vlen,mod[4].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[5].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[5].ilen, mod[5].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[5].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[5].vlen, mod[5].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[6].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[6].ilen,mod[6].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[6].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[6].vlen,mod[6].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[7].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[7].ilen, mod[7].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[7].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[7].vlen, mod[7].vbuf);


//----------------------3d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x80].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x80].vlen, mod[0x80].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x81].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x81].ilen, mod[0x81].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x81].vlen, mod[0x81].vbuf);

	//drawarray: trigon
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 36*mod[0x82].vlen, mod[0x82].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x83].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x83].ilen, mod[0x83].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 36*mod[0x83].vlen, mod[0x83].vbuf);


//----------------------2d---------------------
	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x84].vlen, mod[0x84].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 4*mod[0x85].ilen, mod[0x85].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 24*mod[0x85].vlen, mod[0x85].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x86].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6*mod[0x86].ilen, mod[0x86].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x86].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0, 24*mod[0x86].vlen, mod[0x86].vbuf);
}
void windowread(void* dc,void* df,void* sc,void* sf)
{
	callback_update();
	callback_display();
	eglSwapBuffers(display, surface);
}
void windowwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
}
void windowstop()
{
	if (EGL_NO_DISPLAY != display)
	{
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT)eglDestroyContext(display, context);
		if (surface != EGL_NO_SURFACE)eglDestroySurface(display, surface);
		eglTerminate(display);
	}
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}
void windowstart(struct arena* w)
{
	win = w;
	setandroidapp(win);

	//
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);

	const EGLint configAttribs[] = {
		//		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		//		EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};
	EGLConfig config;
	EGLint numConfigs;
	eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);

	EGLint format;
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	width = ANativeWindow_getWidth(app->window);
	height = ANativeWindow_getHeight(app->window);
	ANativeWindow_setBuffersGeometry(app->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, app->window, NULL);

	EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		LOGI("eglCreateContext failed with error 0x%04x", eglGetError());
		return;
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGI("eglMakeCurrent failed with error 0x%04x", eglGetError());
		return;
	}

	LOGI("GL Version = %s\n", glGetString(GL_VERSION));
	LOGI("GL Vendor = %s\n", glGetString(GL_VENDOR));
	LOGI("GL Renderer = %s\n", glGetString(GL_RENDERER));
	LOGI("GL Extensions = %s\n", glGetString(GL_EXTENSIONS));

	w->type = hex32('w','i','n',0);
	w->fmt = hex32('v','b','o',0);
	w->irel = 0;
	w->orel = 0;

	w->mod = mod;
	w->buf = malloc(0x400000);

	w->width = width;
	w->height = height;
	w->depth = (width+height)/2;

//--------------------font3d-------------------
	//[0000,3fff]
	mod[0].vbuf = malloc(0x200000);
	mod[0].vlen = 0;
	mod[0].ibuf = malloc(0x100000);
	mod[0].ilen = 0;

	//[4000,7fff]
	mod[1].vbuf = malloc(0x200000);
	mod[1].vlen = 0;
	mod[1].ibuf = malloc(0x100000);
	mod[1].ilen = 0;

	//[8000,bfff]
	mod[2].vbuf = malloc(0x200000);
	mod[2].vlen = 0;
	mod[2].ibuf = malloc(0x100000);
	mod[2].ilen = 0;

	//[c000,ffff]
	mod[3].vbuf = malloc(0x200000);
	mod[3].vlen = 0;
	mod[3].ibuf = malloc(0x100000);
	mod[3].ilen = 0;
/*
	//2d screen
	mod[4].vbuf = malloc(0x100000);
	mod[4].vlen = 0;
*/

//--------------------font2d-------------------
	//[0000,3fff]
	mod[4].vbuf = malloc(0x200000);
	mod[4].vlen = 0;
	mod[4].ibuf = malloc(0x100000);
	mod[4].ilen = 0;

	//[4000,7fff]
	mod[5].vbuf = malloc(0x200000);
	mod[5].vlen = 0;
	mod[5].ibuf = malloc(0x100000);
	mod[5].ilen = 0;

	//[8000,bfff]
	mod[6].vbuf = malloc(0x200000);
	mod[6].vlen = 0;
	mod[6].ibuf = malloc(0x100000);
	mod[6].ilen = 0;

	//[c000,ffff]
	mod[7].vbuf = malloc(0x200000);
	mod[7].vlen = 0;
	mod[7].ibuf = malloc(0x100000);
	mod[7].ilen = 0;


//--------------------3d-------------------
	//drawarray.point
	mod[0x80].vbuf = malloc(0x100000);
	mod[0x80].vlen = 0;

	//drawelement.line
	mod[0x81].ibuf = malloc(0x100000);
	mod[0x81].ilen = 0;
	mod[0x81].vbuf = malloc(0x100000);
	mod[0x81].vlen = 0;

	//drawarray.trigon
	mod[0x82].vbuf = malloc(0x1000000);
	mod[0x82].vlen = 0;

	//drawelement.trigon
	mod[0x83].ibuf = malloc(0x100000);
	mod[0x83].ilen = 0;
	mod[0x83].vbuf = malloc(0x1000000);
	mod[0x83].vlen = 0;


//----------------------2d--------------------
	//drawarray.point
	mod[0x84].vbuf = malloc(0x100000);
	mod[0x84].vlen = 0;

	//drawelement.line
	mod[0x85].ibuf = malloc(0x100000);
	mod[0x85].ilen = 0;
	mod[0x85].vbuf = malloc(0x100000);
	mod[0x85].vlen = 0;

	//drawelement.trigon
	mod[0x86].ibuf = malloc(0x100000);
	mod[0x86].ilen = 0;
	mod[0x86].vbuf = malloc(0x100000);
	mod[0x86].vlen = 0;

	//
	initshader();
	initobject();

	//
	asset_create();
	inittexture();
}
void windowcreate()
{
}
void windowdelete()
{
}




void initwindow()
{
	app = getandroidapp();
}
void freewindow()
{
}
void inittray()
{
}
void freetray()
{
}