#include <android/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




int cur=0;
char buffer[0x1000];
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




void createserial()
{
}
void deleteserial()
{
}
