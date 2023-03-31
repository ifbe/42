#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <android/log.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include "libuser.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void openwindow(struct android_app* theapp);
void closewindow(struct android_app* theapp);
void sendtowindow(int, void*);




static struct android_app* theapp;
//
ASensorManager* sensorManager;
ASensorEventQueue* sensorEventQueue;
const ASensor* acc = 0;
const ASensor* gyr = 0;
const ASensor* mag = 0;




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
		if(gyr){
			ASensorEventQueue_enableSensor(sensorEventQueue, gyr);
			ASensorEventQueue_setEventRate(sensorEventQueue, gyr, (1000L/60)*1000);
		}
		if(acc){
			ASensorEventQueue_enableSensor(sensorEventQueue, acc);
			ASensorEventQueue_setEventRate(sensorEventQueue, acc, (1000L/60)*1000);
		}
		if(mag){
			ASensorEventQueue_enableSensor(sensorEventQueue, mag);
			ASensorEventQueue_setEventRate(sensorEventQueue, mag, (1000L/60)*1000);
		}
		break;
	case APP_CMD_LOST_FOCUS:
		say("APP_CMD_LOST_FOCUS\n");
		if(gyr){
			ASensorEventQueue_disableSensor(sensorEventQueue, gyr);
		}
		if(acc){
			ASensorEventQueue_disableSensor(sensorEventQueue, acc);
		}
		if(mag){
			ASensorEventQueue_disableSensor(sensorEventQueue, mag);
		}
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
	int act,cnt,j;
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
			say("act=%x,cnt=%x\n", act, cnt);

			j = (act>>8)&0xf;
			act &= 0xf;
			switch(act){
			case 2:
				for(j=0;j<cnt;j++){
					x = AMotionEvent_getX(ev, j);
					y = AMotionEvent_getY(ev, j);
					why[0] = j;
					why[0] = x+(y<<16)+(why[0]<<48);
					why[1] = touch_abs;
					why[2] = (u64)(theapp->userData);
					//fullwindow_write(theapp->userData, (void*)why);
					eventwrite(why[0], why[1], why[2], why[3]);
				}
				return 0;
			case 0:
			case 5:
				act = touch_onto;
				break;
			case 1:
			case 6:
				act = touch_away;
				break;
			default:
				say("AMotionEvent_getAction=%x\n",act);
				return 0;
			}

			x = AMotionEvent_getX(ev, j);
			y = AMotionEvent_getY(ev, j);
			why[0] = AMotionEvent_getPointerId(ev, j);
			why[0] = x+(y<<16)+(why[0]<<48);
			why[1] = act;
			why[2] = (u64)(theapp->userData);
			//fullwindow_write(theapp->userData, (void*)why);
			eventwrite(why[0], why[1], why[2], why[3]);
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
int checkevent()
{
	int ident;
	int events;
	struct android_poll_source* source;

	while((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
	{
		if(source)source->process(theapp, source);

		if (ident == LOOPER_ID_USER) {
			ASensorEvent ev;
			float* v = ev.vector.v;
			if( (0!=gyr) | (0!=acc) | (0!=mag) ){
				while (ASensorEventQueue_getEvents(sensorEventQueue, &ev, 1) > 0) {
					switch(ev.type){
					case ASENSOR_TYPE_GYROSCOPE:
						//say("gyr: x=%f,y=%f,z=%f\n", v[0], v[1], v[2]);
						//say("time=%lld\n",ev.timestamp);
						sendtowindow('g', v);
						break;
					case ASENSOR_TYPE_ACCELEROMETER:
						//say("acc: x=%f,y=%f,z=%f\n",-v[0],-v[1],-v[2]);
						sendtowindow('a', v);
						break;
					case ASENSOR_TYPE_MAGNETIC_FIELD:
						//say("mag: x=%f,y=%f,z=%f\n", v[0], v[1], v[2]);
						sendtowindow('m', v);
						break;
					}
				}
			}
		}

		if(theapp->destroyRequested)return 0;
	}
	return 0;
}
void* getassetmgr()
{
	return theapp->activity->assetManager;
}
#include <jni.h>
#include <dlfcn.h>
ASensorManager* AcquireASensorManagerInstance(struct android_app* app) {
  if (!app) return 0;

  typedef ASensorManager* (*PF_GETINSTANCEFORPACKAGE)(const char* name);
  void* androidHandle = dlopen("libandroid.so", RTLD_NOW);
  PF_GETINSTANCEFORPACKAGE getInstanceForPackageFunc = (PF_GETINSTANCEFORPACKAGE)dlsym(
      androidHandle, "ASensorManager_getInstanceForPackage");
  if (getInstanceForPackageFunc) {
    JNIEnv* env = 0;
    (*app->activity->vm)->AttachCurrentThread(app->activity->vm, &env, 0);

    jclass android_content_Context = (*env)->GetObjectClass(env, app->activity->clazz);
    jmethodID midGetPackageName = (*env)->GetMethodID(env, android_content_Context, "getPackageName", "()Ljava/lang/String;");
    void* packageName = (*env)->CallObjectMethod(env, app->activity->clazz, midGetPackageName);
    const char* nativePackageName = (*env)->GetStringUTFChars(env, packageName, 0);
    ASensorManager* mgr = getInstanceForPackageFunc(nativePackageName);
    (*env)->ReleaseStringUTFChars(env, packageName, nativePackageName);

    (*app->activity->vm)->DetachCurrentThread(app->activity->vm);
    if (mgr) {
      dlclose(androidHandle);
      return mgr;
    }
  }

  typedef ASensorManager* (*PF_GETINSTANCE)();
  PF_GETINSTANCE getInstanceFunc = (PF_GETINSTANCE)dlsym(androidHandle, "ASensorManager_getInstance");
  // by all means at this point, ASensorManager_getInstance should be available
  dlclose(androidHandle);

  return getInstanceFunc();
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


	sensorManager = AcquireASensorManagerInstance(app);
	sensorEventQueue = ASensorManager_createEventQueue(sensorManager, theapp->looper, LOOPER_ID_USER, 0, 0);
	gyr = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GYROSCOPE);
	acc = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
	mag = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_MAGNETIC_FIELD);


	void* all = origin_create(_ndkmain_, 0, 0, 0);
	void* thr = bootup_create(_guiapp_, 0, 0, 0);
	bootup_delete(thr);
	origin_delete(all);


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
