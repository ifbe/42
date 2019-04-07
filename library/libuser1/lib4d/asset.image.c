#include "libuser.h"
//upng
void* upng_new_from_bytes(void* buf, int len);
void* upng_get_buffer(void* upng);
int upng_decode(void*);
int upng_free(void*);
int upng_get_error(void*);
int upng_get_width(void*);
int upng_get_height(void*);
int upng_get_bpp(void*);
//nanojpeg
void njInit(void);
void njDone(void);
int njDecode(const void* jpeg, const int size);
int njGetWidth(void);
int njGetHeight(void);
int njIsColor(void);
int njGetImageSize(void);
unsigned char* njGetImage(void);




void actorcreatefromjpg(struct actor* act, u8* buf, int len)
{
	int ret;
	u8* rgb;

    njInit();
	ret = njDecode(buf, len);
	if(0 != ret)
	{
		say("error@njDecode:%d\n", ret);
		return;
	}

	rgb = njGetImage();
	len = (njGetImageSize())/3;
	for(ret=0;ret<len;ret++)
	{
		buf[ret*4+0] = rgb[ret*3+2];
		buf[ret*4+1] = rgb[ret*3+1];
		buf[ret*4+2] = rgb[ret*3+0];
		buf[ret*4+3] = 0;
	}

	act->width = njGetWidth();
	act->height = njGetHeight();
	act->buf = buf;
	njDone();
}
void actorcreatefrompng(struct actor* act, u8* buf, int len)
{
	int w,h,d;
	int j,ret;
	u8* src;
	u8* dst;
	void* upng;

	upng = upng_new_from_bytes(buf, len);
	ret = upng_get_error(upng);
	if(0 != ret)
	{
		say("upng_new_from_bytes: %d\n", ret);
		return;
	}

	ret = upng_decode(upng);
	ret = upng_get_error(upng);
	if(0 != ret)
	{
		say("upng_decode: %d\n", ret);
		return;
	}

	w = upng_get_width(upng);
	h = upng_get_height(upng);
	d = upng_get_bpp(upng) / 8;
	say("%d,%d,%d\n",w,h,d);

	src = upng_get_buffer(upng);
	dst = buf;
	for(j=0;j<w*h;j++)
	{
		if(1 == d)
		{
			dst[j*4 + 0] = dst[j*4 + 1] = dst[j*4 + 2] = src[j];
			dst[j*4 + 3] = 255;
			continue;
		}
		if(d >= 3)
		{
			dst[j*4 + 0] = src[j*d + 2];
			dst[j*4 + 1] = src[j*d + 1];
			dst[j*4 + 2] = src[j*d + 0];
			if(3 == d)dst[j*4 + 3] = 255;
			else dst[j*4 + 3] = src[j*d + 3];
		}
	}

	act->width = w;
	act->height = h;
	act->buf = buf;
	upng_free(upng);
}