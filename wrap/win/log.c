#include<stdio.h>




void say(char* p,...)
{
	asm("jmp printf");
}
void waitinput(char* p)
{
	int i;
	for(i=0;i<128;i++)p[i]=0;
	while(1)
	{
		gets(p);
		if( p[0] != 0 )break;
	}
}
