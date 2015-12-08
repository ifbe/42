#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdarg.h>
#include<stdlib.h>
char* outbuf=0;
void initsay(char* addr)
{
	outbuf=addr;
}




void say(char* fmt , ...)
{
	QWORD offsety=*(DWORD*)(outbuf+0xffff0);
        va_list args;

        va_start(args,fmt);
	snprintf(outbuf+offsety,0x21,"[doctor@tardis]                 ");
        vsnprintf(outbuf+offsety+0x20,0x80,fmt,args);
        va_end(args);

	*(DWORD*)(outbuf+0xffff0)+=0x80;
}





/*	int start=0;
	int temp=0;
	QWORD offsety;
	QWORD offsetx;
	while(1)			//举例“123443%d\n\n    44532”
	{
		if(rcx[temp] == 0)		//0
		{
			offsety=*(DWORD*)(outbuf+0xffff0);
			offsetx=*(DWORD*)(outbuf+0xffff8);
			int writecount=snprintf(outbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			*(DWORD*)(outbuf+0xffff8)+=writecount;

			break;
		}
		if( rcx[temp] < 0x10)	//"\n":0xa?		0xd?
		{
			offsety=*(DWORD*)(outbuf+0xffff0);
			offsetx=*(DWORD*)(outbuf+0xffff8);
			snprintf(outbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			*(DWORD*)(outbuf+0xffff8)=0;
			*(DWORD*)(outbuf+0xffff0)=(offsety+0x80)%0xffff0;
			temp+=1;
			start=temp;

			continue;
		}
		temp++;
	}
}*/
