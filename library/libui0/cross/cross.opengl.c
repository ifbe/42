#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<GL/glut.h> 
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




//
static pthread_t id;
static int rotate_x=0,rotate_y=0;




void windowchange(int what)
{
}
void windowlist()
{
}
void windowread(char* where)
{
}
void windowwrite()
{
}
void windowstart(char* addr, char* pixfmt, int x, int y)
{
}
void windowstop()
{
}




void callback_display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	//glClearColor//清除颜色  

	// Reset transformations
	glLoadIdentity();
	glRotatef( rotate_x, 1.0, 0.0, 0.0 );
	glRotatef( rotate_y, 0.0, 1.0, 0.0 );

	//1
	glBegin(GL_POLYGON);
	glColor3f(   0.0,  0.0,  0.5 );
	glVertex3f(  0.5, -0.5, -0.5 );
	glVertex3f(  0.5, -0.5,  0.5 );
	glVertex3f( -0.5, -0.5,  0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	//2
	glBegin(GL_POLYGON);
	glColor3f(   0.0,  0.5,  0.0 );
	glVertex3f(  0.5, 0.5, -0.5 );
	glVertex3f(  0.5, 0.5,  0.5 );
	glVertex3f( -0.5, 0.5,  0.5 );
	glVertex3f( -0.5, 0.5, -0.5 );
	glEnd();

	//3
	glBegin(GL_POLYGON);
	glColor3f(   0.0,  0.5,  0.5 );
	glVertex3f( -0.5,  0.5, -0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glVertex3f( -0.5, -0.5,  0.5 );
	glVertex3f( -0.5,  0.5,  0.5 );
	glEnd();

	//4
	glBegin(GL_POLYGON);
	glColor3f(  0.5,  0.0,  0.0 );
	glVertex3f( 0.5,  0.5, -0.5 );
	glVertex3f( 0.5, -0.5, -0.5 );
	glVertex3f( 0.5, -0.5,  0.5 );
	glVertex3f( 0.5,  0.5,  0.5 );
	glEnd();

	//5
	glBegin(GL_POLYGON);
	glColor3f(   0.5,  0.0,  0.5 );
	glVertex3f(  0.5, -0.5, -0.5 );
	glVertex3f(  0.5,  0.5, -0.5 );
	glVertex3f( -0.5,  0.5, -0.5 );
	glVertex3f( -0.5, -0.5, -0.5 );
	glEnd();

	//6
	glBegin(GL_POLYGON);
	glColor3f(   0.5,  0.5, 0.0 );
	glVertex3f(  0.5, -0.5, 0.5 );
	glVertex3f(  0.5,  0.5, 0.5 );
	glVertex3f( -0.5,  0.5, 0.5 );
	glVertex3f( -0.5, -0.5, 0.5 );
	glEnd();

	//
	glFlush();
	glutSwapBuffers();
}
void callback_keyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_RIGHT)rotate_y += 5;
	else if (key == GLUT_KEY_LEFT)rotate_y -= 5;
	else if (key == GLUT_KEY_UP)rotate_x += 5;
	else if (key == GLUT_KEY_DOWN)rotate_x -= 5;
 
	glutPostRedisplay();
}
void* uievent(void* p)
{
	int argc=1;
	char** argv;
	argv[0] = "./a.out";
	argv[1] = 0;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(512,512);
	glutInitWindowPosition(200,200);

	/*创建窗口*/
	glutCreateWindow("42");
	glEnable(GL_DEPTH_TEST);

	/*绘制与显示*/
	glutDisplayFunc(callback_display);
	glutSpecialFunc(callback_keyboard);

	//
	glutMainLoop();
}
void windowcreate()
{
	pthread_create(&id, NULL, uievent, NULL);
}
void windowdelete()
{
}
