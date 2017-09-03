#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "arena.h"
#define PI 3.1415926535897932384626433832795028841971693993151




u64 startthread(void*, void*);
int readfile(u64, void*, u64, u64);
int writefile(u64, void*, u64, u64);
void sleep_us(int);
//
double squareroot(double);
double cosine(double);
double sine(double);
//
void printmemory(void*, int);
void say(void*, ...);
void eventwrite(u64,u64,u64,u64);




static struct window* win;
static struct window* src;
static int refresh=0;
//
static GLuint vShader;
static GLuint fShader;
static GLuint programHandle;
//
static GLuint axisvao;
static GLuint axis;
//
static GLuint shapevao;
static GLuint vertexhandle;
static GLuint texturehandle;
static GLuint colorhandle;
static GLuint indexhandle;

//
static float camerax = 0.0f;
static float cameray = 1.0f;
static float cameraz = 0.0f;
static float centerx = 0.0f;
static float centery = 0.0f;
static float centerz = 0.0f;
static float abovex = 0.0f;
static float abovey = 0.0f;
static float abovez = 1.0f;
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
float axisData[] = {
	-1000.0, 0.0, 0.0,
	1000.0, 0.0, 0.0,
	0.0, -1000.0, 0.0,
	0.0, 1000.0, 0.0,
	0.0, 0.0, -1000.0,
	0.0, 0.0, 1000.0
};
float positionData[] = {
	-0.5, -0.5, -0.5,
	0.5, -0.5, -0.5,
	0.5, 0.5, -0.5,
	-0.5, 0.5, -0.5,
	-0.5, -0.5, 0.5,
	0.5, -0.5, 0.5,
	0.5, 0.5, 0.5,
	-0.5, 0.5, 0.5,
};
unsigned short indexdata[] = {
	0, 1, 2, 3,
	0, 1, 5, 4,
	5, 6, 2, 1,
	5, 6, 7, 4,
	3, 7, 4, 0,
	3, 7, 6, 2
};
//
static int last_x=0;
static int last_y=0;
//
static float camera_pitch = PI/4;
static float camera_yaw = -PI/2;
static float camera_roll = 0.0;
static float camera_zoom = 2.0;
//
static float object_pitch = 0.0;
static float object_yaw = 0.0;
static float object_roll = 0.0;
static float object_zoom = 1.0;




char vCode[] = {
	"#version 400\n"
	"in vec3 position;\n"
	"out vec3 vertexcolor;\n"
	"uniform mat4 model;\n"
	"uniform mat4 view;\n"
	"uniform mat4 proj;\n"
	"void main()\n"
	"{\n"
		"vertexcolor = position;\n"
		//"gl_Position = vec4(position,1.0);\n"
		"gl_Position = proj * view * model * vec4(position,1.0);\n"
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
    //1. 查看GLSL和OpenGL的版本  
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

    //2. 顶点着色器  
    vShader = glCreateShader(GL_VERTEX_SHADER);
    if (0 == vShader)  
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
            char *log = (char *)malloc(logLen);  
            GLsizei written;  
            //得到日志信息并输出  
            glGetShaderInfoLog(vShader,logLen,&written,log);
            printf("vertex shader compile log: %s\n",log);
            free(log);//释放空间
        }
    }

    //3. 片断着色器  
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
    if (GL_FALSE == compileResult)  
    {  
        GLint logLen;  
        //得到编译日志长度  
        glGetShaderiv(fShader,GL_INFO_LOG_LENGTH,&logLen);  
        if (logLen > 0)  
        {  
            char *log = (char *)malloc(logLen);  
            GLsizei written;  
            //得到日志信息并输出  
            glGetShaderInfoLog(fShader,logLen,&written,log);
            printf("fragment shader compile log: %s\n",log);
            free(log);//释放空间  
        }  
    }  
  
    //4. 着色器程序  
    programHandle = glCreateProgram();  
    if (!programHandle)  
    {  
        printf("ERROR : create program failed");
        exit(1);  
    }

    //将着色器程序链接到所创建的程序中  
    glAttachShader(programHandle,vShader);
    glAttachShader(programHandle,fShader);
    glLinkProgram(programHandle);

    //查询链接的结果  
    GLint linkStatus;  
    glGetProgramiv(programHandle,GL_LINK_STATUS,&linkStatus);  
    if(GL_FALSE == linkStatus)  
    {  
        printf("ERROR : link shader program failed");  
        GLint logLen;  
        glGetProgramiv(programHandle,GL_INFO_LOG_LENGTH, &logLen);  
        if (logLen > 0)  
        {  
            char *log = (char *)malloc(logLen);  
            GLsizei written;  
            glGetProgramInfoLog(programHandle,logLen, &written,log);  
            printf("Program log :%s\n", log);  
        }  
    }  
    else//链接成功，在OpenGL管线中使用渲染程序  
    {  
        glUseProgram(programHandle);  
    }  
}
void initVBO()  
{
	//axis vao
    glGenVertexArrays(1,&axisvao);
    glBindVertexArray(axisvao);

	//axis
    glGenBuffers(1, &axis);
    glBindBuffer(GL_ARRAY_BUFFER, axis);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*6, axisData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);




	//vao
    glGenVertexArrays(1,&shapevao);
    glBindVertexArray(shapevao);

    //position
    glGenBuffers(1, &vertexhandle);
    glBindBuffer(GL_ARRAY_BUFFER, vertexhandle);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*3*8, positionData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
/*
    //color
    glGenBuffers(1, &colorhandle);
    glBindBuffer(GL_COLOR_BUFFER, colorhandle);
    glBufferData(GL_COLOR_BUFFER, 9*sizeof(float), colorData, GL_STATIC_DRAW);

    //common
    glGenBuffers(1, &common);
    glBindBuffer(GL_ARRAY_BUFFER, common);
    glBufferData(GL_ARRAY_BUFFER, 9*sizeof(float), colorData, GL_STATIC_DRAW);
*/
    //index
    glGenBuffers(1, &indexhandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexhandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short)*4*6, indexdata, GL_STATIC_DRAW);

	//
	glGenTextures(1, &texturehandle);
	glBindTexture(GL_TEXTURE_2D, texturehandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}




void fixmodel()
{
	//matrix = movematrix * rotatematrix * scalematrix
	GLint modelLoc = glGetUniformLocation(programHandle, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, modelmatrix);
}
void fixview()
{
	//a X b = [ay*bz - az*by, az*bx-ax*bz, ax*by-ay*bx]
	float norm;

	//Z = center - camera
	float nx = centerx - camerax;
	float ny = centery - cameray;
	float nz = centerz - cameraz;
	norm = squareroot(nx*nx + ny*ny + nz*nz);
	nx /= norm;
	ny /= norm;
	nz /= norm;

	//X = cross(Z, above)
	float ux = ny*abovez - nz*abovey;
	float uy = nz*abovex - nx*abovez;
	float uz = nx*abovey - ny*abovex;
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

	viewmatrix[12] = -camerax*ux - cameray*uy - cameraz*uz;
	viewmatrix[13] = -camerax*vx - cameray*vy - cameraz*vz;
	viewmatrix[14] = camerax*nx + cameray*ny + cameraz*nz;
	viewmatrix[15] = 1.0f;
/*
	viewmatrix[0] = cos(camerax);
	viewmatrix[2] = -sin(camerax);
	viewmatrix[8] = sin(camerax);
	viewmatrix[10] = cos(camerax);
	viewmatrix[14] = -1.0f;
*/
	GLint viewLoc = glGetUniformLocation(programHandle, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, viewmatrix);
}
void fixprojection()
{
/*
	cot45, 0, 0, 0,
	0, cot45, 0, 0,
	0, 0, (f+n)/(f-n), -1,
	0, 0, (2*f*n)/(f-n), 0
*/
	GLint projLoc = glGetUniformLocation(programHandle, "proj");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, projmatrix);
}
void callback_display_vbo()
{
	//void* vbobuf = (void*)(src->buf);
	//int vbolen = sizeof(float)*3*4;
	//void* ibobuf = (void*)(src->buf)+0x400000;
	//int ibolen = sizeof(int)*6;

	//
	//glBufferSubData(        GL_ARRAY_BUFFER, 0, vbolen, vbobuf);
	//glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, ibolen, ibobuf);

	//set
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//matrix
	fixmodel();
	fixview();
	fixprojection();

	//axis
	glBindVertexArray(axisvao);
	glDrawArrays(GL_LINES, 0, 6);

	//shape
	glBindVertexArray(shapevao);
	glDrawElements(GL_QUADS, 4*6, GL_UNSIGNED_SHORT, 0);

	//write
	glFlush();
    glutSwapBuffers();
}
void callback_display_stl()
{
	GLfloat position[4] = {10.0, 20.0, 50.0, 1.0};
	GLfloat redDiffuseMaterial[] = {1.0, 0.0, 0.0, 1.0};
	GLfloat whiteSpecularMaterial[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat greenEmissiveMaterial[] = {0.0, 1.0, 0.0, 1.0};

	GLfloat shininess[] = {128.0};
	GLfloat whiteSpecularLight[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat blackAmbientLight[] = {0.0, 0.0, 0.0, 1.0};
	GLfloat whiteDiffuseLight[] = {1.0, 1.0, 1.0, 1.0};

	float ex,ey,ez;
	ex = -camera_zoom*cosine(camera_pitch)*cosine(camera_yaw);
	ey = camera_zoom*cosine(camera_pitch)*sine(camera_yaw);
	ez = camera_zoom*sine(camera_pitch);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//camera rotate
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
	gluLookAt(
		 ex, ey, ez,
		  0,  0,  0,
		0.0,0.0,1.0
	);

	float* p;
	void* buf = (void*)(src->buf);
	u32 count = *(u32*)(buf+80);
	u32 j;

	if(count > 0x147adf)count = 0x147adf;	//64MB
	buf += 84;
	//printf("count=%d\n", count);
	//printmemory(buf, 50);

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//must!!!
	glDisable(GL_TEXTURE_2D);
	glColor3f(1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);

	//
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
	//glMaterialfv(GL_FRONT, GL_EMISSION, greenEmissiveMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, redDiffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, whiteSpecularMaterial);
	glEnable(GL_COLOR_MATERIAL);

	//
	for(j=0;j<count;j++)
	{
		p = buf;
		buf += 50;

		glBegin(GL_TRIANGLES);
		glVertex3f(p[3], p[4], p[5]);
		glVertex3f(p[6], p[7], p[8]);
		glVertex3f(p[9], p[10], p[11]);
		glEnd();
	}

	//
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(100.0, 0.0, 0.0);
	glEnd();

	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 100.0, 0.0);
	glEnd();

	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 100.0);
	glEnd();

	//
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, blackAmbientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDiffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, whiteSpecularLight);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	//
	glFlush();
	glutSwapBuffers();
}
void callback_display_texture()
{
	float ex,ey,ez;
	ex = -camera_zoom*cosine(camera_pitch)*cosine(camera_yaw);
	ey = camera_zoom*cosine(camera_pitch)*sine(camera_yaw);
	ez = camera_zoom*sine(camera_pitch);
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//camera rotate
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
	gluLookAt(
		 ex, ey, ez,
		  0,  0,  0,
		0.0,0.0,1.0
	);

	glEnable(GL_COLOR_MATERIAL);
	glDisable(GL_TEXTURE_2D);

	//1
	glBegin(GL_QUADS);
	glColor3f(   0.0,  0.0,  0.5 );
	glVertex3f(  0.5, -0.5, -0.5 );
	glVertex3f(  0.5, -0.5,  0.5 );
	glVertex3f( -0.5, -0.5,  0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	//2
	glBegin(GL_QUADS);
	glColor3f(   0.0, 0.5,  0.0 );
	glVertex3f(  0.5, 0.5, -0.5 );
	glVertex3f(  0.5, 0.5,  0.5 );
	glVertex3f( -0.5, 0.5,  0.5 );
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	//3
	glBegin(GL_QUADS);
	glColor3f(   0.0,  0.5,  0.5 );
	glVertex3f( -0.5,  0.5, -0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glVertex3f( -0.5, -0.5,  0.5 );
	glVertex3f( -0.5,  0.5,  0.5 );
	glEnd();

	//4
	glBegin(GL_QUADS);
	glColor3f(  0.5,  0.0,  0.0 );
	glVertex3f( 0.5,  0.5, -0.5 );
	glVertex3f( 0.5, -0.5, -0.5 );
	glVertex3f( 0.5, -0.5,  0.5 );
	glVertex3f( 0.5,  0.5,  0.5 );
	glEnd();

	//5
	glBegin(GL_QUADS);
	glColor3f(   0.5,  0.0, 0.5 );
	glVertex3f(  0.5, -0.5, -0.5 );
	glVertex3f(  0.5,  0.5, -0.5 );
	glVertex3f( -0.5,  0.5, -0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	//0
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texturehandle);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		512,
		512,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,		//像素的数据类型  
		(void*)(src->buf)	//数据指针
	);
	glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, 0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, 0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, 0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, 0.5);
	glEnd();

	//
	glFlush();
	glutSwapBuffers();
}
void callback_display()
{
	if(win->fmt == hex32('v','b','o',0))callback_display_vbo();
	else if(win->fmt == hex32('s','t','l',0))callback_display_stl();
	else callback_display_texture();
}
void callback_idle()
{
	if(refresh <= 0)return;
	refresh--;

	glutPostRedisplay();
	sleep_us(1000*20);
}
void callback_keyboard(unsigned char key, int x, int y)
{
	u64 what,where;
	where = (u64)win;

	printf("%x\n",key);
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

	printf("%x\n",key);
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
	float tx = camerax;
	float ty = cameray;
	float tz = cameraz;
	if(state == GLUT_DOWN)
	{
		last_x = x;
		last_y = y;
	}
	if(state == GLUT_UP)
	{
		if(button == 3)	//wheel_up
		{
			camerax = 0.9*tx + 0.1*centerx;
			cameray = 0.9*ty + 0.1*centery;
			cameraz = 0.9*tz + 0.1*centerz;

			camera_zoom *= 0.95;

			glutPostRedisplay();
		}
		if(button == 4)	//wheel_down
		{
			camerax = 1.1*tx - 0.1*centerx;
			cameray = 1.1*ty - 0.1*centery;
			cameraz = 1.1*tz - 0.1*centerz;

			camera_zoom *= 1.05263158;

			glutPostRedisplay();
		}
		printf("camera_zoom=%f\n",camera_zoom);
	}
}
void callback_move(int x,int y)
{
	float tx = camerax;
	float ty = cameray;
	if(x>last_x)
	{
		camerax = tx*cosine(0.1f) + ty*sine(0.1f);
		cameray = -tx*sine(0.1f) + ty*cosine(0.1f);

		camera_yaw += PI/90;
	}
	else if(x<last_x)
	{
		camerax = tx*cosine(0.1f) - ty*sine(0.1f);
		cameray = tx*sine(0.1f) + ty*cosine(0.1f);

		camera_yaw -= PI/90;
	}

	if(y>last_y)
	{
		cameraz += 0.1;
		if(camera_pitch < PI*44/90)camera_pitch += PI/90;
	}
	else if(y<last_y)
	{
		cameraz -= 0.1;
		if(camera_pitch > -PI*44/90)camera_pitch -= PI/90;
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
	glViewport(0, 0, 512, 512);
	glEnable(GL_DEPTH_TEST);
    initShader();
    initVBO();

	//绘制与显示
	glutIdleFunc(callback_idle);
	glutDisplayFunc(callback_display);
	glutKeyboardFunc(callback_keyboard);
	glutSpecialFunc(callback_special);
	glutMouseFunc(callback_mouse);
	glutMotionFunc(callback_move);
	glutMainLoop();

	//
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//exit
	glDeleteBuffers(1, &vertexhandle);
	glDeleteBuffers(1, &indexhandle);
}
void windowread()
{
}
void windowwrite(struct window* dst, struct window* src)
{
	refresh++;
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
