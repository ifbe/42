#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"
void initmodbuf(void*);
void initshader(void*);
void inittexture(void*);
void initobject(void*);
void callback_update(void*);
void callback_display(void*);




static void callback_keyboard(GLFWwindow* fw, int key, int scan, int action, int mods)
{
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);
    printf("key=%x,scan=%x,action=%x,mods=%x\n", key, scan, action, mods);

	if(0 == action)return;
	if(0x100 == key)
	{
		e.why = 0x1b;
		e.what = hex32('k','b','d',0);
	}
	else if(0x101 == key)
	{
		e.why = 0xd;
		e.what = hex32('c','h','a','r');
	}
	else if(0x103 == key)
	{
		e.why = 0x8;
		e.what = hex32('c','h','a','r');
	}
	else if((key >= 0x106)&&(key <= 0x109))
	{
		if(key == 0x109)e.why = 0x48;		//up
		else if(key == 0x108)e.why = 0x50;	//down
		else if(key == 0x107)e.why = 0x4b;	//left
		else if(key == 0x106)e.why = 0x4d;	//right
		e.what = hex32('k','b','d',0);
	}
	else if((key >= 0x122)&&(key <= 0x12d))
	{
		e.why = 0xf1 + key - 0x122;
		e.what = hex32('k','b','d',0);
	}
	else
	{
		if((key >= 'A')|(key <= 'Z'))key += 0x20;
		e.why = key;
		e.what = hex32('c','h','a','r');
	}

	e.where = (u64)win;
	actorwrite_ev(&e);
	//eventwrite(why, what, where, 0);
}
static void callback_mouse(GLFWwindow* fw, int button, int action, int mods)
{
	u64 x,y,temp;
	struct event e;
	double xpos, ypos;
	struct arena* win = glfwGetWindowUserPointer(fw);

	glfwGetCursorPos(fw, &xpos, &ypos);
	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	if(1 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2b70;
		e.where = (u64)win;
		actorwrite_ev(&e);
	}
	else if(0 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2d70;
		e.where = (u64)win;
		actorwrite_ev(&e);
	}
}
static void callback_move(GLFWwindow* fw, double xpos, double ypos)
{
	u64 x,y,temp;
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);

	if(0 != glfwGetMouseButton(fw, 1))temp = 'r';
	else temp = 'l';

	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	e.why = x + (y<<16) + (temp<<48);
	e.what = 0x4070;
	e.where = (u64)win;
	actorwrite_ev(&e);
}
static void callback_scroll(GLFWwindow* fw, double x, double y)
{
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);
	printf("%f,%f\n", x, y);

	e.where = (u64)win;
	e.what = 0x2b70;
	if(y > 0.0)	//wheel_up
	{
		e.why = ((u64)'f')<<48;
		actorwrite_ev(&e);
		//eventwrite(why, 0x2b70, where, 0);
	}
	else	//wheel_down
	{
		e.why = ((u64)'b')<<48;
		actorwrite_ev(&e);
		//eventwrite(why, 0x2b70, where, 0);
	}
}
static void callback_drop(GLFWwindow* fw, int count, const char** paths)
{
	char dragdata[0x1000];
    int j,ret=0;
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);

    for(j=0;j<count;j++)
	{
		ret += snprintf(dragdata+ret, 0x1000-ret, "%s\n", paths[j]);
		//printf("%s\n", paths[j]);
	}

	e.why = (u64)dragdata;
	e.what = _drag_;
	e.where = (u64)win;
	actorwrite_ev(&e);
}
static void callback_reshape(GLFWwindow* fw, int w, int h)
{
	struct arena* win = glfwGetWindowUserPointer(fw);
	win->width = win->stride = w;
	win->height = h;
}
static void* windowthread(struct arena* w)
{
	//int j,k;

	//1.glfw
	GLFWwindow* fw = glfwCreateWindow(512, 512, "42", NULL, NULL);
	if(fw == NULL)
	{
		printf("error@glfwCreateWindow\n");
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(fw);

	//2.glew
	glewExperimental = 1;
	if(glewInit() != GLEW_OK)
	{
		printf("error@glewInit\n");
		return 0;
	}

	//3.init
	initmodbuf(w);
	initshader(w);
	inittexture(w);
	initobject(w);

	//4.callbacks
	glfwSetWindowUserPointer(fw, w);
	glfwSetDropCallback(fw, callback_drop);
	glfwSetKeyCallback(fw, callback_keyboard);
	glfwSetScrollCallback(fw, callback_scroll);
	glfwSetCursorPosCallback(fw, callback_move);
	glfwSetMouseButtonCallback(fw, callback_mouse);
	glfwSetFramebufferSizeCallback(fw, callback_reshape);

	//5.wait
	while(1)
	{
		if(glfwWindowShouldClose(fw) != 0)break;
/*
		j = gettime();
		k = j - (w->time);
		if(k>0)say("fps=%d\n", 1000000 / k);
		w->time = j;
*/
		callback_update(w);
		callback_display(w);
		sleep_us(10000);

		glfwSwapBuffers(fw);
		glfwPollEvents();
	}

	eventwrite(0,0,0,0);
	return 0;
}




int windowread(void* dc,void* df,void* sc,void* sf)
{
}
void windowwrite(void* dc,void* df,void* sc,void* sf,u8* buf,int len)
{
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
void windowstart(struct arena* w)
{
	w->type = hex32('w','i','n',0);
	w->fmt = hex32('v','b','o',0);
	w->irel = 0;
	w->orel = 0;

	w->width = 512;
	w->height = 512;
	w->depth = 512;
	w->stride = 512;

	threadcreate(windowthread, w);
}
void deletewindow()
{
}
void createwindow()
{
}




void freewindow()
{
	glfwTerminate();
}
void initwindow()
{
	if(glfwInit() == 0)
	{
		printf("error@glfwInit\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);	//4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);	//3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);	//MacOS
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
