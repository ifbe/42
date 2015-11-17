#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>
#include<stdlib.h>




void say(char* rcx,QWORD rdx,QWORD r8,QWORD r9)
{
	int start=0;
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
}
