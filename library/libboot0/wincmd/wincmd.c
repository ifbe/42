#include<stdio.h>
#include<stdarg.h>
#include<windows.h>
#define u8 unsigned char




static HANDLE output = 0;
static u8 last[16] = {0};
static int pos = 0;




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
	if(p[0] != 0x1b)return 0;
	if(p[1] != 0x5b)return 0;

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

				return j+1;
			}
		}
	}

	//1b 5b 4a bf: openwrt special
	if(p[2] == 0x4a)
	{
		if(p[3] == 0xbf) return 4;
	}

	//1b 5b 4b: erase from here to the end
	if(p[2] == 0x4b)
	{
		printf(" \b");
		return 3;
	}

	//1b 5b 41: cursor up
	if(p[2] == 0x41)
	{
		printf("a");
		return 3;
	}

	//1b 5b 42: cursor down
	if(p[2] == 0x42)
	{
		printf("b");
		return 3;
	}

	//1b 5b 43: cursor forward
	if(p[2] == 0x43)
	{
		printf("c");
		return 3;
	}

	//1b 5b 44: cursor backward
	if(p[2] == 0x44)
	{
		printf("d");
		return 3;
	}

	//1b 5b ? m
	if(p[3] == 'm')
	{
		escapecolor(p+2);
		return 4;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 4;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		escapecolor(p+2);
		return 5;
	}

	if(p[3] == ';')
	{
		//1b 5b ? ; ? m
		if(p[5] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 6;
		}

		//1b 5b ? ; ? ? m
		else if(p[6] == 'm')
		{
			escapecolor(p+2);
			escapecolor(p+4);
			return 7;
		}
	}

	//1b 5b ? ? ; ? ? m
	if( (p[4] == ';') && (p[7] == 'm') )
	{
		escapecolor(p+2);
		escapecolor(p+5);
		return 8;
	}

	for(j=2;j<10;j++)
	{
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			escapeposition(p+2);
			return j+1;
		}
	}

	return 0;
}




int lowlevel_input(char* buf)
{
}
void lowlevel_output(char* buf, int len)
{
	int i,j,k=0;
	for(;j<len;j++)
	{
		if(buf[j] == 0x1b)
		{
			i = escapesequence(buf+j);
			if(i != 0)
			{
				if(j>k)printf("%.*s", j-k, buf+k);
				k = j+i;
			}
		}
	}
	if(len>k)printf("%.*s", len-k, buf+k);
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
	ShowWindow(consolewindow,SW_SHOW);
}
void deleteserial()
{
}