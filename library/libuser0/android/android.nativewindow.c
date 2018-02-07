#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "arena.h"
//
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//
void actorwrite(void* p);
void actorread();
//
void actorcreate(void*);
void actordelete();
void arenacreate(void*);
void arenadelete();
void arterycreate(void*);
void arterydelete();
void systemcreate(void*);
void systemdelete();
void bodycreate(void*);
void bodydelete();
void drivercreate(void*);
void driverdelete();
//
void initstdin(void*);
void initstdout(void*);
void initstdev(void*);
void initstdrel(void*);
//
void* birth();
void death();




//
static void* world;
static struct window* arena;
static ANativeWindow* native;
static ANativeWindow_Buffer buffer;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Read(JNIEnv* env, jobject obj)
{
	if(ANativeWindow_lock(native, &buffer, NULL) != 0)
	{
		LOGI("error@read\n");
		return;
	}
	

	//draw pixel
	arena[1].buf = buffer.bits;
	arena[1].w = buffer.stride;
	actorread();

	//
	ANativeWindow_unlockAndPost(native);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong value)
{
	u64 p[4] = {value, type, (u64)&arena[1], 0};
	actorwrite(p);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Start(JNIEnv* env, jobject obj, jobject surface)
{
	int w,h;
	LOGI("@start\n");

	native = ANativeWindow_fromSurface(env, surface);
	LOGI("ANativeWindow_fromSurface:%llx\n", (u64)native);

	w = ANativeWindow_getWidth(native);
	h = ANativeWindow_getHeight(native);
	ANativeWindow_setBuffersGeometry(native, w, h, WINDOW_FORMAT_RGBA_8888);
	LOGI("w=%d,h=%d\n", w, h);

	arena[1].type = hex32('b','u','f',0);
	arena[1].fmt = hex64('r','g','b','a','8','8','8','8');
	arena[1].w = w;
	arena[1].h = h;
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Stop(JNIEnv* env, jobject obj)
{
}
//correct:"On","Load"        wrong:"on","load"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnLoad\n");

	//libboot0
	world = birth();
	arena = world+0xc00000;

	//libboot1
	initstdin( world+0x000000);
	initstdout(world+0x100000);
	initstdev( world+0x200000);
	initstdrel(world+0x300000);

	//libsoft
	drivercreate(world+0x400000);
	bodycreate(  world+0x400000);

	//libsoft
	systemcreate(world+0x800000);
	arterycreate(world+0x800000);

	//libuser
	arenacreate(world+0xc00000);
	actorcreate(world+0xc00000);

	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnUnLoad\n");
	death();
}




void uievent()
{
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
