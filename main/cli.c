#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include<stdio.h>




static BYTE buffer[128];//键盘输入专用

void command(char* buffer);
void initmaster(QWORD addr);
int compare(char* first,char* second);

QWORD whereisworld();



void waitinput(char* p)
{
	int i;
	for(i=0;i<128;i++)p[i]=0;
	while(1)
	{
		fgets(p,128,stdin);
		if( p[0] != 0 )break;
	}
	for(i=0;i<128;i++)
	{
		if(p[i]<=0xd)p[i]=0;
	}
}




void main()
{
	bribelibrarian(8);

	while(1)
	{
		//1.等输入，再把这段里面所有的0x20变成0
		waitinput(buffer);

		if(compare(buffer, "exit" ) == 0)break;
		else command(buffer);
	}

	byelibrarian();
}
