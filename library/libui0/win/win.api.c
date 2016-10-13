#define WINVER 0x0601
#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
#include<windowsx.h>
#include<winuser.h>
#include<commctrl.h>

#define WM_POINTERUPDATE 0x0245
#define WM_POINTERDOWN 0x0246
#define WM_POINTERUP 0x0247
#define WM_TRAY (WM_USER + 1)
#define menu1 0x1111
#define menu2 0x2222
#define menu3 0x3333
#define menu4 0x4444
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void windowwrite();
void say(char* fmt,...);




//window
static HWND consolewindow;		//console window
static HWND window;				//my window
static HDC realdc;
static BITMAPINFO info;
static NOTIFYICONDATA nid;     //托盘属性 
static HMENU hMenu;            //托盘菜单
static TOUCHINPUT touchpoint[10];
static char dragpath[MAX_PATH];

//
static int pointercount=0;
static u8 pointerid[10];

//
static int this=-1;
static int that=-1;
static u64 type[10];
static u64 key[10];

//
static int leftdown=0,rightdown=0;
static POINT pt, pe;
static RECT rt, re;

//
static char* screenbuf;
static int width=1024;
static int height=768;




LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
		//托盘
		case WM_TRAY:
		{
			switch(lparam) 
			{
				case WM_LBUTTONDOWN:
				{
					//window开着：显示consolewindow，隐藏window
					if( IsWindowVisible(window) )
					{
						ShowWindow(consolewindow,SW_SHOW);
						ShowWindow(window,SW_HIDE);
					}

					//window隐藏：隐藏consolewindow，显示window
					else
					{
						ShowWindow(consolewindow,SW_HIDE);
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
						if( IsWindowVisible(consolewindow) )
						{
							//ShowTrayMsg();
							ShowWindow(consolewindow,SW_HIDE);
						}
						else
						{
							ShowWindow(consolewindow,SW_SHOW);
						}
					}
					if(cmd == menu2)
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
					}
					if(cmd == menu3)
					{
						//lstrcpy(nid.szInfoTitle, "message");
						lstrcpy(nid.szInfo, TEXT("i am groot"));
						nid.uTimeout = 0;
						Shell_NotifyIcon(NIM_MODIFY, &nid);
					}
					if(cmd == menu4)
					{
						PostMessage(window, WM_DESTROY, 0, 0);
					}
					break;
				}
			}
			//say("tray:\n");
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

			say("drag:%s\n",dragpath);
			type[0]=(0x706f7264)+((u64)0x656c6966<<32);	//'drop''file'
			key[0]=(u64)dragpath;
			this=0;
			that=-1;
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
					type[0]=0x64626b;		//kbd
					key[0]=wparam;
					this=0;
					that=-1;
					break;
				}
			}
			//say("key:%x\n",wparam);
			return 0;
		}

		//文字
		case WM_CHAR:
		{
			if(wparam==0x1b)
			{
				type[0]=0x64626b;		//'kbd'
				key[0]=wparam;
			}
			else
			{
				type[0]=0x72616863;		//'char'
				key[0]=wparam;
			}
			this=0;
			that=-1;
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
			pt.y=GET_Y_LPARAM(lparam);
			pt.x=GET_X_LPARAM(lparam);
			ScreenToClient(window, &pt);

			type[0]=0x2b70;		//p+
			key[0]=pointercount;
			key[0]=(key[0]<<16) + 0;
			key[0]=(key[0]<<16) + pt.y;
			key[0]=(key[0]<<16) + pt.x;

			pointerid[pointercount]=(u8)(wparam);
			pointercount++;
			this=0;
			that=-1;
			return 0;
		}
		case WM_POINTERUP:
		{
			int x,y;
			for(x=0;x<pointercount;x++)
			{
				say("%2d,",pointerid[x]);
				if( pointerid[x] == (u8)(wparam) )
				{
					for(y=x;y<pointercount-1;y++)
					{
						pointerid[y]=pointerid[y+1];
					}
					break;
				}
			}

			say("<-------->%2d\n",(u8)wparam);
			if(x>=pointercount)
			{
				say("wrong!!!!!!!!!!\n");
				this=-1;
				return 0;
			}

			pt.y=GET_Y_LPARAM(lparam);
			pt.x=GET_X_LPARAM(lparam);
			ScreenToClient(window, &pt);

			type[0]=0x2d70;		//p-
			key[0]=x;
			key[0]=(key[0]<<16) + 0;
			key[0]=(key[0]<<16) + pt.y;
			key[0]=(key[0]<<16) + pt.x;
			this=0;
			that=-1;

			pointercount--;
			return 0;
		}
		case WM_POINTERUPDATE:
		{
			int x;
			for(x=0;x<pointercount;x++)
			{
				if( pointerid[x] == (u8)(wparam) )break;
			}
			if(x>=pointercount)
			{
				this=-1;return 0;
			}

			pt.y=GET_Y_LPARAM(lparam);
			pt.x=GET_X_LPARAM(lparam);
			ScreenToClient(window, &pt);

			type[0]=0x4070;		//p@
			key[0]=x;
			key[0]=(key[0]<<16) + 0;
			key[0]=(key[0]<<16) + pt.y;
			key[0]=(key[0]<<16) + pt.x;
			this=0;
			that=-1;
			return 0;
		}

		//滚轮
		case WM_MOUSEWHEEL:
		{
			type[0] = 0x2b6d;
			GetCursorPos(&pt);
			ScreenToClient(window, &pt);

			if( ((wparam>>16) & 0xffff ) < 0xf000 )
			{
				key[0] = pt.x + (pt.y<<16) + (4<<48);
			}
			else
			{
				key[0] = pt.x + (pt.y<<16) + (5<<48);
			}

			this=0;
			that=-1;
			return 0;
		}

		//鼠标移动
		case WM_MOUSEMOVE:
		{
			//say("moving\n");
			if(leftdown>0)
			{
				//say("moving\n");
				GetCursorPos(&pe);		// 获取光标指针的新位置

				if(rightdown==0xff)		//左右一起按下
				{
					re.left=rt.left+(pe.x - pt.x);		// 窗口新的水平位置
					re.top =rt.top+(pe.y - pt.y);		// 窗口新的垂直位置
					MoveWindow(window,re.left,re.top,re.right,re.bottom,1);// 移动窗口
				}

				else		//只是左键在拖动
				{
					//'xyz ','move'
					type[0] = 0x406d;
					key[0] = (pe.x-pt.x) + ((pe.y-pt.y)<<16) + ((u64)1<<48);

					//say("%d,%d\n",pe.x,pe.y);
					GetCursorPos(&pt);	// 获取鼠标当前位置
					this=0;
					that=-1;
				}
			}
			return 0;
		}

		//鼠标左键弹起
		case WM_LBUTTONUP:
		{
			if(leftdown==1)
			{
				type[0] = 0x2d6d;
				key[0] = lparam + ((u64)1<<48);
				this=0;
				that=-1;
			}

			leftdown=0;
			return 0;
		}

		//鼠标右键弹起
		case WM_RBUTTONUP:
		{
			if(rightdown==1)
			{
				type[0] = 0x2d6d;
				key[0] = lparam + ((u64)1<<48);
				this=0;
				that=-1;
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
			width=lparam&0xffff;
			height=(lparam>>16)&0xffff;
			printf("wm_size:%d,%d\n",width,height);

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

			//say("WM_SIZE:wparam=%llx,lparam=%llx\n",wparam,lparam);
			type[0]=0x657a6973;			//'size'
			key[0]=lparam;
			this=0;
			that=-1;

			return 0;
		}
/*
		//显示
		case WM_PAINT:
		{
			say("WM_PAINT\n");
			windowwrite();

			//这里必须调这个函数，不然cpu占用满
			return DefWindowProc(window, msg, wparam, lparam);
		}
*/
		//摧毁
		case WM_DESTROY:
		{
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
int uievent(u64* who, u64* what, u64* how)
{
throw:
	//没扔完的消息一个个扔，没消息了就下去等
	if(this!=-1)
	{
		*what = type[this];
		*how = key[this];

		if(that==-1)
		{
			this=-1;
		}
		else
		{
			this++;
			if(this==that)
			{
				this=that=-1;
			}
		}

		return 1;
	}

	//收得到就一直收+处理
	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		//交给WindowProc，试着处理看看
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		//WindowProc处理不了，交给用户来处理
		if(this==0)goto throw;
	}

	//收不到就返回失败消息
	*what = 0;
	return 0;
}








void windowlist()
{
}
void windowchange(u32 size,char* addr)
{
	RECT rc;
	width=size&0xffff;
	height=(size>>16)&0xffff;

	//GetWindowRect(window,&rc);
	//MoveWindow(window , rc.left , rc.top , width+16 , height+40 , 0);
}








// Step 3: the Window Procedure
u64 windowread(u64 what)
{
	if(what==0x657a6973)	//'size'
	{
		return (height<<16)+width;
	}
}
void windowwrite(u64 type,u64 value)
{
	if(type==0x656c746974)		//'title'
	{
		SetWindowText(window,"hahahaha");
		return;
	}

	//写屏
	SetDIBitsToDevice(
		realdc,
		0,0,			//目标位置x,y
		width,height,	//dib宽,高
		0,0,			//来源起始x,y
		0,height,		//起始扫描线,数组中扫描线数量,
		(void*)screenbuf,	//rbg颜色数组
		&info,			//bitmapinfo
		DIB_RGB_COLORS	//颜色格式
	);
	//printf("result:%x\n",result);
}








void windowstart(char* addr, char* pixfmt, int x, int y)
{
	//构造info
	width = x;
	height = y;
	screenbuf = addr;

	SetWindowPos(
		window, 0, 0, 0,
		width+16, height+39,
		SWP_NOMOVE|SWP_NOZORDER|SWP_NOOWNERZORDER
	);

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
void windowstop()
{
}








void createconsolewindow()
{
	//int ret;
	//char buf[128];
	//ret=GetEnvironmentVariable("cmdcmdline",buf,128);
	//say("%x:cmdcmdline=%s\n",ret,buf);

	//拿console窗口并且隐藏起来
	consolewindow=GetConsoleWindow();
	//ShowWindow(consolewindow,SW_HIDE);
}
//int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
int createmywindow()
{
	//Step 1: Registering the Window Class+Creating the Window
	char *AppTitle="run as administrator to see the real world (=.=)";
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
	SetLayeredWindowAttributes(window, 0, 0xf0, LWA_ALPHA);  

	//显示窗口
	ShowWindow(window,SW_SHOW);			//nCmdShow);
	UpdateWindow(window);
}
void InitUIPIFilter()
{
	typedef BOOL (WINAPI *ChangeWindowMessageFilterProc)(UINT,u32);

	//1
	HMODULE hUser = LoadLibraryA("user32.dll");
	if (!hUser){say("failed to load\n");exit(-1);}

	//2
	ChangeWindowMessageFilterProc proc;
	proc=(ChangeWindowMessageFilterProc)GetProcAddress(hUser, "ChangeWindowMessageFilter");
	if(!proc){say("can't drag\n");exit(-1);}

	//3
	proc(WM_COPYDATA,1);
	proc(WM_DROPFILES,1);
	proc(0x0049, 1);
}
void createtray()
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
	AppendMenu(hMenu, MF_STRING, menu1, TEXT("console")); 
	AppendMenu(hMenu, MF_STRING, menu2, TEXT("window")); 
	AppendMenu(hMenu, MF_STRING, menu3, TEXT("about")); 
	AppendMenu(hMenu, MF_STRING, menu4, TEXT("exit")); 
}
void windowcreate()
{
	//终端窗口
	createconsolewindow();

	//图形窗口
	createmywindow();
	realdc=GetDC(window);

	//打开拖拽
	DragAcceptFiles(window, TRUE);
	InitUIPIFilter();

	//打开触摸
	RegisterTouchWindow(window, 0);

	//打开托盘
	createtray();
}
//__attribute__((destructor)) void destorysdl()
void windowdelete()
{
	//必须亮出来再关
	//ShowWindow(consolewindow,SW_SHOW);
	//ShowWindow(window,SW_SHOW);

	//关闭托盘
	Shell_NotifyIcon(NIM_DELETE, &nid);

	//关闭触摸
	UnregisterTouchWindow(window);

	//释放dc
	ReleaseDC(window,realdc);
}
