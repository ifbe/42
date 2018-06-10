#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "libuser.h"
void fixmatrix(float*, void*);




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
void fixlight(struct arena* win)
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
void callback_display(struct arena* win)
{
	struct texandobj* mod;
	GLfloat cameramvp[4*4];
	fixmatrix(cameramvp, win);
	mat4_transpose((void*)cameramvp);
	mod = win->mod;


	//set
	glViewport(0, 0, win->width, win->height);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);


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
	fixlight(win);

	glBindVertexArray(mod[0x82].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[0x82].vlen);

	glBindVertexArray(mod[0x83].vao);
	glDrawElements(GL_TRIANGLES, 3*mod[0x83].ilen, GL_UNSIGNED_SHORT, 0);


	//
	glDepthMask(GL_FALSE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


//--------------------font3dprogram------------------
	glUseProgram(font3dprogram);
	glUniformMatrix4fv(glGetUniformLocation(font3dprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	glUniform1i(glGetUniformLocation(font3dprogram, "tex2d"), 0);

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
//------------------directprogram-------------------
	glUseProgram(directprogram);
	glUniformMatrix4fv(glGetUniformLocation(font3dprogram, "prettymvp"), 1, GL_FALSE, cameramvp);
	glUniform1i(glGetUniformLocation(font3dprogram, "tex2d"), 0);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_2D, mod[4].tex);
	glBindVertexArray(mod[4].vao);
	glDrawArrays(GL_TRIANGLES, 0, mod[4].vlen);
*/

	//
	glDisable(GL_BLEND);
	glDepthMask(GL_TRUE);
}