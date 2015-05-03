#include<stdio.h>
#include<stdlib.h>

#include<windows.h>
#include<Commctrl.h>
#include<Winuser.h>


int width=1024;
int height=640;
unsigned int* mypixel;

HWND window;
HDC realdc;
unsigned char* mypixels;
MSG msg;




// Step 3: the Window Procedure
LRESULT CALLBACK WindowProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
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
			free(mypixels);
			PostQuitMessage(0);
			break;
		}
    }
    return DefWindowProc(window, msg, wparam, lparam);
}




int WINAPI WinMain(HINSTANCE hInst,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    //Step 1: Registering the Window Class+Creating the Window
    char *AppTitle="Win1";
    WNDCLASS wc;
    wc.style=CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc=WindowProc;
    wc.cbClsExtra=0;
    wc.cbWndExtra=0;
    wc.hInstance=hInst;
    wc.hIcon=LoadIcon(NULL,IDI_WINLOGO);
    wc.hCursor=LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground=(HBRUSH)COLOR_WINDOWFRAME;
    wc.lpszMenuName=NULL;
    wc.lpszClassName=AppTitle;
    if (!RegisterClass(&wc)) return 0;

    window = CreateWindow(AppTitle,AppTitle,
				WS_OVERLAPPEDWINDOW,
				CW_USEDEFAULT,CW_USEDEFAULT,1024,600,
				NULL,NULL,hInst,NULL);
    if (!window) return 0;

	//fakedc=CreateCompatibleDC(NULL);
	//HBITMAP bmp = CreateCompatibleBitmap(realdc,1024,768);
	//SelectObject(fakedc,bmp);
	realdc=GetDC(window);
	mypixels=(unsigned char*)malloc(1024*600*4);
	int x,y;
	for(x=0;x<1024;x++)
	{
		for(y=0;y<600;y++)
		{
			//SetPixel(fakedc,x,y,RGB(255,0,255));
			*(DWORD*)(mypixels+y*4096+x*4)=0x88888888;
		}
	}

    ShowWindow(window,nCmdShow);
    UpdateWindow(window);

    // Step 2: The Message Loop
    while (GetMessage(&msg,NULL,0,0) > 0)
    {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}
__attribute__((constructor)) void initsdl()
{
	//准备sdl

	//准备rgb点阵
	mypixel=(unsigned int*)malloc(width*height*4);
}
__attribute__((destructor)) void destorysdl()
{
	//释放点阵
	free(mypixel);

	//释放sdl
}




//inner
//outer



void whereisscreenbuf(unsigned long long* p)
{
	*p=(unsigned long long)mypixel;
}
void point(int x,int y,int color)
{
	mypixel[y*width+x]=color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}
void writescreen()
{
	BITMAPINFO info;
	info.bmiHeader.biSize=sizeof(BITMAPINFOHEADER);
	info.bmiHeader.biWidth=1024;
	info.bmiHeader.biHeight=600;
	info.bmiHeader.biPlanes=1;
	info.bmiHeader.biBitCount=32;
	info.bmiHeader.biCompression=0;
	info.bmiHeader.biSizeImage=1024*600*4;
	info.bmiHeader.biXPelsPerMeter=0;
	info.bmiHeader.biYPelsPerMeter=0;
	info.bmiHeader.biClrUsed=0;
	info.bmiHeader.biClrImportant=0;

	info.bmiColors[0].rgbBlue=255;
	info.bmiColors[0].rgbGreen=255;
	info.bmiColors[0].rgbRed=255;
	info.bmiColors[0].rgbReserved=255;

	int result=SetDIBitsToDevice(realdc,
			0,0,		//目标位置x,y
			1024,600,		//dib宽,高
			0,0,		//来源起始x,y
			0,-600,			//起始扫描线,数组中扫描线数量,
			mypixels,		//rbg颜色数组
			&info,			//bitmapinfo
			DIB_RGB_COLORS);		//颜色格式
	//printf("result:%x\n",result);
}


//自定义的种类码，不是sdl的不要混淆
//0:退出
//1:键盘按下
//2:键盘松开
//3:鼠标按下
//4:鼠标松开
//5:鼠标移动
//0xff:时间
int waitevent()
{
}