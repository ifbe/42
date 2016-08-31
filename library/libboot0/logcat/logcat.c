#include <android/log.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#define  LOG_TAG    "lib42"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




char buffer[256];
int diary(char* mem, int max, char* fmt, ...)
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
	va_list args;
	va_start(args,fmt);
	vsnprintf(buffer , 256 , fmt , args);
	va_end(args);
	LOGI("%s",buffer);
}




void createserial()
{
}
void deleteserial()
{
}
