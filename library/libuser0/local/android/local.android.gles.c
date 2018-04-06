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
void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);
//
void matrixmultiply_4(float*, float*);
double squareroot(double);
double cosine(double);
double sine(double);




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
static GLuint simpleprogram;
static GLuint prettyprogram;
static GLuint myfontprogram;
//
static GLfloat modelmatrix[4*4] = {  
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
static GLfloat viewmatrix[4*4] = {  
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
};
static GLfloat projmatrix[4*4] = {  
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, -1.0f, -1.0f,
	0.0f, 0.0f, -0.2f, 0.0f
};
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
char myfontvert[] = {
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
char myfontfrag[] = {
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
	simpleprogram = createProgram(simplevert, simplefrag);
	if (!simpleprogram) {
		LOGI("Could not create program");
		return;
	}

	prettyprogram = createProgram(prettyvert, prettyfrag);
	if (!prettyprogram) {
		LOGI("Could not create program");
		return;
	}

	myfontprogram = createProgram(myfontvert, myfontfrag);
	if (!myfontprogram) {
		LOGI("Could not create program");
		return;
	}
}
void inittexture()
{
	int j;
	u8* buf;

	//[0000,3fff]
	buf = mod[0].tbuf;
	if(buf != 0)
	{
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

		//tex0
		glGenTextures(1, &(mod[0].tex));
		glActiveTexture(GL_TEXTURE0 + 0);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mod[0].tex);
		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_ALPHA, 2048, 2048, 0,
			GL_ALPHA, GL_UNSIGNED_BYTE, mod[0].tbuf
		);

		//param
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	}


	//[4000,7fff]
	buf = mod[1].tbuf;
	if(buf != 0)
	{
		for(j=0;j<0x4000;j++)
		{
			drawunicode_alpha(buf, 2048, 2048,
				(j&0x7f)<<4, (j&0xff80)>>3, j+0x4000
			);
		}

		//tex1
		glGenTextures(1, &(mod[1].tex));
		glActiveTexture(GL_TEXTURE0 + 1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mod[1].tex);
		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_RED, 2048, 2048, 0,
			GL_RED, GL_UNSIGNED_BYTE, mod[1].tbuf
		);

		//param
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	}


	//[8000,bfff]
	buf = mod[2].tbuf;
	if(buf != 0)
	{
		for(j=0;j<0x4000;j++)
		{
			drawunicode_alpha(buf, 2048, 2048,
				(j&0x7f)<<4, (j&0xff80)>>3, j+0x8000
			);
		}

		//tex2
		glGenTextures(1, &(mod[2].tex));
		glActiveTexture(GL_TEXTURE0 + 2);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mod[2].tex);
		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_RED, 2048, 2048, 0,
			GL_RED, GL_UNSIGNED_BYTE, mod[2].tbuf
		);

		//param
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	}


	//[c000,ffff]
	buf = mod[3].tbuf;
	if(buf != 0)
	{
		for(j=0;j<0x4000;j++)
		{
			drawunicode_alpha(buf, 2048, 2048,
				(j&0x7f)<<4, (j&0xff80)>>3, j+0xc000
			);
		}

		//texture3
		glGenTextures(1, &(mod[3].tex));
		glActiveTexture(GL_TEXTURE0 + 3);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mod[3].tex);
		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_RED, 2048, 2048, 0,
			GL_RED, GL_UNSIGNED_BYTE, mod[3].tbuf
		);

		//param
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

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
	}

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
void initobject()  
{
	//drawarray.point(vertex,color)
	glGenVertexArrays(1, &mod[0x81].vao);
	glBindVertexArray(mod[0x81].vao);

	glGenBuffers(1, &mod[0x81].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x81].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


	//drawarray.line(vertex,color)
	glGenVertexArrays(1, &mod[0x82].vao);
	glBindVertexArray(mod[0x82].vao);

	glGenBuffers(1, &mod[0x82].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x82].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);


	//drawarray.trigon(vertex,colour,normal)
	glGenVertexArrays(1, &mod[0x83].vao);
	glBindVertexArray(mod[0x83].vao);

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


	//drawelement.line(vertex,color)
	glGenVertexArrays(1, &mod[0x84].vao);
	glBindVertexArray(mod[0x84].vao);

	glGenBuffers(1, &mod[0x84].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x84].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x84].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x84].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x100000,
		mod[0x84].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 24, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 24, (void*)12);
	glEnableVertexAttribArray(1);



	//drawelement.trigon(vertex,color,normal)
	glGenVertexArrays(1, &mod[0x85].vao);
	glBindVertexArray(mod[0x85].vao);

	glGenBuffers(1, &mod[0x85].ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000,
		mod[0x85].ibuf, GL_STATIC_DRAW
	);

	glGenBuffers(1, &mod[0x85].vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000,
		mod[0x85].vbuf, GL_STATIC_DRAW
	);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 36, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 36, (void*)12);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 36, (void*)24);
	glEnableVertexAttribArray(2);
}




void fixmodel()
{
	//matrix = movematrix * rotatematrix * scalematrix
}
void fixview()
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;
	float cx = win->cx;
	float cy = win->cy;
	float cz = win->cz;

	//uvn.n = front
	float nx = win->fx;
	float ny = win->fy;
	float nz = win->fz;
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//uvn.u = right = cross(front,(0,0,1))
	float ux = ny*1 - nz*0;
	float uy = nz*0 - nx*1;
	float uz = nx*0 - ny*0;
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//uvn.v = above cross(right, front)
	float vx = uy*nz - uz*ny;
	float vy = uz*nx - ux*nz;
	float vz = ux*ny - uy*nx;
	norm = squareroot(vx*vx + vy*vy + vz*vz);
	vx /= norm;
	vy /= norm;
	vz /= norm;

	viewmatrix[0] = ux;
	viewmatrix[1] = vx;
	viewmatrix[2] = -nx;
	viewmatrix[3] = 0.0f;

	viewmatrix[4] = uy;
	viewmatrix[5] = vy;
	viewmatrix[6] = -ny;
	viewmatrix[7] = 0.0f;

	viewmatrix[8] = uz;
	viewmatrix[9] = vz;
	viewmatrix[10] = -nz;
	viewmatrix[11] = 0.0f;

	viewmatrix[12] = -cx*ux - cy*uy - cz*uz;
	viewmatrix[13] = -cx*vx - cy*vy - cz*vz;
	viewmatrix[14] = cx*nx + cy*ny + cz*nz;
	viewmatrix[15] = 1.0f;
}
void fixprojection()
{
/*
	cot45, 0, 0, 0,
	0, cot45, 0, 0,
	0, 0, (f+n)/(f-n), -1,
	0, 0, (2*f*n)/(f-n), 0
*/
	float w = (float)width;
	float h = (float)height;
	projmatrix[0] = h / w;
	glViewport(0, 0, width, height);
	say("width=%d,height=%d\n",width,height);
}
void fixmatrix(GLfloat* cameramvp)
{
	int x;

	fixmodel();
	fixview();
	fixprojection();

	for(x=0;x<16;x++)cameramvp[x] = modelmatrix[x];
	matrixmultiply_4(cameramvp, viewmatrix);
	matrixmultiply_4(cameramvp, projmatrix);
}
void fixlight()
{
	GLfloat light0[4] = {0.0f, 0.0f, 1000.0f};
	GLfloat ambientcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat lightcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat camera[3];
	camera[0] = win->cx;
	camera[1] = win->cy;
	camera[2] = win->cz;

	GLint ac = glGetUniformLocation(prettyprogram, "ambientcolor");
	glUniform3fv(ac, 1, ambientcolor);

	GLint dc = glGetUniformLocation(prettyprogram, "lightcolor");
	glUniform3fv(dc, 1, lightcolor);

	GLint dp = glGetUniformLocation(prettyprogram, "lightposition");
	glUniform3fv(dp, 1, light0);

	GLint ep = glGetUniformLocation(prettyprogram, "eyeposition");
	glUniform3fv(ep, 1, camera);
}
void callback_display()
{
	GLfloat cameramvp[4*4];
	fixmatrix(cameramvp);

	//set
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//point,line
	glUseProgram(simpleprogram);
	GLint mvp1 = glGetUniformLocation(simpleprogram, "simplemvp");
	glUniformMatrix4fv(mvp1, 1, GL_FALSE, cameramvp);

	glBindVertexArray(mod[0x81].vao);
	glDrawArrays(GL_POINTS, 0, mod[0x81].vlen);

	//glBindVertexArray(mod[0x82].vao);
	//glDrawArrays(GL_LINES, 0, 2*mod[0x82].vlen);

	glBindVertexArray(mod[0x84].vao);
	glDrawElements(GL_LINES, 2*mod[0x84].ilen, GL_UNSIGNED_SHORT, 0);

	//stl,triangle
	glUseProgram(prettyprogram);
	fixlight();

	GLint mvp2 = glGetUniformLocation(prettyprogram, "prettymvp");
	glUniformMatrix4fv(mvp2, 1, GL_FALSE, cameramvp);

	//glBindVertexArray(mod[0x83].vao);
	//glDrawArrays(GL_TRIANGLES, 0, 3*mod[0x83].vlen);

	glBindVertexArray(mod[0x85].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x85].ilen, GL_UNSIGNED_SHORT, 0);

	//font0000,4000,8000,c000
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(myfontprogram);

	GLint mvp3 = glGetUniformLocation(myfontprogram, "prettymvp");
	glUniformMatrix4fv(mvp3, 1, GL_FALSE, cameramvp);

	glUniform1i(glGetUniformLocation(myfontprogram, "texdata"), 0);
	glBindVertexArray(mod[0].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0].ilen, GL_UNSIGNED_SHORT, 0);

	glUniform1i(glGetUniformLocation(myfontprogram, "texdata"), 1);
	glBindVertexArray(mod[1].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[1].ilen, GL_UNSIGNED_SHORT, 0);

	glUniform1i(glGetUniformLocation(myfontprogram, "texdata"), 2);
	glBindVertexArray(mod[2].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[2].ilen, GL_UNSIGNED_SHORT, 0);

	glUniform1i(glGetUniformLocation(myfontprogram, "texdata"), 3);
	glBindVertexArray(mod[3].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[3].ilen, GL_UNSIGNED_SHORT, 0);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}
void callback_update()
{
	//font0000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
		6*mod[0].ilen, mod[0].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		36*mod[0].vlen,mod[0].vbuf);

	//font4000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[1].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
		6*mod[1].ilen, mod[1].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[1].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		36*mod[1].vlen, mod[1].vbuf);

	//font8000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[2].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
		6*mod[2].ilen,mod[2].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[2].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		36*mod[2].vlen,mod[2].vbuf);

	//fontc000
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[3].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
		6*mod[3].ilen, mod[3].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[3].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		36*mod[3].vlen, mod[3].vbuf);

	//drawarray: point
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x81].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		24*mod[0x81].vlen, mod[0x81].vbuf);

	//drawarray: line
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x82].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		24*mod[0x82].vlen, mod[0x82].vbuf);

	//drawarray: trigon
	glBindBuffer(GL_ARRAY_BUFFER, mod[0x83].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		36*mod[0x83].vlen, mod[0x83].vbuf);

	//drawelement: line
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x84].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
		4*mod[0x84].ilen, mod[0x84].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x84].vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0,
		24*mod[0x84].vlen, mod[0x84].vbuf);

	//drawelement: trigon
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mod[0x85].ibo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
		6*mod[0x85].ilen, mod[0x85].ibuf);

	glBindBuffer(GL_ARRAY_BUFFER, mod[0x85].vbo);
	glBufferSubData(GL_ARRAY_BUFFER,0,
		36*mod[0x85].vlen, mod[0x85].vbuf);

}
void windowread(void* dc,void* df,void* sc,void* sf)
{
	struct arena* win = sc;
	actorread(win, 0, 0, 0);

/*
	float x,y;
	x = camera[0];
	y = camera[1];
	camera[0] = cos(0.1)*x - sin(0.1)*y;
	camera[1] = sin(0.1)*x + cos(0.1)*y;

	x = trianglevertex[1][0];
	y = trianglevertex[1][1];
	trianglevertex[1][0] = cos(0.1)*x - sin(0.1)*y;
	trianglevertex[1][1] = sin(0.1)*x + cos(0.1)*y;

	x = trianglevertex[2][0];
	y = trianglevertex[2][1];
	trianglevertex[2][0] = cos(0.1)*x - sin(0.1)*y;
	trianglevertex[2][1] = sin(0.1)*x + cos(0.1)*y;

	glClearColor(1.0f, 1.0f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(program);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, trianglevertex);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, trianglecolour);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, trianglenormal);
	glEnableVertexAttribArray(2);
	glDrawArrays(GL_TRIANGLES, 0, 3);
*/
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

	w->width = width;
	w->height = height;
	w->depth = (width+height)/2;

	w->cx = 512.0;
	w->cy = -512.0;
	w->cz = 512.0;

	w->fx = -(w->cx);
	w->fy = -(w->cy);
	w->fz = -(w->cz);

	w->ux = 0.0;
	w->uy = 0.0;
	w->uz = 1.0;

	w->len = 0x100;
	w->buf = mod;

	//[0000,3fff]
	mod[0].tbuf = malloc(0x400000);
	mod[0].tlen = 0x400000;
	mod[0].vbuf = malloc(0x200000);
	mod[0].vlen = 0;
	mod[0].ibuf = malloc(0x100000);
	mod[0].ilen = 0;

	//[4000,7fff]
	mod[1].tbuf = malloc(0x400000);
	mod[1].tlen = 0x400000;
	mod[1].vbuf = malloc(0x200000);
	mod[1].vlen = 0;
	mod[1].ibuf = malloc(0x100000);
	mod[1].ilen = 0;

	//[8000,bfff]
	mod[2].tbuf = malloc(0x400000);
	mod[2].tlen = 0x400000;
	mod[2].vbuf = malloc(0x200000);
	mod[2].vlen = 0;
	mod[2].ibuf = malloc(0x100000);
	mod[2].ilen = 0;

	//[c000,ffff]
	mod[3].tbuf = malloc(0x400000);
	mod[3].tlen = 0x400000;
	mod[3].vbuf = malloc(0x200000);
	mod[3].vlen = 0;
	mod[3].ibuf = malloc(0x100000);
	mod[3].ilen = 0;

	//drawarray.point
	mod[0x81].vbuf = malloc(0x100000);
	mod[0x81].vlen = 0;

	//drawarray.line
	mod[0x82].vbuf = malloc(0x100000);
	mod[0x82].vlen = 0;

	//drawarray.trigon
	mod[0x83].vbuf = malloc(0x1000000);
	mod[0x83].vlen = 0;

	//drawelement.line
	mod[0x84].ibuf = malloc(0x100000);
	mod[0x84].ilen = 0;
	mod[0x84].vbuf = malloc(0x100000);
	mod[0x84].vlen = 0;

	//drawelement.trigon
	mod[0x85].ibuf = malloc(0x100000);
	mod[0x85].ilen = 0;
	mod[0x85].vbuf = malloc(0x1000000);
	mod[0x85].vlen = 0;

	initshader();
	inittexture();
	initobject();
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