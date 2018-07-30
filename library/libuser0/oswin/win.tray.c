#include <windows.h>
#include <xinput.h>
#include "libuser.h"
#define WM_TRAY (WM_USER + 1)
#define menu1 0x1111
#define menu2 0x2222
#define _dl_ hex16('d','l')
#define _dr_ hex16('d','r')
#define _dn_ hex16('d','n')
#define _df_ hex16('d','f')
#define _ka_ hex16('k','a')
#define _kb_ hex16('k','b')
#define _kx_ hex16('k','x')
#define _ky_ hex16('k','y')
#define _lt_ hex16('l','t')
#define _rt_ hex16('r','t')
#define _lb_ hex16('l','b')
#define _rb_ hex16('r','b')
#define _ls_ hex16('l','s')
#define _rs_ hex16('r','s')
#define _ll_ hex16('l','l')
#define _rr_ hex16('r','r')
#define _joy_ hex32('j','o','y',0)
int lowlevel_input();




struct xxxx
{
	u16 val;
	char* name;
};
static struct xxxx xtab[16] = {
	_df_, "XINPUT_GAMEPAD_DPAD_UP",		//0x0001
	_dn_, "XINPUT_GAMEPAD_DPAD_DOWN",		//0x0002
	_dl_, "XINPUT_GAMEPAD_DPAD_LEFT",		//0x0004
	_dr_, "XINPUT_GAMEPAD_DPAD_RIGHT",	//0x0008
	_rr_, "XINPUT_GAMEPAD_START",			//0x0010
	_ll_, "XINPUT_GAMEPAD_BACK",			//0x0020
	_ls_, "XINPUT_GAMEPAD_LEFT_THUMB",	//0x0040
	_rs_, "XINPUT_GAMEPAD_RIGHT_THUMB",	//0x0080
	_lb_, "XINPUT_GAMEPAD_LEFT_SHOULDER",		//0x0100
	_rb_, "XINPUT_GAMEPAD_RIGHT_SHOULDER",	//0x0200
	0, "????1",	//0x0400
	0, "????2",	//0x0800
	_ka_, "XINPUT_GAMEPAD_A",		//0x1000
	_kb_, "XINPUT_GAMEPAD_B",		//0x2000
	_kx_, "XINPUT_GAMEPAD_X",		//0x4000
	_ky_, "XINPUT_GAMEPAD_Y"		//0x8000
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

	for(j=0;j<16;j++)
	{
		if(0 == xtab[j].val)continue;

		k = 1<<j;
		if((0 != (g.wButtons&k)) && (0 == (btn&k)))
		{
			//say("	%x:%s\n", k, xtab[j].name);

			t[0] = 0;
			t[1] = 0;
			t[2] = xtab[j].val;
			t[3] = id;
			ev.why = *(u64*)t;
			ev.what = _joy_;
			ev.where = 0;
			actorwrite(0, 0, 0, 0, &ev, 0x20);
		}
	}
	btn = g.wButtons;

	if(g.bLeftTrigger > 8)
	{
		t[0] = g.bLeftTrigger;
		t[1] = 0;
		t[2] = _lt_;
		t[3] = id;
		ev.why = *(u64*)t;
		ev.what = _joy_;
		ev.where = 0;
		actorwrite(0, 0, 0, 0, &ev, 0x20);
	}

	if(g.bRightTrigger > 8)
	{
		t[0] = g.bRightTrigger;
		t[1] = 0;
		t[2] = _rt_;
		t[3] = id;
		ev.why = *(u64*)t;
		ev.what = _joy_;
		ev.where = 0;
		actorwrite(0, 0, 0, 0, &ev, 0x20);
	}

	if(	(g.sThumbLX < -2048) | (g.sThumbLX > 2048) | (g.sThumbLY < -2048) | (g.sThumbLY > 2048) )
	{
		t[0] = g.sThumbLX;
		t[1] = g.sThumbLY;
		t[2] = 'l';
		t[3] = id;
		ev.why = *(u64*)t;
		ev.what = _joy_;
		ev.where = 0;
		actorwrite(0, 0, 0, 0, &ev, 0x20);
	}

	if(	(g.sThumbRX < -2048) | (g.sThumbRX > 2048) | (g.sThumbRY < -2048) | (g.sThumbRY > 2048) )
	{
		t[0] = g.sThumbRX;
		t[1] = g.sThumbRY;
		t[2] = 'r';
		t[3] = id;
		ev.why = *(u64*)t;
		ev.what = _joy_;
		ev.where = 0;
		actorwrite(0, 0, 0, 0, &ev, 0x20);
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




void inittray()
{
	console = GetConsoleWindow();
	threadcreate(joystickthread, 0);

	//threadcreate(trayworker, 0);
}
void freetray()
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}