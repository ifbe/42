#include<stdio.h>
/*
void say(char* string,unsigned long long data)
{
	printf("%s",string);
	if(data!=0)printf("%llx",data);
	printf("\n");
}
*/
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
                fgets(p,128,stdin);
                if( p[0] != 0 )break;
        }
}
