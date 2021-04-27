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




char buffer[0x1000];
int cur=0;




int lowlevel_input()
{
	usleep(1000);
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




int freeserial()
{
	return 0;
}
int initserial()
{
	return 0;
}
