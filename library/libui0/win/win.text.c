#include <stdio.h>
#include <windows.h>
#define QWORD unsigned long long
#define DWORD unsigned int




//
static HANDLE h;
static int width,height;
//
static char* content;




char clibuffer[128];
int uievent(QWORD* first,QWORD* second)
{
        int i;
        char* ret;
	//say("@uievent.start\n");

        for(i=0;i<128;i++)clibuffer[i]=0;
        while(1)
        {
                ret=fgets(clibuffer,128,stdin);
		//say("uievent.ret=%x\n",ret);

		if( ret == NULL )
		{
			first[0]=0;
			break;
		}
                if( clibuffer[0] != 0 )
		{
			first[0]=0x727473;
			second[0]=(QWORD)clibuffer;
			break;
		}
        }

	//say("@uievent.return\n");
	return 1;
}




void windowlist()
{
}
void windowchange()
{
}




void windowread(char* where)
{
}
void windowwrite()
{
	COORD pos = {0,0};
	SetConsoleCursorPosition(h,pos);

	printf("%s",content);
	fflush(stdout);
}




void windowstart(char* addr, char* pixfmt, int x, int y)
{
	content = addr;
	*(unsigned int*)pixfmt = 0x74786574;
}
void windowstop()
{
}




void windowinit()
{
	CONSOLE_SCREEN_BUFFER_INFO bInfo;

	h = GetStdHandle(STD_OUTPUT_HANDLE);
	GetConsoleScreenBufferInfo(h, &bInfo );
	width = bInfo.srWindow.Right - bInfo.srWindow.Left + 1;
	height = bInfo.srWindow.Bottom - bInfo.srWindow.Top + 1;

}
void windowkill()
{
}
