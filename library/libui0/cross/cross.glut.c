#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "arena.h"
#define PI 3.1415926535897932384626433832795028841971693993151




//
void matrixmultiply_4(float*, float*);
void quaternionnormalize(float*);
void quaternionrotate(float*, float*);
//
void vectornormalize(float*);
void vectorcross(float*, float*);
float vectordot(float*, float*);
float vectorcosine(float*, float*);
//
double squareroot(double);
double cosine(double);
double sine(double);
//
int readfile(u64, void*, u64, u64);
int writefile(u64, void*, u64, u64);
u64 startthread(void*, void*);
void sleep_us(int);
//
void eventwrite(u64,u64,u64,u64);
void printmemory(void*, int);
void say(void*, ...);




//
static struct window* win;
static struct window* src;
static int queuehead = 0;
static int queuetail = 0;
static int last_x = 0;
static int last_y = 0;
//
static GLuint vShader;
static GLuint fShader;
static GLuint programhandle;
static GLuint picturehandle;
//
static GLuint vertexvbo;
static GLuint normalvbo;
static GLuint colorvbo;
static GLuint texturevbo;
//
static GLuint pointvbo;
static GLuint linevbo;
static GLuint trianglevbo;
static GLuint rectanglevbo;
//
static GLuint pointvao;
static GLuint linevao;
static GLuint trianglevao;
static GLuint rectanglevao;
//
static float camera[4] = {1.0f, -2.0f, 1.0f};
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




char vCode[] = {
	"#version 400\n"
	"layout(location = 0)in vec3 position;\n"
	"layout(location = 1)in vec3 normal;\n"
	"layout(location = 2)in vec3 color;\n"
	"uniform vec3 ambientcolor;\n"
	"uniform vec3 lightcolor;\n"
	"uniform vec3 lightposition;\n"
	"uniform vec3 eyeposition;\n"
	"uniform mat4 modelviewproj;\n"
	"uniform mat4 normalmatrix;\n"
	"out vec3 vertexcolor;\n"
	"void main()\n"
	"{\n"
		"vec3 N = normalize(normal);\n"
		"vec3 L = normalize(vec3(lightposition - position));\n"
		"vec3 E = normalize(eyeposition-position);\n"
		"vec3 R = reflect(-L, N);\n"
		"float SN = max(dot(N, L), 0.0);\n"
		"float RV = max(dot(R, E), 0.0);\n"
		"vec3 ambient = ambientcolor;\n"
		"vec3 diffuse = lightcolor * SN;\n"
		"vec3 specular = vec3(0.0, 0.0, 0.0);\n"
		"if(SN>0.0)specular = lightcolor * pow(RV, 8);\n"
		"vertexcolor = color*(ambient + diffuse + specular);\n"
		"gl_Position = modelviewproj * vec4(position,1.0);\n"
	"}\n"
};
char fCode[] = {
	"#version 400\n"
	"in vec3 vertexcolor;\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
		"FragColor = vec4(vertexcolor,1.0);\n"
	"}\n"
};
void initShader()  
{  
    //1.查看GLSL和OpenGL的版本  
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

    //2.顶点着色器  
    vShader = glCreateShader(GL_VERTEX_SHADER);
    if(0 == vShader)
    {
        printf("ERROR : Create vertex shader failed\n");
        exit(1);
    }

    //把着色器源代码和着色器对象相关联
	const GLchar* vCodeArray[1] = {vCode};
    glShaderSource(vShader, 1, vCodeArray, NULL);
    glCompileShader(vShader);

    //检查编译是否成功
    GLint compileResult;
    glGetShaderiv(vShader,GL_COMPILE_STATUS,&compileResult);
    if (GL_FALSE == compileResult)
    {
        GLint logLen;
        //得到编译日志长度
        glGetShaderiv(vShader,GL_INFO_LOG_LENGTH,&logLen);
        if (logLen > 0)
        {
            GLsizei written;
            char *log = (char *)malloc(logLen);

            //得到日志信息并输出
            glGetShaderInfoLog(vShader,logLen,&written,log);
            printf("vertex shader compile log: %s\n",log);
            free(log);
        }
    }

    //3.片断着色器
    fShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (0 == fShader)
    {
        printf("ERROR : Create fragment shader failed");
        exit(1);
    }

    //把着色器源代码和着色器对象相关联
	const GLchar* fCodeArray[1] = {fCode};
    glShaderSource(fShader, 1, fCodeArray, NULL);
    glCompileShader(fShader);

    //检查编译是否成功
    glGetShaderiv(fShader,GL_COMPILE_STATUS,&compileResult);
    if(GL_FALSE == compileResult)
    {
        //得到编译日志长度
        GLint logLen;
        glGetShaderiv(fShader,GL_INFO_LOG_LENGTH,&logLen);
        if(logLen > 0)
        {
            GLsizei written;
            char *log = (char *)malloc(logLen);

            //得到日志信息并输出
            glGetShaderInfoLog(fShader,logLen,&written,log);
            printf("fragment shader compile log: %s\n",log);
            free(log);
        }
    }
  
    //4.着色器程序
    programhandle = glCreateProgram();
    if(!programhandle)
    {
        printf("ERROR : create program failed");
        exit(1);
    }

    //将着色器程序链接到所创建的程序中
    glAttachShader(programhandle,vShader);
    glAttachShader(programhandle,fShader);
    glLinkProgram(programhandle);

    //查询链接的结果
    GLint linkStatus;
    glGetProgramiv(programhandle,GL_LINK_STATUS,&linkStatus);
    if(GL_FALSE == linkStatus)
    {
        printf("ERROR : link shader program failed");
        GLint logLen;
        glGetProgramiv(programhandle,GL_INFO_LOG_LENGTH, &logLen);
        if(logLen > 0)
        {
            char *log = (char *)malloc(logLen);
            GLsizei written;
            glGetProgramInfoLog(programhandle,logLen, &written,log);
            printf("Program log :%s\n", log);
        }
    }
    else	//链接成功，在OpenGL管线中使用渲染程序
    {
        glUseProgram(programhandle);
    }
}
void initVBO()  
{
	void* picture = (void*)(src->buf);
	void* vertexxyz = (void*)(src->buf)+0x800000;
	void* normalxyz = (void*)(src->buf)+0x900000;
	void* colorrgb = (void*)(src->buf)+0xa00000;
	void* texturexyz = (void*)(src->buf)+0xb00000;

	void* pointindex = (void*)(src->buf)+0xc00000;
	void* lineindex = (void*)(src->buf)+0xd00000;
	void* triangleindex = (void*)(src->buf)+0xe00000;
	void* rectangleindex = (void*)(src->buf)+0xf00000;




	//[0]picture
	glGenTextures(1, &picturehandle);
	glBindTexture(GL_TEXTURE_2D, picturehandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

    //[8]vertex
    glGenBuffers(1, &vertexvbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
    glBufferData(GL_ARRAY_BUFFER, 0x100000, vertexxyz, GL_STATIC_DRAW);

	//[9]normal
    glGenBuffers(1, &normalvbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    glBufferData(GL_ARRAY_BUFFER, 0x100000, normalxyz, GL_STATIC_DRAW);

	//[a]color
    glGenBuffers(1, &colorvbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorvbo);
    glBufferData(GL_ARRAY_BUFFER, 0x100000, colorrgb, GL_STATIC_DRAW);

	//[c]point
    glGenVertexArrays(1,&pointvao);
    glBindVertexArray(pointvao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorvbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

	glGenBuffers(1, &pointvbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pointvbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, pointindex, GL_STATIC_DRAW);

	//[d]line
    glGenVertexArrays(1,&linevao);
    glBindVertexArray(linevao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorvbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

	glGenBuffers(1, &linevbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, linevbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, lineindex, GL_STATIC_DRAW);

	//[e]triangle
    glGenVertexArrays(1,&trianglevao);
    glBindVertexArray(trianglevao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorvbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &trianglevbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, trianglevbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, triangleindex, GL_STATIC_DRAW);

	//[f]rectangle
    glGenVertexArrays(1,&rectanglevao);
    glBindVertexArray(rectanglevao);
    glBindBuffer(GL_ARRAY_BUFFER, vertexvbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, normalvbo);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorvbo);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &rectanglevbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectanglevbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 0x100000, rectangleindex, GL_STATIC_DRAW);
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
	float w = (float)(win->w);
	float h = (float)(win->h);
	projmatrix[0] = h / w;
	glViewport(0, 0, win->w, win->h);
}
void fixmatrix()
{
	int x;
	GLfloat temp[4*4];

	fixmodel();
	fixview();
	fixprojection();

	for(x=0;x<16;x++)temp[x] = modelmatrix[x];
	matrixmultiply_4(temp, viewmatrix);
	matrixmultiply_4(temp, projmatrix);

	GLint mvp = glGetUniformLocation(programhandle, "modelviewproj");
	glUniformMatrix4fv(mvp, 1, GL_FALSE, temp);
}
void fixlight()
{
	GLfloat ambientcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat lightcolor[3] = {0.5f, 0.5f, 0.5f};
	GLfloat lightposition[3] = {0.0f, 0.0f, 10.0f};

	GLint ac = glGetUniformLocation(programhandle, "ambientcolor");
	glUniform3fv(ac, 1, ambientcolor);

	GLint dc = glGetUniformLocation(programhandle, "lightcolor");
	glUniform3fv(dc, 1, lightcolor);

	GLint dp = glGetUniformLocation(programhandle, "lightposition");
	glUniform3fv(dp, 1, lightposition);

	GLint ep = glGetUniformLocation(programhandle, "eyeposition");
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

	//...
	fixmatrix();
	fixlight();
	fixtexture();

	//
	glBindVertexArray(pointvao);
	glDrawElements(GL_POINTS, src->info[12], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(linevao);
	glDrawElements(GL_LINES, src->info[13], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(trianglevao);
	glDrawElements(GL_TRIANGLES, src->info[14], GL_UNSIGNED_SHORT, 0);

	glBindVertexArray(rectanglevao);
	glDrawElements(GL_QUADS, src->info[15], GL_UNSIGNED_SHORT, 0);

	//write
	glFlush();
    glutSwapBuffers();
}
void callback_idle()
{
	u64 vertexcount, normalcount, colorcount, texturecount;
	u64 pointcount, linecount, tricount, rectcount;
	float* vertexdata;
	float* normaldata;
	float* colordata;
	float* texturedata;
	u16* pointindex;
	u16* lineindex;
	u16* triindex;
	u16* rectindex;

	if(queuehead == queuetail)return;

	vertexcount = src->info[8];
	normalcount = src->info[9];
	colorcount = src->info[10];
	texturecount = src->info[11];

	pointcount = src->info[12];
	linecount = src->info[13];
	tricount = src->info[14];
	rectcount = src->info[15];

	vertexdata = (void*)(src->buf)+0x800000;
	normaldata = (void*)(src->buf)+0x900000;
	colordata = (void*)(src->buf)+0xa00000;
	texturedata = (void*)(src->buf)+0xb00000;

	pointindex = (void*)(src->buf)+0xc00000;
	lineindex = (void*)(src->buf)+0xd00000;
	triindex = (void*)(src->buf)+0xe00000;
	rectindex = (void*)(src->buf)+0xf00000;

	glBindBuffer(   GL_ARRAY_BUFFER, vertexvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*vertexcount, vertexdata);

	glBindBuffer(   GL_ARRAY_BUFFER, normalvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*normalcount, normaldata);

	glBindBuffer(   GL_ARRAY_BUFFER, colorvbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*colorcount, colordata);
/*
	glBindBuffer(   GL_ARRAY_BUFFER, texturevbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12*texturecount, texturedata);
*/
	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, pointvbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*pointcount, pointindex);

	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, linevbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*linecount, lineindex);

	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, trianglevbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*tricount, triindex);

	glBindBuffer(   GL_ELEMENT_ARRAY_BUFFER, rectanglevbo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 2*rectcount, rectindex);

/*
	glBindTexture(GL_TEXTURE_2D, texturevbo);
	glTexImage2D(GL_TEXTURE_2D,	0,
		GL_RGBA, 512, 512, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, (void*)(src->buf)
	);
*/
	queuetail++;
	glutPostRedisplay();
}
void callback_reshape(int w, int h)
{
	win->w = w;
	win->h = h;
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

	if(key == GLUT_KEY_F1)key = 0xf1;
	else if(key == GLUT_KEY_F2)key = 0xf2;
	else if(key == GLUT_KEY_F3)key = 0xf3;
	else if(key == GLUT_KEY_F4)key = 0xf4;
	else if(key == GLUT_KEY_LEFT)key = 0x25;
	else if(key == GLUT_KEY_UP)key = 0x26;
	else if(key == GLUT_KEY_RIGHT)key = 0x27;
	else if(key == GLUT_KEY_DOWN)key = 0x28;
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

	if(win->cw == 4)
	{
		where = (u64)win;
		xx = x&0xffff;
		yy = ((win->h) - y)&0xffff;
		xx = (xx<<16) / (win->w);
		yy = (yy<<16) / (win->h);

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

	if(win->cw == 4)
	{
		where = (u64)win;
		xx = x&0xffff;
		yy = ((win->h) - y)&0xffff;
		xx = (xx<<16) / (win->w);
		yy = (yy<<16) / (win->h);

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
		camera[0] = v[0]*cosine(0.05f) + v[1]*sine(0.05f);
		camera[1] = -v[0]*sine(0.05f) + v[1]*cosine(0.05f);

		//camera_yaw += PI/90;
	}
	else if(x<last_x)
	{
		camera[0] = v[0]*cosine(0.05f) - v[1]*sine(0.05f);
		camera[1] = v[0]*sine(0.05f) + v[1]*cosine(0.05f);

		//camera_yaw -= PI/90;
	}

	if(y > last_y)
	{
		vectorcross(v, t);
		vectornormalize(v);

		v[0] *= sine(0.02f);
		v[1] *= sine(0.02f);
		v[2] *= sine(0.02f);
		v[3] = cosine(0.02f);
		quaternionrotate(camera, v);
	}
	else if(y<last_y)
	{
		vectorcross(v, t);
		vectornormalize(v);

		v[0] *= sine(-0.02f);
		v[1] *= sine(-0.02f);
		v[2] *= sine(-0.02f);
		v[3] = cosine(-0.02f);
		quaternionrotate(camera, v);
	}

	last_x = x;
	last_y = y;
	glutPostRedisplay();
}




void* uievent(struct window* p)
{
	int ret;
	int argc=1;
	char* argv[2];
	argv[0] = "./a.out";
	argv[1] = 0;

	//glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(p->w, p->h);
	glutInitWindowPosition(200, 200);
	glutCreateWindow("42");
	glEnable(GL_DEPTH_TEST);

	//glew
	ret = glewInit();

	//
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
    initShader();
    initVBO();

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
void windowread()
{
}
void windowwrite(struct window* dst, struct window* src)
{
	queuehead++;
}
void windowchange(int what)
{
}
void windowlist()
{
}
void windowstop()
{
}
void windowstart(struct window* this)
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
	this->thread = startthread(uievent, this);
}
void windowdelete()
{
}
void windowcreate()
{
}