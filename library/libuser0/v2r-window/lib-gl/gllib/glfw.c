#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"
//
void nonewindow_create(void*);
void nonewindow_delete(void*);
void nonewindow_read(void*);
void nonewindow_write(void*, void*);
//
void easywindow_create(void*);
void easywindow_delete(void*);
void easywindow_read(void*);
void easywindow_write(void*, void*);
//
void fullwindow_create(void*);
void fullwindow_delete(void*);
void fullwindow_read(void*);
void fullwindow_write(void*, void*);
//
int gl41fboc_create(void*, void*);
int gl41fboc_delete(void*, void*);
int gl41fboc_read( void*, void*, void*, int, void*, int);
int gl41fboc_write(void*, void*, void*, int, void*, int);
int gl41fbod_create(void*, void*);
int gl41fbod_delete(void*, void*);
int gl41fbod_read( void*, void*, void*, int, void*, int);
int gl41fbod_write(void*, void*, void*, int, void*, int);
int gl41fbog_create(void*, void*);
int gl41fbog_delete(void*, void*);
int gl41fbog_read( void*, void*, void*, int, void*, int);
int gl41fbog_write(void*, void*, void*, int, void*, int);
int gl41wnd0_create(void*, void*);
int gl41wnd0_delete(void*, void*);
int gl41wnd0_read( void*, void*, void*, int, void*, int);
int gl41wnd0_write(void*, void*, void*, int, void*, int);
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
void windowdispatch(struct supply* ogl, struct event* ev)
{
	if(0 == ogl)return;
	//say("ogl=%llx, fmt=%.8s\n", ogl, &ogl->fmt);

	switch(ogl->fmt){
		case _none_:nonewindow_write(ogl, ev);break;
		case _easy_:easywindow_write(ogl, ev);break;
		case _full_:
		default:fullwindow_write(ogl, ev);break;
	}
}




static void callback_keyboard(GLFWwindow* fw, int key, int scan, int action, int mods)
{
	struct event e;
	struct supply* ogl = glfwGetWindowUserPointer(fw);
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

	e.where = (u64)ogl;
	windowdispatch(ogl, &e);
	//eventwrite(why, what, where, 0);
}
static void callback_mouse(GLFWwindow* fw, int button, int action, int mods)
{
	u64 x,y,temp;
	struct event e;
	double xpos, ypos;
	struct supply* ogl = glfwGetWindowUserPointer(fw);

	glfwGetCursorPos(fw, &xpos, &ypos);
	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	if(1 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2b70;
		e.where = (u64)ogl;
		windowdispatch(ogl, &e);
	}
	else if(0 == action)
	{
		if(0 == button)temp = 'l';
		else temp = 'r';

		e.why = x + (y<<16) + (temp<<48);
		e.what = 0x2d70;
		e.where = (u64)ogl;
		windowdispatch(ogl, &e);
	}
}
static void callback_move(GLFWwindow* fw, double xpos, double ypos)
{
	u64 x,y,temp;
	struct event e;
	struct supply* ogl = glfwGetWindowUserPointer(fw);

	if(GLFW_PRESS == glfwGetMouseButton(fw, GLFW_MOUSE_BUTTON_LEFT))temp = 'l';
	else temp = 'l';

	x = ((int)xpos)&0xffff;
	y = ((int)ypos)&0xffff;

	e.why = x + (y<<16) + (temp<<48);
	e.what = 0x4070;
	e.where = (u64)ogl;
	windowdispatch(ogl, &e);
}
static void callback_scroll(GLFWwindow* fw, double x, double y)
{
	struct event e;
	struct supply* ogl = glfwGetWindowUserPointer(fw);
	//printf("%llx: %f,%f\n", (u64)ogl, x, y);

	e.where = (u64)ogl;
	e.what = 0x2b70;
	if(y > 0.0)	//wheel_up
	{
		e.why = ((u64)'f')<<48;
		windowdispatch(ogl, &e);
		//eventwrite(why, 0x2b70, where, 0);
	}
	else	//wheel_down
	{
		e.why = ((u64)'b')<<48;
		windowdispatch(ogl, &e);
		//eventwrite(why, 0x2b70, where, 0);
	}
}
static void callback_drop(GLFWwindow* fw, int count, const char** paths)
{
	char dragdata[0x1000];
	int j,ret=0;
	struct event e;
	struct supply* ogl = glfwGetWindowUserPointer(fw);

	for(j=0;j<count;j++)
	{
		ret += snprintf(dragdata+ret, 0x1000-ret, "%s\n", paths[j]);
		//printf("%s\n", paths[j]);
	}

	e.why = (u64)dragdata;
	e.what = _drag_;
	e.where = (u64)ogl;
	windowdispatch(ogl, &e);
}
static void callback_reshape(GLFWwindow* fw, int w, int h)
{
	struct supply* ogl = glfwGetWindowUserPointer(fw);
	ogl->fbwidth = ogl->fbstride = w;
	ogl->fbheight = h;

	glfwGetWindowSize(fw, &w, &h);
	ogl->width = ogl->stride = w;
	ogl->height = h;
}




void windowopen_root(struct supply* w, struct supply* r)
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
	w->glwnd = fw;

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
void windowopen_coop(struct supply* w, struct supply* r)
{
	int x,y,j;
	GLFWwindow* parent = 0;
	if(r)parent = r->glwnd;

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
	w->glwnd = fw;

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




void windowread(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct supply* ogl;
	GLFWwindow* fw;
	//say("@windowread\n");

	ogl = self->pchip;
	if(_gl41fboc_ == ogl->fmt){
		gl41fboc_read(self, peer, arg, idx, buf, len);
		return;
	}
	if(_gl41fbod_ == ogl->fmt){
		gl41fbod_read(self, peer, arg, idx, buf, len);
		return;
	}
	if(_gl41fbog_ == ogl->fmt){
		gl41fbog_read(self, peer, arg, idx, buf, len);
		return;
	}
	if(_gl41wnd0_ == ogl->fmt){
		gl41wnd0_read(self, peer, arg, idx, buf, len);
		return;
	}
	//0: context current
	fw = ogl->glwnd;
	glfwMakeContextCurrent(fw);

	//1: render everything
	switch(ogl->fmt){
		case _none_:nonewindow_read(ogl);break;
		case _easy_:easywindow_read(ogl);break;
		case _full_:
		default:fullwindow_read(ogl);break;
	}

	//2: swap buffer
	glfwSwapBuffers(fw);

	//3: title
	char str[64];
	snprintf(str, 64, "%dx%d(%d,%d)", ogl->width, ogl->height, ogl->fbwidth, ogl->fbheight);
	glfwSetWindowTitle(fw, str);

	//4: poll event
	if(glfwWindowShouldClose(fw)){eventwrite(0,0,0,0);return;}
	glfwPollEvents();
}
void windowwrite(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct supply* ogl;
	if(0 == self)return;
	ogl = self->pchip;

	switch(ogl->fmt){
		case _gl41wnd0_:gl41wnd0_write(self, peer, arg, idx, buf, len);break;
		default:windowdispatch(ogl, buf);
	}
}
void windowchange()
{
}
void windowlist()
{
}
void windowstop(struct supply* ogl)
{
}
void windowstart(struct supply* ogl)
{
	say("@windowstart:%llx\n", ogl);
}
void windowdelete(struct supply* ogl)
{
	switch(ogl->fmt){
		case _gl41fboc_:gl41fboc_delete(ogl, 0);break;
		case _gl41fbod_:gl41fbod_delete(ogl, 0);break;
		case _gl41fbog_:gl41fbog_delete(ogl, 0);break;
		case _gl41wnd0_:gl41wnd0_delete(ogl, 0);break;
		case _none_:nonewindow_delete(ogl);break;
		case _easy_:easywindow_delete(ogl);break;
		default:fullwindow_delete(ogl);break;
	}

	glfwDestroyWindow(ogl->glwnd);
}
void windowcreate(struct supply* ogl, void* arg)
{
	struct relation* rel = 0;
	struct supply* share = 0;

	switch(ogl->fmt){
	case _gl41fboc_:{
		gl41fboc_create(ogl, arg);
		break;
	}
	case _gl41fbod_:{
		gl41fbod_create(ogl, arg);
		break;
	}
	case _gl41fbog_:{
		gl41fbog_create(ogl, arg);
		break;
	}
	case _gl41wnd0_:{
		gl41wnd0_create(ogl, arg);
		break;
	}
	case _coop_:{
		rel = ogl->orel0;
		if(rel)share = (void*)(rel->dstchip);

		ogl->width = 1024;
		ogl->height = 768;
		ogl->depth = 1024;
		ogl->stride = 1024;

		windowopen_coop(ogl, share);
		break;
	}
	case _none_:{
		ogl->width = 1024;
		ogl->height = 768;
		ogl->depth = 1024;
		ogl->stride = 1024;

		windowopen_root(ogl, 0);
		nonewindow_create(ogl);
		break;
	}
	case _easy_:{
		ogl->width = 1024;
		ogl->height = 768;
		ogl->depth = 1024;
		ogl->stride = 1024;

		windowopen_root(ogl, 0);
		easywindow_create(ogl);
		break;
	}
	default:{
		ogl->width = 1024;
		ogl->height = 768;
		ogl->depth = 1024;
		ogl->stride = 1024;

		windowopen_root(ogl, 0);
		fullwindow_create(ogl);
		break;
	}
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
