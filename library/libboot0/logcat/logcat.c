#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <android/log.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
void eventwrite(u64,u64,u64,u64);
int actorwrite_ev(void*);




char buffer[0x1000];
int cur=0;




int lowlevel_input(char* buf)
{
	while(1)usleep(1000*1000);
	return 0;
}
int lowlevel_output(char* buf, int len)
{
	int j;
	for(j=0;j<len;j++)
	{
		buffer[cur] = buf[j];
		cur++;

		if((buf[j] == '\n') | (cur >= 0xfff))
		{
			buffer[cur] = 0;
			LOGI("%s",buffer);

			cur = 0;
		}
	}
	return len;
}




void* pollenv()
{
	usleep(1000);
	return 0;
}
int argv2line(char* src, char* dst)
{
	return snprintf(dst, 0x1000, "%s\n", src);
}




static u8* rawuniverse;
void death()
{
}
void* birth()
{
#define __size__ 0x1001000
	u64 j;
	u64 temp;
	char buf[0x100];


	//0.chdir
	chdir("/sdcard/42/");
	getcwd(buf, 0x100);
	LOGI("%.100s\n", buf);


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