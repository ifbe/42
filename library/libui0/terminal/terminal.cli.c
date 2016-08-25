#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#include<stdio.h>
#include<stdlib.h>




char clibuffer[128];
int uievent(u64* first,u64* second)
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
			second[0]=(u64)clibuffer;
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




u64 windowread(u64 what)
{
	if(what==0x657a6973)
	{
		return 80 + (25<<16);
	}
}
void windowwrite()
{
}




void windowstart()
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
