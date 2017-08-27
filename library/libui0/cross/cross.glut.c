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
double cosine(double);
double sine(double);
//
void printmemory(void*, int);
void say(void*, ...);
void eventwrite(u64,u64,u64,u64);




static struct window* win;
static void* buffer;
//
static int refresh=0;
static GLuint texture[1];
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




void callback_display_vbo()
{
	u32 vbo, ibo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 0x400000, buffer, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &vbo);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4, buffer+0x400000, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &ibo);
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

	float* p;
	void* buf = buffer;
	u32 count = *(u32*)(buf+80);
	u32 j;

	if(count > 0x147adf)count = 0x147adf;	//64MB
	buf += 84;
	//printf("count=%d\n", count);
	//printmemory(buf, 50);

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
	//thing rotate
	//glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	//glRotatef( rotate_y, 0.0, 1.0, 0.0 );

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
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		512,
		512,
		0,
		GL_RGBA,
		GL_UNSIGNED_BYTE,	//像素的数据类型  
		buffer		//数据指针
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
	float ex,ey,ez;
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	//glShadeModel(GL_SMOOTH);
	glLoadIdentity();

	//camera rotate
	ex = -camera_zoom*cosine(camera_pitch)*cosine(camera_yaw);
	ey = camera_zoom*cosine(camera_pitch)*sine(camera_yaw);
	ez = camera_zoom*sine(camera_pitch);
	gluPerspective(45.0, 1.0, 0.1, 1000.0);
	gluLookAt(
		 ex, ey, ez,
		  0,  0,  0,
		0.0,0.0,1.0
	);

	if(win->fmt == hex32('v','b','o',0))
	{
		callback_display_vbo();
	}
	else if(win->fmt == hex32('s','t','l',0))
	{
		callback_display_stl();
	}
	else		//if(win->fmt == hex64('r','g','b','a','8','8','8','8'))
	{
		callback_display_texture();
	}
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
	if(state == GLUT_DOWN)
	{
		last_x = x;
		last_y = y;
	}
	if(state == GLUT_UP)
	{
		if(button == 3)	//wheel_up
		{
			camera_zoom *= 0.95;
			glutPostRedisplay();
		}
		if(button == 4)	//wheel_down
		{
			camera_zoom *= 1.05263158;
			glutPostRedisplay();
		}
		printf("camera_zoom=%f\n",camera_zoom);
	}
}
void callback_move(int x,int y)
{
	if(x>last_x)camera_yaw += PI/90;
	if(x<last_x)camera_yaw -= PI/90;
	if(y>last_y)
	{
		if(camera_pitch < PI*44/90)camera_pitch += PI/90;
	}
	if(y<last_y)
	{
		if(camera_pitch > -PI*44/90)camera_pitch -= PI/90;
	}

	last_x = x;
	last_y = y;
	glutPostRedisplay();
}




void* uievent(struct window* p)
{
	int argc=1;
	char* argv[2];

	//初始化
	argv[0] = "./a.out";
	argv[1] = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(p->w, p->h);
	glutInitWindowPosition(200, 200);

	//创建窗口
	glutCreateWindow("42");
	glEnable(GL_DEPTH_TEST);

	//
	glGenTextures(1, texture);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//绘制与显示
	glutIdleFunc(callback_idle);
	glutDisplayFunc(callback_display);
	glutKeyboardFunc(callback_keyboard);
	glutSpecialFunc(callback_special);
	glutMouseFunc(callback_mouse);
	glutMotionFunc(callback_move); 

	//
	glutMainLoop();
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
		buffer = (void*)(this->buf);
		return;
	}
	win = this;
	this->type = hex32('w','i','n',0);
	this->fmt = hex64('r','g','b','a','8','8','8','8');//hex32('v','b','o',0);
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
