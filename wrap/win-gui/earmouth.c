#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

#include<stdio.h>
#include<stdlib.h>

static unsigned char* logbuf;
static QWORD offsetx=0;
static QWORD offsety=0;


__attribute__((constructor)) void initlog()
{
	logbuf=(unsigned char*)malloc(0x100000);
}
__attribute__((destructor)) void destorylog()
{
	free(logbuf);
}








void whereislogbuf(unsigned long long* p)
{
	*p=(unsigned long long)logbuf;
}
void say(char* rcx,QWORD rdx,QWORD r8,QWORD r9)
{
	printf(rcx,rdx,r8,r9);

	int start=0;
	int temp=0;
	while(1)			//举例“123443%d\n\n    44532”
	{
		if(rcx[temp] == 0)		//0
		{
			int writecount=snprintf(logbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			offsetx+=writecount;

			break;
		}
		if( rcx[temp] < 0x10)	//"\n":0xa?		0xd?
		{
			snprintf(logbuf+offsety+offsetx,0x80-offsetx,
					rcx+start,rdx,r8,r9);

			offsetx=0;
			offsety=(offsety+0x80)%0x100000;
			temp+=1;
			start=temp;

			continue;
		}
		temp++;
	}

}