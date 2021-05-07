#include "libuser.h"
int startfile(void*, int);
int stopfile(int);
int readfile( void*, int, void*, int, void*, int);
int writefile(void*, int, void*, int, void*, int);
void* getsuffix(void* p);
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




void savejpgfromimg(char* name, int offs, u8* buf, int len, int width, int height)
{
}
void loadimgfromjpg(u8* buf, int len, int* width, int* height, int* depth, int* stride)
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
		buf[ret*4+3] = 0xff;
	}

	*width = njGetWidth();
	*height = njGetHeight();
	*depth = 3;
	njDone();
}




void savepngfromimg(char* name, int offs, u8* buf, int len, int width, int height)
{
}
void loadimgfrompng(u8* buf, int len, int* width, int* height, int* depth, int* stride)
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

	*width = upng_get_width(upng);
	*height = upng_get_height(upng);
	*depth = upng_get_bpp(upng) / 8;

	w = *width;
	h = *height;
	d = *depth;
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

	upng_free(upng);
}




void saveppmfromimg(char* name, int offs, u8* buf, int bpp, int width, int height)
{
	int j,fd,ret;
	u8 tmp[64];

	fd = startfile(name, 'w');
	ret = mysnprintf(tmp, 64, "P6\n%d %d\n%d\n", width, height, 255);
	writefile(0,fd, 0,0, tmp,ret);

	for(j=0;j<2048*2048*4;j+=4)writefile(0,fd, 0,0, buf+j,3);
	stopfile(fd);
}
void loadimgfromppm(u8* buf, int len, int* width, int* height, int* depth, int* stride)
{
}




void savetgafromimg(char* name, int offs, u8* buf, int len, int width, int height)
{
}
void loadimgfromtga(u8* buf, int len, int* width, int* height, int* depth, int* stride)
{
}




void savefilefromtex(char* name, struct texture* tex)
{
}
void loadtexfromfile(struct texture* tex, char* name)
{
	int w,h,d,s;
	int len;
	u8* buf;
	u8* tmp;
	if(0 == tex)return;
	if(0 == name)return;

	buf = tex->data;
	if(0 == buf)return;

	tmp = getsuffix(name);
	if(0 == tmp)return;

	len = openreadclose(name, 0, buf, 0x1000000);
	if(len <= 0){
		say("len=%d, %s\n", len, name);
		return;
	}

	if(0 == tmp[0])return;
	else if(0 == ncmp(tmp, "jpg", 3))loadimgfromjpg(buf, len, &w, &h, &d, &s);
	else if(0 == ncmp(tmp, "png", 3))loadimgfrompng(buf, len, &w, &h, &d, &s);
	else if(0 == ncmp(tmp, "ppm", 3))loadimgfromppm(buf, len, &w, &h, &d, &s);
	else if(0 == ncmp(tmp, "tga", 3))loadimgfromtga(buf, len, &w, &h, &d, &s);
	else w=h=0;

	tex->w = w;
	tex->h = h;
}




void loadtexfromcolor(struct texture* tex, u32 rgb, int w, int h)
{
	if(0 == tex)return;

	u32* buf = tex->data;
	if(0 == buf)return;

	int x,y;
	for(y=0;y<h;y++){
		for(x=0;x<w;x++){
			buf[y*w + x] = rgb;
		}
	}
	tex->w = w;
	tex->h = h;
}
