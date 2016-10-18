#include<stdio.h>
#include<conio.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char*,...);




//
static int mode = 0;




int uievent(u64* who, u64* what, u64* where)
{
	int i;
	u8* ret=(u8*)where;

	//
	while(1)
	{
		ret[0] = getch();
		if(ret[0] == 0xe0)
		{
			ret[1] = getch();
			if(ret[1] == 0x48)	//up
			{
				*where = 0x415b1b;
				break;
			}
			else if(ret[1] == 0x50)	//down
			{
				*where = 0x425b1b;
				break;
			}
			else if(ret[1] == 0x4b)	//left
			{
				*where = 0x445b1b;
				break;
			}
			else if(ret[1] == 0x4d)	//right
			{
				*where = 0x435b1b;
				break;
			}
		}
		else
		{
			ret[1] = 0;
			break;
		}
	}

	//printf("%x\n",ret[0]);
	return 1;
}




void windowchange(int what)
{
	mode = what;
}
void windowlist()
{
}
void windowread(char* where)
{
}
void windowwrite(u64 who, u64 what, u64 how)
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
}
void windowdelete()
{
}
