#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>




char clibuffer[128];
int uievent(QWORD* first,QWORD* second)
{
        int i;
        char* ignore;
        for(i=0;i<128;i++)clibuffer[i]=0;
        while(1)
        {
                ignore=fgets(clibuffer,128,stdin);
                if( clibuffer[0] != 0 )break;
        }
        for(i=0;i<128;i++)
        {
                if(clibuffer[i]<=0xd)clibuffer[i]=0;
        }

	//
	first[0]=0x727473;
	second[0]=(QWORD)clibuffer;
	return 1;
}




QWORD readwindow(QWORD what)
{
	if(what==0x657a6973)
	{
		return 80 + (25<<16);
	}
}
void writewindow()
{
}




void initwindowworker()
{
	
}
void killwindowworker()
{
	
}
