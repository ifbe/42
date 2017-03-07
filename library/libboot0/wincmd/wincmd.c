#include<stdio.h>
#include<stdarg.h>
#include<windows.h>
#define u8 unsigned char




//
static HANDLE output=0;
//
u8 last[8]={0};
u8 buf[0x1000];
//
static void attr(int val)
{
	if(output == 0)output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(output, val);
}
static int escapecolor(u8* p)
{
	//reset
	if(p[0] == '0')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		return 0;
	}

	//heavy
	else if(p[0] == '1')
	{
		attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
		return 1;
	}

	//foreground
	if(p[0] == '3')
	{
		if(p[1] == '0')
		{
			attr(0);
		}
		else if(p[1] == '1')
		{
			attr(FOREGROUND_RED);
		}
		else if(p[1] == '2')
		{
			attr(FOREGROUND_GREEN);
		}
		else if(p[1] == '3')
		{
			attr(FOREGROUND_GREEN|FOREGROUND_RED);
		}
		else if(p[1] == '4')
		{
			attr(FOREGROUND_BLUE);
		}
		else if(p[1] == '5')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_RED);
		}
		else if(p[1] == '6')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_GREEN);
		}
		else if(p[1] == '7')
		{
			attr(FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		}
		return 3;
	}

	//background
	else if(p[0] == '4')
	{
		if(p[1] == '0')
		{
			attr(0);
		}
		else if(p[1] == '1')
		{
			attr(BACKGROUND_RED);
		}
		else if(p[1] == '2')
		{
			attr(BACKGROUND_GREEN);
		}
		else if(p[1] == '3')
		{
			attr(BACKGROUND_GREEN|BACKGROUND_RED);
		}
		else if(p[1] == '4')
		{
			attr(BACKGROUND_BLUE);
		}
		else if(p[1] == '5')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_RED);
		}
		else if(p[1] == '6')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_GREEN);
		}
		else if(p[1] == '7')
		{
			attr(BACKGROUND_BLUE|BACKGROUND_GREEN|BACKGROUND_RED);
		}
		return 4;
	}
}
static void gotoxy(int x, int y)
{
	COORD pos;
	pos.X=x;
	pos.Y=y;

	if(output == 0)output = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(output, pos);
}
static int escapeposition(u8* p)
{
	int t;
	int x=0,y=0;
	for(t=0;t<4;t++)
	{
		if(p[t] == ';')
		{
			t++;
			break;
		}
		y = (y*10) + p[t] - 0x30;
	}
	for(;t<8;t++)
	{
		if( (p[t] == 'H') | (p[t] == 'f') )
		{
			t++;
			break;
		}
		x = (x*10) + p[t] - 0x30;
	}
//printf("\n(%d,%d)",x,y);
	gotoxy(x-1, y-1);
	return 0;
}
static int escapesequence(u8* p)
{
	int j;
	int x,y;

	if(p[2] == '?')
	{
		for(j=3;j<8;j++)
		{
			if( (p[j] == 'h') | (p[j] == 'l') )
			{
				gotoxy(0,0);
				for(y=0;y<128;y++)
				{
					for(x=0;x<79;x++)
					{
						printf(" ");
					}
					printf("\b\n");
				}
				gotoxy(0,0);

				return j;
			}
		}
	}

	//1b 5b 4a bf: openwrt special
	if(p[2] == 0x4a)
	{
		if(p[3] == 0xbf) return 3;
	}

	//1b 5b 4b: erase from here to the end
	if(p[2] == 0x4b)
	{
		printf(" \b");
		return 2;
	}

	//1b 5b 41: cursor up
	if(p[2] == 0x41)
	{
		return 2;
	}

	//1b 5b 42: cursor down
	if(p[2] == 0x42)
	{
		return 2;
	}

	//1b 5b 43: cursor forward
	if(p[2] == 0x43)
	{
		return 2;
	}

	//1b 5b 44: cursor backward
	if(p[2] == 0x44)
	{
		return 2;
	}

	//1b 5b ? m
	if(p[3] == 'm')
	{
		escapecolor(p+2);
		return 3;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 3;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		escapecolor(p+2);
		return 4;
	}

	if(p[3] == ';')
	{
		//1b 5b ? ; ? m
		if(p[5] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 5;
		}

		//1b 5b ? ; ? ? m
		else if(p[6] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 6;
		}
	}

	//1b 5b ? ? ; ? ? m
	if( (p[4] == ';') && (p[7] == 'm') )
	{
		escapecolor(p+2);
		escapecolor(p+5);
		return 7;
	}

	for(j=2;j<10;j++)
	{
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			escapeposition(p+2);
			return j;
		}
	}

	return 0;
}




void createserial(u8* arg)
{
	//opened?
	HWND consolewindow = GetConsoleWindow();
	if(consolewindow != NULL)return;

	//new
	AllocConsole();
	freopen("CONIN$","r",stdin);
	freopen("CONOUT$","w",stdout);

	//hide
	consolewindow = GetConsoleWindow();
	//ShowWindow(consolewindow,SW_HIDE);
}
void deleteserial()
{
}
int fmt(u8* mem, int max, u8* fmt, ...)
{
	int ret;
	va_list args;

	va_start(args,fmt);
	ret = vsnprintf(mem, max, fmt, args);
	va_end(args);

	return ret;
}
void say1()
{
}
void say2(u8* fmt , ...)
{
	asm("jmp printf");
}
void say(u8* fmt , ...)
{
	int j,k,ret;
	va_list args;

	va_start(args,fmt);
	if(last[0] != 0)
	{
		j = snprintf(buf,10,"%s",last);
		ret = j + vsnprintf(buf+j, 0x1000, fmt, args);
		last[0] = 0;
	}
	else
	{
		ret = vsnprintf(buf, 0x1000, fmt, args);
	}
	va_end(args);

	for(j=0;j<ret;j++)
	{
		//printf("{%x}",buf[j]);

		if(buf[j] == 0x1b)
		{
			//printf("...........{%x,%x,%x,%x,%x}",buf[j+0],buf[j+1],buf[j+2],buf[j+3],buf[j+4]);

			if(buf[j+1] == 0x5b)
			{
				//escape sequence must complete
				for(k=2;k<8;k++)
				{
					if(buf[j+k] == 0)
					{
						for(;k>=0;k--)last[k] = buf[j+k];
						return;
					}
				}
				j += escapesequence(buf+j);
			}//5b
		}//1b
		else if(buf[j] == 0x8)
		{
			printf("\b");
		}
		else if(buf[j] == 0x7f)
		{
			printf("\b \b");
		}
		else
		{
			printf("%c",buf[j]);
		}
	}
}
