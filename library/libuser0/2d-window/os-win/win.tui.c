#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "libuser.h"
#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif
void stdout_sethelpseiral(void*);
int lowlevel_input();
void* supply_alloc();
void* supply_recycle(void*);
void tuinode_take( void*,int, void*,int, p64,int, void*,int);
void tuinode_give(void*,int, void*,int, p64,int, void*,int);




//
static HANDLE output;




DWORD WINAPI windowthread(_obj* win)
{
	int j,ret,tmp;
	u64 x,y,w;
	u64 why, what, where;
	DWORD cNumRead, fdwMode, fdwSaveOldMode;
	HANDLE hStdin, hStdout;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	INPUT_RECORD irInBuf[128];
	KEY_EVENT_RECORD keyrec;
	MOUSE_EVENT_RECORD mouserec;
	WINDOW_BUFFER_SIZE_RECORD wbsrec;

	//Get the standard input handle
	hStdin = GetStdHandle(STD_INPUT_HANDLE);
	if(hStdin == INVALID_HANDLE_VALUE)
	{
		printf("GetStdHandle");
		return 0;
	}

	hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	if(hStdout == INVALID_HANDLE_VALUE)
	{
		printf("hStdout\n");
		return 0;
	}

	//Save the current input mode, to be restored on exit
	if(!GetConsoleMode(hStdin, &fdwSaveOldMode) )
	{
		printf("GetConsoleMode");
		return 0;
	}

	//Enable the window and mouse input events
	fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
	if(!SetConsoleMode(hStdin, fdwMode) )
	{
		printf("SetConsoleMode");
		return 0;
	}

	//Loop to read and handle the next 100 input events
	while(1)
	{
		//Wait for the events
		if(!ReadConsoleInput(
			hStdin,		//input buffer handle
			irInBuf,	//buffer to read into
			128,		//size of read buffer
			&cNumRead) )	//number of records read
		{
			printf("ReadConsoleInput");
			return 0;
		}

		//Dispatch the events to the appropriate handler
		for(j=0;j<cNumRead;j++)
		{
			ret = irInBuf[j].EventType;
			if(KEY_EVENT == ret)
			{
				keyrec = irInBuf[j].Event.KeyEvent;
				if(0 == keyrec.bKeyDown)continue;

				if(0 == keyrec.uChar.AsciiChar)
				{
					ret = keyrec.wVirtualKeyCode;

					if((ret < 0x25) | (ret > 0x28))
					{
						eventwrite(ret, _kbd_, 0, 0);
					}
					else
					{
						if(ret == 0x26)ret = 0x415b1b;
						else if(ret == 0x28)ret = 0x425b1b;
						else if(ret == 0x27)ret = 0x435b1b;
						else if(ret == 0x25)ret = 0x445b1b;
						eventwrite(ret, _char_, 0, 0);
					}
				}
				else
				{
					ret = keyrec.uChar.UnicodeChar;
					if(ret >= 0x80)
					{
						j++;
						keyrec = irInBuf[j].Event.KeyEvent;
						ret = ret | (keyrec.uChar.UnicodeChar << 8);
						//printf("%x\n", ret);

						tmp = 0;
						MultiByteToWideChar(
							CP_ACP, 0,
							(void*)&ret, -1,
							(void*)&tmp, 2
						);
						//printf("%x\n", tmp);

						ret = 0;
						WideCharToMultiByte(
							CP_UTF8, 0,
							(void*)&tmp, -1,
							(void*)&ret, 4,
							NULL, NULL
						);
						//printf("%x\n", ret);
					}
					eventwrite(ret, _char_, 0, 0);
				}
			}
			else if(MOUSE_EVENT == ret)
			{
				mouserec = irInBuf[j].Event.MouseEvent;
				ret = mouserec.dwEventFlags;

				if(0 == ret)
				{
					if(mouserec.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
					{
						GetConsoleScreenBufferInfo(hStdout, &bInfo);
						x = mouserec.dwMousePosition.X;
						y = mouserec.dwMousePosition.Y - bInfo.srWindow.Top;
						w = 'l';
						eventwrite(x+(y<<16)+(w<<48), 0x2b70, 0, 0);
						//printf("left button press \n");
					}
					else if(mouserec.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
					{
						//printf("right button press \n");
					}
					else
					{
						//printf("button press\n");
					}
				}
				if(DOUBLE_CLICK == ret)
				{
					//printf("double click\n");
				}
				if(MOUSE_HWHEELED == ret)
				{
					//printf("horizontal mouse wheel\n");
				}
				if(MOUSE_MOVED == ret)
				{
					//printf("%x,%x\n", mouserec.dwMousePosition.X, mouserec.dwMousePosition.Y);
				}
				if(MOUSE_WHEELED == ret)
				{
					//printf("vertical mouse wheel\n");
				}
			}
			else if(WINDOW_BUFFER_SIZE_EVENT == ret)
			{
				wbsrec = irInBuf[j].Event.WindowBufferSizeEvent;
				//printf("Resize:%x,%x\n", wbsrec.dwSize.X, wbsrec.dwSize.Y);

				GetConsoleScreenBufferInfo(hStdout, &bInfo);
				x = bInfo.srWindow.Right - bInfo.srWindow.Left + 1;
				y = bInfo.srWindow.Bottom - bInfo.srWindow.Top + 1;
				win->whdf.width = x;
				win->whdf.height = y;

				win->whdf.fbwidth = x*4;
				//eventwrite(x+(y<<16), _size_, 0, 0);
			}
			else if(MENU_EVENT == ret)
			{
				//printf("MENU_EVENT\n");
				eventwrite(0,0,0,0);
			}
			else if(FOCUS_EVENT == ret)
			{
				//printf("FOCUS_EVENT\n");
			}
		}//for
	}//while

	//Restore input mode on exit
	SetConsoleMode(hStdin, fdwSaveOldMode);
	return 0;
}
static void windowsutf8(char* utf8)
{
	int ret;
	char unicode[8];
	char gbk[8];
	ret = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, (void*)unicode, 4);
	ret = WideCharToMultiByte(CP_ACP, 0, (void*)unicode, -1, gbk, 4, NULL, NULL);
	printf("%s",gbk);
}
static void attr(u8 bg, u8 fg)
{
	int a = 0;

	switch(bg&7){
	case 7:a |= BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;break;
	case 6:a |= BACKGROUND_BLUE | BACKGROUND_GREEN;break;
	case 5:a |= BACKGROUND_BLUE | BACKGROUND_RED;break;
	case 4:a |= BACKGROUND_BLUE;break;
	case 3:a |= BACKGROUND_GREEN | BACKGROUND_RED;break;
	case 2:a |= BACKGROUND_GREEN;break;
	case 1:a |= BACKGROUND_RED;break;
	}//bg

	switch(fg&7){
	case 7:a |= FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;break;
	case 6:a |= FOREGROUND_BLUE | FOREGROUND_GREEN;break;
	case 5:a |= FOREGROUND_BLUE | FOREGROUND_RED;break;
	case 4:a |= FOREGROUND_BLUE;break;
	case 3:a |= FOREGROUND_GREEN | FOREGROUND_RED;break;
	case 2:a |= FOREGROUND_GREEN;break;
	case 1:a |= FOREGROUND_RED;break;
	}//fg

	if(fg >= 8)a |= FOREGROUND_INTENSITY;
	SetConsoleTextAttribute(output, a);
}




static void gotoxy(int x, int y)
{
	COORD pos;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	output = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(output, &bInfo);

	pos.X = x;
	pos.Y = bInfo.srWindow.Top + y;
	SetConsoleCursorPosition(output, pos);
}
void windowdraw(_obj* win)
{
	int x,y;
	u8 ch, bg=0, fg=7;
	u8* p;
	u8* buf = (u8*)(win->tuitext.buf);
	int width = win->whdf.width;
	int height = win->whdf.height;
	gotoxy(0, 0);

	//
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			if((x == width-1)&&(y == height-1))break;
			p = buf + ((width*y + x)<<2);
			if(p[0] > 0x80)
			{
				//先颜色
				if((bg != p[7]) | (fg != p[6]) )
				{
					bg = p[7];
					fg = p[6];
					attr(bg, fg);
				}

				//这是汉字
				windowsutf8(p);
				x++;
			}
			else
			{
				//先颜色
				if((bg != p[3]) | (fg != p[2]) )
				{
					bg = p[3];
					fg = p[2];
					attr(bg, fg);
				}

				//这是ascii
				ch = p[0];
				if(ch < 0x20)ch = 0x20;
				printf("%c",ch);
			}
		}
	}
}




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//read context
	tuinode_take(wnd,0, stack,sp, arg,key, buf,len);

	stdout_sethelpseiral(0);

	//update screen
	windowdraw(wnd);

	//stdout_sethelpseiral((void*)1);
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
void window_detach()
{
}
void window_attach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* w)
{
}
void window_create(_obj* w)
{
	int width,height;
	CONSOLE_SCREEN_BUFFER_INFO bInfo;
	GetConsoleScreenBufferInfo(output, &bInfo);
	width = bInfo.srWindow.Right - bInfo.srWindow.Left + 1;
	height = bInfo.srWindow.Bottom - bInfo.srWindow.Top + 1;

	w->vfmt = _tui_;

	w->whdf.width = width;
	w->whdf.height = height;

	w->whdf.fbwidth = width*4;
	//w->fbheight = 0;

	w->tuitext.buf = malloc(0x100000);
	//threadcreate(windowthread, w);

	int j;
	printf("will print 64 empty lines\n");
	for(j=0;j<64;j++)printf("%d\n", j);
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
	output = GetStdHandle(STD_OUTPUT_HANDLE);
}
void freewindow()
{
}
