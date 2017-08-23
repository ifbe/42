#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <android/bitmap.h>
#include "arena.h"
//
void actorwrite(void* p);
void actorread();
//
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
	u64 p[4] = {value, type, (u64)&win[1], 0};
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

	win[0].type = hex32('b','u','f',0);
	win[0].fmt = hex64('r','g','b','a','8','8','8','8');
	win[0].fd = pixels;
	win[0].dc = 0;

	win[1].type = hex32('w','i','n',0);
	win[1].fmt = hex64('r','g','b','a','8','8','8','8');
	win[1].w = info.width;
	win[1].h = info.height;

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
