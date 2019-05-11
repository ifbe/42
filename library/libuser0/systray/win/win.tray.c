#include <windows.h>
#include <xinput.h>
#include "libuser.h"
#define WM_TRAY (WM_USER + 1)
#define menu1 0x1111
#define menu2 0x2222




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
void traycreate(struct arena* win)
{
	threadcreate(trayworker, win);
}




void inittray()
{
	console = GetConsoleWindow();
}
void freetray()
{
	Shell_NotifyIcon(NIM_DELETE, &nid);
}
