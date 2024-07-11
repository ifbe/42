#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);
//
void nonewindow_create(void*);
void nonewindow_delete(void*);
void nonewindow_take(void*,void*, void*,int, p64,int, void*,int);
void nonewindow_give(void*,void*, void*,int, p64,int, void*,int);
//
void easywindow_create(void*);
void easywindow_delete(void*);
void easywindow_take(void*,void*, void*,int, p64,int, void*,int);
void easywindow_give(void*,void*, void*,int, p64,int, void*,int);
//
void fullwindow_create(void*,void*,int,char**);
void fullwindow_delete(void*);
void fullwindow_take(void*,void*, void*,int, p64,int, void*,int);
void fullwindow_give(void*,void*, void*,int, p64,int, void*,int);
//
void cmdqwindow_create(void*);
void cmdqwindow_delete(void*);
void cmdqwindow_take(void*,void*, void*,int, p64,int, void*,int);
void cmdqwindow_give(void*,void*, void*,int, p64,int, void*,int);
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


int window_draw(_obj* ogl,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	u64 t0,t1,t2,t3;
t0 = ogl->gl41list.gltime;

	//0: context current
	GLFWwindow* fw = ogl->gl41list.glwnd;
	glfwMakeContextCurrent(fw);
t1 = timeread_us();

	//1: render everything
	//logtoall("vfmt=%.8s\n", &ogl->vfmt);
	switch(ogl->vfmt){
		case _gl41none_:nonewindow_take(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41easy_:easywindow_take(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41cmdq_:cmdqwindow_take(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41list_:
		default:fullwindow_take(ogl,foot, stack,sp, arg,idx, buf,len);break;
	}
t2 = timeread_us();

	//2: swap buffer
	glfwSwapBuffers(fw);
t3 = timeread_us();

	//3: title
	char str[64];
	snprintf(str, 64, "(%dx%d)(%dx%d)(%lld,%lld,%lld,%lld)",
		ogl->whdf.width, ogl->whdf.height, ogl->whdf.fbwidth, ogl->whdf.fbheight,
		t1-t0, t2-t1, t3-t2, t3-t0
	);
	glfwSetWindowTitle(fw, str);
ogl->gl41list.gltime = t3;

	//4: poll event
	u64 save[2];
	save[0] = (u64)stack;
	save[1] = sp;

	ogl->gl41list.glsave = save;
	if(glfwWindowShouldClose(fw)){eventwrite(0,0,(u64)ogl,ogl->gl41list.gltime);return 0;}
	glfwPollEvents();
	ogl->gl41list.glsave = 0;

	return 0;
}


int window_take(_obj* ogl,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	//logtoall("@window_take\n");
	window_draw(ogl,foot, stack,sp, arg,idx, buf,len);
	return 0;
}
int window_give(_obj* ogl,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	//logtoall("@window_give: stack=%p,sp=%d\n", stack, sp);
	if(stack && (sp>=2)){
		if(_clk_ == idx){
			window_draw(ogl,foot, stack,sp, arg,idx, buf,len);
			return 0;
		}
	}

	//logtoall("@window_give:%.8s\n", &ogl->vfmt);
	switch(ogl->vfmt){
		case _gl41none_:nonewindow_give(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41easy_:easywindow_give(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41cmdq_:cmdqwindow_give(ogl,foot, stack,sp, arg,idx, buf,len);break;
		case _gl41list_:
		default:fullwindow_give(ogl,foot, stack,sp, arg,idx, buf,len);break;
	}
	return 0;
}
void window_attach(struct halfrel* self, struct halfrel* peer)
{
}
void window_detach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@windowlinkup:%llx\n", self->pchip);
}




static void restorestackdeliverevent(_obj* ogl, struct event* ev)
{
	u64* save = ogl->gl41list.glsave;
	if(0 == save){
		eventwrite(ev->why, ev->what, ev->where, 0);
		return;
	}

	struct halfrel* stack = (void*)save[0];
	int depth = save[1];
	window_give(ogl,0, stack,depth, 0,0, ev,0);
}
static void callback_keyboard(GLFWwindow* fw, int key, int scan, int action, int mods)
{
	struct event e;
	_obj* ogl = glfwGetWindowUserPointer(fw);
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
static void callback_scroll(GLFWwindow* fw, double x, double y)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	//printf("%llx: %f,%f\n", (u64)ogl, x, y);

	double xpos, ypos;
	glfwGetCursorPos(fw, &xpos, &ypos);

	struct event ev;
	ev.where = (u64)ogl;
	ev.what = 0x4070;

	short* t = (void*)&ev.why;
	t[0] = (short)xpos;
	t[1] = (short)ypos;
	t[2] = (short)y;
	t[3] = (y>0.0) ? 'f' : 'b';
	restorestackdeliverevent(ogl, &ev);
}
static void callback_mouse(GLFWwindow* fw, int button, int action, int mods)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);

	double xpos, ypos;
	glfwGetCursorPos(fw, &xpos, &ypos);

	struct event ev;
	ev.where = (u64)ogl;
	ev.what = (1 == action) ? 0x2b70 : 0x2d70;

	short* t = (void*)&ev.why;
	t[0] = (short)xpos;
	t[1] = (short)ypos;
	t[3] = (0 == button) ? 'l' : 'r';

	restorestackdeliverevent(ogl, &ev);
}
static void callback_move(GLFWwindow* fw, double xpos, double ypos)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);

	struct event ev;
	ev.where = (u64)ogl;
	ev.what = 0x4070;

	short* t = (void*)&ev.why;
	t[0] = (short)xpos;
	t[1] = (short)ypos;

	if(GLFW_PRESS == glfwGetMouseButton(fw, GLFW_MOUSE_BUTTON_LEFT))t[3] = 'l';
	else t[3] = 'l';

	restorestackdeliverevent(ogl, &ev);
}
static void callback_drop(GLFWwindow* fw, int count, const char** paths)
{
	logtoall("drop: %p,%d,%p\n",fw, count, paths);
	//char dragdata[0x1000];
/*	int j,ret=0;
	for(j=0;j<count;j++)
	{
		//ret += snprintf(dragdata+ret, 0x1000-ret, "%s\n", paths[j]);
		printf("%s\n", paths[j]);
	}
*/
	_obj* ogl = glfwGetWindowUserPointer(fw);
	struct event ev;
	ev.why = (u64)paths;		//(u64)dragdata;
	ev.what = _drag_ptrtbl_;
	ev.where = (u64)ogl;
	ev.when = 0;			//timeread_ns();
	restorestackdeliverevent(ogl, &ev);
}
static void callback_reshape(GLFWwindow* fw, int w, int h)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	ogl->whdf.fbwidth = w;
	ogl->whdf.fbheight = h;

	glfwGetWindowSize(fw, &w, &h);
	ogl->whdf.width = w;
	ogl->whdf.height = h;
}




void windowopen_root(_obj* wnd)
{
	if(1){
			printf("set GLFW_TRANSPARENT_FRAMEBUFFER = true\n");
			glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	}

	int w,h;
	GLFWmonitor* primary = 0;
	if(0){
		primary = glfwGetPrimaryMonitor();
		//glfwGetMonitorPhysicalSize(primary, &w, &h);	//wrong, this is length(mm)

		const GLFWvidmode* mode = glfwGetVideoMode(primary);
		w = mode->width;
		h = mode->height;
		wnd->whdf.width = w;
		wnd->whdf.height = h;
		printf("%s: w=%d,h=%d\n", __FUNCTION__, w, h);
	}
	else{
		w = wnd->whdf.width;
		h = wnd->whdf.height;
	}

	//1.glfw
	GLFWwindow* fw = glfwCreateWindow(w, h, "42", primary, NULL);
	if(0 == fw)
	{
		printf("error@glfwCreateWindow\n");
		return;
	}

	//transparency_fullwindow
	if(0){
		glfwSetWindowOpacity(fw, 0.8);
	}

	//transparency_perpixel
	int glfw_trans_fb = glfwGetWindowAttrib(fw, GLFW_TRANSPARENT_FRAMEBUFFER);
	printf("get GLFW_TRANSPARENT_FRAMEBUFFER = %d\n", glfw_trans_fb);

	//2.setup
	glfwGetFramebufferSize(fw, &w, &h);
	wnd->whdf.fbwidth = w;
	wnd->whdf.fbheight = h;

	glfwSetWindowUserPointer(fw, wnd);
	wnd->gl41list.glwnd = fw;

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
/*
	glfwSetWindowPos(fw, 0,0);
	glfwSetWindowSize(fw, 1920, 1080);
*/
	glfwPollEvents();		//window will show after this func
}
void windowopen_coop(_obj* wnd, _obj* r)
{
	GLFWwindow* parent = 0;
	if(r)parent = r->gl41list.glwnd;

	//1.glfw
	int w,h;
	w = wnd->whdf.width;
	h = wnd->whdf.height;
	GLFWwindow* fw = glfwCreateWindow(w, h, "coop", NULL, parent);
	if(0 == fw)
	{
		printf("error@glfwCreateWindow\n");
		return;
	}

	//2.setup
	glfwSetWindowUserPointer(fw, wnd);
	wnd->gl41list.glwnd = fw;

	glfwGetFramebufferSize(fw, &w, &h);
	wnd->whdf.fbwidth = w;
	wnd->whdf.fbheight = h;

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




void window_read(_obj* ogl)
{
}
void window_write(_obj* ogl)
{
}
void window_delete(_obj* ogl)
{
	switch(ogl->vfmt){
		case _gl41none_:nonewindow_delete(ogl);glfwDestroyWindow(ogl->gl41list.glwnd);break;
		case _gl41easy_:easywindow_delete(ogl);glfwDestroyWindow(ogl->gl41list.glwnd);break;
		case _gl41cmdq_:cmdqwindow_delete(ogl);glfwDestroyWindow(ogl->gl41list.glwnd);break;
		default:        fullwindow_delete(ogl);glfwDestroyWindow(ogl->gl41list.glwnd);break;
	}
}
void window_create(_obj* ogl, void* arg, int argc, char** argv)
{
	struct relation* rel = 0;
	_obj* share = 0;

	switch(ogl->vfmt){
	case _gl41none_:{
		ogl->whdf.width = 1024;
		ogl->whdf.height = 768;
		ogl->whdf.depth = 1024;

		windowopen_root(ogl);
		nonewindow_create(ogl);
		break;
	}
	case _gl41easy_:{
		ogl->whdf.width = 1024;
		ogl->whdf.height = 768;
		ogl->whdf.depth = 1024;

		windowopen_root(ogl);
		easywindow_create(ogl);
		break;
	}
	case _gl41cmdq_:{
		rel = ogl->orel0;
		if(rel)share = (void*)(rel->dstchip);

		ogl->whdf.width = 1024;
		ogl->whdf.height = 768;
		ogl->whdf.depth = 1024;

		windowopen_root(ogl);
		cmdqwindow_create(ogl);
		break;
	}
	default:{
		ogl->vfmt = _gl41list_;
		ogl->whdf.width = 1024;
		ogl->whdf.height = 768;
		ogl->whdf.depth = 1024;

		windowopen_root(ogl);
		fullwindow_create(ogl, arg, argc, argv);
		break;
	}
	}
}




void* window_alloc()
{
	return supply_alloc();
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
