#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

#include<stdio.h>
#include<stdlib.h>

static unsigned char* logbuf;




void say(char* rcx,QWORD rdx,QWORD r8,QWORD r9)
{
	int dest=0;
	int source=0;
	QWORD offsety;
	QWORD offsetx;
	char* temp;
	while(1)			//举例“123443%d\n\n    44532”
	{
		if(rcx[source] == 0)		//0
		{
			offsety=*(DWORD*)(logbuf+0xffff0);
			offsetx=*(DWORD*)(logbuf+0xffff8);

			temp=logbuf+offsety+offsetx;
			*(QWORD*)temp=0x3031323334353637;
			*(QWORD*)(temp+8)=0x3839414243444546;
			*(QWORD*)(temp+0x10)=0x3031323334353637;
			*(QWORD*)(temp+0x18)=0x2020202020202020;
			int writecount=snprintf(temp+32,0x80-offsetx,
					rcx+dest,rdx,r8,r9);
			*(DWORD*)(logbuf+0xffff8)+=writecount;

			break;
		}
		if( rcx[source] < 0x10)	//"\n":0xa?		0xd?
		{
			offsety=*(DWORD*)(logbuf+0xffff0);
			offsetx=*(DWORD*)(logbuf+0xffff8);
			snprintf(logbuf+offsety+offsetx,0x80-offsetx,
					rcx+dest,rdx,r8,r9);

			*(DWORD*)(logbuf+0xffff8)=0;
			*(DWORD*)(logbuf+0xffff0)=(offsety+0x80)%0xffff0;
			source+=1;
			dest=source;

			continue;
		}
		source++;
	}
}








void initlog(QWORD addr)
{
	logbuf=(char*)addr;
}
