

#include <jni.h>
#include <dlfcn.h>
#include <android/log.h>
#include <android/sensor.h>
#include <android_native_app_glue.h>
#include "libuser.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
void sendtowindow_sensor(int, void*);



static struct android_app* theapp;
//
ASensorManager* sensorManager;
ASensorEventQueue* sensorEventQueue;
const ASensor* acc = 0;
const ASensor* gyr = 0;
const ASensor* mag = 0;




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
			float* iv = ev.vector.v;
            vec4 ov;
			if( (0!=gyr) | (0!=acc) | (0!=mag) ){
				while (ASensorEventQueue_getEvents(sensorEventQueue, &ev, 1) > 0) {
					switch(ev.type){
					case ASENSOR_TYPE_GYROSCOPE:
						//say("gyr: x=%f,y=%f,z=%f,time=%lld\n", iv[0], iv[1], iv[2], ev.timestamp);
                        ov[0] = iv[0];
                        ov[1] = iv[1];
                        ov[2] = iv[2];
                        ov[3] = (float)ev.timestamp;
						sendtowindow_sensor('g', ov);
						break;
					case ASENSOR_TYPE_ACCELEROMETER:
						//say("acc: x=%f,y=%f,z=%f,time=%lld\n", iv[0], iv[1], iv[2], ev.timestamp);
                        ov[0] = iv[0];
                        ov[1] = iv[1];
                        ov[2] = iv[2];
                        ov[3] = (float)ev.timestamp;
						sendtowindow_sensor('a', ov);
						break;
					case ASENSOR_TYPE_MAGNETIC_FIELD:
						//say("mag: x=%f,y=%f,z=%f,time=%lld\n", iv[0], iv[1], iv[2], ev.timestamp);
                        ov[0] = iv[0];
                        ov[1] = iv[1];
                        ov[2] = iv[2];
                        ov[3] = (float)ev.timestamp;
						sendtowindow_sensor('m', ov);
						break;
					}
				}
			}
		}

		if(theapp->destroyRequested)return 0;
	}
	return 0;
}


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




void androidsensor_start()
{
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
}
void androidsensor_stop()
{
    if(gyr){
        ASensorEventQueue_disableSensor(sensorEventQueue, gyr);
    }
    if(acc){
        ASensorEventQueue_disableSensor(sensorEventQueue, acc);
    }
    if(mag){
        ASensorEventQueue_disableSensor(sensorEventQueue, mag);
    }
}



void androidsensor_init(struct android_app* app)
{
	theapp = app;
	sensorManager = AcquireASensorManagerInstance(theapp);


    ASensorList list;
    int n = ASensorManager_getSensorList(sensorManager, &list);
    LOGI("ASensorManager_getSensorList = %d\n",n) ;
    for(int i=0;i<n;i++){
        LOGI("%d: Type=%d, StringType=%s\n", i, ASensor_getType(list[i]), ASensor_getStringType(list[i]));
    }


	gyr = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_GYROSCOPE);
	acc = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_ACCELEROMETER);
	mag = ASensorManager_getDefaultSensor(sensorManager, ASENSOR_TYPE_MAGNETIC_FIELD);


	sensorEventQueue = ASensorManager_createEventQueue(sensorManager, theapp->looper, LOOPER_ID_USER, 0, 0);
}
void androidsensor_exit()
{
}