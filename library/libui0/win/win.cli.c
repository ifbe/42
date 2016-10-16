#include<stdio.h>
#include<conio.h>
#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
void say(char*,...);




//
static int mode = 0;
static char buffer[256];




int uievent(u64* who, u64* what, u64* where)
{
	int i;
	char* ret=(char*)where;

	//
	*what = 0x64626b;
	ret[0] = getch();

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
