#include <jni.h>
#include <time.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void iamhere(unsigned long long type,void* addr);
void readcharacter();
void say(char* , ...);




//
static int alive=0;
static int xxxx=0;
static int yyyy=0;




static void fill_plasma( AndroidBitmapInfo*  info, void*  pixels)
{
    int x,y;
	int xxxx,yyyy;
    uint32_t* this;
	uint32_t* src;

	//ensure
	if(alive == 0)
	{
		alive=1;
		birth();
	}

	//拿
	readcharacter( ((info->height)<<16) + (info->width) , pixels );
}

JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_renderPlasma(JNIEnv * env, jobject  obj, jobject bitmap,  jlong  time_ms)
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
    fill_plasma(&info, pixels);

	//
    AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_ProcessEvent(JNIEnv * env, jobject  obj , jlong type , jlong value)
{
	xxxx=value&0xffff;
	yyyy=(value>>16)&0xffff;
	say(">>>>>>>>>>>>>(%llx,%llx),(%x,%x)\n",type,value,xxxx,yyyy);
}