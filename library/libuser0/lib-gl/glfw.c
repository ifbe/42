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




static void joystick_sendevent(void* p, int j)
{
	struct event ev;
	ev.where = 0;
	ev.when = 0;

	ev.why = *(u64*)(p+0);
	ev.what = joy_left;
	arenaevent(&ev);

	ev.why = *(u64*)(p+8);
	ev.what = joy_right;
	arenaevent(&ev);
}
static void joystick_gamepad(struct xyzwpair* pair, int j)
{
	GLFWgamepadstate state;
	if(!glfwGetGamepadState(GLFW_JOYSTICK_1 + j, &state))return;

	pair->x0 = (int)( 32767.0 * state.axes[GLFW_GAMEPAD_AXIS_LEFT_X]);
	pair->y0 = (int)(-32767.0 * state.axes[GLFW_GAMEPAD_AXIS_LEFT_Y]);
	pair->z0 = (int)(   127.0 * (1.0+state.axes[GLFW_GAMEPAD_AXIS_LEFT_TRIGGER]));
	pair->w0 =
		(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_LEFT]   << 0) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_RIGHT]  << 1) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_DOWN]   << 2) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_DPAD_UP]     << 3) +
		((!!pair->z0) << 4) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_BUMPER] << 5) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_LEFT_THUMB]  << 6) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_BACK]        << 7);

	pair->xn = (int)( 32767.0 * state.axes[GLFW_GAMEPAD_AXIS_RIGHT_X]);
	pair->yn = (int)(-32767.0 * state.axes[GLFW_GAMEPAD_AXIS_RIGHT_Y]);
	pair->zn = (int)(   127.0 * (1.0+state.axes[GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER]));
	pair->wn =
		(state.buttons[GLFW_GAMEPAD_BUTTON_X]            << 0) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_B]            << 1) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_A]            << 2) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_Y]            << 3) +
		((!!pair->zn) << 4) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER] << 5) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_RIGHT_THUMB]  << 6) +
		(state.buttons[GLFW_GAMEPAD_BUTTON_START]        << 7);

	//printmemory(pair, 16);
}
static void joystick_8bitdo(struct xyzwpair* pair, const float* f, const u8* u)
{
	pair->x0 = (short)(32767*f[0]);
	pair->y0 = (short)(32767*f[1]);
	pair->z0 = (short)(127*(1.0+f[4]));
	pair->w0 = 0;
	pair->xn = (short)(32767*f[2]);
	pair->yn = (short)(32767*f[3]);
	pair->zn = (short)(127*(1.0+f[5]));
	pair->wn = 0;

	switch(u[10] | (u[11]<<1) | (u[12]<<2) | (u[13]<<3))
	{
		case  8:pair->w0 |= joyl_left;break;
		case  2:pair->w0 |= joyl_right;break;
		case  4:pair->w0 |= joyl_down;break;
		case  1:pair->w0 |= joyl_up;break;
		case 12:pair->w0 |= joyl_down|joyl_left;break;
		case  6:pair->w0 |= joyl_down|joyl_right;break;
		case  3:pair->w0 |= joyl_up|joyl_right;break;
		case  9:pair->w0 |= joyl_up|joyl_left;break;
	}
	if(u[0])pair->wn |= joyr_down;
	if(u[1])pair->wn |= joyr_right;
	if(u[2])pair->wn |= joyr_left;
	if(u[3])pair->wn |= joyr_up;
	if(u[4])pair->w0 |= joyl_bumper;
	if(u[5])pair->wn |= joyr_bumper;
	if(u[6])pair->w0 |= joyl_select;
	if(u[7])pair->wn |= joyr_start;
	if(u[8])pair->w0 |= joyl_thumb;
	if(u[9])pair->wn |= joyr_thumb;
	if(f[4]>0.0)pair->w0 |= joyl_trigger;
	if(f[5]>0.0)pair->wn |= joyr_trigger;
}
static void joystick_xbox(struct xyzwpair* pair, const float* f, const u8* u)
{
	//  mac,xbox: 01?, 23?
	//  win,xbox:
	//linux,xbox:
	pair->x0 = (short)( 32767*f[0]);
	pair->y0 = (short)(-32767*f[1]);
	pair->z0 = (short)(127*(1.0+f[4]));
	pair->w0 = 0;
	pair->xn = (short)( 32767*f[2]);
	pair->yn = (short)(-32767*f[3]);
	pair->zn = (short)(127*(1.0+f[5]));
	pair->wn = 0;

	switch(u[15] | (u[16]<<1) | (u[17]<<2) | (u[18]<<3))
	{
		case  9:pair->w0 |= joyl_left;break;
		case  6:pair->w0 |= joyl_right;break;
		case 12:pair->w0 |= joyl_down;break;
		case  3:pair->w0 |= joyl_up;break;
		case  8:pair->w0 |= joyl_down|joyl_left;break;
		case  4:pair->w0 |= joyl_down|joyl_right;break;
		case  2:pair->w0 |= joyl_up|joyl_right;break;
		case  0:pair->w0 |= joyl_up|joyl_left;break;
	}
	if(u[0])pair->wn |= joyr_down;
	if(u[1])pair->wn |= joyr_right;
	//if(u[2])pair->wn |= joyr_left;
	if(u[3])pair->wn |= joyr_left;
	if(u[4])pair->wn |= joyl_up;
	//if(u[5])pair->wn |= joyr_bumper;
	if(u[6])pair->w0 |= joyl_bumper;
	if(u[7])pair->wn |= joyr_bumper;
	//if(u[8])pair->w0 |= joyl_stick;
	//if(u[9])pair->wn |= joyr_stick;
	//if(u[10])pair->w0 |= joyl_stick;
	if(u[11])pair->wn |= joyr_start;
	//if(u[12])pair->wn |= joyr_start;
	if(u[13])pair->w0 |= joyl_thumb;
	if(u[14])pair->wn |= joyr_thumb;
}
static void joystick_ds4(struct xyzwpair* pair, const float* f, const u8* u)
{
	//  mac, ds4: 014, 235
	//  win, ds4: 013, 254
	//linux, ds4: 012, 345
	pair->x0 = (short)( 32767*f[0]);
	pair->y0 = (short)(-32767*f[1]);
	pair->z0 = (short)(127*(1.0+f[3]));
	pair->w0 = 0;
	pair->xn = (short)( 32767*f[2]);
	pair->yn = (short)(-32767*f[5]);
	pair->zn = (short)(127*(1.0+f[4]));
	pair->wn = 0;

	if(u[ 0])pair->wn |= joyr_left;
	if(u[ 1])pair->wn |= joyr_down;
	if(u[ 2])pair->wn |= joyr_right;
	if(u[ 3])pair->wn |= joyr_up;
	if(u[ 4])pair->w0 |= joyl_bumper;
	if(u[ 5])pair->wn |= joyr_bumper;
	if(u[ 6])pair->w0 |= joyl_trigger;
	if(u[ 7])pair->wn |= joyr_trigger;
	if(u[ 8])pair->w0 |= joyl_select;
	if(u[ 9])pair->wn |= joyr_start;
	if(u[10])pair->w0 |= joyl_thumb;
	if(u[11])pair->wn |= joyr_thumb;
	//if(u[12])pair->w0 |= joyl_bumper;
	//if(u[13])pair->wn |= joyr_bumper;
	if(u[14])pair->w0 |= joyl_up;
	if(u[15])pair->w0 |= joyl_right;
	if(u[16])pair->w0 |= joyl_down;
	if(u[17])pair->w0 |= joyl_left;
}
static void thread_joystick(struct arena* win)
{
	int j, k;
	int c1, c2;
	const u8* u;
	const float* f;
	struct xyzwpair pair;

	while(1)
	{
		for(j=0;j<16;j++)
		{
			if(0 == glfwJoystickPresent(GLFW_JOYSTICK_1 + j))continue;

			if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1 + j))
			{
				joystick_gamepad(&pair, j);
				joystick_sendevent(&pair, j);
				continue;
			}

			f = glfwGetJoystickAxes(GLFW_JOYSTICK_1 + j, &c1);
			if(0 == f)continue;
			if(0 == c1)continue;
			//for(k=0;k<c1;k++)say("a%d:%f\n", k, f[k]);

			u = glfwGetJoystickButtons(GLFW_JOYSTICK_1 + j, &c2);
			if(0 == u)continue;
			if(0 == c2)continue;
			//for(k=0;k<c2;k++)say("b%d:%x\n", k, u[k]);

			if(0){
				joystick_8bitdo(&pair, f, u);
				joystick_sendevent(&pair, j);
			}//win, 8bitdo

			if(0){
				joystick_xbox(&pair, f, u);
				joystick_sendevent(&pair, j);
			}//xbox

			if(1){
				joystick_ds4(&pair, f, u);
				joystick_sendevent(&pair, j);
			}//ds4
		}
		sleep_us(10000);
	}
}
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
static void callback_joystick(int id, int ev)
{
	say("joystick: %d,%d\n", id, ev);
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
	if(_root_ == win->type)
	{
		rel = win->irel0;
		if(0 == rel)return;

		tmp = (void*)(rel->srcchip);
		if(0 == tmp)return;

		fw = tmp->win;
		if(0 == fw)return;

		vbonode_sread(win, 0);

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

		hostwindow_render(win);

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
		win->fmt = _vbo_;
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

	glfwSetJoystickCallback(callback_joystick);
	threadcreate(thread_joystick, 0);
}
