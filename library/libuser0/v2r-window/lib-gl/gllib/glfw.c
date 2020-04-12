#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"
//
void nonewindow_create(void*);
void nonewindow_delete(void*);
void nonewindow_read( void*,int, void*,int, void*,int, void*,int);
void nonewindow_write(void*,int, void*,int, void*,int, void*,int);
//
void easywindow_create(void*);
void easywindow_delete(void*);
void easywindow_read( void*,int, void*,int, void*,int, void*,int);
void easywindow_write(void*,int, void*,int, void*,int, void*,int);
//
void fullwindow_create(void*);
void fullwindow_delete(void*);
void fullwindow_read( void*,int, void*,int, void*,int, void*,int);
void fullwindow_write(void*,int, void*,int, void*,int, void*,int);
//
int gl41fbo6_create(void*, void*);
int gl41fbo6_delete(void*, void*);
int gl41fbo6_read( void*,int, void*,int, void*,int, void*,int);
int gl41fbo6_write(void*,int, void*,int, void*,int, void*,int);
int gl41fboc_create(void*, void*);
int gl41fboc_delete(void*, void*);
int gl41fboc_read( void*,int, void*,int, void*,int, void*,int);
int gl41fboc_write(void*,int, void*,int, void*,int, void*,int);
int gl41fbod_create(void*, void*);
int gl41fbod_delete(void*, void*);
int gl41fbod_read( void*,int, void*,int, void*,int, void*,int);
int gl41fbod_write(void*,int, void*,int, void*,int, void*,int);
int gl41fbog_create(void*, void*);
int gl41fbog_delete(void*, void*);
int gl41fbog_read( void*,int, void*,int, void*,int, void*,int);
int gl41fbog_write(void*,int, void*,int, void*,int, void*,int);
int gl41wnd0_create(void*, void*);
int gl41wnd0_delete(void*, void*);
int gl41wnd0_read( void*,int, void*,int, void*,int, void*,int);
int gl41wnd0_write(void*,int, void*,int, void*,int, void*,int);
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




int windowread(_sup* ogl,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	u64 t0,t1,t2,t3;
	GLFWwindow* fw;
	//say("@windowread\n");

	switch(ogl->fmt){
	case _gl41fboc_:return gl41fboc_read(ogl,foot, stack,sp, arg,idx, buf,len);
	case _gl41fbod_:return gl41fbod_read(ogl,foot, stack,sp, arg,idx, buf,len);
	case _gl41fbog_:return gl41fbog_read(ogl,foot, stack,sp, arg,idx, buf,len);
	case _gl41wnd0_:return gl41wnd0_read(ogl,foot, stack,sp, arg,idx, buf,len);
	}//switch
t0 = ogl->gltime;

	//0: context current
	fw = ogl->glwnd;
	glfwMakeContextCurrent(fw);
t1 = timeread();

	//1: render everything
	switch(ogl->fmt){
		case _none_:nonewindow_read(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _easy_:easywindow_read(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _full_:
		default:fullwindow_read(ogl,foot, stack,sp, arg,idx, buf,len);break;
	}
t2 = timeread();

	//2: swap buffer
	glfwSwapBuffers(fw);
t3 = timeread();

	//3: title
	char str[64];
	snprintf(str, 64, "(%dx%d)(%dx%d)(%lld,%lld,%lld,%lld)",
		ogl->width, ogl->height, ogl->fbwidth, ogl->fbheight,
		t1-t0, t2-t1, t3-t2, t3-t0
	);
	glfwSetWindowTitle(fw, str);
ogl->gltime = t3;

	//4: poll event
	u64 save[2];
	save[0] = (u64)stack;
	save[1] = sp;

	ogl->glsave = save;
	if(glfwWindowShouldClose(fw)){eventwrite(0,0,(u64)ogl,ogl->gltime);return 0;}
	glfwPollEvents();
	ogl->glsave = 0;

	return 0;
}
void windowwrite(_sup* ogl,int foot, _syn* stack,int sp, void* arg,int idx, void* buf,int len)
{
	switch(ogl->fmt){
		case _gl41fboc_:gl41fboc_write(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41fbod_:gl41fbod_write(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41fbog_:gl41fbog_write(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41wnd0_:gl41wnd0_write(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _none_:nonewindow_write(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _easy_:easywindow_write(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _full_:
		default:fullwindow_write(ogl,foot, stack,sp, arg,idx, buf,len);break;
	}
}
void windowdiscon(struct halfrel* self, struct halfrel* peer)
{
}
void windowlinkup(struct halfrel* self, struct halfrel* peer)
{
	say("@windowlinkup:%llx\n", self->pchip);
}




static void restorestackdeliverevent(struct supply* ogl, struct event* ev)
{
	u64* save = ogl->glsave;
	if(0 == save){
		eventwrite(ev->why, ev->what, ev->where, 0);
		return;
	}

	struct halfrel* stack = (void*)save[0];
	int depth = save[1];
	windowwrite(ogl,0, stack,depth, 0,0, ev,0);
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
	else if(0x102 == key)
	{
		e.why = '\t';
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
	restorestackdeliverevent(ogl, &e);
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

	if(0 == button)temp = 'l';
	else temp = 'r';
	e.why = x + (y<<16) + (temp<<48);

	if(1 == action)e.what = 0x2b70;
	else e.what = 0x2d70;

	e.where = (u64)ogl;
	restorestackdeliverevent(ogl, &e);
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
	restorestackdeliverevent(ogl, &e);
}
static void callback_scroll(GLFWwindow* fw, double x, double y)
{
	struct event e;
	struct supply* ogl = glfwGetWindowUserPointer(fw);
	//printf("%llx: %f,%f\n", (u64)ogl, x, y);

	e.where = (u64)ogl;
	e.what = 0x2b70;
	if(y > 0.0)e.why = ((u64)'f')<<48;	//wheel_up
	else e.why = ((u64)'b')<<48;	//wheel_down
	restorestackdeliverevent(ogl, &e);
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
	restorestackdeliverevent(ogl, &e);
}
static void callback_reshape(GLFWwindow* fw, int w, int h)
{
	struct supply* ogl = glfwGetWindowUserPointer(fw);
	ogl->fbwidth = w;
	ogl->fbheight = h;

	glfwGetWindowSize(fw, &w, &h);
	ogl->width = w;
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
	w->fbwidth = x;
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
	w->fbwidth = x;
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




void windowsearch(struct supply* ogl)
{
}
void windowmodify(struct supply* ogl)
{
}
void windowdelete(struct supply* ogl)
{
	switch(ogl->fmt){
		case _gl41fbo6_:gl41fbo6_delete(ogl, 0);break;
		case _gl41fboc_:gl41fboc_delete(ogl, 0);break;
		case _gl41fbod_:gl41fbod_delete(ogl, 0);break;
		case _gl41fbog_:gl41fbog_delete(ogl, 0);break;
		case _gl41wnd0_:gl41wnd0_delete(ogl, 0);break;
		case _none_:nonewindow_delete(ogl);glfwDestroyWindow(ogl->glwnd);break;
		case _easy_:easywindow_delete(ogl);glfwDestroyWindow(ogl->glwnd);break;
		default:    fullwindow_delete(ogl);glfwDestroyWindow(ogl->glwnd);break;
	}
}
void windowcreate(struct supply* ogl, void* arg)
{
	struct relation* rel = 0;
	struct supply* share = 0;

	switch(ogl->fmt){
	case _gl41fbo6_:{
		gl41fbo6_create(ogl, arg);
		break;
	}
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

		windowopen_coop(ogl, share);
		break;
	}
	case _none_:{
		ogl->width = 1024;
		ogl->height = 768;
		ogl->depth = 1024;

		windowopen_root(ogl, 0);
		nonewindow_create(ogl);
		break;
	}
	case _easy_:{
		ogl->width = 1024;
		ogl->height = 768;
		ogl->depth = 1024;

		windowopen_root(ogl, 0);
		easywindow_create(ogl);
		break;
	}
	default:{
		ogl->width = 1024;
		ogl->height = 768;
		ogl->depth = 1024;

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
