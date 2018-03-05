#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
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




//
static void* world;
static struct window* arena;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswer_Read(JNIEnv* env, jobject obj)
{
	actorread();
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswer_Write(JNIEnv* env, jobject obj, jlong type, jlong value)
{
	u64 p[4] = {value, type, (u64)&arena[1], 0};
	actorwrite(p);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswer_Start(JNIEnv* env, jobject obj, jobject surface)
{
	LOGI("start\n");

	arena[0].type = hex32('b','u','f',0);
	arena[0].fmt = hex64('r','g','b','a','8','8','8','8');
	arena[0].buf = (u64)(buffer.bits);
	arena[0].len = 0;

	arena[1].type = hex32('w','i','n',0);
	arena[1].fmt = hex64('r','g','b','a','8','8','8','8');
	arena[1].w = 1080;
	arena[1].h = 1920;
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswer_Stop(JNIEnv* env, jobject obj)
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
