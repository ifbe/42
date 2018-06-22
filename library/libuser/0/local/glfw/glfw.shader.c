#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "libuser.h"




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
char font3dfrag[] = {
	"#version 300 es\n"
	"in mediump vec3 origcolor;\n"
	"in mediump vec2 texuv;\n"
	"uniform sampler2D tex2d;\n"
	"out mediump vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(origcolor,1.0)*texture(tex2d, texuv).rrrr;\n"
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
		"FragColor = vec4(origcolor,1.0)*texture(tex2d, texuv).rrrr;\n"
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




static GLuint font3dprogram;
static GLuint font2dprogram;
static GLuint simpleprogram;
static GLuint prettyprogram;
static GLuint directprogram;
static GLuint glsl2dprogram;
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
			say("Could not compile shader %d:\n%s", type, infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
	}
	return 0;
}
GLuint shaderprogram(void* v, void* f)
{
	GLuint vShader = compileShader(GL_VERTEX_SHADER, v);
	if (!vShader)return 0;

	GLuint fShader = compileShader(GL_FRAGMENT_SHADER, f);
	if (!fShader)return 0;

	//3.glsl program
	GLuint prog = glCreateProgram();
	if(0 == prog)
	{
		printf("ERROR : create program failed");
		exit(1);
	}

	glAttachShader(prog, vShader);
	glAttachShader(prog, fShader);
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
void initshader(struct arena* w)  
{
	struct texandobj* mod = w->mod;

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

	font3dprogram = shaderprogram(font3dvert, font3dfrag);
	mod[0x00].program = font3dprogram;
	mod[0x01].program = font3dprogram;
	mod[0x02].program = font3dprogram;
	mod[0x03].program = font3dprogram;

	font2dprogram = shaderprogram(font2dvert, font2dfrag);
	mod[0x04].program = font2dprogram;
	mod[0x05].program = font2dprogram;
	mod[0x06].program = font2dprogram;
	mod[0x07].program = font2dprogram;
/*
	directprogram = shaderprogram(directvert, directfrag);
	mod[0x04].program = directprogram;
*/
	simpleprogram = shaderprogram(simplevert, simplefrag);
	mod[0x80].program = simpleprogram;
	mod[0x81].program = simpleprogram;

	prettyprogram = shaderprogram(prettyvert, prettyfrag);
	mod[0x82].program = prettyprogram;
	mod[0x83].program = prettyprogram;

	glsl2dprogram = shaderprogram(glsl2dvert, glsl2dfrag);
	mod[0x84].program = glsl2dprogram;
	mod[0x85].program = glsl2dprogram;
	mod[0x86].program = glsl2dprogram;
	mod[0x87].program = glsl2dprogram;
}