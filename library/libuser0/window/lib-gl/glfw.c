#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"
int vbonode_sread(struct arena* win, struct style* stack);
int vbonode_swrite(struct arena* win, struct style* stack, struct event* ev);
int arenaevent(struct event* ev);
//
int fbodelete(struct arena* win);
int fbocreate(struct arena* win, char* arg);
//
void coopwindow_create(void*);
void coopwindow_update(void*);
void coopwindow_render(void*);
//
void hostwindow_create(void*);
void hostwindow_render(void*);
void hostwindow_update(void*);
//
void testwindow_render(void*);
void testwindow_event(void*, void*);
//
static u8 uppercase[] = {
	' ', '!','\"', '#', '$', '%', '&','\"',		//20,27
	'(', ')', '*', '+', '<', '_', '>', '?',		//28,2f
	')', '!', '@', '#', '$', '%', '^', '&',		//30,37
	'*', '(', ':', ':', '<', '+', '>', '?',		//38,3f
	'@', 'A', 'B', 'C', 'D', 'E', 'F', 'G',		//40,47
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',		//48,4f
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',		//50,57
	'X', 'Y', 'Z', '{', '|', '}', '^', '_',		//58,5f
	'~', 'A', 'B', 'C', 'D', 'E', 'F', 'G',		//60,67
	'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O',		//68,6f
	'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W',		//70,77
	'X', 'Y', 'Z', '{', '|', '}', '~', ' ',		//78,7f
};




static void callback_keyboard(GLFWwindow* fw, int key, int scan, int action, int mods)
{
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);
    //printf("key=%x,scan=%x,action=%x,mods=%x\n", key, scan, action, mods);

	if(0 == action)return;
	if(0x118 == key)return;		//capslock
	if((key >= 0x154)&&(key <= 0x15b))return;	//ctrl,alt...

	if(0x100 == key)
	{
		e.why = 0x1b;
		e.what = _kbd_;
	}
	else if((key >= 0x106)&&(key <= 0x109))
	{
		if(key == 0x109)e.why = 0x48;		//up
		else if(key == 0x108)e.why = 0x50;	//down
		else if(key == 0x107)e.why = 0x4b;	//left
		else if(key == 0x106)e.why = 0x4d;	//right
		e.what = _kbd_;
	}
	else if((key >= 0x122)&&(key <= 0x12d))
	{
		e.why = key + 0xf1 - 0x122;
		e.what = _kbd_;
	}
	else if(0x101 == key)
	{
		e.why = 0xd;
		e.what = _char_;
	}
	else if(0x103 == key)
	{
		e.why = 0x8;
		e.what = _char_;
	}
	else if((key >= 'A')&&(key <= 'Z'))
	{
		if(0 == (mods&1))key += 0x20;
		e.why = key;
		e.what = _char_;
	}
	else
	{
		if(mods&1)key = uppercase[key-0x20];
		e.why = key;
		e.what = _char_;
	}

	e.where = (u64)win;
	arenaevent(&e);
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
		arenaevent(&e);
	}
	else if(0 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2d70;
		e.where = (u64)win;
		arenaevent(&e);
	}
}
static void callback_move(GLFWwindow* fw, double xpos, double ypos)
{
	u64 x,y,temp;
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);

	if(GLFW_PRESS == glfwGetMouseButton(fw, GLFW_MOUSE_BUTTON_LEFT))temp = 'l';
	else temp = 'l';

	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	e.why = x + (y<<16) + (temp<<48);
	e.what = 0x4070;
	e.where = (u64)win;
	arenaevent(&e);
}
static void callback_scroll(GLFWwindow* fw, double x, double y)
{
	struct event e;
	struct arena* win = glfwGetWindowUserPointer(fw);
	//printf("%llx: %f,%f\n", (u64)win, x, y);

	e.where = (u64)win;
	e.what = 0x2b70;
	if(y > 0.0)	//wheel_up
	{
		e.why = ((u64)'f')<<48;
		arenaevent(&e);
		//eventwrite(why, 0x2b70, where, 0);
	}
	else	//wheel_down
	{
		e.why = ((u64)'b')<<48;
		arenaevent(&e);
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
	arenaevent(&e);
}
static void callback_reshape(GLFWwindow* fw, int w, int h)
{
	struct arena* win = glfwGetWindowUserPointer(fw);
	win->fbwidth = win->fbstride = w;
	win->fbheight = h;

	glfwGetWindowSize(fw, &w, &h);
	win->width = win->stride = w;
	win->height = h;
}




void windowopen_root(struct arena* w, struct arena* r)
{
	int x,y,j;

	//1.glfw
	x = w->width;
	y = w->height;
	GLFWwindow* fw = glfwCreateWindow(x, y, "42", NULL, NULL);
	if(0 == fw)
	{
		printf("error@glfwCreateWindow\n");
		return;
	}

	//2.setup
	glfwSetWindowUserPointer(fw, w);
	w->win = fw;

	glfwGetFramebufferSize(fw, &x, &y);
	w->fbwidth = w->fbstride = x;
	w->fbheight = y;

	//3.callback
	glfwSetDropCallback(fw, callback_drop);
	glfwSetKeyCallback(fw, callback_keyboard);
	glfwSetScrollCallback(fw, callback_scroll);
	glfwSetCursorPosCallback(fw, callback_move);
	glfwSetMouseButtonCallback(fw, callback_mouse);
	glfwSetFramebufferSizeCallback(fw, callback_reshape);

	//2.glew
	glfwMakeContextCurrent(fw);
	glewExperimental = 1;
	if(glewInit() != GLEW_OK)
	{
		printf("error@glewInit\n");
		return;
	}

	//3.init
	//hostctx_create(w);
	//initshader(w);
	//inittexture(w);
	//initvertex(w);
}
void windowopen_coop(struct arena* w, struct arena* r)
{
	int x,y,j;
	GLFWwindow* parent = 0;
	if(r)parent = r->win;

	//1.glfw
	x = w->width;
	y = w->height;
	GLFWwindow* fw = glfwCreateWindow(x, y, "coop", NULL, parent);
	if(0 == fw)
	{
		printf("error@glfwCreateWindow\n");
		return;
	}

	//2.setup
	glfwSetWindowUserPointer(fw, w);
	w->win = fw;

	glfwGetFramebufferSize(fw, &x, &y);
	w->fbwidth = w->fbstride = x;
	w->fbheight = y;

	//3.callback
	glfwSetDropCallback(fw, callback_drop);
	glfwSetKeyCallback(fw, callback_keyboard);
	glfwSetScrollCallback(fw, callback_scroll);
	glfwSetCursorPosCallback(fw, callback_move);
	glfwSetMouseButtonCallback(fw, callback_mouse);
	glfwSetFramebufferSizeCallback(fw, callback_reshape);

	//2.glew
	glfwMakeContextCurrent(fw);
	glewExperimental = 1;
	if(glewInit() != GLEW_OK)
	{
		printf("error@glewInit\n");
		return;
	}

	//vao mapping
	//coopctx_create(w);
}




void windowread(struct arena* win)
{
	struct relation* rel;
	struct arena* tmp;
	GLFWwindow* fw;
	//say("@windowread.start:%.8s,%.8s,%llx\n", &win->type, &win->fmt, win->win);

	//
	if(_ctx_ == win->type)
	{
		rel = win->irel0;
		if(0 == rel)return;

		tmp = (void*)(rel->srcchip);
		if(0 == tmp)return;

		fw = tmp->win;
		if(0 == fw)return;

		vbonode_sread(win, 0);
		say("@@@@@@%llx\n",win);

		glfwMakeContextCurrent(fw);
		hostwindow_update(win);
	}
	else if(_fbo_ == win->fmt)
	{
		fw = win->win;
		glfwMakeContextCurrent(fw);
		hostwindow_render(win);
	}
	else if(_coop_ == win->fmt)
	{
		fw = win->win;
		glfwMakeContextCurrent(fw);

		coopwindow_render(win);

		glfwSwapBuffers(fw);

		//cleanup events
		//if(glfwWindowShouldClose(fw)){eventwrite(0,0,0,0);return;}
		glfwPollEvents();
	}
	else
	{
		fw = win->win;
		glfwMakeContextCurrent(fw);

		rel = win->orel0;
		if(rel)hostwindow_render(win);
		else testwindow_render(win);

		glfwSwapBuffers(fw);

		//cleanup events
		if(glfwWindowShouldClose(fw)){eventwrite(0,0,0,0);return;}
		glfwPollEvents();
	}
	//say("@windowread.end\n");
}
void windowwrite(struct arena* win, struct event* ev)
{
	struct relation* rel;
	struct arena* ctx;
	struct style* st;

	rel = win->oreln;
	if(0 == rel){
		testwindow_event(win, ev);
		return;
	}

	while(1)
	{
		if(0 == rel)break;

		if(_win_ == rel->dsttype){
			ctx = (void*)(rel->dstchip);
			vbonode_swrite(ctx, 0, ev);
			break;
		}

		rel = samesrcprevdst(rel);
	}
}
void windowchange()
{
}
void windowlist()
{
}
void windowstop(struct arena* w)
{
}
void windowstart(struct arena* w)
{
}
void windowdelete(struct arena* win)
{
	if(_fbo_ == win->fmt)
	{
		fbodelete(win);
	}
	else if(_coop_ == win->fmt)
	{
		glfwDestroyWindow(win->win);
	}
	else
	{
		glfwDestroyWindow(win->win);
	}
}
void windowcreate(struct arena* win)
{
	struct relation* rel = 0;
	struct arena* share = 0;

	if(_fbo_ == win->fmt)
	{
		win->fbwidth = win->width = 1024;
		win->fbheight = win->height = 1024;
		win->fbdepth = win->depth = 1024;
		win->fbstride = win->stride = 1024;
		fbocreate(win, 0);
		win->fmt = _fbo_;
	}
	else if(_coop_ == win->fmt)
	{
		rel = win->orel0;
		if(rel)share = (void*)(rel->dstchip);

		win->width = 1024;
		win->height = 768;
		win->depth = 1024;
		win->stride = 1024;
		windowopen_coop(win, share);
		win->fmt = _coop_;
	}
	else
	{
		win->width = 1024;
		win->height = 768;
		win->depth = 1024;
		win->stride = 1024;
		windowopen_root(win, 0);
		win->fmt = _host_;
	}
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

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}
