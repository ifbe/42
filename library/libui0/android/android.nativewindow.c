#define u64 unsigned long long
#define u32 unsigned long long
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include <android/native_window_jni.h>
//
void characterstart(char* pixbuf, char* pixfmt, int width, int height);
void characterstop();
void characterwrite(u64* p);
void characterread();
//
void motion_explain(u64* p);
void network_explain(u64* p);
void sound_explain(u64* p);
void vision_explain(u64* p);
//
void say(char* , ...);
void birth();
void death();



struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	u64 thread;
};
static struct window* win;
static ANativeWindow* window;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Read(JNIEnv* env, jobject obj)
{
	characterread();
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong value)
{
	u64 p[4] = {value, type, 0, 0};
	motion_explain(p);
	//say("(%llx,%llx),%d,%d\n",type,value,sizeof(type),sizeof(value));

	//
	if(ANativeWindow_lock(window, &pixbuf, NULL) == 0)
	{
		characterwrite(p);
		ANativeWindow_unlockAndPost(window);
	}
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Start(JNIEnv* env, jobject obj, jobject surface)
{
	say("start\n");

	//window(surface?)
	window = ANativeWindow_fromSurface(env, surface);
	ANativeWindow_setBuffersGeometry(window, 0, 0, WINDOW_FORMAT_RGBA_8888);

	//my toy
	win->buf1 = surface.buffer;
	win->fmt = 0x3838383861626772;
	win->w = surface.width;
	win->h = surface.height;
	characterstart(pixbuf, pixfmt, width, height-64);
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Stop(JNIEnv* env, jobject obj)
{
}
//correct:"On","Load"        wrong:"on","load"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	say("JNI_OnLoad\n");
	birth();
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	say("JNI_OnUnLoad\n");
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
