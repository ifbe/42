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
void network_explain(u64* p);
void sound_explain(u64* p);
void vision_explain(u64* p);
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
	arena[1].buf = (u64)(buffer.bits);
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
	world = birth();
	arena = world+0x400000;
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
