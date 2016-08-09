#include <jni.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define QWORD unsigned long long
void characterstart(QWORD size,void* addr);
void characterread();
void characterwrite(QWORD type,QWORD value);
void say(char* , ...);




//
static int pressed=0;
static int xxxx=0;
static int yyyy=0;




JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_Read(JNIEnv * env, jobject  obj, jobject bitmap)
{
	AndroidBitmapInfo  info;
	void*              pixels;
	int                ret;

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
		say("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}

	if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
		say("Bitmap format is not RGBA_8888 !");
		return;
	}

	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
	{
		say("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	/* Now fill the values with a nice little plasma */
	//fill_plasma(&info, pixels);
	characterread();

	//
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_Write(JNIEnv * env, jobject  obj , jlong type , jlong value)
{
	//say("(%llx,%llx),%d,%d\n",type,value,sizeof(type),sizeof(value));
	characterwrite(type,value);
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_Start(JNIEnv * env, jobject  obj, jobject bitmap)
{
	AndroidBitmapInfo  info;
	int                ret;
	void*              pixels;
	say("start\n");

	if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
		say("AndroidBitmap_getInfo() failed ! error=%d", ret);
		return;
	}
	if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
	{
		say("AndroidBitmap_lockPixels() failed ! error=%d", ret);
	}

	characterstart( ((info.height-64)<<16) + info.width , pixels );
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_Stop(JNIEnv * env, jobject  obj)
{
}
//correct:"On","Load"        wrong:"on","load"
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm,void* reserved)
{
	say("JNI_OnLoad\n");
	birth();
	return JNI_VERSION_1_6;
}
JNIEXPORT void JNICALL JNI_OnUnLoad(JavaVM* vm,void* reserved)
{
	say("JNI_OnUnLoad\n");
	death();
}




void uievent()
{
}
void windowread()
{
}
void windowwrite()
{
}
void windowstart()
{
}
void windowstop()
{
}
void windowinit()
{
}
void windowkill()
{
}
