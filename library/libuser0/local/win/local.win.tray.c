#include <windows.h>
#include <xinput.h>
#include "arena.h"
#define WM_TRAY (WM_USER + 1)
#define menu1 0x1111
#define menu2 0x2222
int lowlevel_input();
void sleep_us(int);




struct xxxx
{
	u8 val;
	char* name;
};
static struct xxxx xtab[16] = {
	0x48, "XINPUT_GAMEPAD_DPAD_UP",		//0x0001
	0x50, "XINPUT_GAMEPAD_DPAD_DOWN",		//0x0002
	0x4b, "XINPUT_GAMEPAD_DPAD_LEFT",		//0x0004
	0x4d, "XINPUT_GAMEPAD_DPAD_RIGHT",	//0x0008
	's', "XINPUT_GAMEPAD_START",			//0x0010
	'b', "XINPUT_GAMEPAD_BACK",			//0x0020
	0xff, "XINPUT_GAMEPAD_LEFT_THUMB",	//0x0040
	0xff, "XINPUT_GAMEPAD_RIGHT_THUMB",	//0x0080
	0xff, "XINPUT_GAMEPAD_LEFT_SHOULDER",		//0x0100
	0xff, "XINPUT_GAMEPAD_RIGHT_SHOULDER",	//0x0200
	0xff, "????1",	//0x0400
	0xff, "????2",	//0x0800
	'a', "XINPUT_GAMEPAD_A",		//0x1000
	'b', "XINPUT_GAMEPAD_B",		//0x2000
	'x', "XINPUT_GAMEPAD_X",		//0x4000
	'y', "XINPUT_GAMEPAD_Y"		//0x8000
};
static int btn = 0;
//
static HWND console;		//console window
static HWND dummy;
//
static NOTIFYICONDATA nid;	//托盘属性 
static HMENU hMenu;		//托盘菜单




LRESULT CALLBACK trayproc(HWND hd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch(msg)
	{
	case WM_TRAY:
	{
	switch(lparam) 
	{
		case WM_LBUTTONDOWN:
		{
			if( IsWindowVisible(console) )ShowWindow(console, SW_HIDE);
			else ShowWindow(console, SW_SHOW);
			break;
		}
		case WM_RBUTTONDOWN: 
		{ 
			//获取鼠标坐标 
			POINT pt;
			GetCursorPos(&pt); 
 
			//解决在菜单外单击左键菜单不消失的问题 
			SetForegroundWindow(hd); 

			//显示并获取选中的菜单 
			int cmd=TrackPopupMenu(hMenu, TPM_RETURNCMD, pt.x, pt.y, 0, hd,0);
			if(cmd == menu1)
			{
				//lstrcpy(nid.szInfoTitle, "message");
				lstrcpy(nid.szInfo, TEXT("i am groot"));
				nid.uTimeout = 0;
				Shell_NotifyIcon(NIM_MODIFY, &nid);
			}
			else if(cmd == menu2)
			{
				exit(-1);
			}
			break;
		}//case WM_RBUTTONDOWN
	}//switch(lparam) 
	}//case WM_TRAY
	}//switch(msg)

	return DefWindowProc(hd, msg, wparam, lparam);
}
DWORD WINAPI trayworker()
{
	MSG msg;
	WNDCLASS wc;
	char* str = "MessageOnly";

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = trayproc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;				//hInst;
	wc.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = str;
	if(!RegisterClass(&wc))return 0;

	//the dummy
	dummy = CreateWindow(
		str, str, 0,
		0, 0, 0, 0,
		HWND_MESSAGE, 0, 0, 0
	);

	//the tray
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = dummy;
	nid.uID = 0xabef;
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
	nid.uCallbackMessage = WM_TRAY;
	nid.hIcon = LoadIcon(NULL,IDI_WINLOGO);
	lstrcpy(nid.szTip, "i am groot!");

	Shell_NotifyIcon(NIM_ADD, &nid);
	hMenu = CreatePopupMenu();
	AppendMenu(hMenu, MF_STRING, menu1, TEXT("about"));
	AppendMenu(hMenu, MF_STRING, menu2, TEXT("exit"));

	//forever
	while(GetMessage(&msg,NULL,0,0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}




void joyprint(int id, XINPUT_GAMEPAD g)
{
	int j,k;
	short t[4]; 
	struct event ev;

/*
	if(	(0 == g.wButtons) &&
		(8 >= g.bLeftTrigger) &&
		(8 >= g.bRightTrigger) &&
		(-2048 < g.sThumbLX) && (2048 > g.sThumbLX) &&
		(-2048 < g.sThumbLY) && (2048 > g.sThumbLY) &&
		(-2048 < g.sThumbRX) && (2048 > g.sThumbRX) &&
		(-2048 < g.sThumbRY) && (2048 > g.sThumbRY) )
	{
		return;
	}
	say(
		"%x:\n"
		"	%x,%x\n"
		"	%d,%d\n"
		"	%d,%d\n",
		id,
		g.bLeftTrigger, g.bRightTrigger,
		g.sThumbLX, g.sThumbLY,
		g.sThumbRX, g.sThumbRY
	);*/

	for(j=0;j<16;j++)
	{
		//say("	%s\n", xtab[j].name);

		k = 1<<j;
		if((0 != (g.wButtons&k)) && (0 == (btn&k)))
		{
			eventwrite(xtab[j].val, _kbd_, 0, 0);
		}
	}
	btn = g.wButtons;

	if(	(g.sThumbLX < -2048) |
		(g.sThumbLX > 2048) |
		(g.sThumbLY < -2048) |
		(g.sThumbLY > 2048) )
	{
		t[0] = g.sThumbLX;
		t[1] = g.sThumbLY;
		t[2] = 'l';
		t[3] = id;
		ev.why = *(u64*)t;
		ev.what = hex32('j','o','y',0);
		ev.where = 0;
		actorwrite(0, 0, 0, 0, &ev, 0x20);
	}

	if(	(g.sThumbRX < -2048) |
		(g.sThumbRX > 2048) |
		(g.sThumbRY < -2048) |
		(g.sThumbRY > 2048) )
	{
		t[0] = g.sThumbRX;
		t[1] = g.sThumbRY;
		t[2] = 'r';
		t[3] = id;
		ev.why = *(u64*)t;
		ev.what = hex32('j','o','y',0);
		ev.where = 0;
		actorwrite(0, 0, 0, 0, &ev, 0x20);
	}
}
void* joystickthread(void* win)
{
	XINPUT_STATE s;

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




void* terminalthread(void* win)
{
	u64 why, what;
	while(1)
	{
		why = lowlevel_input();
		what = hex32('c', 'h', 'a', 'r');
		eventwrite(why, what, 0, 0);
	}
}




void inittray()
{
	console = GetConsoleWindow();
	threadcreate(terminalthread, 0);

	threadcreate(joystickthread, 0);

	//threadcreate(trayworker, 0);
}
void freetray()
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}