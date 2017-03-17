#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h> 
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




struct gldata
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;

	u64 thread;
	u64 datafmt;
	void* databuf;
};
static struct gldata* gdata;
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



void callback_display_texture()
{
	//thing rotate
	//glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	//glRotatef( rotate_y, 0.0, 1.0, 0.0 );

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
		GL_UNSIGNED_BYTE,// 像素的数据类型  
		(void*)gdata->buf	// 数据指针
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
void callback_display_stl()
{
/*
	GLfloat position[4] = {100.0, 100.0, 200.0, 1.0};
	GLfloat ambient[4]={1.0, 1.0, 1.0, 0.0};
	GLfloat diffuse[4]={1.0, 0.0, 0.0, 0.0};
*/
	GLfloat color[8][3]=
	{
		0.0, 0.0, 0.0,
		0.0, 0.0, 1.0,
		0.0, 1.0, 0.0,
		0.0, 1.0, 1.0,
		1.0, 0.0, 0.0,
		1.0, 0.0, 1.0,
		1.0, 1.0, 0.0,
		1.0, 1.0, 1.0
	};
	float* p;
	void* pointer;
	u32 j, count;

	pointer = gdata->databuf;
	count = *(u32*)(pointer+80);
	if(count > 0x147adf)count = 0x147adf;	//64MB

	pointer += 84;
	//printf("count=%d\n", count);
	//printmemory(pointer, 50);

	for(j=0;j<count;j++)
	{
		p = pointer;
		pointer += 50;

		glColor3fv(color[j%8]);
		glBegin(GL_TRIANGLES);
		glVertex3f(p[3], p[4], p[5]);
		glVertex3f(p[6], p[7], p[8]);
		glVertex3f(p[9], p[10], p[11]);
		glEnd();
	}
/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glEnable(GL_COLOR_MATERIAL);
*/
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

	if(gdata->datafmt == 0x6c7473)
	{
		callback_display_stl();
	}
	else
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
	printf("%x\n",key);
	if(key == 0x1b)eventwrite(0x1b,0x64626b,0,0);
	else eventwrite(key, 0x72616863, 0, 0);
}
void callback_special(int key, int x, int y)
{
	printf("%x\n",key);
	if (key == GLUT_KEY_LEFT){eventwrite(0x25,0x64626b,0,0);}
	else if (key == GLUT_KEY_UP){eventwrite(0x26,0x64626b,0,0);}
	else if (key == GLUT_KEY_RIGHT){eventwrite(0x27,0x64626b,0,0);}
	else if (key == GLUT_KEY_DOWN){eventwrite(0x28,0x64626b,0,0);}
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




void* uievent(struct gldata* p)
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
void windowread(char* p)
{
	if(p == 0)
	{
		gdata->datafmt = 0;
		free(gdata->databuf);
	}
	else
	{
		gdata->datafmt = 0x6c7473;
		if(gdata->databuf == 0)gdata->databuf = malloc(0x100000*64);
		readfile((u64)p, gdata->databuf, 0, 0x100000*64);
	}
}
void windowwrite()
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
void windowstart(struct gldata* p)
{
	gdata = p;
	p->buf = (u64)malloc(2048*1024*4);
	p->fmt = 0x3838383861626772;
	p->w = 512;
	p->h = 512;
	p->datafmt = 0;
	p->databuf = 0;
	p->thread = startthread(uievent, p);
}
void windowdelete()
{
}
void windowcreate()
{
}
