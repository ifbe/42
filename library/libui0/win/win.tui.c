#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include "arena.h"
//
u64 startthread(void*, void*);
void stopthread();
//
int lowlevel_input();
void eventwrite(u64,u64,u64,u64);
void say(void*, ...);




//
static HANDLE output;
static int lastwidth=0,lastheight=0;
static int width,height;




#ifndef MOUSE_HWHEELED
#define MOUSE_HWHEELED 0x0008
#endif




DWORD WINAPI terminalthread(void* win)
{
	int j;
	u64 x,y,w;
	u64 why, what, where;
    DWORD cNumRead, fdwMode, fdwSaveOldMode;
    INPUT_RECORD irInBuf[128];

	HANDLE hStdin;
	KEY_EVENT_RECORD keyrec;
	MOUSE_EVENT_RECORD mouserec;
	WINDOW_BUFFER_SIZE_RECORD wbsrec;

    // Get the standard input handle
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if(hStdin == INVALID_HANDLE_VALUE)
	{
        printf("GetStdHandle");
		return 0;
	}

    // Save the current input mode, to be restored on exit
    if(!GetConsoleMode(hStdin, &fdwSaveOldMode) )
	{
        printf("GetConsoleMode");
		return 0;
	}

    // Enable the window and mouse input events
    fdwMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS;
    if(!SetConsoleMode(hStdin, fdwMode) )
	{
        printf("SetConsoleMode");
		return 0;
	}

    // Loop to read and handle the next 100 input events
    while(1)
    {
        // Wait for the events
        if(!ReadConsoleInput(
                hStdin,      // input buffer handle
                irInBuf,     // buffer to read into
                128,         // size of read buffer
                &cNumRead) ) // number of records read
		{
            printf("ReadConsoleInput");
			return 0;
		}

        // Dispatch the events to the appropriate handler
        for(j = 0; j < cNumRead; j++)
        {
            switch(irInBuf[j].EventType)
            {
                case KEY_EVENT:
				{
					keyrec = irInBuf[j].Event.KeyEvent;
					if(keyrec.bKeyDown)
					{
						if(keyrec.uChar.AsciiChar == 0)
						{
							eventwrite(keyrec.wVirtualKeyCode, __kbd__, 0, 0);
						}
						else
						{
							eventwrite(keyrec.uChar.UnicodeChar, __char__, 0, 0);
						}
					}
                    break;
				}
                case MOUSE_EVENT:
				{
					mouserec = irInBuf[j].Event.MouseEvent;
					switch(mouserec.dwEventFlags)
					{
						case 0:
						{
							if(mouserec.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
							{
								x = mouserec.dwMousePosition.X;
								y = 0;	//mouserec.dwMousePosition.Y;
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
							break;
						}
						case DOUBLE_CLICK:
						{
							//printf("double click\n");
							break;
						}
						case MOUSE_HWHEELED:
						{
							//printf("horizontal mouse wheel\n");
							break;
						}
						case MOUSE_MOVED:
						{
							//printf("%x,%x\n", mouserec.dwMousePosition.X, mouserec.dwMousePosition.Y);
							break;
						}
						case MOUSE_WHEELED:
						{
							//printf("vertical mouse wheel\n");
							break;
						}
						default:
						{
							//printf("unknown\n");
							break;
						}
					}
                    break;
				}
                case WINDOW_BUFFER_SIZE_EVENT:
				{
					wbsrec = irInBuf[j].Event.WindowBufferSizeEvent;
					//printf("Resize:%x,%x\n", wbsrec.dwSize.X, wbsrec.dwSize.Y);
                    break;
				}
                case MENU_EVENT:
				{
					//printf("MENU_EVENT\n");
                    break;
				}
                case FOCUS_EVENT:
				{
					//printf("FOCUS_EVENT\n");
                    break;
				}
                default:
				{
                    //printf("Unknown:%x", irInBuf[j].EventType);
                    break;
				}
            }
        }
    }

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
static void attr(u8 bg,u8 fg)
{
	int color;
	if(bg == 7)color = BACKGROUND_BLUE | BACKGROUND_GREEN | BACKGROUND_RED;
	else if(bg == 6)color = BACKGROUND_BLUE | BACKGROUND_GREEN;
	else if(bg == 5)color = BACKGROUND_BLUE | BACKGROUND_RED;
	else if(bg == 4)color = BACKGROUND_BLUE;
	else if(bg == 3)color = BACKGROUND_GREEN | BACKGROUND_RED;
	else if(bg == 2)color = BACKGROUND_GREEN;
	else if(bg == 1)color = BACKGROUND_RED;
	else color = 0;

	//
	SetConsoleTextAttribute(output, color | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}




void windowwrite(struct window* dst, struct window* src)
{
	int x,y;
	u8 ch,bg=0,fg=0;
	COORD pos = {0,0};
	u8* p;
	u8* buf = (u8*)(src->buf);
	SetConsoleCursorPosition(output,pos);
/*
	//
	buf[width*height]=0;
	printf("%s",buf);
*/
	//
	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			p = buf + ((width*y + x)<<2);
			if(p[0] > 0x80)
			{
				//先颜色
				if(bg != p[7])
				{
					bg = p[7];
					attr(bg,fg);
				}

				//这是汉字
				windowsutf8(p);
				x++;
			}
			else
			{
				//先颜色
				if(bg != p[3])
				{
					bg = p[3];
					attr(bg,fg);
				}

				//这是ascii
				ch = p[0];
				if(ch < 0x20)ch = 0x20;
				printf("%c",ch);
			}
		}
	}
	if(bg != 0)attr(0,0);

	SetConsoleCursorPosition(output,pos);
}
void windowread()
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstart(struct window* this)
{
	if(this->type == hex32('b','u','f',0))
	{
		this->fmt = hex64('b', 'g', 'r', 'a', '8', '8', '8', '8');
		return;
	}
	else
	{
		this->type = hex32('w','i','n',0);
		this->fmt = hex32('t','u','i',0);

		this->w = width;
		this->h = height;
		this->d = 0;

		this->thread = startthread(terminalthread, this);
	}
}
void windowstop()
{
}
void windowcreate()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	output = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(output, &bInfo );
	width = bInfo.srWindow.Right - bInfo.srWindow.Left + 1;
	height = bInfo.srWindow.Bottom - bInfo.srWindow.Top + 1;
}
void windowdelete()
{
}
