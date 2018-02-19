#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "arena.h"
#define __int__ hex32('i','n','t',0)
#define __fd__ hex32('f','d',0,0)
#define __win__ hex32('w','i','n',0)
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
void* eventread();
void eventwrite(u64,u64,u64,u64);




//
u64 temp;
static void* world;
static struct window* arena;
static ANativeWindow* native;
static ANativeWindow_Buffer buffer;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Read(JNIEnv* env, jobject obj)
{
	int ret;
	struct event* ev;
	while(1)
	{

		//
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

again:
		ev = eventread();
		if(ev == 0)break;
say("@Read: %llx,%llx,%llx\n", ev->why, ev->what, ev->where);

		//libsoft.file/socket: receiving events
		ret = (ev->what)&0xff;
		if((ev->what == __fd__)|(ret == 's')|(ret == 'v'))
		{
			//network rawdata -> my event
			ret = artery_explain(ev);
			if(ret != 42)goto again;
		}

		//foreach changed: actor_read, window_write
		actorwrite(ev);
	}
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong data)
{
	int dx, dy;
	u64 why, what, where;
	say("@Write:%x,%x\n", type, data);

	where = (u64)&arena[1];
	eventwrite(data, type, where, 0);

	if(0x2b70 == type)temp = data;
	if(0x2d70 == type)
	{
		dx = (data&0xffff) - (temp&0xffff);
		dy = ((data>>16)&0xffff) - ((temp>>16)&0xffff);

		if(dy > 1000)
		{
			if(dx > 500)why = 0xfc;
			else if(dx < -500)why = 0xfb;
		}
		else if(dy < -200)why = 0x48;
		else if(dy > 200)why = 0x50;
		else if(dx < -200)why = 0x4b;
		else if(dx > 200)why = 0x4d;
		else return;

		what = hex32('k','b','d',0);
		eventwrite(why, what, where, 0);
	}
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

	arena = world+0xc00000;
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnUnLoad\n");

	//libuser
	actordelete();
	arenadelete();

	//libsoft
	arterydelete();
	systemdelete();

	//libhard
	bodydelete();
	driverdelete();

	death();
}




void windowlist()
{
	say("@windowlist\n");
}
void windowchange()
{
	say("@windowchange\n");
}
void windowread()
{
	say("@windowread\n");
}
void windowwrite()
{
	say("@windowwrite\n");
}
void windowstart()
{
	say("@windowstart\n");
}
void windowstop()
{
	say("@windowstop\n");
}
void windowcreate()
{
	say("@windowcreate\n");
}
void windowdelete()
{
	say("@windowdelete\n");
}