#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libuser.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void androidsensor_init(struct android_app* app);
void androidsensor_exit();
void androidsensor_start();
void androidsensor_stop();
//
void openwindow(struct android_app* theapp);
void closewindow(struct android_app* theapp);
void sendtowindow_touch(void*);




static struct android_app* theapp;




static void handle_cmd(struct android_app* app, int32_t cmd)
{
	say("app=%llx,cmd=%x\n", (u64)app, cmd);
	switch(cmd){
	case APP_CMD_DESTROY:
		say("APP_CMD_DESTROY\n");
		eventwrite(0,0,0,0);
		break;
	case APP_CMD_SAVE_STATE:
		say("APP_CMD_SAVE_STATE\n");
		//app->savedState = malloc(sizeof(SavedState));
		//app->savedStateSize = sizeof(SavedState);
		//app->savedState = appState->savedState;
		break;

	case APP_CMD_START:
		say("APP_CMD_START\n");
		break;
	case APP_CMD_STOP:
		say("APP_CMD_STOP\n");
		break;

	case APP_CMD_RESUME:
		say("APP_CMD_RESUME\n");
		break;
	case APP_CMD_PAUSE:
		say("APP_CMD_PAUSE\n");
		break;

	case APP_CMD_INIT_WINDOW:
		say("APP_CMD_INIT_WINDOW\n");
		openwindow(theapp);
		break;
	case APP_CMD_TERM_WINDOW:
		say("APP_CMD_TERM_WINDOW\n");
		closewindow(theapp);
		break;

	case APP_CMD_GAINED_FOCUS:
		say("APP_CMD_GAINED_FOCUS\n");
		androidsensor_start();
		break;
	case APP_CMD_LOST_FOCUS:
		say("APP_CMD_LOST_FOCUS\n");
		androidsensor_stop();
		break;

	case APP_CMD_WINDOW_RESIZED:
		say("APP_CMD_WINDOW_RESIZED\n");
		break;
	case APP_CMD_CONFIG_CHANGED:
		say("APP_CMD_CONFIG_CHANGED\n");
		break;
	case APP_CMD_INPUT_CHANGED:
		say("APP_CMD_INPUT_CHANGED\n");
		break;
	case APP_CMD_WINDOW_REDRAW_NEEDED:
		say("APP_CMD_WINDOW_REDRAW_NEEDED\n");
		break;
	case APP_CMD_CONTENT_RECT_CHANGED:
		say("APP_CMD_CONTENT_RECT_CHANGED\n");
		break;
	case APP_CMD_LOW_MEMORY:
		say("APP_CMD_LOW_MEMORY\n");
		break;

	default:
		say("Unknown CMD: %d\n", cmd);
	}
	//appState->running = (appState->resumed && appState->windowInitialized && appState->focused);
}
static int32_t handle_input(struct android_app* app, AInputEvent* ev)
{
	u64 why[4];
	int x,y,z;
	int act,cnt,index,pointerid;
	int32_t type;
	int32_t source;
	//say("app=%llx,ev=%llx\n", (u64)app, (u64)ev);

	type = AInputEvent_getType(ev);
	switch(type){
	case AINPUT_EVENT_TYPE_KEY:
		act = AKeyEvent_getKeyCode(ev);
		say("AINPUT_EVENT_TYPE_KEY:%x\n", act);
		//app->destroyRequested = 1;
		break;
	case AINPUT_EVENT_TYPE_MOTION:
		source = AInputEvent_getSource(ev);
		if(AINPUT_SOURCE_TOUCHSCREEN == source){
			act = AMotionEvent_getAction(ev);
			cnt = AMotionEvent_getPointerCount(ev);
			switch(act&0xf){
			case 2:
				for(index=0;index<cnt;index++){
					x = AMotionEvent_getX(ev, index);
					y = AMotionEvent_getY(ev, index);
					pointerid = AMotionEvent_getPointerId(ev, index);
					say("move(act=%x,cnt=%x,index=%x,pointerid=%x)x=%x,y=%x\n", act,cnt,index,pointerid, x,y);

					why[0] = pointerid;
					why[0] = x+(y<<16)+(why[0]<<48);
					why[1] = touch_abs;
					why[2] = (u64)(theapp->userData);
					sendtowindow_touch(why);
					//eventwrite(why[0], why[1], why[2], why[3]);
				}
				return 0;
			case 0:
			case 5:
				why[1] = touch_onto;
				break;
			case 1:
			case 6:
				why[1] = touch_away;
				break;
			default:
				say("AMotionEvent_getAction=%x\n",act);
				return 0;
			}

			index = (act>>8)&0xf;
			x = AMotionEvent_getX(ev, index);
			y = AMotionEvent_getY(ev, index);
			pointerid = AMotionEvent_getPointerId(ev, index);
			say("other(act=%x,cnt=%x,index=%x,pointerid=%x)x=%x,y=%x\n", act,cnt,index,pointerid, x,y);

			why[0] = pointerid;
			why[0] = x+(y<<16)+(why[0]<<48);
			//why[1] = why[1];
			why[2] = (u64)(theapp->userData);
			sendtowindow_touch(why);
			//eventwrite(why[0], why[1], why[2], why[3]);
		}//AINPUT_SOURCE_TOUCHSCREEN
		else if(AINPUT_SOURCE_TRACKBALL == source){
			say("AINPUT_SOURCE_TRACKBALL\n");
		}
		break;
	default:
		say("AInputEvent_getType=%x\n",type);
	}//switch

	return 1;
}


void* getapp()
{
	return theapp;
}


void* getassetmgr()
{
	return theapp->activity->assetManager;
}




void android_main(struct android_app* app)
{
	//app_dummy();
	theapp = app;
	theapp->onAppCmd = handle_cmd;
	theapp->onInputEvent = handle_input;
	LOGI("internalDataPath=%s\n",theapp->activity->internalDataPath);
	LOGI("externalDataPath=%s\n",theapp->activity->externalDataPath);
	LOGI("assetManager=%p\n",theapp->activity->assetManager);

	androidsensor_init(app);

	void* all = origin_create(_ndkmain_, 0, 0, 0);
	void* thr = bootup_create(_guiapp_, 0, 0, 0);
	bootup_delete(thr);
	origin_delete(all);

	androidsensor_exit(app);

	exit(0);
}




void* pollenv()
{
	return 0;
}
void* waitenv()
{
	return 0;
}
