#include<stdio.h>
#include<conio.h>
#include<windows.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
u64 startthread(void*, void*);
void stopthread();
//
void eventwrite(u64,u64,u64,u64);
void say(char*,...);




//
static u64 thread=0;




DWORD WINAPI uievent(LPVOID pM)
{
	u8 ch;

	//
	while(1)
	{
		ch = getch();
		if(ch == 0xe0)
		{
			ch = getch();
			if(ch == 0x48)	//up
			{
				eventwrite(0x415b1b, 0x64626b, 0, 0);
			}
			else if(ch == 0x50)	//down
			{
				eventwrite(0x425b1b, 0x64626b, 0, 0);
			}
			else if(ch == 0x4d)	//right
			{
				eventwrite(0x435b1b, 0x64626b, 0, 0);
			}
			else if(ch == 0x4b)	//left
			{
				eventwrite(0x445b1b, 0x64626b, 0, 0);
			}
		}
		else
		{
			if(ch == 0x8)ch = 0x7f;
			eventwrite(ch, 0x64626b, 0, 0);
		}
	}//while
}




void windowchange()
{
}
void windowlist()
{
}
void windowread()
{
}
void windowwrite()
{
}
void windowstart(char* addr, char* pixfmt, int x, int y)
{
}
void windowstop()
{
}
void windowcreate()
{
	thread = startthread(uievent, 0);
}
void windowdelete()
{
}
