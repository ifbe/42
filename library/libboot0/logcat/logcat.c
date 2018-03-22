#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)




char buffer[0x1000];
int cur=0;




int lowlevel_input(char* buf)
{
	return 0;
}
int lowlevel_output(char* buf, int len)
{
	int j;
	for(j=0;j<len;j++)
	{
		buffer[cur] = buf[j];
		cur++;

		if((buf[j] == '\n') | (cur >= 0xfff))
		{
			buffer[cur] = 0;
			LOGI("%s",buffer);

			cur = 0;
		}
	}
	return len;
}




void* waitenv()
{
	return 0;
}
void* pollenv()
{
	return 0;
}
void fixarg(u8* dst, u8* src)
{
	snprintf(dst, 0x1000, "%s", src);
}




static u8* rawuniverse;
void death()
{
}
void* birth()
{
#define __size__ 0x1001000
	u64 j;
	u64 temp;


	//1.alloc
	rawuniverse = malloc(__size__);
	if(NULL == rawuniverse)
	{
		printf("no enough momery\n");
		exit(-1);
	}
	for(j=0;j<__size__;j++)rawuniverse[j]=0;


	//2.align
	//[0x   0,0x1001000)	->	[0x0000,0x1000000)
	//[0x 234,0x1001234)	->	[0x1000,0x1001000)
	//[0x fff,0x1001fff)	->	[0x1000,0x1001000)
	//[0x1001,0x1002001)	->	[0x1000,0x1002000)
	temp = ( (u64)rawuniverse ) & 0xfff;
	if(0 != temp)temp = 0x1000 - temp;
	return rawuniverse + temp;
}




static void* theapp;
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
	u64 why;
	int x,y,a,c,j;
	int32_t type;
	int32_t source;
	//LOGI("app=%llx,ev=%llx\n", (u64)app, (u64)ev);

	type = AInputEvent_getType(ev);
	if(AINPUT_EVENT_TYPE_KEY == type)
	{
		LOGI("!!!!!!!\n");
		eventwrite(0,0,0,0);
		app->destroyRequested = 1;
	}
	else if(AINPUT_EVENT_TYPE_MOTION == type)
	{
		source = AInputEvent_getSource(ev);
		if(AINPUT_SOURCE_TOUCHSCREEN == source)
		{
			a = AMotionEvent_getAction(ev);
			c = AMotionEvent_getPointerCount(ev);
			say("a=%x,c=%x\n",a,c);

			j = (a>>8)&0xf;
			a &= 0xf;
			if(2 == a)
			{
				for(j=0;j<c;j++)
				{
					x = AMotionEvent_getX(ev, j);
					y = AMotionEvent_getY(ev, j);
					why = j;
					why = x+(y<<16)+(why<<48);
					eventwrite(why, 0x4070, (u64)thewin, 0);
				}
			}
			else
			{
				if((0==a)|(5==a))a = 0x2b70;
				else if((1==a)|(6==a))a = 0x2d70;

				x = AMotionEvent_getX(ev, j);
				y = AMotionEvent_getY(ev, j);
				why = AMotionEvent_getPointerId(ev, j);
				why = x+(y<<16)+(why<<48);
				eventwrite(why, a, (u64)thewin, 0);
			}
		}
		else if(AINPUT_SOURCE_TRACKBALL == source)
		{
		}
	}
}
void android_loop(struct android_app* app)
{
	theapp = app;
	app->onAppCmd = handle_cmd;
    app->onInputEvent = handle_input;
	while(1)
	{
		int ident;
		int events;
		struct android_poll_source* source;
		while ((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
		{
			if(source)source->process(app, source);
			if(app->destroyRequested)return;
		}
	}
}