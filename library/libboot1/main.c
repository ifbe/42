#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
#define hex64(a,b,c,d,e,f,g,h) (hex32(a,b,c,d) | (((u64)hex32(e,f,g,h))<<32))
//libuser1
#define _char_ hex32('c','h','a','r')
void freeactor();
void initactor(void*);
int actorread();
int actorevent(void*);
//libuser0
#define _win_ hex32('w','i','n',0)
void freearena();
void initarena(void*);
int arenaread();
int arenaevent(void*);
//libsoft1
#define _art_ hex32('a','r','t',0)
void freeartery();
void initartery(void*);
int arteryread();
int arteryevent(void*);
//libsoft0
#define _fd_ hex32('f','d',0,0)
void freesystem();
void initsystem(void*);
int systemread();
int systemevent(void*);
int sleep_us(int);
//libhard1
#define _dri_ hex32('d','r','v',0)
void freedriver();
void initdriver(void*);
int driverread();
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
int term_read(void*);
int term_write(void*);
//libboot0
#define _0101_ hex32('0','1','0','1')
void death();
void* birth();
void eventwrite(u64,u64,u64,u64);
void* eventread();
void printmemory(void*, int);
void say(void*, ...);
void fixarg(void*, void*);




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};




void* beforedawn()
{
	//libboot
	void* addr = birth();
	initstdin( addr+0x000000);
	initstdout(addr+0x100000);
	initstdev( addr+0x200000);
	initstdrel(addr+0x300000);

	//libsoft
	initdevice(addr+0x400000);
	initdriver(addr+0x400000);

	//libsoft
	initsystem(addr+0x800000);
	initartery(addr+0x800000);

	//libuser
	initarena(addr+0xc00000);
	initactor(addr+0xc00000);

	return addr;
}
void afterdusk()
{
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
int main(int argc, char* argv[])
{
	int ret;
	void* addr;
	struct event* ev;

	addr = beforedawn();
	for(ret=1;ret<argc;ret++)
	{
		fixarg(addr, argv[ret]);
		term_read(addr);
	}
	term_write("\n");

	while(1)
	{
		actorread();
		arenaread();
		//arteryread();
		//systemread();
		//driverread();
		//deviceread();
		//say("after\n");

again:
		ev = eventread();
		if(0 == ev)
		{
			sleep_us(10000);
			goto again;
		}
		if(0 == ev->what)break;

		//say("ev:%x,%x,%x,%x\n",ev->why,ev->what,ev->where,ev->when);
		if((_char_ == ev->what)&&(0 == ev->where))
		{
			term_write(ev);
			continue;
		}
/*
		//libhard0
		if(_dev_ == ev->what)
		{
			ret = deviceevent(ev);
			if(ret != 42)goto again;
		}

		//libhard1
		if(_dri_ == ev->what)
		{
			ret = driverevent(ev);
			if(ret != 42)goto again;
		}
*/
		//libsoft0
		if(_fd_ == ev->what)
		{
			ret = systemevent(ev);
			if(42 == ret)continue;
			else goto again;
		}

		//libsoft1
		if(_art_ == ev->what)
		{
			ret = arteryevent(ev);
			if(42 == ret)continue;
			else goto again;
		}

		//libuser0
		ret = (ev->what)&0xff;
		if('w' == ret)
		{
			ret = arenaevent(ev);
			continue;
		}

		//libuser1
		actorevent(ev);
	}

	afterdusk();
	return 0;
}


/*
#include <efi.h>
#include <efilib.h>
static EFI_HANDLE H;
static EFI_SYSTEM_TABLE* T;
void gethandleandtable(void** handle, void** table)
{
	*handle = H;
	*table = T;
}
EFI_STATUS efi_main(EFI_HANDLE handle, EFI_SYSTEM_TABLE *table)
{
	int ret;
	H = handle;
	T = table;

	ret = table->ConOut->OutputString(table->ConOut, L"42!!\r\n");
	if(EFI_ERROR(ret))return ret;

	ret = table->ConIn->Reset(table->ConIn, FALSE);
	if(EFI_ERROR(ret))return ret;

	main(0, 0);
	return EFI_SUCCESS;
}
void atexit(){}
*/


#ifdef __ANDROID__
#include <stdio.h>
#include <stdlib.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
static struct android_app* theapp;
static void* thewin;
void* getandroidapp()
{
	return theapp;
}
void setandroidapp(void* win)
{
	thewin = win;
}
static void handle_cmd(struct android_app* app, int32_t cmd)
{
	LOGI("app=%llx,cmd=%x\n", (u64)app, cmd);
}
static int32_t handle_input(struct android_app* app, AInputEvent* ev)
{
	int32_t type;
	int32_t source;
	//LOGI("app=%llx,ev=%llx\n", (u64)app, (u64)ev);

	type = AInputEvent_getType(ev);
	if(AINPUT_EVENT_TYPE_MOTION == type)
	{
		source = AInputEvent_getSource(ev);
		if(AINPUT_EVENT_TYPE_KEY)
		{
			eventwrite(0,0,0,0);
			theapp->destroyRequested = 1;
		}
		else if(AINPUT_SOURCE_TOUCHSCREEN == source)
		{
		}
		else if(AINPUT_SOURCE_TRACKBALL == source)
		{
		}
	}
}
void android_main(struct android_app* app)
{
	app_dummy();
	app->onAppCmd = handle_cmd;
    app->onInputEvent = handle_input;

	theapp = app;
	startthread(main, 0);

	while(1)
	{
		int ident;
		int events;
		struct android_poll_source* source;
		while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
			if(source)source->process(app, source);
			if(app->destroyRequested)goto byebye;
		}
	}

byebye:
	exit(0);
}
#endif