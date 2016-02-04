#include <jni.h>
#include <time.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define  LOG_TAG    "libplasma"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)




//
static int xxxx=0;
static int yyyy=0;




static void fill_plasma( AndroidBitmapInfo*  info, void*  pixels)
{
    int  y,x;
    uint32_t* this = (uint32_t*)pixels;
    for (y = 0; y < info->height*1/4; y++)
	{
		for(x=0; x < info->width; x++)
		{
			this[y*(info->width) + x]=0xff0000ff;
		}
    }
    for (; y < info->height*2/4; y++)
	{
		for(x=0; x < info->width; x++)
		{
			this[y*(info->width) + x]=0xff00ff00;
		}
    }
    for (; y < info->height*3/4; y++)
	{
		for(x=0; x < info->width; x++)
		{
			this[y*(info->width) + x]=0xffff0000;
		}
    }
    for (; y < info->height; y++)
	{
		for(x=0; x < info->width; x++)
		{
			this[y*(info->width) + x]=0xffffffff;
		}
    }

	//////////////////////////////
	xxxx = xxxx % info->width;
	yyyy = yyyy % info->height;
    for (y=0; y < yyyy; y++)
	{
		for(x=0; x < xxxx; x++)
		{
			this[y*(info->width) + x]=0xffbb7733;
		}
    }
}

JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_renderPlasma(JNIEnv * env, jobject  obj, jobject bitmap,  jlong  time_ms)
{
    AndroidBitmapInfo  info;
    void*              pixels;
    int                ret;

    if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
	{
        LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
        return;
    }

    if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
	{
        LOGE("Bitmap format is not RGBA_8888 !");
        return;
    }

    if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
	{
        LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
    }

    /* Now fill the values with a nice little plasma */
    fill_plasma(&info, pixels);

    AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_ProcessEvent(JNIEnv * env, jobject  obj , jlong type , jlong value)
{
	xxxx=value&0xffff;
	yyyy=(value>>16)&0xffff;
	LOGI(">>>>>>>>>>>>>(%llx,%llx),(%x,%x)\n",type,value,xxxx,yyyy);
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_birth(JNIEnv * env, jobject  obj)
{
	birth();
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_death(JNIEnv * env, jobject  obj)
{
	death();
}
