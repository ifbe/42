#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




static u8* rawuniverse;
int arg2utf8(char* src, char* dst)
{
	return snprintf(dst, 0x1000, "%s", src);
}




void* pollenv()
{
	usleep(1000);
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
	//char buf[0x100];


	//0.chdir
	chdir("/sdcard/42/");
	//getcwd(buf, 0x100);
	//say("%.100s\n", buf);


	//1.alloc
	rawuniverse = malloc(__size__);
	if(NULL == rawuniverse)
	{
		printf("no enough momery\n");
		exit(-1);
	}
	for(j=0;j<__size__;j++)rawuniverse[j]=0;


	//2.align
	//[0x   0,0x1001000)	->	[0x0000,0x1000000)
	//[0x 234,0x1001234)	->	[0x1000,0x1001000)
	//[0x fff,0x1001fff)	->	[0x1000,0x1001000)
	//[0x1001,0x1002001)	->	[0x1000,0x1002000)
	temp = ( (u64)rawuniverse ) & 0xfff;
	if(0 != temp)temp = 0x1000 - temp;
	return rawuniverse + temp;
}
