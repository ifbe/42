#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libuser.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
void* getapp();
void* pollenv();
void* supply_alloc();
void* supply_recycle(void*);




static struct android_app* theapp = 0;
static int status = 0;
static _obj* thewin = 0;
static ANativeWindow* native;




static void handle_cmd(struct android_app* app, int32_t cmd)
{
	LOGI("app=%llx,cmd=%x\n", (u64)app, cmd);
	if(APP_CMD_START == cmd)
	{
		LOGI("APP_CMD_START");
	}
	else if(APP_CMD_RESUME == cmd)
	{
		LOGI("APP_CMD_RESUME");
	}
	else if(APP_CMD_PAUSE == cmd)
	{
		LOGI("APP_CMD_PAUSE");
	}
	else if(APP_CMD_STOP == cmd)
	{
		LOGI("APP_CMD_STOP");
	}
	else if(APP_CMD_DESTROY == cmd)
	{
		LOGI("APP_CMD_DESTROY");
	}
	else if(APP_CMD_GAINED_FOCUS == cmd)
	{
		LOGI("APP_CMD_GAINED_FOCUS");
	}
	else if(APP_CMD_LOST_FOCUS == cmd)
	{
		LOGI("APP_CMD_LOST_FOCUS");
	}
	else if(APP_CMD_INIT_WINDOW == cmd)
	{
		LOGI("APP_CMD_INIT_WINDOW");
		native = app->window;
		int w = ANativeWindow_getWidth(native);
		int h = ANativeWindow_getHeight(native);
		ANativeWindow_setBuffersGeometry(native, w, h, WINDOW_FORMAT_RGBA_8888);
		status = 1;
	}
	else if(APP_CMD_WINDOW_RESIZED == cmd)
	{
		LOGI("APP_CMD_WINDOW_RESIZED");
	}
	else if(APP_CMD_TERM_WINDOW == cmd)
	{
		LOGI("APP_CMD_TERM_WINDOW");
		//appState->windowInitialized = false;
		//termDisplay(appState);
	}
	else if(APP_CMD_SAVE_STATE == cmd)
	{
		LOGI("APP_CMD_SAVE_STATE");
		//app->savedState = malloc(sizeof(SavedState));
		//app->savedStateSize = sizeof(SavedState);
		//app->savedState = appState->savedState;
	}
	else if(APP_CMD_CONFIG_CHANGED == cmd)
	{
		LOGI("APP_CMD_CONFIG_CHANGED");
	}
	else
	{
		LOGI("Unknown CMD: %d", cmd);
	}
	//appState->running = (appState->resumed && appState->windowInitialized && appState->focused);
}
static int32_t handle_input(struct android_app* app, AInputEvent* ev)
{
	u64 why[4];
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
			LOGI("a=%x,c=%x\n",a,c);

			j = (a>>8)&0xf;
			a &= 0xf;
			if(2 == a)
			{
				for(j=0;j<c;j++)
				{
					x = AMotionEvent_getX(ev, j);
					y = AMotionEvent_getY(ev, j);
					why[0] = j;
					why[0] = x+(y<<16)+(why[0]<<48);
					//eventwrite(why, 0x4070, (u64)thewin, 0);

					why[1] = 0x4070;
					why[2] = (u64)thewin;
					supplyevent((void*)why);
				}
			}
			else
			{
				if((0==a)|(5==a))a = 0x2b70;
				else if((1==a)|(6==a))a = 0x2d70;

				x = AMotionEvent_getX(ev, j);
				y = AMotionEvent_getY(ev, j);
				why[0] = AMotionEvent_getPointerId(ev, j);
				why[0] = x+(y<<16)+(why[0]<<48);
				why[1] = a;
				why[2] = (u64)thewin;
				supplyevent((void*)why);
			}
		}
		else if(AINPUT_SOURCE_TRACKBALL == source)
		{
		}
	}
	return 0;
}




void windowread_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
	ANativeWindow_Buffer buffer;
	struct android_poll_source* source;
	int ident;
	int events;

	if(status)
	{
		//get addr and size
		ANativeWindow_lock(native, &buffer, NULL);

		//set addr and size
		thewin->buf = buffer.bits;
		thewin->width = buffer.width;
		thewin->height = buffer.height;

		thewin->fbwidth = buffer.stride;
		//thewin->fbheight = 0;

		//read data
		supply_rootread(thewin, 0, 0, 0, 0, 0);

		//write screen
		ANativeWindow_unlockAndPost(native);
	}

	//events
	pollenv();
}
void window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
void window_detach()
{
}
void window_attach()
{
}




void window_read()
{
}
void window_write()
{
}
void window_delete(_obj* win)
{
}
void window_create(_obj* win)
{
	thewin = win;
	win->fmt = hex64('r','g','b','a','8','8','8','8');

	say("@windowcreate\n");
	while(!status)pollenv();
}




void initwindow()
{
	theapp = getapp();
	theapp->onAppCmd = handle_cmd;
	theapp->onInputEvent = handle_input;
}
void freewindow()
{
}
