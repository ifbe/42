#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include <android/log.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include "supply.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
//libuser1
#define _char_ hex32('c','h','a','r')
void freeentity();
void initentity(void*);
int entityevent(void*);
//libuser0
#define _sup_ hex32('w','i','n',0)
void freesupply();
void initsupply(void*);
int supplyevent(void*);
//libsoft1
#define _art_ hex32('a','r','t',0)
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
static _obj* supply;
static ANativeWindow* native;
static ANativeWindow_Buffer buffer;




JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Read(JNIEnv* env, jobject obj)
{
	int ret;
	u64 time;
	struct event* ev;
	while(1)
	{
		time = timeread_us();
		if(ANativeWindow_lock(native, &buffer, NULL) != 0)
		{
			LOGI("error@read\n");
			return;
		}

		//draw pixel
		//LOGI("width=%d,stride=%d\n", buffer.width, buffer.stride);
		supply[0].buf = buffer.bits;
		supply[0].width = buffer.width;

		supply[0].fbwidth = buffer.stride;
		//supply[0].fbwidth = 0;
		entityread(&supply[0],0,0,0);

		//
		ANativeWindow_unlockAndPost(native);

		time = timeread_us() - time;
		logtoall("delta=%d\n",time);
again:
		ev = eventread();
		if(ev == 0)break;
//logtoall("@Read: %llx,%llx,%llx\n", ev->why, ev->what, ev->where);

		//libsoft.file/socket: receiving events
		ret = (ev->what)&0xff;
		if(_sys_ == ev->what)
		{
			//network rawdata -> my event
			ret = systemevent(ev);
			if(42 == ret)continue;
			else goto again;
		}

		//foreach changed: entity_read, window_write
		entityevent(ev);
	}
}
JNIEXPORT void JNICALL Java_com_example_finalanswer_FinalAnswerView_Write(JNIEnv* env, jobject obj, jlong type, jlong data)
{
	int dx, dy;
	u64 why, what, where;
	//logtoall("@Write:%llx,%llx\n", type, data);

	where = (u64)&supply[0];
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

	supply[0].type = hex32('w','i','n',0);
	supply[0].fmt = hex64('r','g','b','a','8','8','8','8');
	supply[0].width = w;
	supply[0].height = h;
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
	initsupply(world+0xc00000);
	initentity(world+0xc00000);

	//
	supply = world+0xc00000;
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm, void* reserved)
{
	LOGI("JNI_OnUnLoad\n");

	//libuser
	freeentity();
	freesupply();

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




void window_read()
{
	logtoall("@windowlist\n");
}
void window_write()
{
	logtoall("@windowchange\n");
}
void window_attach()
{
	logtoall("@windowstart\n");
}
void window_detach()
{
	logtoall("@windowstop\n");
}




void window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@windowread\n");
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	logtoall("@windowwrite\n");
}
void window_create()
{
	logtoall("@windowcreate\n");
}
void window_delete()
{
	logtoall("@windowdelete\n");
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
}
void freewindow()
{
}
