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




static void fill_plasma( AndroidBitmapInfo*  info, void*  pixels)
{
	int x;
	uint32_t temp;
	uint32_t* this;

	//拿
	characterread();

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
	fill_plasma(&info, pixels);

	//
	AndroidBitmap_unlockPixels(env, bitmap);
}
JNIEXPORT void JNICALL Java_com_example_plasma_PlasmaView_Write(JNIEnv * env, jobject  obj , jlong type , jlong value)
{
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
			if(upx>upy)characterwrite(0x64626b,0x27);
			//下
			else characterwrite(0x64626b,0x28);
		}
		else if( (upx<0) && (upy<0) )
		{
			//左
			if(upx<upy)characterwrite(0x64626b,0x25);
			//上
			else characterwrite(0x64626b,0x26);
		}
		else if( (upx<0) && (upy>0) )
		{
			//左
			if(upx+upy<0)characterwrite(0x64626b,0x25);
			//下
			else characterwrite(0x64626b,0x28);
		}
		else if( (upx>0) && (upy<0) )
		{
			//右
			if(upx+upy>0)characterwrite(0x64626b,0x27);
			//上
			else characterwrite(0x64626b,0x26);
		}
		return;
	}
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

	characterstart(
		((info.height)<<16) + (info.width) ,
		pixels
	);

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
