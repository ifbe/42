#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libboot.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long




static u8* rawuniverse;
static struct android_app* APP;
void android_main(struct android_app* app)
{
	//app_dummy();
	APP = app;
	chdir("/sdcard/42/");

	void* all = origincreate(_ndkmain_, 0, 0, 0);
	void* thr = workercreate(0, 0, 0, 0);

	workerdelete(thr);
	origindelete(all);

	exit(0);
}
void* getapp()
{
	return APP;
}




void* pollenv()
{
	return 0;
}
void* waitenv()
{
	return 0;
}
