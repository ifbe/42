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
static WNDCLASS wc;
static char* AppTitle="haha";
static char dragpath[MAX_PATH];

//each
struct privdata
{
	u64 thread;
	HWND wnd;
	HDC dc;
};

//temp
static BITMAPINFO info;
static TOUCHINPUT touchpoint[10];
static int pointercount=0;
static int pointerid[10];

//temp
static int leftdown=0;
static int rightdown=0;
static POINT pt, pe;
static RECT rt, re;








void bitmapinfo(int w, int h)
{
	info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth=w;
	info.bmiHeader.biHeight=-h;
	info.bmiHeader.biPlanes=1;
	info.bmiHeader.biBitCount=32;
	info.bmiHeader.biCompression=0;
	info.bmiHeader.biSizeImage=w*h*4;
	info.bmiHeader.biXPelsPerMeter=0;
	info.bmiHeader.biYPelsPerMeter=0;
	info.bmiHeader.biClrUsed=0;
	info.bmiHeader.biClrImportant=0;
	info.bmiColors[0].rgbBlue=255;
	info.bmiColors[0].rgbGreen=255;
	info.bmiColors[0].rgbRed=255;
	info.bmiColors[0].rgbReserved=255;
}
LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	struct window* data = (void*)GetWindowLongPtr(wnd, GWLP_USERDATA);
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

			eventwrite((u64)dragpath, 0x656c6966, 0, 0);
			return 0;
		}

		//按键
		case WM_KEYDOWN:
		{
			switch(wparam)
			{
				case 0x70:	//f1
				{
					eventwrite(0xf1, 0x64626b, 0, 0);
					break;
				}
				case 0x71:	//f2
				{
					eventwrite(0xf2, 0x64626b, 0, 0);
					break;
				}
				case 0x72:	//f3
				{
					eventwrite(0xf3, 0x64626b, 0, 0);
					break;
				}
				case 0x73:	//f4
				{
					eventwrite(0xf4, 0x64626b, 0, 0);
					break;
				}
				case VK_UP:				//up
				case VK_LEFT:			//left
				case VK_RIGHT:			//right
				case VK_DOWN:			//down
				{
					eventwrite(wparam, 0x64626b, 0, 0);
					break;
				}
			}
			//say("key:%x\n", wparam);
			return 0;
		}

		//文字
		case WM_CHAR:
		{
			if(wparam==0x1b)eventwrite(0x1b, 0x64626b, 0, 0);
			else eventwrite(wparam, 0x72616863, 0, 0);
			return 0;
		}
/*
		//触摸
		case WM_TOUCH:
		{
			int i=0;
			int count=wparam;
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
				if( pointerid[k] == -1 )
				{
					pointerid[k]=(u8)(wparam);
					break;
				}
			}
			if(k >= 10)return 0;

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (data->h);
			x = (pt.x<<16) / (data->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2b70, 0, 0);
			return 0;
		}
		case WM_POINTERUP:
		{
			u64 x,y,k;
			for(k=0;k<10;k++)
			{
				if( pointerid[k] == (u8)(wparam) )
				{
					pointerid[k]=-1;
					break;
				}
			}
			if(k >= 10)return 0;

			pt.y = GET_Y_LPARAM(lparam);
			pt.x = GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (data->h);
			x = (pt.x<<16) / (data->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2d70, 0, 0);
			return 0;
		}
		case WM_POINTERUPDATE:
		{
			u64 x,y,k;
			for(k=0;k<10;k++)
			{
				if( pointerid[k] == (u8)(wparam) )break;
			}
			if(k >= 10)return 0;

			pt.y=GET_Y_LPARAM(lparam);
			pt.x=GET_X_LPARAM(lparam);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (data->h);
			x = (pt.x<<16) / (data->w);
			eventwrite(x + (y<<16) + (k<<48), 0x4070, 0, 0);
			return 0;
		}

		//滚轮
		case WM_MOUSEWHEEL:
		{
			u64 x,y,k;
			GetCursorPos(&pt);
			ScreenToClient(wnd, &pt);

			y = (pt.y<<16) / (data->h);
			x = (pt.x<<16) / (data->w);
			if( ((wparam>>16) & 0xffff ) < 0xf000 )
			{
				k = 'f';
				eventwrite(x + (y<<16) + (k<<48), 0x2b70, 0, 0);
			}
			else
			{
				k = 'b';
				eventwrite(x + (y<<16) + (k<<48), 0x2b70, 0, 0);
			}

			return 0;
		}

		//鼠标移动
		case WM_MOUSEMOVE:
		{
			u64 x,y,k;
			if(leftdown>0)
			{
				//say("moving\n");
				GetCursorPos(&pe);		// 获取光标指针的新位置

				if(rightdown==0xff)		//左右一起按下
				{
					re.left=rt.left+(pe.x - pt.x);		// 窗口新的水平位置
					re.top =rt.top+(pe.y - pt.y);		// 窗口新的垂直位置
					MoveWindow(wnd, re.left, re.top, re.right, re.bottom, 1);// 移动窗口
				}

				else		//只是左键在拖动
				{
					k = 'l';
					y = (pe.y<<16) / (data->h);
					x = (pe.x<<16) / (data->w);
					eventwrite(x + (y<<16) + (k<<48), 0x4070, 0, 0);

					//say("%d,%d\n", pe.x, pe.y);
					GetCursorPos(&pt);	// 获取鼠标当前位置
				}
			}
			return 0;
		}

		//鼠标左键弹起
		case WM_LBUTTONUP:
		{
			u64 x,y,k;
			if(leftdown==1)
			{
				k = 'l';
				y = (lparam&0xffff0000) / (data->h);
				x = ((lparam&0xffff)<<16) / (data->w);
				eventwrite(x + (y<<16) + (k<<48), 0x2d70, 0, 0);
			}
			leftdown=0;
			return 0;
		}

		//鼠标右键弹起
		case WM_RBUTTONUP:
		{
			u64 x,y,k;
			if(rightdown==1)
			{
				k = 'r';
				y = (lparam&0xffff0000) / (data->h);
				x = ((lparam&0xffff)<<16) / (data->w);
				eventwrite(x + (y<<16) + (k<<48), 0x2d70, 0, 0);
			}

			rightdown=0;
			return 0;
		}

		//鼠标左键按下
		case WM_LBUTTONDOWN:		//鼠标左键点下
		{
			u64 x,y,k;
			leftdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(rightdown>0)
			{
				leftdown=rightdown=0xff;
				GetWindowRect(wnd, &rt);   // 获取窗口位置与大小
				re.right=rt.right-rt.left;               // 保存窗口宽度
				re.bottom=rt.bottom-rt.top; // 保存窗口高度
			}
			
			k = 'l';
			y = (lparam&0xffff0000) / (data->h);
			x = ((lparam&0xffff)<<16) / (data->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2b70, 0, 0);
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
				leftdown=rightdown=0xff;
				GetWindowRect(wnd, &rt);   // 获取窗口位置与大小
				re.right=rt.right-rt.left;               // 保存窗口宽度
				re.bottom=rt.bottom-rt.top; // 保存窗口高度
			}

			k = 'r';
			y = (lparam&0xffff0000) / (data->h);
			x = ((lparam&0xffff)<<16) / (data->w);
			eventwrite(x + (y<<16) + (k<<48), 0x2b70, 0, 0);
			return 0;
		}

		//窗口尺寸改变
		case WM_SIZE:
		{
			int w = lparam&0xffff;
			int h = (lparam>>16)&0xffff;
			printf("wm_size:%d,%d\n", w, h);

			if(data != 0)
			{
				data->w = w;
				data->h = h;
			}

			bitmapinfo(w, h);
			eventwrite(lparam, 0x657a6973, 0, 0);
			return 0;
		}

		//摧毁
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		//显示
		case WM_PAINT:
		{
			say("WM_PAINT\n");
			goto theend;
		}
	}

theend:
	return DefWindowProc(wnd, msg, wparam, lparam);
}
int registermyclass()
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
	if(!RegisterClass(&wc))return 0;

	return 1;
}
HWND createmywindow(int w, int h)
{
	//创建窗口
	HWND win = CreateWindow(
		AppTitle, AppTitle, WS_OVERLAPPEDWINDOW,		//WS_POPUP | WS_MINIMIZEBOX=无边框
		100, 100, w+16, h+38,
		NULL, NULL, 0, NULL);
	if(!win)return 0;

	//透明
	LONG t = GetWindowLong(win, GWL_EXSTYLE);
	SetWindowLong(win, GWL_EXSTYLE, t | WS_EX_LAYERED);
	SetLayeredWindowAttributes(win, 0, 0xf8, LWA_ALPHA);

	//显示窗口
	ShowWindow(win, SW_SHOW);
	UpdateWindow(win);

	return win;
}
void enabledrag(HWND wnd)
{
	typedef BOOL (WINAPI *ChangeWindowMessageFilterProc)(UINT, u32);

	DragAcceptFiles(wnd, TRUE);

	//1
	HMODULE hUser = LoadLibraryA("user32.dll");
	if (!hUser){say("failed to load\n");exit(-1);}

	//2
	ChangeWindowMessageFilterProc proc;
	proc=(ChangeWindowMessageFilterProc)GetProcAddress(hUser, "ChangeWindowMessageFilter");
	if(!proc){say("can't drag\n");exit(-1);}

	//3
	proc(WM_COPYDATA, 1);
	proc(WM_DROPFILES, 1);
	proc(0x0049, 1);
}
DWORD WINAPI uievent(struct window* win)
{
	struct privdata* priv = (void*)(win->priv);
	MSG msg;

	//图形窗口
	priv->wnd = createmywindow(win->w, win->h);
	SetWindowLongPtr(priv->wnd, GWLP_USERDATA, (u64)win);

	//打开拖拽
	enabledrag(priv->wnd);

	//打开触摸
	RegisterTouchWindow(priv->wnd, 0);

	//bmp
	priv->dc = GetDC(priv->wnd);
	bitmapinfo(win->w, win->h);

	//一个一个处理
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	//关闭触摸
	UnregisterTouchWindow(priv->wnd);

	//释放dc
	ReleaseDC(priv->wnd, priv->dc);

	//退出main
	eventwrite(0, 0, 0, 0);
	return 0;
}








void windowread()
{
}
void windowwrite(struct window* win)
{
	struct privdata* priv = (void*)(win->priv);
	SetDIBitsToDevice(
		priv->dc,
		0, 0,			//目标位置x,y
		win->w, win->h,		//dib宽,高
		0, 0,			//来源起始x,y
		0, win->h,		//起始扫描线,数组中扫描线数量,
		(void*)win->buf,	//rbg颜色数组
		&info,			//bitmapinfo
		DIB_RGB_COLORS	//颜色格式
	);
	//printf("result:%x\n",result);
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
void windowstart(struct window* win)
{
	struct privdata* priv = (void*)(win->priv);

	win->type = 0;
	win->fmt = 0x6267726138383838;
	win->len = 2048*1024*4;
	win->buf = (u64)malloc(win->len);

	win->w = 512;
	win->h = 512;

	priv->thread = startthread(uievent, win);
}
void windowstop()
{
}
void windowcreate()
{
	int x;
	for(x=0;x<10;x++)pointerid[x] = -1;

	//
	registermyclass();
}
void windowdelete()
{
}
