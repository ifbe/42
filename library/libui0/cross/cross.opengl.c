#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<GL/glut.h> 
//
u64 startthread(void*, void*);
void stopthread();
//
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




//
static int refresh=0;
static u64 thread;
static int width;
static int height;
static void* pData;
static GLuint texture[1];
static int rotate_x=-5,rotate_y=5;



void callback_display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glClearColor//清除颜色  

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
	glVertex3f(  0.5, -0.5, 0.5 );
	glVertex3f(  0.5,  0.5, 0.5 );
	glVertex3f( -0.5,  0.5, 0.5 );
	glVertex3f( -0.5, -0.5, 0.5 );
	glEnd();

	//0
	glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexImage2D(
		GL_TEXTURE_2D,// 目标  
		0,// 级别  
		GL_RGBA,// 纹理内部格式  
		width,// 纹理的宽（最好2的次方）  
		height,// 纹理的高（最好2的次方）  
		0,// 纹理的深度（最好2的次方）  
		GL_RGBA,
		GL_UNSIGNED_BYTE,// 像素的数据类型  
		pData		// 数据指针
	);
    glBegin(GL_QUADS);
	glColor3f(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, 0.5, -0.5);
	glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
	glTexCoord2f(1.0, 1.0); glVertex3f(0.5, -0.5, -0.5);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.5, 0.5, -0.5);
    glEnd();

	// Reset transformations
	glLoadIdentity();
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );

	//
	glFlush();
	glutSwapBuffers();
}
void callback_idle()
{
	if(refresh <= 0)return;

	glutPostRedisplay();
	refresh--;
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
	//glutPostRedisplay();
}
void callback_mouse(int button, int state, int x, int y)
{
	if(state==GLUT_DOWN)rotate_x += 5;
}
void callback_move(int x,int y)
{
	rotate_y += 1;
}
void* uievent(void* p)
{
	int argc=1;
	char* argv[2];

	//初始化
	argv[0] = "./a.out";
	argv[1] = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(width, height);
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




void windowread(char* where)
{
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
void windowstart(char* addr, char* pixfmt, int x, int y)
{
	pData = addr;
	pixfmt[0]='r';pixfmt[1]='g';pixfmt[2]='b';pixfmt[3]='a';
	width = x;
	height = y;
	thread = startthread(uievent, 0);
}
void windowdelete()
{
}
void windowcreate()
{
}