#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"
void initobject(void*);
void initshader(void*);
void inittexture(void*);
void initvertex(void*);
void callback_update(void*);
void callback_display(void*, void*);




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
	printf("%llx: %f,%f\n", (u64)win, x, y);

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
	win->fwidth = win->fstride = w;
	win->fheight = h;

	glfwGetWindowSize(fw, &w, &h);
	win->width = win->stride = w;
	win->height = h;
}




void windowclose(struct arena* w)
{
	glfwTerminate();
}
void windowopen(struct arena* r, struct arena* w)
{
	int x,y,j;
	GLFWwindow* rw = 0;

	//1.glfw
	if(r)rw = r->win;
	GLFWwindow* fw = glfwCreateWindow(512, 512, "42", NULL, rw);
	if(0 == fw)
	{
		printf("error@glfwCreateWindow\n");
		return;
	}

	//2.setup
	glfwSetWindowUserPointer(fw, w);
	w->win = fw;
	w->map = 0;
	glfwGetFramebufferSize(fw, &x, &y);
	w->fwidth = w->fstride = x;
	w->fheight = y;

	//3.callback
	glfwSetDropCallback(fw, callback_drop);
	glfwSetKeyCallback(fw, callback_keyboard);
	glfwSetScrollCallback(fw, callback_scroll);
	glfwSetCursorPosCallback(fw, callback_move);
	glfwSetMouseButtonCallback(fw, callback_mouse);
	glfwSetFramebufferSizeCallback(fw, callback_reshape);

	if(0 == r)
	{
		//2.glew
		glfwMakeContextCurrent(fw);
		glewExperimental = 1;
		if(glewInit() != GLEW_OK)
		{
			printf("error@glewInit\n");
			return;
		}

		//3.init
		initobject(w);
		initshader(w);
		inittexture(w);
		initvertex(w);
	}
	else	//_coop_
	{
		w->map = malloc(0x100000);
		memset(w->map, 0, 0x100000);
	}
}




void windowread(struct arena* w)
{
	GLFWwindow* fw;
	struct arena* c;
	struct relation* rel;
//u64 oldtime,newtime;
//oldtime = timeread();




	//read world
	actorread_all(w);
//newtime = timeread();
//say("actorread:%d\n", newtime-oldtime);
//oldtime = newtime;




	//draw master
	fw = w->win;
	glfwMakeContextCurrent(fw);
	callback_update(w);
	callback_display(w, 0);
	glfwSwapBuffers(fw);
	if(glfwWindowShouldClose(fw)){eventwrite(0,0,0,0);return;}
//newtime = timeread();
//say("drawmaster:%d\n", newtime-oldtime);
//oldtime = newtime;




	//draw slave
	rel = w->orel0;
	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype)
		{
			c = (void*)(rel->dstchip);
			fw = c->win;
			if(0 == fw)windowopen(w, c);

			if(fw)
			{
				glfwMakeContextCurrent(fw);
				callback_display(w, c);
				glfwSwapBuffers(fw);
				if(glfwWindowShouldClose(fw)){eventwrite(0,0,0,0);return;}
			}
		}

		rel = samesrcnextdst(rel);
	}
//newtime = timeread();
//say("drawslave:%d\n", newtime-oldtime);
//oldtime = newtime;




	//cleanup events
	glfwPollEvents();
//newtime = timeread();
//say("pollevents:%d\n", newtime-oldtime);
//oldtime = newtime;
}
void windowwrite(struct arena* w)
{
}
void windowchange()
{
}
void windowlist()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct arena* w)
{
	glfwTerminate();
}
void windowcreate(struct arena* w)
{
	w->width = 512;
	w->height = 512;
	w->depth = 512;
	w->stride = 512;

	w->fmt = _vbo_;
	if(_win_ == w->type)
	{
		if(glfwInit() == 0)
		{
			printf("error@glfwInit\n");
			return;
		}
		glfwWindowHint(GLFW_SAMPLES, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		windowopen(0, w);
	}
}




void freewindow()
{
}
void initwindow()
{
}
