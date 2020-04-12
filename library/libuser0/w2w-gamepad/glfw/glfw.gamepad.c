#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "libuser.h"



static void* matchtable[10];




static void joystick_sendevent(struct halfrel* stack,int sp, struct xyzwpair* pair, int j)
{
	u8 buf[4];
	struct event ev;
	struct supply* joy;
	//say("@joystick_sendevent:%d\n", j);
	//printmemory(matchtable, 32);

	ev.where = 0;
	ev.when = 0;

	joy = matchtable[j];
	if(0 == joy)return;

	if((0 == joy->orel0) && (0 == joy->irel0)){
		say("@2\n");
		ev.why = *(u64*)(&pair->x0);
		ev.what = joy_left;
		printmemory(&ev, 16);

		ev.why = *(u64*)(&pair->xn);
		ev.what = joy_right;
		printmemory(&ev, 16);

		return;
	}

	ev.why = *(u64*)(&pair->x0);
	ev.what = joy_left;
	relationwrite(joy,_dst_, stack,sp, 0,0, &ev,32);

	ev.why = *(u64*)(&pair->xn);
	ev.what = joy_right;
	relationwrite(joy,_dst_, stack,sp, 0,0, &ev,32);
/*
	while(1){
		if(0 == rel)break;

		self = (void*)&rel->dstchip;
		peer = (void*)&rel->srcchip;
		if(_fd_ == rel->dsttype)
		{
			buf[0] = '.';
			buf[1] = '\n';

			if(pair->w0 & joyl_left)buf[0] = 'a';
			if(pair->w0 & joyl_right)buf[0] = 'd';
			if(pair->w0 & joyl_down)buf[0] = 's';
			if(pair->w0 & joyl_up)buf[0] = 'w';
			if(pair->w0 & joyl_trigger)buf[0] = ' ';
			if(pair->w0 & joyl_bumper)buf[0] = '.';

			systemwrite(self, peer, buf, 2);
		}
		else if(_ent_ == rel->dsttype)
		{
			ev.why = *(u64*)(&pair->x0);
			ev.what = joy_left;
			entitywrite(self, peer, &ev, 0);

			ev.why = *(u64*)(&pair->xn);
			ev.what = joy_right;
			entitywrite(self, peer, &ev, 0);
		}

		rel = samesrcnextdst(rel);
	}
*/
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
static void thread_joystick(struct supply* joy)
{
	int j, k;
	int c1, c2;
	const u8* u;
	const float* f;
	struct xyzwpair pair;
	struct halfrel stack[0x80];

	while(1)
	{
		for(j=0;j<16;j++)
		{
			if(0 == glfwJoystickPresent(GLFW_JOYSTICK_1 + j))continue;

			if(glfwJoystickIsGamepad(GLFW_JOYSTICK_1 + j))
			{
				joystick_gamepad(&pair, j);
				joystick_sendevent(stack,0, &pair, j);
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
				joystick_sendevent(stack,0, &pair, j);
			}//windows, 8bitdo

			if(0){
				joystick_xbox(&pair, f, u);
				joystick_sendevent(stack,0, &pair, j);
			}//xbox

			if(1){
				joystick_ds4(&pair, f, u);
				joystick_sendevent(stack,0, &pair, j);
			}//ds4
		}
		sleep_us(10000);
	}
}
static void callback_joystick(int id, int ev)
{
	say("joystick: %d,%d\n", id, ev);
}




void joydelete(struct supply* joy)
{
}
void joycreate(struct supply* joy)
{
	int j;
	for(j=0;j<10;j++){
		if(0 == matchtable[j]){
			matchtable[j] = joy;
			break;
		}
	}
}
void freejoy()
{
}
void initjoy()
{
	int j;
	for(j=0;j<10;j++)matchtable[j] = 0;

	glfwSetJoystickCallback(callback_joystick);
	threadcreate(thread_joystick, 0);
}
