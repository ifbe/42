#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <android/bitmap.h>
#include <android/log.h>
#include "arena.h"
//
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//
void actorwrite(void* p);
void actorread();
//
void network_explain(u64* p);
void sound_explain(u64* p);
void vision_explain(u64* p);
//
void* birth();
void death();




static void* world;
static struct window* arena;
//
static int pressed=0;
static int xxxx=0;
static int yyyy=0;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Read(JNIEnv* env, jobject obj, jobject bitmap)
{
	AndroidBitmapInfo  info;
	void*              pixels;
	int                ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
		LOGI("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
	{
		LOGI("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	//draw pixel
	actorread();

	//
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong data)
{
	u64 p[4] = {data, type, (u64)&arena[1], 0};
	say("@Write:%x,%x\n", type, data);
	actorwrite(p);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Start(JNIEnv* env, jobject obj, jobject bitmap)
{
	int ret;
	u64 pixels;
	AndroidBitmapInfo info;
	LOGI("start\n");

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
		LOGI("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}
	if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
		LOGI("Bitmap format is not RGBA_8888 !");
		return;
	}
	if ((ret = AndroidBitmap_lockPixels(env, bitmap, (void*)&pixels)) < 0)
	{
		LOGI("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	arena[1].type = hex32('b','u','f',0);
	arena[1].fmt = hex64('r','g','b','a','8','8','8','8');
	arena[1].buf = pixels;
	arena[1].len = 0;
	arena[1].w = info.width;
	arena[1].h = info.height;

	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Stop(JNIEnv* env, jobject obj)
{
}
//correct:"On","Load"        wrong:"on","load"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnLoad\n");
	world = birth();
	arena = world+0x400000;
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnUnLoad\n");
	death();
}




void windowread()
{
}
void windowwrite()
{
}
void windowstart()
{
}
void windowstop()
{
}
void windowcreate()
{
}
void windowdelete()
{
}




void initwindow()
{
}
void freewindow()
{
}
