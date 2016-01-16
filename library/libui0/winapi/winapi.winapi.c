#include<stdio.h>
#include<stdlib.h>

#include<windows.h>
#include<Commctrl.h>
#include<Winuser.h>

#define QWORD unsigned long long
#define DWORD unsigned int

#define WM_TRAY (WM_USER + 1)
#define menu1 0x1234
#define menu2 0x5678
void diary(char* fmt,...);




//window
HWND window;
HDC realdc;
BITMAPINFO info;
NOTIFYICONDATA nid;     //托盘属性 
HMENU hMenu;            //托盘菜单

// Step 3: the Window Procedure
static int width=1024;
static int height=768;
static unsigned int* mypixel;
static char dragpath[MAX_PATH];

//
static int solved=1;
static QWORD my1;
static QWORD my2;

//
static int leftdown=0,rightdown=0;
static POINT pt, pe;
static RECT rt, re;








QWORD readwindow(QWORD what)
{
	if(what==0x6572656877)
	{
		return (QWORD)mypixel;
	}
	else if(what==0x657a6973)
	{
		return (height<<16)+width;
	}
}
void writewindow(QWORD what)
{
	int temp=height;
	if(temp>1024)temp=1024;

	SetDIBitsToDevice(realdc,
			0,0,		//目标位置x,y
			1024,temp,		//dib宽,高
			0,0,		//来源起始x,y
			0,temp,			//起始扫描线,数组中扫描线数量,
			mypixel,		//rbg颜色数组
			&info,			//bitmapinfo
			DIB_RGB_COLORS);		//颜色格式
	//printf("result:%x\n",result);
}
void waitevent(QWORD* first,QWORD* second)
{
	//收得到就一直收+处理
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		//交给WindowProc，试着处理看看
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//WindowProc处理不了，交给用户来处理
		if(solved==0)
		{
			*first=my1;
			*second=my2;
			solved=1;
			return;
		}
	}

	//收不到就返回失败消息
	*first=0;
}








LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
		//托盘
		case WM_TRAY:
		{
			switch(lparam) 
			{
				case WM_LBUTTONDBLCLK:		//双击
				{
					ShowWindow(window,SW_SHOW);
					break;
				}
				case WM_LBUTTONDOWN:
				{
					if( IsWindowVisible(window) )
					{
						//ShowTrayMsg();
						ShowWindow(window,SW_HIDE);
					}
					else
					{
						ShowWindow(window,SW_SHOW);
					}
					break;
				}
				case WM_RBUTTONDOWN: 
				{ 
					//获取鼠标坐标 
					POINT pt;
					GetCursorPos(&pt); 
   
					//解决在菜单外单击左键菜单不消失的问题 
					SetForegroundWindow(window); 
   
					//显示并获取选中的菜单 
					int cmd=TrackPopupMenu(hMenu,TPM_RETURNCMD,pt.x,pt.y,0,window,0); 
					if(cmd == menu1)
					{
						//lstrcpy(nid.szInfoTitle, "message");
						lstrcpy(nid.szInfo, TEXT("i am groot"));
						nid.uTimeout = 0;
						Shell_NotifyIcon(NIM_MODIFY, &nid);
					}
					if(cmd == menu2)
					{
						PostMessage(window, WM_DESTROY, 0, 0);
					}
					break;
				}
			}
			//diary("tray:\n");
			return 0;
		}

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

			diary("drag:%s\n",dragpath);
			my1=0x656c6966706f7264;		//'dropfile'
			my2=(QWORD)dragpath;
			solved=0;
			return 0;
		}

		//按键
		case WM_KEYDOWN:
		{
			switch(wparam)
			{
				case VK_UP:				//up
				case VK_LEFT:			//left
				case VK_RIGHT:			//right
				case VK_DOWN:			//down
				case 0x70:				//f1
				case 0x71:				//f2
				case 0x72:				//f3
				case 0x73:				//f4
				{
					my1=0x64626b;		//kbd
					my2=wparam;
					solved=0;
					break;
				}
			}
			//diary("key:%x\n",wparam);
			return 0;
		}

		//文字
		case WM_CHAR:
		{
			if(wparam==0x1b)
			{
				my1=0x64626b;			//kbd
				my2=wparam;
				solved=0;
				return 0;
			}
			else
			{
				my1=0x72616863;			//char
				my2=wparam;
				solved=0;
				return 0;
			}
		}
		//case WM_TOUCH:
		//{
		//	
		//}

		//滚轮
		case WM_MOUSEWHEEL:
		{
			if( ((wparam>>16) & 0xffff ) < 0xf000 )
			{
				my1=0x6E6F7266207A7978;		//'xyz fron'
			}
			else my1=0x6B636162207A7978;		//'xyz back'

			GetCursorPos(&pt);
			ScreenToClient(window, &pt);
			my2=(pt.y<<16) + pt.x;

			solved=0;
			return 0;
		}

		//鼠标移动
		case WM_MOUSEMOVE:
		{
			//diary("moving\n");
			if(leftdown>0)
			{
				//diary("moving\n");
				GetCursorPos(&pe);		// 获取光标指针的新位置

				if(rightdown==0xff)		//左右一起按下
				{
					re.left=rt.left+(pe.x - pt.x);		// 窗口新的水平位置
					re.top =rt.top+(pe.y - pt.y);		// 窗口新的垂直位置
					MoveWindow(window,re.left,re.top,re.right,re.bottom,1);// 移动窗口
				}

				else		//只是左键在拖动
				{
					my1=0x65766F6D207A7978;		//'xyz move'
					my2=( ( pe.y - pt.y ) << 16 ) + ( pe.x - pt.x );
					solved=0;

					//diary("%d,%d\n",pe.x,pe.y);
					GetCursorPos(&pt);		// 获取鼠标光标指针当前位置
				}
			}
			return 0;
		}

		//鼠标左键弹起
		case WM_LBUTTONUP:
		{
			if(leftdown==1)
			{
				my1=0x7466656C207A7978;		//'xyz left'
				my2=lparam;
				solved=0;
			}

			leftdown=0;
			return 0;
		}

		//鼠标右键弹起
		case WM_RBUTTONUP:
		{
			if(rightdown==1)
			{
				my1=0x72676968207A7978;		//'xyz righ'
				my2=lparam;
				solved=0;
			}

			rightdown=0;
			return 0;
		}

		//鼠标左键按下
		case WM_LBUTTONDOWN:		//鼠标左键点下
		{
			leftdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(rightdown>0)
			{
				leftdown=rightdown=0xff;
				GetWindowRect(window,&rt);   // 获取窗口位置与大小
				re.right=rt.right-rt.left;               // 保存窗口宽度
				re.bottom=rt.bottom-rt.top; // 保存窗口高度
			}
			return 0;
		}

		//鼠标右键按下
		case WM_RBUTTONDOWN:
		{
			rightdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(leftdown>0)
			{
				leftdown=rightdown=0xff;
				GetWindowRect(window,&rt);   // 获取窗口位置与大小
				re.right=rt.right-rt.left;               // 保存窗口宽度
				re.bottom=rt.bottom-rt.top; // 保存窗口高度
			}
			return 0;
		}

		//窗口尺寸改变
		case WM_SIZE:
		{
			//diary("wparam:%llx,lparam:%llx\n",wparam,lparam);
			my1=0x657a6973;		//tfel//left	//2;
			my2=lparam-( (40<<16) + 16 );
			width=lparam&0xffff;
			height=(lparam>>16)&0xffff;
			solved=0;
			return DefWindowProc(window, msg, wparam, lparam);
		}

		//显示
		case WM_PAINT:
		{
			writewindow(0);
			return DefWindowProc(window, msg, wparam, lparam);
		}

		//摧毁
		case WM_DESTROY:
		{
			//Shell_NotifyIcon(NIM_DELETE, &nid);
			//ReleaseDC(window,fakedc);  
			PostQuitMessage(0);
			return 0;
		}

		//。。。。。。。
		default:
		{
			return DefWindowProc(window, msg, wparam, lparam);
		}
	}
}








//int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
int initmywindow()
{
	//Step 1: Registering the Window Class+Creating the Window
	char *AppTitle="i am groot!";
	WNDCLASS wc;
	wc.style=CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc=WindowProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=0;				//hInst;
	wc.hIcon=LoadIcon(NULL,IDI_WINLOGO);
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=(HBRUSH)COLOR_WINDOWFRAME;
	wc.lpszMenuName=NULL;
	wc.lpszClassName=AppTitle;
	if (!RegisterClass(&wc)) return 0;

	//创建窗口
	window = CreateWindow(AppTitle,AppTitle,
				WS_OVERLAPPEDWINDOW,		//WS_POPUP | WS_MINIMIZEBOX=无边框
				100,100,width+16,height+40,
				NULL,NULL,0,NULL);		//NULL,NULL,hInst,NULL);
	if (!window) return 0;

	//透明
	LONG t = GetWindowLong(window, GWL_EXSTYLE);
	SetWindowLong(window, GWL_EXSTYLE, t | WS_EX_LAYERED);
	SetLayeredWindowAttributes(window, 0, 0xe0, LWA_ALPHA);  

	//显示窗口
	ShowWindow(window,SW_SHOW);			//nCmdShow);
	UpdateWindow(window);
}
void InitUIPIFilter()
{
	typedef BOOL (WINAPI *ChangeWindowMessageFilterProc)(UINT,DWORD);

	//1
	HMODULE hUser = LoadLibraryA("user32.dll");
	if (!hUser){diary("failed to load\n");exit(-1);}

	//2
	ChangeWindowMessageFilterProc proc;
	proc=(ChangeWindowMessageFilterProc)GetProcAddress(hUser, "ChangeWindowMessageFilter");
	if(!proc){diary("can't drag\n");exit(-1);}

	//3
	proc(WM_COPYDATA,1);
	proc(WM_DROPFILES,1);
	proc(0x0049, 1);
}
void inittray()
{
	//Step 2:托盘
	nid.cbSize = sizeof(NOTIFYICONDATA); 
	nid.hWnd = window; 
	nid.uID = 0xabef;		//ID_TRAY_APP_ICON; 
	nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO; 
	nid.uCallbackMessage = WM_TRAY; 
	nid.hIcon = LoadIcon(NULL,IDI_WINLOGO); 
	lstrcpy(nid.szTip, "i am groot!"); 
	Shell_NotifyIcon(NIM_ADD, &nid); 

	hMenu = CreatePopupMenu();    //生成托盘菜单 
	AppendMenu(hMenu, MF_STRING, menu1, TEXT("i am groot")); 
	AppendMenu(hMenu, MF_STRING, menu2, TEXT("i am groot")); 
}
void initdib()
{
	//构造info
	info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth=width;
	info.bmiHeader.biHeight=-height;
	info.bmiHeader.biPlanes=1;
	info.bmiHeader.biBitCount=32;
	info.bmiHeader.biCompression=0;
	info.bmiHeader.biSizeImage=width*height*4;
	info.bmiHeader.biXPelsPerMeter=0;
	info.bmiHeader.biYPelsPerMeter=0;
	info.bmiHeader.biClrUsed=0;
	info.bmiHeader.biClrImportant=0;

	info.bmiColors[0].rgbBlue=255;
	info.bmiColors[0].rgbGreen=255;
	info.bmiColors[0].rgbRed=255;
	info.bmiColors[0].rgbReserved=255;
}








void initwindowworker()
{
	//准备rgb点阵
	//mypixel=(unsigned int*)malloc(width*height*4);
	mypixel=(unsigned int*)malloc(0x400000);

	//准备beforemain
	initmywindow();

	//允许拖拽
	DragAcceptFiles(window, TRUE);
	InitUIPIFilter();

	//托盘
	inittray();

	//dib
	initdib();

	//拿dc
	realdc=GetDC(window);
}
//__attribute__((destructor)) void destorysdl()
void killwindowworker()
{
	//释放dc
	ReleaseDC(window,realdc);

	//释放托盘
	Shell_NotifyIcon(NIM_DELETE, &nid);

	//释放点阵
	free(mypixel);
}
