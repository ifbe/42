#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "arena.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//libuser1
#define _char_ hex32('c','h','a','r')
void freeactor();
void initactor(void*);
int actorread();
int actorwrite(void*);
//libuser0
#define _win_ hex32('w','i','n',0)
void freearena();
void initarena(void*);
int arenaread();
int arenawrite(void*);
//libsoft1
#define _fd_ hex32('f','d',0,0)
void freeartery();
void initartery(void*);
int arteryread();
int arterywrite(void*);
//libsoft0
#define _sys_ hex32('s','y','s',0)
void freesystem();
void initsystem(void*);
int systemread();
int systemwrite();
//libhard1
#define _drv_ hex32('d','r','v',0)
void freedriver();
void initdriver(void*);
int driverread();
int driverwrite(void*);
//libhard0
#define _dev_ hex32('i','n','t',0)
void freedevice();
void initdevice(void*);
int deviceread();
int devicewrite(void*);
//libboot1
#define _hash_ hex32('h','a','s','h')
void freestdin();
void initstdin(void*);
void freestdout();
void initstdout(void*);
void freestdev();
void initstdev(void*);
void freestdrel();
void initstdrel(void*);
//libboot0
#define _01_ hex32('0','1','0','1')
void death();
void* birth();
void eventwrite(u64,u64,u64,u64);
void* eventread();
void printmemory(void*, int);
void say(void*, ...);




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
		arena[0].buf = buffer.bits;
		arena[0].w = buffer.stride;
		actorread();

		//
		ANativeWindow_unlockAndPost(native);

again:
		ev = eventread();
		if(ev == 0)break;
say("@Read: %llx,%llx,%llx\n", ev->why, ev->what, ev->where);

		//libsoft.file/socket: receiving events
		ret = (ev->what)&0xff;
		if(_fd_ == ev->what)
		{
			//network rawdata -> my event
			ret = arterywrite(ev);
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

	where = (u64)&arena[0];
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

	arena[0].type = hex32('w','i','n',0);
	arena[0].fmt = hex64('r','g','b','a','8','8','8','8');
	arena[0].w = w;
	arena[0].h = h;
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Stop(JNIEnv* env, jobject obj)
{
}
//correct:"On","Load"        wrong:"on","load"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnLoad\n");

	//libboot
	world = birth();
	initstdin( world+0x000000);
	initstdout(world+0x100000);
	initstdev( world+0x200000);
	initstdrel(world+0x300000);

	//libsoft
	initdevice(world+0x400000);
	initdriver(world+0x400000);

	//libsoft
	initsystem(world+0x800000);
	initartery(world+0x800000);

	//libuser
	initarena(world+0xc00000);
	initactor(world+0xc00000);

	//
	arena = world+0xc00000;
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnUnLoad\n");

	//libuser
	freeactor();
	freearena();

	//libsoft
	freeartery();
	freesystem();

	//libhard
	freedriver();
	freedevice();

	//libboot
	freestdev();
	freestdrel();
	freestdout();
	freestdin();
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