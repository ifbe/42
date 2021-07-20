#define WINVER 0x0601
#define WM_POINTERUPDATE 0x0245
#define WM_POINTERDOWN 0x0246
#define WM_POINTERUP 0x0247
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <windowsx.h>
#include <winuser.h>
#include <commctrl.h>
#include "libuser.h"
int arg2utf8(void*, void*);
int rgbanode_take(void*,void*, void*,int, void*,int, void*,int);
int rgbanode_give(void*,void*, void*,int, void*,int, void*,int);




//global
static int alivecount = 0;
static char* AppTitle="haha";
static HANDLE hStartEvent;
static WNDCLASS wc;
//temp
static int leftdown=0;
static int rightdown=0;
static POINT pt, pe;
static RECT rt, re;




int window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	MSG msg;
	BITMAPINFO info;

	//read context
	rgbanode_read(wnd,foot, stack,sp, arg,key, buf,len);

	//update screen
	int w = wnd->width;
	int h = wnd->height;
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = 0;
	info.bmiHeader.biXPelsPerMeter = 0;
	info.bmiHeader.biYPelsPerMeter = 0;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;
	info.bmiColors[0].rgbBlue = 255;
	info.bmiColors[0].rgbGreen = 255;
	info.bmiColors[0].rgbRed = 255;
	info.bmiColors[0].rgbReserved = 255;
	info.bmiHeader.biWidth = w;
	info.bmiHeader.biHeight = -h;
	info.bmiHeader.biSizeImage = w*h*4;
	SetDIBitsToDevice(
		wnd->hdc,
		0, 0,w, h,		//dst: x,y,w,h
		0, 0,0, h,		//src: x,y,0,h
		wnd->rgbabuf, &info, DIB_RGB_COLORS
	);

	//cleanup events
	u64 save[2];
	save[0] = (u64)stack;
	save[1] = sp;
	wnd->spsave = save;

	while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	wnd->spsave = 0;
	return 0;
}
int window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int windowstop(_obj* win)
{
	return 0;
}
int windowstart(_obj* win)
{
	return 0;
}




int windowlist()
{
	return 0;
}
int windowchange()
{
	//RECT rc;
	//GetWindowRect(win, &rc);
	//MoveWindow(win, rc.left, rc.top, width+16, height+39, 0);

	//窗口标题
	//SetWindowText(win, "hahahaha");
	return 0;
}
int windowdelete(_obj* wnd)
{/*
	PostThreadMessage(
		uithread,
		WM_USER,
		hex16('w','-'),
		(LPARAM)wnd
	);*/
	HWND hwnd = wnd->hwnd;
	HDC dc = wnd->hdc;

	ReleaseDC(hwnd, dc);

	UnregisterTouchWindow(hwnd);

	DestroyWindow(hwnd);

	alivecount--;
	if(alivecount == 0)eventwrite(0,0,0,0);
	return 0;
}
int windowcreate(_obj* wnd)
{
	if(0 == wnd)return 0;

	wnd->fmt = _rgba_;
	wnd->vfmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');

	wnd->width = 1024;
	wnd->height = 768;

	wnd->fbwidth = 1024*4;
	//wnd->fbheight = 0;

	wnd->hwnd = 0;
	wnd->hdc = 0;

	//per window data
	int j;
	int* finger = wnd->perwnd = malloc(0x1000);
	for(j=0;j<10;j++)finger[j] = -1;

	//rgba buffer
	wnd->rgbabuf = malloc(2048*2048*4);




	//创建窗口
	RECT tmp;
	tmp.left = 0;
	tmp.right = wnd->width;
	tmp.top = 0;
	tmp.bottom = wnd->height;
	AdjustWindowRect(&tmp, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(
		AppTitle,
		AppTitle,
		WS_OVERLAPPEDWINDOW,		//WS_POPUP | WS_MINIMIZEBOX=无边框
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		tmp.right-tmp.left,
		tmp.bottom-tmp.top,
		NULL,
		NULL,
		0,
		NULL
	);
	if(!hwnd)return 0;

	//dc
	HDC dc = GetDC(hwnd);
	if(!dc)return 0;

	//透明
	LONG t = GetWindowLong(hwnd, GWL_EXSTYLE);
	SetWindowLong(hwnd, GWL_EXSTYLE, t | WS_EX_LAYERED);
	SetLayeredWindowAttributes(hwnd, 0, 0xf8, LWA_ALPHA);

	//显示窗口
	ShowWindow(hwnd, SW_SHOW);
	UpdateWindow(hwnd);

	//打开触摸
	RegisterTouchWindow(hwnd, 0);

	//打开拖拽
	typedef BOOL (WINAPI *ChangeWindowMessageFilterProc)(UINT, u32);
	DragAcceptFiles(hwnd, TRUE);

	HMODULE hUser = LoadLibraryA("user32.dll");
	if(!hUser){say("failed to load\n");exit(-1);}

	ChangeWindowMessageFilterProc hProc;
	hProc = (ChangeWindowMessageFilterProc)GetProcAddress(hUser, "ChangeWindowMessageFilter");
	if(!hProc){say("can't drag\n");exit(-1);}

	hProc(WM_COPYDATA, 1);
	hProc(WM_DROPFILES, 1);
	hProc(0x0049, 1);

	//完成
	wnd->hwnd = hwnd;
	wnd->hdc = dc;
	SetWindowLongPtr(hwnd, GWLP_USERDATA, (u64)wnd);
	alivecount++;

	MSG msg;
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);
	return 0;
}




static int whichfinger(_obj* wnd, int old, int new)
{
	int k;
	int* tab = wnd->perwnd;
	for(k=0;k<10;k++)
	{
		if(old == tab[k])goto found;
	}
	return 99;

found:
	if(old != new)tab[k] = new;
	return k;
}
static void restorestackdeliverevent(_obj* wnd, struct event* ev)
{
	u64* save = wnd->spsave;
	if(0 == save){
		eventwrite(ev->why, ev->what, ev->where, 0);
		return;
	}

	struct halfrel* stack = (void*)save[0];
	int depth = save[1];
	rgbanode_write(wnd,0, stack,depth, 0,0, ev,0);
}
LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	u64 addr = GetWindowLongPtr(wnd, GWLP_USERDATA);
	_obj* win = (void*)addr;

	struct event ev;
	switch (msg)
	{
		case WM_KEYDOWN:
		{
			u64 val;
			switch(wparam)
			{
				case VK_UP:{val = 0x48;break;}
				case VK_LEFT:{val = 0x4b;break;}
				case VK_RIGHT:{val = 0x4d;break;}
				case VK_DOWN:{val = 0x50;break;}
				case 0x70:{val = 0xf1;break;}
				case 0x71:{val = 0xf2;break;}
				case 0x72:{val = 0xf3;break;}
				case 0x73:{val = 0xf4;break;}
				case 0x74:{val = 0xf5;break;}
				case 0x75:{val = 0xf6;break;}
				case 0x76:{val = 0xf7;break;}
				case 0x77:{val = 0xf8;break;}
				case 0x78:{val = 0xf9;break;}
				case 0x79:{val = 0xfa;break;}
				case 0x7a:{val = 0xfb;break;}
				case 0x7b:{val = 0xfc;break;}
				default:return 0;
			}

			//printf("key:%x\n", wparam);
			//eventwrite(val, 0x64626b, addr, 0);
			ev.why = val;
			ev.what = 0x64626b;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_CHAR:
		{
			//printf("%x\n",wparam);
			if(wparam==0x1b)
			{
				//eventwrite(0x1b, 0x64626b, addr, 0);
				ev.why = 0x1b;
				ev.what = 0x64626b;
				ev.where = addr;
				restorestackdeliverevent(win, &ev);
			}
			else
			{
				//eventwrite(wparam, 0x72616863, addr, 0);
				ev.why = wparam;
				ev.what = 0x72616863;
				ev.where = addr;
				restorestackdeliverevent(win, &ev);
			}
			return 0;
		}
/*
		//obsolete
		case WM_TOUCH:
		{
			int i=0;
			int count=wparam;
			TOUCHINPUT touchpoint[10];
			GetTouchInputInfo(
				(HTOUCHINPUT)lparam,
				count,
				touchpoint,
				sizeof(TOUCHINPUT)
			);

			for(i=0;i<count;i++)
			{
				type[i]=0x4070;

				key[i]=i;
				key[i]=(value<<16)+0;
				key[i]=(value<<16)+touchpoint[i].y;
				key[i]=(value<<16)+touchpoint[i].x;

				say("id=%x:x=%x,y=%x\n",
					touchpoint[i].dwID,
					touchpoint[i].x,
					touchpoint[i].y
				);

			}
			this=0;
			that=count-1;
			return 0;
		}
*/
		case WM_POINTERDOWN:
		{
			u64 x,y,k;
			k = whichfinger(win, -1, (u16)wparam);
			//say("k=%d\n",k);
			if(k >= 10)return 0;

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);
			y = pt.y;
			x = pt.x;
			//eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = touch_onto;
			ev.where = addr;
			//printmemory(&ev,16);
			restorestackdeliverevent(win, &ev);
			return 0;
		}
		case WM_POINTERUP:
		{
			u64 x,y,k;
			k = whichfinger(win, (u16)wparam, -1);
			//say("k=%d\n",k);
			if(k >= 10)return 0;

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = pt.y;
			x = pt.x;
			//eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = touch_away;
			ev.where = addr;
			//printmemory(&ev,16);
			restorestackdeliverevent(win, &ev);
			return 0;
		}
		case WM_POINTERUPDATE:
		{
			u64 x,y,k;
			k = whichfinger(win, (u16)wparam, (u16)wparam);
			//say("k=%d\n",k);
			if(k >= 10)return 0;

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = pt.y;
			x = pt.x;
			//eventwrite(x + (y<<16) + (k<<48), 0x4070, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = touch_move;
			ev.where = addr;
			//printmemory(&ev,16);
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_MOUSEWHEEL:
		{
			u64 x,y,k;
			GetCursorPos(&pt);
			ScreenToClient(wnd, &pt);

			short* t = (void*)&ev.why;
			t[0] = pt.x;
			t[1] = pt.y;
			t[2] = (wparam>16)&0xffff;
			t[3] = (t[2]>0) ? 'f' : 'b';

			ev.what = 0x4070;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			u64 x,y,k;
			if((leftdown>0)&&(rightdown>0))
			{
				GetCursorPos(&pe);		// 获取光标指针的新位置
				re.left=rt.left+(pe.x - pt.x);		// 窗口新的水平位置
				re.top =rt.top+(pe.y - pt.y);		// 窗口新的垂直位置
				MoveWindow(wnd, re.left, re.top, re.right, re.bottom, 1);// 移动窗口
				GetCursorPos(&pt);			// 获取鼠标当前位置
				GetWindowRect(wnd, &rt);	// 获取窗口位置与大小
				return 0;
			}
			else if(rightdown>0)k = 'r';
			else if(leftdown>0)k = 'l';
			else k = 'l';

			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x4070, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x4070;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			u64 x,y,k;
			leftdown=0;

			k = 'l';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2d70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_RBUTTONUP:
		{
			u64 x,y,k;
			rightdown=0;

			k = 'r';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2d70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			u64 x,y,k;
			leftdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(rightdown>0)
			{
				GetWindowRect(wnd, &rt);	// 获取窗口位置与大小
				re.right=rt.right-rt.left;	// 保存窗口宽度
				re.bottom=rt.bottom-rt.top;	// 保存窗口高度
			}
			
			k = 'l';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2b70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			u64 x,y,k;
			rightdown=1;
			GetCursorPos(&pt);

			if(leftdown>0)
			{
				GetWindowRect(wnd, &rt);
				re.right=rt.right-rt.left;
				re.bottom=rt.bottom-rt.top;
			}

			k = 'r';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2b70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_DROPFILES:
		{
			int j,ret=0;
			char tmp[0x1000];
			char buf[0x1000];
			HDROP hDrop = (HDROP)wparam;

			UINT nNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
			for(j=0;j<nNum;j++)  
			{
				DragQueryFile(hDrop, j, tmp, MAX_PATH);
				//printf("%d,%s\n", ret, buf);

				ret += arg2utf8(tmp, buf+ret);
				buf[ret] = '\n';
				ret++;
			}
			DragFinish(hDrop);
			buf[ret] = 0;

			ev.why = (u64)buf;
			ev.what = _drag_;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_SIZE:
		{
			int w = lparam&0xffff;
			int h = (lparam>>16)&0xffff;
			//printf("wm_size:%d,%d\n", w, h);

			if(win != 0)
			{
				win->width = w;
				win->height = h;

				win->fbwidth = w*4;
			}

			//eventwrite(0x657a6973, 0x4077, addr, 0);
			return 0;
		}

		case WM_PAINT:
		{
			goto theend;
		}

		case WM_CLOSE:
		{
			eventwrite(0,0,0,0);
			PostQuitMessage(0);
			return 0;
		}

		case WM_DESTROY:
		{
			DestroyWindow(wnd);
			return 0;
		}
	}

theend:
	return DefWindowProc(wnd, msg, wparam, lparam);
}




void freewindow()
{
}
void initwindow()
{
	//
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = 0;				//hInst;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOWFRAME;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = AppTitle;
	if(!RegisterClass(&wc))
	{
		printf("error@RegisterClass\n");
		return;
	}
/*
	//createevent
	hStartEvent = CreateEvent(0,FALSE,FALSE,0);

	//uithread
	uithread = threadcreate(windowthread, 0);

	//waitevent
	WaitForSingleObject(hStartEvent,INFINITE);

	//deleteevent
	CloseHandle(hStartEvent);
*/
}
