#include <windows.h>
#include <xinput.h>
#include "libuser.h"




void joyprint(int id, XINPUT_GAMEPAD g)
{
	//0x0001	XINPUT_GAMEPAD_DPAD_UP
	//0x0002	XINPUT_GAMEPAD_DPAD_DOWN
	//0x0004	XINPUT_GAMEPAD_DPAD_LEFT
	//0x0008	XINPUT_GAMEPAD_DPAD_RIGHT
	//0x0010	XINPUT_GAMEPAD_START
	//0x0020	XINPUT_GAMEPAD_BACK
	//0x0040	XINPUT_GAMEPAD_LEFT_THUMB
	//0x0080	XINPUT_GAMEPAD_RIGHT_THUMB
	//0x0100	XINPUT_GAMEPAD_LEFT_SHOULDER
	//0x0200	XINPUT_GAMEPAD_RIGHT_SHOULDER
	//0x0400
	//0x0800
	//0x1000	XINPUT_GAMEPAD_A
	//0x2000	XINPUT_GAMEPAD_B
	//0x4000	XINPUT_GAMEPAD_X
	//0x8000	XINPUT_GAMEPAD_Y

	struct event ev;
	short* s;
	u8* t;

	ev.why = 0;
	ev.what = 0;
	ev.where = 0;
	ev.when = 0;
	s = (void*)&(ev.why);
	t = (void*)&(ev.what);

	//left
	s[0] = g.sThumbLX;
	s[1] = g.sThumbLY;
	s[2] = g.bLeftTrigger;
	s[3] = 0;
	if(g.wButtons & 0x0004)s[3] |= joyl_left;
	if(g.wButtons & 0x0008)s[3] |= joyl_right;
	if(g.wButtons & 0x0002)s[3] |= joyl_down;
	if(g.wButtons & 0x0001)s[3] |= joyl_up;
	if(s[2] > 9)           s[3] |= joyl_trigger;
	if(g.wButtons & 0x0100)s[3] |= joyl_bumper;
	if(g.wButtons & 0x0040)s[3] |= joyl_thumb;
	if(g.wButtons & 0x0020)s[3] |= joyl_select;
	if(	(s[3] != 0) |
		(s[1] < -2048) | (s[1] > 2048) |
		(s[0] < -2048) | (s[0] > 2048) )
	{
		t[0] = 'j';
		t[1] = 0x30 + id;
		t[2] = 'l';
		t[3] = '0';
		eventwrite(ev.why, ev.what, ev.where, ev.when);
	}

	//right
	s[0] = g.sThumbRX;
	s[1] = g.sThumbRY;
	s[2] = g.bRightTrigger;
	s[3] = 0;
	if(g.wButtons & 0x4000)s[3] |= joyr_left;
	if(g.wButtons & 0x2000)s[3] |= joyr_right;
	if(g.wButtons & 0x1000)s[3] |= joyr_down;
	if(g.wButtons & 0x8000)s[3] |= joyr_up;
	if(s[2] > 9)           s[3] |= joyr_trigger;
	if(g.wButtons & 0x0200)s[3] |= joyr_bumper;
	if(g.wButtons & 0x0080)s[3] |= joyr_thumb;
	if(g.wButtons & 0x0010)s[3] |= joyr_start;
	if(	(s[3] != 0) |
		(s[1] < -2048) | (s[1] > 2048) |
		(s[0] < -2048) | (s[0] > 2048) )
	{
		t[0] = 'j';
		t[1] = 0x30 + id;
		t[2] = 'r';
		t[3] = '0';
		eventwrite(ev.why, ev.what, ev.where, ev.when);
	}
}
void* gamepadthread(void* win)
{
	XINPUT_STATE s;
	Sleep(1);

	while(1)
	{
		if(ERROR_SUCCESS == XInputGetState(0, &s))
		{
			joyprint(0, s.Gamepad);
		}
		if(ERROR_SUCCESS == XInputGetState(1, &s))
		{
			joyprint(1, s.Gamepad);
		}
		if(ERROR_SUCCESS == XInputGetState(2, &s))
		{
			joyprint(2, s.Gamepad);
		}
		if(ERROR_SUCCESS == XInputGetState(3, &s))
		{
			joyprint(3, s.Gamepad);
		}

		sleep_us(10000);
	}
}
static void rawinputlistall()
{
	int j, ret, count = 0;
	RAWINPUTDEVICELIST* ridl;

	ret = GetRawInputDeviceList(NULL, &count, sizeof(RAWINPUTDEVICELIST));
	if(0 != ret)return;

	ridl = malloc(count * sizeof(RAWINPUTDEVICELIST));
	if(0 == ridl)return;

	ret = GetRawInputDeviceList(ridl, &count, sizeof(RAWINPUTDEVICELIST));
	if(-1 == ret)goto byebye;

	for(j=0;j<count;j++)
	{
		//type
		char* type = 0;
		switch(ridl[j].dwType){
		case RIM_TYPEKEYBOARD:type = "keyboard";break;
		case RIM_TYPEMOUSE:   type = "mouse";   break;
		case RIM_TYPEHID:     type = "hid";     break;
		default:break;
		}
		if(0 == type)continue;
		say("%d:	%s\n", j, type);

		//info
		RID_DEVICE_INFO rdi;
		UINT size;
		ZeroMemory(&rdi, sizeof(rdi));
		rdi.cbSize = sizeof(rdi);

		size = sizeof(rdi);
		ret = GetRawInputDeviceInfoA(ridl[j].hDevice, RIDI_DEVICEINFO, &rdi, &size);
		if(-1 == ret)continue;

		//name
		char name[256];
		memset(name, 0, sizeof(name));

		size = sizeof(name);
		ret = GetRawInputDeviceInfoA(ridl[j].hDevice, RIDI_DEVICENAME, name, &size);
		if(-1 == ret)continue;
		say("%s\n", name);
	}

byebye:
	free(ridl);
}




void joy_create(_obj* win)
{
	rawinputlistall();
	threadcreate(gamepadthread, win);
}
void joy_delete(_obj* win)
{
}




void initjoy()
{
}
void freejoy()
{
}
