#include <windows.h>
#include <xinput.h>
#include "libuser.h"
#define WM_TRAY (WM_USER + 1)
#define menu1 0x1111
#define menu2 0x2222
int lowlevel_input();
int arenaevent(void*);




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
	if(g.wButtons & 0x0040)s[3] |= joyl_stick;
	if(g.wButtons & 0x0020)s[3] |= joyl_select;
	if(	(s[3] != 0) |
		(s[1] < -2048) | (s[1] > 2048) |
		(s[0] < -2048) | (s[0] > 2048) )
	{
		t[0] = 'j';
		t[1] = 0x30 + id;
		t[2] = 'l';
		t[3] = '0';
		arenaevent(&ev);
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
	if(g.wButtons & 0x0080)s[3] |= joyr_stick;
	if(g.wButtons & 0x0010)s[3] |= joyr_start;
	if(	(s[3] != 0) |
		(s[1] < -2048) | (s[1] > 2048) |
		(s[0] < -2048) | (s[0] > 2048) )
	{
		t[0] = 'j';
		t[1] = 0x30 + id;
		t[2] = 'r';
		t[3] = '0';
		arenaevent(&ev);
	}
}
void* joystickthread(void* win)
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




void terminalthread(void* win)
{
	while(1)
	{
		eventwrite(lowlevel_input(), _char_, 0, 0);
	}
}
void traycreate(struct arena* win)
{
	threadcreate(terminalthread, 0);
}




void inittray()
{
	console = GetConsoleWindow();
	threadcreate(joystickthread, 0);
}
void freetray()
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}
