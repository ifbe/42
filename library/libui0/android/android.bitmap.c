#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <android/bitmap.h>
#include "arena.h"
//
void actorstart(int);
void actorstop(int);
void actorwrite(void* p);
void actorread();
//
void motion_explain(u64* p);
void network_explain(u64* p);
void sound_explain(u64* p);
void vision_explain(u64* p);
//
void say(char* , ...);
void* birth();
void death();




static void* world;
static struct window* win;
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
		say("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
	{
		say("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	//draw pixel
	actorread();

	//
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong value)
{
	u64 p[4] = {value, type, (u64)&win[0], 0};
	motion_explain(p);
	say("event:%x,%x,%x,0\n", p[0], p[1], p[2]);

	actorwrite(p);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Start(JNIEnv* env, jobject obj, jobject bitmap)
{
	int ret;
	u64 pixels;
	AndroidBitmapInfo info;
	say("start\n");

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
		say("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}
	if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
		say("Bitmap format is not RGBA_8888 !");
		return;
	}
	if ((ret = AndroidBitmap_lockPixels(env, bitmap, (void*)&pixels)) < 0)
	{
		say("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	win[0].type = 0;
	win[0].fmt = 0x3838383861626772;
	win[0].buf = pixels;
	win[0].len = 0;

	win[0].w = info.width;
	win[0].h = info.height;
	win[0].dim = 2;
	actorstart(0);
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Stop(JNIEnv* env, jobject obj)
{
}
//correct:"On","Load"        wrong:"on","load"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	world = birth();
	say("JNI_OnLoad\n");

	win = world+0x400000;
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	say("JNI_OnUnLoad\n");
	death();
}




void windowread()
{
}
void windowwrite()
{
}
void windowstart(struct window* win)
{
	win->fmt = 0x3838383861626772;
	win->dim = 2;
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
