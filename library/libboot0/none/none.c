#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<termios.h>
#include<sys/ioctl.h>
#include<sys/select.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
static u8* rawuniverse;




int lowlevel_input()
{
	u8 buf[8];
	while(1)
	{
		buf[0] = getchar();
		if( (buf[0] == 0) | (buf[0]==0xff) )
		{
			usleep(10000);
			continue;
		}
		if(buf[0] != 0x1b)return buf[0];

		buf[1] = getchar();
		if(buf[1] == 0xff)return 0x1b;

		if(buf[1] == 0x5b)
		{
			buf[2] = getchar();

			if( (buf[2]>=0x41) && (buf[2]<=0x44) )
			{
				buf[3] = 0;
				return *(int*)buf;
			}
		}//5b
		else continue;
	}
}
void lowlevel_output(char* buf, int len)
{
	printf("%.*s", len, buf);
}




void* pollenv()
{
	return 0;
}
void* waitenv()
{
	return 0;
}




void death()
{
}
void* birth()
{
#define __size__ 0x1001000
	u64 j;
	u64 temp;


	//1.alloc
	rawuniverse = malloc(__size__);
	if(NULL == rawuniverse)
	{
		printf("no enough momery\n");
		exit(-1);
	}
	for(j=0;j<__size__;j++)rawuniverse[j]=0;


	//2.align
	//[0x   0,0x1001000)	->	[0x1000,0x1001000)
	//[0x 234,0x1001234)	->	[0x1000,0x1001000)
	//[0x fff,0x1001fff)	->	[0x1000,0x1001000)
	//[0x1001,0x1002001)	->	[0x1000,0x1002000)
	temp = ( (u64)rawuniverse ) & 0xfff;
	return rawuniverse + 0x1000 - temp;
}