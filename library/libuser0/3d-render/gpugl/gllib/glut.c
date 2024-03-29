#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




void* supply_alloc();
void* supply_recycle(void*);
//
void drawascii_alpha(void* buf, int w, int h, int x, int y, u8 c);
void drawunicode_alpha(void* buf, int w, int h, int x, int y, u32 c);
void matrixmultiply_4(float*, float*);
void quaternionnormalize(float*);
void quaternionrotate(float*, float*);
//
void vectornormalize(float*);
void vectorcross(float*, float*);
//
double squareroot(double);
double getcos(double);
double getsin(double);




//
static _obj* win;
static _obj* src;
static u8* fontdata;
//
static int queuehead = 0;
static int queuetail = 0;
static int width = 512;
static int height = 512;
static int last_x = 0;
static int last_y = 0;
//
static GLuint simpleprogram;
static GLuint prettyprogram;
static GLuint shadowprogram;
static GLuint pickerprogram;
static GLuint myfontprogram;
//
static GLuint shadowfb;
//
static GLuint simpletexture;
static GLuint prettytexture;
static GLuint shadowtexture;
static GLuint pickertexture;
static GLuint myfonttexture;
//
static GLuint pointvao;
static GLuint linevao;
static GLuint trianglevao;
static GLuint fontvao;
//
static GLuint vertexvbo;
static GLuint normalvbo;
static GLuint colourvbo;
static GLuint texcorvbo;
//
static GLuint pointvbo;
static GLuint linevbo;
static GLuint trianglevbo;
static GLuint fontvbo;
//
static float light0[4] = {0.0f, 0.0f, 1000.0f};
static float light1[4] = {0.0f, 10.0f, 0.0f};
static float light2[4] = {10.0f, 0.0f, 0.0f};
static float camera[4] = {50.0f, -100.0f, 70.0f};
static float center[4] = {0.0f, 0.0f, 0.0f};
static float above[4] = {0.0f, 0.0f, 1.0f};
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
static GLfloat cameramvp[4*4];
static GLfloat light0mvp[4*4];




char simplevert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 position;\n"
	"layout(location = 2)in mediump vec3 color;\n"
	"uniform mat4 simplemvp;\n"
	"out mediump vec3 vertexcolor;\n"
	"void main()\n"
	"{\n"
		"vertexcolor = color;\n"
		"gl_Position = simplemvp * vec4(position,1.0);\n"
	"}\n"
};
char simplefrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vertexcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vertexcolor,1.0);\n"
	"}\n"
};
char prettyvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 position;\n"
	"layout(location = 1)in mediump vec3 normal;\n"
	"layout(location = 2)in mediump vec3 color;\n"
	"uniform mat4 prettymvp;\n"
	"uniform mat4 light0mvp;\n"
	"uniform mediump vec3 ambientcolor;\n"
	"uniform mediump vec3 lightcolor;\n"
	"uniform mediump vec3 lightposition;\n"
	"uniform mediump vec3 eyeposition;\n"
	"out mediump vec3 vertexcolor;\n"
	"void main()\n"
	"{\n"
		"mediump vec3 N = normalize(normal);\n"
		"mediump vec3 L = normalize(vec3(lightposition - position));\n"
		"mediump vec3 E = normalize(eyeposition-position);\n"
		"mediump vec3 R = reflect(-L, N);\n"
		"mediump float SN = max(dot(N, L), 0.0);\n"
		"mediump float RV = max(dot(R, E), 0.0);\n"
		"mediump vec3 ambient = ambientcolor;\n"
		"mediump vec3 diffuse = lightcolor * SN;\n"
		"mediump vec3 specular = vec3(0.0, 0.0, 0.0);\n"
		"if(SN>0.0)specular = lightcolor * pow(RV, 8.0);\n"
		"vertexcolor = color*(ambient + diffuse + specular);\n"
		"gl_Position = prettymvp * vec4(position,1.0);\n"
	"}\n"
};
char prettyfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vertexcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vertexcolor,1.0);\n"
	"}\n"
};
char shadowvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 position;\n"
	"uniform mat4 lightmvp;\n"
	"uniform sampler2D shadowmap;\n"
	"out mediump vec3 vertexcolor;\n"
	"void main()\n"
	"{\n"
		"vertexcolor = position;\n"
		"gl_Position = vec4(position,1.0);\n"
	"}\n"
};
char shadowfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vertexcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vertexcolor,1.0);\n"
	"}\n"
};
char pickervert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 position;\n"
	"out mediump vec3 vertexcolor;\n"
	"void main()\n"
	"{\n"
		"vertexcolor = position;\n"
		"gl_Position = vec4(position,1.0);\n"
	"}\n"
};
char pickerfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 vertexcolor;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vertexcolor,1.0);\n"
	"}\n"
};
char myfontvert[] = {
	"#version 300 es\n"
	"layout(location = 0)in mediump vec3 position;\n"
	"layout(location = 2)in mediump vec3 color;\n"
	"layout(location = 3)in mediump vec2 texcoord;\n"
	"uniform mat4 prettymvp;\n"
	"out mediump vec3 origcolor;\n"
	"out mediump vec2 texuv;\n"
	"void main()\n"
	"{\n"
		"gl_Position = prettymvp * vec4(position,1.0);\n"
		"origcolor = color;\n"
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
void initshader_one(GLuint* prog, void* vert, void* frag)
{
	//1.vertex shader
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	if(0 == vShader)
	{
		printf("ERROR : Create vertex shader failed\n");
		exit(1);
	}

	const GLchar* vCodeArray[1] = {vert};
	glShaderSource(vShader, 1, vCodeArray, NULL);
	glCompileShader(vShader);

	GLint compileResult;
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &compileResult);
	if (GL_FALSE == compileResult)
	{
		GLint logLen;
		//得到编译日志长度
		glGetShaderiv(vShader, GL_INFO_LOG_LENGTH, &logLen);
		if (logLen > 0)
		{
			GLsizei written;
			char *log = (char*)malloc(logLen);

			//得到日志信息并输出
			glGetShaderInfoLog(vShader, logLen, &written, log);
			printf("vertex shader compile log: %s\n",log);
			free(log);
		}
	}

	//2.fragment shader
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	if (0 == fShader)
	{
		printf("ERROR : Create fragment shader failed");
		exit(1);
	}

	const GLchar* fCodeArray[1] = {frag};
	glShaderSource(fShader, 1, fCodeArray, NULL);
	glCompileShader(fShader);

	glGetShaderiv(fShader, GL_COMPILE_STATUS, &compileResult);
	if(GL_FALSE == compileResult)
	{
		GLint logLen;
		glGetShaderiv(fShader, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0)
		{
			GLsizei written;
			char *log = (char*)malloc(logLen);

			glGetShaderInfoLog(fShader, logLen, &written, log);
			printf("fragment shader compile log: %s\n",log);
			free(log);
		}
	}

	//3.glsl program
	*prog = glCreateProgram();
	if(*prog == 0)
	{
		printf("ERROR : create program failed");
		exit(1);
	}

	glAttachShader(*prog, vShader);
	glAttachShader(*prog, fShader);
	glLinkProgram(*prog);

	GLint linkStatus;
	glGetProgramiv(*prog, GL_LINK_STATUS, &linkStatus);
	if(GL_FALSE == linkStatus)
	{
		printf("ERROR : link shader program failed");
		GLint logLen;
		glGetProgramiv(*prog, GL_INFO_LOG_LENGTH, &logLen);
		if(logLen > 0)
		{
			char *log = (char*)malloc(logLen);
			GLsizei written;
			glGetProgramInfoLog(*prog, logLen, &written, log);
			printf("Program log :%s\n", log);
		}
	}
}
void initshader()  
{  
	//1.check version
	const GLubyte *renderer = glGetString( GL_RENDERER );
	const GLubyte *vendor = glGetString( GL_VENDOR );
	const GLubyte *version = glGetString( GL_VERSION );
	const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );
	GLint major, minor;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	printf("GL Vendor: %s\n", vendor);
	printf("GL Renderer: %s\n", renderer);
	printf("GL Version (string): %s\n", version);
	printf("GLSL Version: %s\n", glslVersion);
	printf("GL Version (integer): %x.%x\n", major, minor);

	initshader_one(&simpleprogram, simplevert, simplefrag);
	initshader_one(&prettyprogram, prettyvert, prettyfrag);
	initshader_one(&shadowprogram, shadowvert, shadowfrag);
	initshader_one(&pickerprogram, pickervert, pickerfrag);
	initshader_one(&myfontprogram, myfontvert, myfontfrag);
	glUseProgram(prettyprogram);
}
void inittexture()
{
	int j;
	glGenTextures(1, &myfonttexture);
	glBindTexture(GL_TEXTURE_2D, myfonttexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	fontdata = malloc(0x1000*0x1000);
	if(fontdata != 0)
	{
		for(j=0;j<0x10000;j++)
		{
			drawunicode_alpha(
				fontdata, 0x1000, 0x1000,
				(j&0xff)<<4, (j&0xff00)>>4, j
			);
		}
		for(j=0x20;j<0x80;j++)
		{
			drawascii_alpha(
				fontdata, 0x1000, 0x1000,
				(j&0xff)<<4, 0, j
			);
		}
		glTexImage2D(GL_TEXTURE_2D, 0,
			GL_ALPHA, 0x1000, 0x1000, 0,
			GL_ALPHA, GL_UNSIGNED_BYTE, fontdata
		);
	}

/*
	glGenTextures(1, &shadowtexture);
	glBindTexture(GL_TEXTURE_2D, shadowtexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0,
		GL_DEPTH_COMPONENT, 1024, 1024, 0,
		GL_DEPTH_COMPONENT, GL_FLOAT, NULL
	);
*/
/*
	glGenFramebuffers(1, &shadowfb);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowfb);

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
	void* temp = (void*)(src->buf);

	//[0m,2m) vertex
	glGenBuffers(1, &vertexvbo);
	glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, temp, GL_STATIC_DRAW);

	//[2m,4m) normal
	glGenBuffers(1, &normalvbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, temp, GL_STATIC_DRAW);

	//[4m,6m) color
	glGenBuffers(1, &colourvbo);
	glBindBuffer(GL_ARRAY_BUFFER, colourvbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, temp, GL_STATIC_DRAW);

	//[6m,8m) texuv
	glGenBuffers(1, &texcorvbo);
	glBindBuffer(GL_ARRAY_BUFFER, texcorvbo);
	glBufferData(GL_ARRAY_BUFFER, 0x1000000, temp, GL_STATIC_DRAW);

	//[8m,10m) point
	glGenVertexArrays(1,&pointvao);
	glBindVertexArray(pointvao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourvbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &pointvbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointvbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, temp, GL_STATIC_DRAW);

	//[10m,12m) line
	glGenVertexArrays(1,&linevao);
	glBindVertexArray(linevao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourvbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &linevbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linevbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, temp, GL_STATIC_DRAW);

	//[12m,14m) triangle
	glGenVertexArrays(1,&trianglevao);
	glBindVertexArray(trianglevao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourvbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glGenBuffers(1, &trianglevbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglevbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, temp, GL_STATIC_DRAW);

	//[14m,16m) font
	glGenVertexArrays(1,&fontvao);
	glBindVertexArray(fontvao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, colourvbo);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, texcorvbo);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(3);

	glGenBuffers(1, &fontvbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fontvbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, temp, GL_STATIC_DRAW);
}




void fixmodel()
{
	//matrix = movematrix * rotatematrix * scalematrix
}
void fixview()
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;

	//Z = center - camera
	float nx = center[0] - camera[0];
	float ny = center[1] - camera[1];
	float nz = center[2] - camera[2];
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//X = cross(Z, above)
	float ux = ny*above[2] - nz*above[1];
	float uy = nz*above[0] - nx*above[2];
	float uz = nx*above[1] - ny*above[0];
	norm = squareroot(ux*ux + uy*uy + uz*uz);
	ux /= norm;
	uy /= norm;
	uz /= norm;

	//above = cross(X, Z)
	float vx = uy*nz - uz*ny;
	float vy = uz*nx - ux*nz;
	float vz = ux*ny - uy*nx;

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

	viewmatrix[12] = -camera[0]*ux - camera[1]*uy - camera[2]*uz;
	viewmatrix[13] = -camera[0]*vx - camera[1]*vy - camera[2]*vz;
	viewmatrix[14] = camera[0]*nx + camera[1]*ny + camera[2]*nz;
	viewmatrix[15] = 1.0f;
/*
	viewmatrix[0] = cos(camera[0]);
	viewmatrix[2] = -sin(camera[0]);
	viewmatrix[8] = sin(camera[0]);
	viewmatrix[10] = cos(camera[0]);
	viewmatrix[14] = -1.0f;
*/
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
}
void fixmatrix()
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
	GLfloat ambientcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat lightcolor[3] = {0.5f, 0.5f, 0.5f};

	GLint ac = glGetUniformLocation(prettyprogram, "ambientcolor");
	glUniform3fv(ac, 1, ambientcolor);

	GLint dc = glGetUniformLocation(prettyprogram, "lightcolor");
	glUniform3fv(dc, 1, lightcolor);

	GLint dp = glGetUniformLocation(prettyprogram, "lightposition");
	glUniform3fv(dp, 1, light0);

	GLint ep = glGetUniformLocation(prettyprogram, "eyeposition");
	glUniform3fv(ep, 1, camera);
}
void fixtexture()
{
	//GLint t0 = glGetUniformLocation(u, "texture0");
	//glUniform1i(t0, 0);
	//GLint t1 = glGetUniformLocation(u, "texture0");
	//glUniform1i(t1, 0);
}
void callback_display()
{
	//set
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	fixmatrix();
	fixlight();
	fixtexture();

	//point&line
	glUseProgram(simpleprogram);
	GLint mvp1 = glGetUniformLocation(simpleprogram, "simplemvp");
	glUniformMatrix4fv(mvp1, 1, GL_FALSE, cameramvp);

	glBindVertexArray(pointvao);
	glDrawElements(GL_POINTS, src->pointcount, GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(linevao);
	glDrawElements(GL_LINES, src->linecount, GL_UNSIGNED_SHORT, 0);

	//triangle
	glUseProgram(prettyprogram);
	GLint mvp2 = glGetUniformLocation(prettyprogram, "prettymvp");
	glUniformMatrix4fv(mvp2, 1, GL_FALSE, cameramvp);

	glBindVertexArray(trianglevao);
	glDrawElements(GL_TRIANGLES, src->tricount, GL_UNSIGNED_SHORT, 0);

	//texture
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUseProgram(myfontprogram);

	GLint mvp3 = glGetUniformLocation(myfontprogram, "prettymvp");
	glUniformMatrix4fv(mvp3, 1, GL_FALSE, cameramvp);
	GLint tex = glGetUniformLocation(myfontprogram, "texdata");
	glUniform1i(tex, 0);

	glBindVertexArray(fontvao);
	glDrawElements(GL_TRIANGLES, src->fontcount, GL_UNSIGNED_SHORT, 0);

	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);

	//write
	glFlush();
	glutSwapBuffers();
}
void callback_idle()
{
	u64 vertexcount, normalcount, colourcount, texcorcount;
	u64 pointcount, linecount, tricount, fontcount;
	float* vertexdata;
	float* normaldata;
	float* colourdata;
	float* texcordata;
	u16* pointindex;
	u16* lineindex;
	u16* triindex;
	u16* fontindex;

	if(queuehead == queuetail)return;

	vertexcount = src->vertexcount;
	normalcount = src->normalcount;
	colourcount = src->colourcount;
	texcorcount = src->texcorcount;

	pointcount = src->pointcount;
	linecount = src->linecount;
	tricount = src->tricount;
	fontcount = src->fontcount;

	vertexdata = (void*)(src->buf)+0x000000;
	normaldata = (void*)(src->buf)+0x200000;
	colourdata = (void*)(src->buf)+0x400000;
	texcordata = (void*)(src->buf)+0x600000;

	pointindex = (void*)(src->buf)+0x800000;
	lineindex = (void*)(src->buf)+0xa00000;
	triindex = (void*)(src->buf)+0xc00000;
	fontindex = (void*)(src->buf)+0xe00000;

	glBindBuffer(   GL_ARRAY_BUFFER, vertexvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*vertexcount, vertexdata);

	glBindBuffer(   GL_ARRAY_BUFFER, normalvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*normalcount, normaldata);

	glBindBuffer(   GL_ARRAY_BUFFER, colourvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*colourcount, colourdata);

	glBindBuffer(   GL_ARRAY_BUFFER, texcorvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*texcorcount, texcordata);

	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, pointvbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*pointcount, pointindex);

	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, linevbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*linecount, lineindex);

	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, trianglevbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*tricount, triindex);

	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, fontvbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*fontcount, fontindex);

	queuetail++;
	glutPostRedisplay();
}
void callback_reshape(int w, int h)
{
	width = w;
	height = h;
}
void callback_keyboard(unsigned char key, int x, int y)
{
	u64 what,where;
	where = (u64)win;
	//printf("%x\n",key);

	if(key == 0x1b)
	{
		what = hex32('k','b','d',0);
	}
	else
	{
		what = hex32('c','h','a','r');
	}
	eventwrite(key, what, where, 0);
}
void callback_special(int key, int x, int y)
{
	u64 what,where;
	where = (u64)win;
	//printf("%x\n",key);

	if(key == GLUT_KEY_UP)key = 0x48;
	else if(key == GLUT_KEY_LEFT)key = 0x4b;
	else if(key == GLUT_KEY_RIGHT)key = 0x4d;
	else if(key == GLUT_KEY_DOWN)key = 0x50;
	else if(key == GLUT_KEY_F1)key = 0xf1;
	else if(key == GLUT_KEY_F2)key = 0xf2;
	else if(key == GLUT_KEY_F3)key = 0xf3;
	else if(key == GLUT_KEY_F4)key = 0xf4;
	else if(key == GLUT_KEY_F5)key = 0xf5;
	else if(key == GLUT_KEY_F6)key = 0xf6;
	else if(key == GLUT_KEY_F7)key = 0xf7;
	else if(key == GLUT_KEY_F8)key = 0xf8;
	else if(key == GLUT_KEY_F9)key = 0xf9;
	else if(key == GLUT_KEY_F10)key = 0xfa;
	else if(key == GLUT_KEY_F11)key = 0xfb;
	else if(key == GLUT_KEY_F12)key = 0xfc;
	else return;

	what = 0x64626b;
	eventwrite(key, what, where, 0);
}
void callback_mouse(int button, int state, int x, int y)
{
	float tx, ty, tz;
	u64 xx,yy,temp;
	u64 why, what, where;
	//printf("1111: %x,%x\n",x,y);

	if(win->flag0 == 12)
	{
		where = (u64)win;
		xx = x&0xffff;
		yy = (height - y)&0xffff;

		if(state == GLUT_DOWN)
		{
			if(button == 0)
			{
				temp = 'l';
				why = xx + (yy<<16) + (temp<<48);
				eventwrite(why, 0x2b70, where, 0);
			}
		}
		else
		{
			if(button == 0)
			{
				temp = 'l';
				why = xx + (yy<<16) + (temp<<48);
				eventwrite(why, 0x2d70, where, 0);
			}
			else if(button == 3)	//wheel_up
			{
				temp = 'f';
				why = xx + (yy<<16) + (temp<<48);
				eventwrite(why, 0x2b70, where, 0);
			}
			else if(button == 4)	//wheel_down
			{
				temp = 'b';
				why = xx + (yy<<16) + (temp<<48);
				eventwrite(why, 0x2b70, where, 0);
			}
		}
		return;
	}

	if(state == GLUT_DOWN)
	{
		last_x = x;
		last_y = y;
	}
	if(state == GLUT_UP)
	{
		tx = camera[0];
		ty = camera[1];
		tz = camera[2];
		if(button == 3)	//wheel_up
		{
			camera[0] = 0.9*tx + 0.1*center[0];
			camera[1] = 0.9*ty + 0.1*center[1];
			camera[2] = 0.9*tz + 0.1*center[2];

			//camera_zoom *= 0.95;
			glutPostRedisplay();
		}
		else if(button == 4)	//wheel_down
		{
			camera[0] = 1.1*tx - 0.1*center[0];
			camera[1] = 1.1*ty - 0.1*center[1];
			camera[2] = 1.1*tz - 0.1*center[2];

			//camera_zoom *= 1.05263158;
			glutPostRedisplay();
		}
	}
}
void callback_move(int x,int y)
{
	float t[3];
	float v[4];
	u64 xx,yy,temp;
	u64 why, what, where;
	//printf("2222: %d,%d\n",x,y);

	if(win->flag0 == 12)
	{
		where = (u64)win;
		xx = x&0xffff;
		yy = (height - y)&0xffff;

		temp = 'l';
		why = xx + (yy<<16) + (temp<<48);
		eventwrite(why, 0x4070, where, 0);
		return;
	}

	t[0] = 0.0;
	t[1] = 0.0;
	t[2] = 1.0;
	v[0] = camera[0];
	v[1] = camera[1];
	v[2] = camera[2];
	v[3] = 0.0;
	if(x>last_x)
	{
		camera[0] = v[0]*getcos(0.05f) + v[1]*getsin(0.05f);
		camera[1] = -v[0]*getsin(0.05f) + v[1]*getcos(0.05f);

		//camera_yaw += PI/90;
	}
	else if(x<last_x)
	{
		camera[0] = v[0]*getcos(0.05f) - v[1]*getsin(0.05f);
		camera[1] = v[0]*getsin(0.05f) + v[1]*getcos(0.05f);

		//camera_yaw -= PI/90;
	}

	if(y > last_y)
	{
		vectorcross(v, t);
		vectornormalize(v);

		v[0] *= getsin(0.02f);
		v[1] *= getsin(0.02f);
		v[2] *= getsin(0.02f);
		v[3] = getcos(0.02f);
		quaternionrotate(camera, v);
	}
	else if(y<last_y)
	{
		vectorcross(v, t);
		vectornormalize(v);

		v[0] *= getsin(-0.02f);
		v[1] *= getsin(-0.02f);
		v[2] *= getsin(-0.02f);
		v[3] = getcos(-0.02f);
		quaternionrotate(camera, v);
	}

	last_x = x;
	last_y = y;
	glutPostRedisplay();
}




void* uievent(_obj* p)
{
	int ret;
	int argc=1;
	char* argv[2];
	argv[0] = "./a.out";
	argv[1] = 0;

	//glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(p->w, p->h);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("42");
	glEnable(GL_DEPTH_TEST);

	//glew
	ret = glewInit();

	//
	initshader();
	inittexture();
	initobject();

	//绘制与显示
	glutIdleFunc(callback_idle);
	glutDisplayFunc(callback_display);
	glutReshapeFunc(callback_reshape);
	glutKeyboardFunc(callback_keyboard);
	glutSpecialFunc(callback_special);
	glutMouseFunc(callback_mouse);
	glutMotionFunc(callback_move);
	glutMainLoop();
}




void window_take(_obj* wnd)
{
}
void window_give(_obj* wnd)
{
	queuehead++;
}
void window_attach()
{
}
void window_detach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete()
{
}
void window_create(_obj* this)
{
	if(this->type == hex32('b','u','f',0))
	{
		src = this;
		return;
	}
	win = this;
	this->type = hex32('w','i','n',0);
	//this->fmt = hex64('r','g','b','a','8','8','8','8');
	this->fmt = hex32('v','b','o',0);
	//this->fmt = hex32('s','t','l',0);

	this->w = 512;
	this->h = 512;
	startthread(uievent, this);
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
}
void freewindow()
{
}
