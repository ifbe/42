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
