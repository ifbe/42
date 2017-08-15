#define WINVER 0x0601
#define WM_POINTERUPDATE 0x0245
#define WM_POINTERDOWN 0x0246
#define WM_POINTERUP 0x0247
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<windowsx.h>
#include<winuser.h>
#include<commctrl.h>
#include "arena.h"
//
u64 startthread(void*, void*);
void stopthread();
//
void eventwrite(u64, u64, u64, u64);
void say(char* fmt, ...);




//global
static u64 uithread;
static HANDLE hStartEvent;
static WNDCLASS wc;
static char* AppTitle="haha";
static char dragpath[MAX_PATH];

//temp
static int leftdown=0;
static int rightdown=0;
static POINT pt, pe;
static RECT rt, re;








LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	u64 addr = GetWindowLongPtr(wnd, GWLP_USERDATA);
	struct window* win = (void*)addr;
	switch (msg)
	{
		//拖拽文件
		case WM_DROPFILES:
		{
			HDROP hDrop = (HDROP)wparam;
			UINT nFileNum = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0); // 拖拽文件个数
			int i;
			for (i = 0; i < nFileNum; i++)  
			{
				DragQueryFile(hDrop, i, dragpath, MAX_PATH);//获得拖曳的文件名
			}
			DragFinish(hDrop);      //释放hDrop

			eventwrite((u64)dragpath, 0x656c6966, addr, 0);
			return 0;
		}

		//按键
		case WM_KEYDOWN:
		{
			u64 val;
			switch(wparam)
			{
				case 0x70:	//f1
				{
					val = 0xf1;
					break;
				}
				case 0x71:	//f2
				{
					val = 0xf2;
					break;
				}
				case 0x72:	//f3
				{
					val = 0xf3;
					break;
				}
				case 0x73:	//f4
				{
					val = 0xf4;
					break;
				}
				case VK_UP:				//up
				case VK_LEFT:			//left
				case VK_RIGHT:			//right
				case VK_DOWN:			//down
				{
					val = wparam;
					break;
				}
				default:return 0;
			}

			//printf("key:%x\n", wparam);
			eventwrite(val, 0x64626b, addr, 0);
			return 0;
		}

		//文字
		case WM_CHAR:
		{
			if(wparam==0x1b)eventwrite(0x1b, 0x64626b, addr, 0);
			else eventwrite(wparam, 0x72616863, addr, 0);
			return 0;
		}
/*
		//触摸
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
			for(k=0;k<10;k++)
			{
				if( win->touch[k].id == (u16)wparam )
				{
					//find self
					break;
				}
				if( win->touch[k].id == 0xffff )
				{
					//find empty
					win->touch[k].id = (u16)wparam;
					break;
				}
			}

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (win->h);
			x = (pt.x<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);
			return 0;
		}
		case WM_POINTERUP:
		{
			u64 x,y,k;
			for(k=0;k<10;k++)
			{
				if( win->touch[k].id == (u16)wparam )
				{
					//find self
					win->touch[k].id = 0xffff;
					break;
				}
			}

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (win->h);
			x = (pt.x<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);
			return 0;
		}
		case WM_POINTERUPDATE:
		{
			u64 x,y,k;
			for(k=0;k<10;k++)
			{
				if( win->touch[k].id == (u16)wparam )break;
			}
			if(k >= 10)return 0;

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (win->h);
			x = (pt.x<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x4070, addr, 0);
			return 0;
		}

		//滚轮
		case WM_MOUSEWHEEL:
		{
			u64 x,y,k;
			GetCursorPos(&pt);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (win->h);
			x = (pt.x<<16) / (win->w);
			if( ((wparam>>16) & 0xffff ) < 0xf000 )
			{
				k = 'f';
				eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);
			}
			else
			{
				k = 'b';
				eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);
			}

			return 0;
		}

		//鼠标移动
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
			else k = '?';

			y = (lparam&0xffff0000) / (win->h);
			x = ((lparam&0xffff)<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x4070, addr, 0);
			return 0;
		}

		//鼠标左键弹起
		case WM_LBUTTONUP:
		{
			u64 x,y,k;
			leftdown=0;

			k = 'l';
			y = (lparam&0xffff0000) / (win->h);
			x = ((lparam&0xffff)<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);
			return 0;
		}

		//鼠标右键弹起
		case WM_RBUTTONUP:
		{
			u64 x,y,k;
			rightdown=0;

			k = 'r';
			y = (lparam&0xffff0000) / (win->h);
			x = ((lparam&0xffff)<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);
			return 0;
		}

		//鼠标左键按下
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
			y = (lparam&0xffff0000) / (win->h);
			x = ((lparam&0xffff)<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);
			return 0;
		}

		//鼠标右键按下
		case WM_RBUTTONDOWN:
		{
			u64 x,y,k;
			rightdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(leftdown>0)
			{
				GetWindowRect(wnd, &rt);	// 获取窗口位置与大小
				re.right=rt.right-rt.left;	// 保存窗口宽度
				re.bottom=rt.bottom-rt.top;	// 保存窗口高度
			}

			k = 'r';
			y = (lparam&0xffff0000) / (win->h);
			x = ((lparam&0xffff)<<16) / (win->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);
			return 0;
		}

		//窗口尺寸改变
		case WM_SIZE:
		{
			int w = lparam&0xffff;
			int h = (lparam>>16)&0xffff;
			//printf("wm_size:%d,%d\n", w, h);

			if(win != 0)
			{
				win->w = w;
				win->h = h;
			}

			eventwrite(lparam, 0x657a6973, addr, 0);
			return 0;
		}

		//显示
		case WM_PAINT:
		{
			//say("WM_PAINT\n");
			goto theend;
		}

		//摧毁
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}

theend:
	return DefWindowProc(wnd, msg, wparam, lparam);
}
void createmywindow(struct window* this)
{
	HWND wnd;
	HDC dc;

	//创建窗口
	wnd = CreateWindow(
		AppTitle, AppTitle, WS_OVERLAPPEDWINDOW,		//WS_POPUP | WS_MINIMIZEBOX=无边框
		100, 100, (this->w)+16, (this->h)+39,
		NULL, NULL, 0, NULL);
	if(!wnd)return;

	//dc
	dc = GetDC(wnd);

	//透明
	LONG t = GetWindowLong(wnd, GWL_EXSTYLE);
	SetWindowLong(wnd, GWL_EXSTYLE, t | WS_EX_LAYERED);
	SetLayeredWindowAttributes(wnd, 0, 0xf8, LWA_ALPHA);

	//显示窗口
	ShowWindow(wnd, SW_SHOW);
	UpdateWindow(wnd);

	//打开触摸
	RegisterTouchWindow(wnd, 0);

	//打开拖拽
	typedef BOOL (WINAPI *ChangeWindowMessageFilterProc)(UINT, u32);
	DragAcceptFiles(wnd, TRUE);

	HMODULE hUser = LoadLibraryA("user32.dll");
	if(!hUser){say("failed to load\n");exit(-1);}

	ChangeWindowMessageFilterProc hProc;
	hProc = (ChangeWindowMessageFilterProc)GetProcAddress(hUser, "ChangeWindowMessageFilter");
	if(!hProc){say("can't drag\n");exit(-1);}

	hProc(WM_COPYDATA, 1);
	hProc(WM_DROPFILES, 1);
	hProc(0x0049, 1);

	//完成
	this->fd = (u64)wnd;
	this->dc = (u64)dc;
	SetWindowLongPtr(wnd, GWLP_USERDATA, (u64)this);
}
DWORD WINAPI uievent()
{
	MSG msg;

	//create queue
	PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	//fire event
	SetEvent(hStartEvent);

	//wait
	while(GetMessage(&msg, NULL, 0, 0))
	{
		if(msg.message == WM_USER)
		{
			createmywindow((void*)(msg.lParam));
		}
		else
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	//退出main
	eventwrite(0, 0, 0, 0);
	return 0;
}




void windowwrite(struct window* dst, struct window* src)
{
	BITMAPINFO info;
	int w = dst->w;
	int h = dst->h;
	HDC dc = (void*)(dst->dc);
	void* buf = (void*)(src->buf);

	//bitmapinfo(w,h);
	info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth = w;
	info.bmiHeader.biHeight = -h;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = 32;
	info.bmiHeader.biCompression = 0;
	info.bmiHeader.biSizeImage = w*h*4;
	info.bmiHeader.biXPelsPerMeter = 0;
	info.bmiHeader.biYPelsPerMeter = 0;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;
	info.bmiColors[0].rgbBlue = 255;
	info.bmiColors[0].rgbGreen = 255;
	info.bmiColors[0].rgbRed = 255;
	info.bmiColors[0].rgbReserved = 255;

	//write bmp to win
	SetDIBitsToDevice(
		dc,
		0, 0,		//目标位置x,y
		w, h,		//dib宽,高
		0, 0,		//来源起始x,y
		0, h,		//起始扫描线,数组中扫描线数量,
		buf,		//rbg颜色数组
		&info,		//bitmapinfo
		DIB_RGB_COLORS	//颜色格式
	);
	//printf("result:%x\n",result);
}
void windowread()
{
}
void windowlist()
{
}
void windowchange()
{
	//RECT rc;
	//GetWindowRect(win, &rc);
	//MoveWindow(win, rc.left, rc.top, width+16, height+39, 0);

	//窗口标题
	//SetWindowText(win, "hahahaha");
}
void windowstart(struct window* this)
{
	int ret;
	this->w = 512;
	this->h = 512;
	if(this->type == hex32('b','u','f',0))
	{
		this->fmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');
		return;
	}
	else
	{
		this->type = hex32('w', 'i', 'n', 0);
		this->fmt = hex32('a', 'p', 'i', 0);

		for(ret=0;ret<16;ret++)
		{
			(this->touch[ret]).id = 0xffff;
		}
		ret = PostThreadMessage(uithread, WM_USER, hex16('w','+'), (LPARAM)this);
	}
}
void windowstop(struct window* this)
{
	HWND wnd = (void*)(this->fd);
	HDC dc = (void*)(this->dc);

	//关闭触摸
	UnregisterTouchWindow(wnd);

	//释放dc
	ReleaseDC(wnd, dc);
}
void windowcreate()
{
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

	//createevent
	hStartEvent = CreateEvent(0,FALSE,FALSE,0);

	//createthread
	uithread = startthread(uievent, 0);

	//waitevent
	WaitForSingleObject(hStartEvent,INFINITE);

	//deleteevent
    CloseHandle(hStartEvent);
}
void windowdelete()
{
}
