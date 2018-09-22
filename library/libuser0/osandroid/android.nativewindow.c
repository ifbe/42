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
int actorevent(void*);
//libuser0
#define _win_ hex32('w','i','n',0)
void freearena();
void initarena(void*);
int arenaevent(void*);
//libsoft1
#define _fd_ hex32('f','d',0,0)
void freeartery();
void initartery(void*);
int arteryevent(void*);
//libsoft0
#define _sys_ hex32('s','y','s',0)
void freesystem();
void initsystem(void*);
int systemevent();
//libhard1
#define _dri_ hex32('d','r','i',0)
void freedriver();
void initdriver(void*);
int driverevent(void*);
//libhard0
#define _dev_ hex32('d','e','v',0)
void freedevice();
void initdevice(void*);
int deviceread();
int deviceevent(void*);
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
#define _0101_ hex32('0','1','0','1')
void death();
void* birth();




static void* world;
static struct arena* arena;
static ANativeWindow* native;
static ANativeWindow_Buffer buffer;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Read(JNIEnv* env, jobject obj)
{
	int ret;
	u64 time;
	struct event* ev;
	while(1)
	{
		time = timeread();
		if(ANativeWindow_lock(native, &buffer, NULL) != 0)
		{
			LOGI("error@read\n");
			return;
		}

		//draw pixel
		//LOGI("width=%d,stride=%d\n", buffer.width, buffer.stride);
		arena[0].buf = buffer.bits;
		arena[0].width = buffer.width;
		arena[0].stride = buffer.stride;
		actorread(&arena[0],0,0,0);

		//
		ANativeWindow_unlockAndPost(native);

		time = timeread() - time;
		say("delta=%d\n",time);
again:
		ev = eventread();
		if(ev == 0)break;
//say("@Read: %llx,%llx,%llx\n", ev->why, ev->what, ev->where);

		//libsoft.file/socket: receiving events
		ret = (ev->what)&0xff;
		if(_fd_ == ev->what)
		{
			//network rawdata -> my event
			ret = systemevent(ev);
			if(42 == ret)continue;
			else goto again;
		}

		//foreach changed: actor_read, window_write
		actorevent(ev);
	}
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong data)
{
	int dx, dy;
	u64 why, what, where;
	//say("@Write:%llx,%llx\n", type, data);

	where = (u64)&arena[0];
	eventwrite(data, type, where, 0);
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
	arena[0].width = w;
	arena[0].height = h;
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Stop(JNIEnv* env, jobject obj)
{
	LOGI("@stop\n");
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
void windowread(void* dc,void* df,void* sc,void* sf)
{
	say("@windowread\n");
}
void windowwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
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




void initwindow()
{
}
void freewindow()
{
}
