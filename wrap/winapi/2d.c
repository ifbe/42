#include<stdio.h>
#include<stdlib.h>

#include<windows.h>
#include<Commctrl.h>
#include<Winuser.h>

#define WM_TRAY (WM_USER + 1)
#define menu1 0x1234
#define menu2 0x5678
#define	BSWAP_8(x)	((x) & 0xff)
#define	BSWAP_16(x)	((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define	BSWAP_32(x)	((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))


int width=1024;
int height=640;
unsigned int* mypixel;

HWND window;
HDC realdc;
BITMAPINFO info;
MSG msg;
NOTIFYICONDATA nid;     //托盘属性 
HMENU hMenu;            //托盘菜单 









//inner
//outer



void point(int x,int y,int color)
{
	mypixel[y*width+x]=color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}
void whereisscreenbuf(unsigned long long* p)
{
	*p=(unsigned long long)mypixel;
}
void writescreen()
{
	//int result=SetDIBitsToDevice(realdc,
	SetDIBitsToDevice(realdc,
			0,0,		//目标位置x,y
			width,height,		//dib宽,高
			0,0,		//来源起始x,y
			0,height,			//起始扫描线,数组中扫描线数量,
			mypixel,		//rbg颜色数组
			&info,			//bitmapinfo
			DIB_RGB_COLORS);		//颜色格式
	//printf("result:%x\n",result);
}




// Step 3: the Window Procedure
int solved=1;
int my1;
int my2;
LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
		case WM_TRAY:		//托盘
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
			break;
		}
		case WM_KEYDOWN:		//键盘点下
		{
			solved=0;
			my1=2;
			my2=wparam;
		}
		case WM_LBUTTONDOWN:		//鼠标左键点下
		{
			solved=0;
			my1=3;
			my2=lparam;
		}
		case WM_PAINT:		//显示
		{
			writescreen();
			break;
		}
		case WM_DESTROY:		//摧毁
		{
			//Shell_NotifyIcon(NIM_DELETE, &nid);
			//ReleaseDC(window,fakedc);  
			ReleaseDC(window,realdc);
			free(mypixel);
			Shell_NotifyIcon(NIM_DELETE, &nid);
			PostQuitMessage(0);
			break;
		}
    }

    return DefWindowProc(window, msg, wparam, lparam);
}
int waitevent(unsigned long long* first,unsigned long long* second)
{
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
}




//int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
int initwindow()
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

    window = CreateWindow(AppTitle,AppTitle,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,CW_USEDEFAULT,width,height+40,
				NULL,NULL,0,NULL);		//NULL,NULL,hInst,NULL);
    if (!window) return 0;

	//fakedc=CreateCompatibleDC(NULL);
	//HBITMAP bmp = CreateCompatibleBitmap(realdc,width,height);
	//SelectObject(fakedc,bmp);

    ShowWindow(window,SW_SHOW);			//nCmdShow);
    UpdateWindow(window);
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
__attribute__((constructor)) void initsdl()
{
	//准备beforemain
	initwindow();

	//托盘
	inittray();

	//dib
	initdib();

	//拿dc
	realdc=GetDC(window);

	//准备rgb点阵
	mypixel=(unsigned int*)malloc(width*height*4);

}
__attribute__((destructor)) void destorysdl()
{
	//释放点阵
	free(mypixel);

	//释放sdl
}