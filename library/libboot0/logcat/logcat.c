#include <android/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




static u8* rawuniverse;
char buffer[0x1000];
int cur=0;




/*
int fmt(char* mem, int max, char* fmt, ...)
{
	int ret;
	va_list args;

	va_start(args,fmt);
	ret = vsnprintf(mem, max, fmt, args);
	va_end(args);

	return ret;
}
void say(char* fmt , ...)
{
	int i,j;
	va_list args;
	va_start(args,fmt);
	cur += vsnprintf(buffer+cur , 0x1000-cur , fmt , args);
	va_end(args);

	i = 0;
	for(j=0;j<cur;j++)
	{
		if(buffer[j] == 0)break;
		if(buffer[j] == '\n')
		{
			buffer[j] = 0;
			LOGI("%s",buffer+i);

			i=j+1;
		}
	}

	if(i == j)cur = 0;
	else
	{
		for(cur=0;cur<j-i;cur++)
		{
			buffer[cur] = buffer[cur+i];
		}
	}
}
*/




int lowlevel_input(char* buf)
{
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




void* waitenv()
{
	return 0;
}
void* pollenv()
{
	return 0;
}
void fixarg(u8* dst, u8* src)
{
	snprintf(dst, 0x1000, "%s", src);
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