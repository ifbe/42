#include<stdio.h>
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
