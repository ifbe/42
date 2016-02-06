#include <jni.h>
#include <time.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define QWORD unsigned long long
void readcharacter();
void writecharacter(QWORD type,QWORD value);
void say(char* , ...);




//ugly check,where can i put my birth()?
static int alive=0;
//
static int pressed=0;
static int xxxx=0;
static int yyyy=0;




static void fill_plasma( AndroidBitmapInfo*  info, void*  pixels)
{
    int x;
	uint32_t temp;
    uint32_t* this;

	//ensure
	if(alive == 0)
	{
		alive=1;
		birth();
	}

	//拿
	readcharacter( ((info->height)<<16) + (info->width) , pixels );

	//反色
	this=pixels;
	for(x=0;x<info->width*info->height;x++)
	{
		temp=this[x];
		this[x]=0xff000000 +
			((temp&0xff)<<16) +
			(temp&0xff00) +
			((temp&0xff0000)>>16);
	}
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
	//ensure
	if(alive == 0)
	{
		alive=1;
		birth();
	}

	//
	say(">>>>>>>>>>>>>(%llx,%llx)\n",type,value);
	if(type==0x6e776f64)		//'down'
	{
		pressed=1;
		xxxx=value&0xffff;
		yyyy=(value>>16)&0xffff;
		return;
	}
	if(type==0x7075)			//'up'
	{
		int upx=value&0xffff;
		int upy=(value>>16)&0xffff;
		pressed=0;
		say(">>>>>>>>>>>>>>(%d,%d)->(%d,%d)\n",xxxx,yyyy,upx,upy);

		upx-=xxxx;
		upy-=yyyy;
		if( (upx>0) && (upy>0) )
		{
			//右
			if(upx>upy)writecharacter(0x64626b,0x27);
			//下
			else writecharacter(0x64626b,0x28);
		}
		else if( (upx<0) && (upy<0) )
		{
			//左
			if(upx<upy)writecharacter(0x64626b,0x25);
			//上
			else writecharacter(0x64626b,0x26);
		}
		else if( (upx<0) && (upy>0) )
		{
			//左
			if(upx+upy<0)writecharacter(0x64626b,0x25);
			//下
			else writecharacter(0x64626b,0x28);
		}
		else if( (upx>0) && (upy<0) )
		{
			//右
			if(upx+upy>0)writecharacter(0x64626b,0x27);
			//上
			else writecharacter(0x64626b,0x26);
		}
		return;
	}
}