#include "libuser.h"
void content_create(void*);
void overview_init(void*);
//
void* getsuffix(void* p);
void initutf8(void*);
int openreadclose(void* name, int off, void* buf, int len);
int openwriteclose(void* name, int off, void* buf, int len);
//
void loadimgfromjpg(u8* buf, int len, int* width, int* height, int* depth, int* stride);
void loadimgfrompng(u8* buf, int len, int* width, int* height, int* depth, int* stride);
//
void entitycreatefromobj(struct entity* act, u8* buf, int len);
void entitycreatefromstl(struct entity* act, u8* buf, int len);
void entitycreatefrommp3(struct entity* act, u8* buf, int len);
void entitycreatefromogg(struct entity* act, u8* buf, int len);
void entitycreatefromwav(struct entity* act, u8* buf, int len);




static u8* utf8data = 0;
static u16* bgmdata = 0;
//static struct fstyle def2d;
static struct fstyle def3d;




void asset_create()
{
	int j,k;
	if(0 == utf8data)
	{
		//malloc
		utf8data = memorycreate(0x200000, 0);
		if(0 == utf8data)return;

		//read
		j = openreadclose("datafile/unicode.raw", 0, utf8data, 0x200000);
		if(j < 0x200000){
			say("error@unicode\n");
			memorydelete(utf8data);
			return;
		}

		//config
		for(j=0;j<0x200000;j++)
		{
			k = utf8data[j];
			k = ((k&0x0f)<<4) | ((k&0xf0)>>4);
			k = ((k&0x33)<<2) | ((k&0xcc)>>2);
			k = ((k&0x55)<<1) | ((k&0xaa)>>1);
			utf8data[j] = k;
		}
		initutf8(utf8data);
	}
}
void asset_delete()
{
}




void loadtexfromfile(struct glsrc* src, int idx, char* name)
{
	int w,h,d,s;
	int len;
	u8* buf;
	u8* tmp;
	if(0 == src)return;
	if(0 == name)return;

	buf = src->tex[idx].data;
	if(0 == buf)return;

	tmp = getsuffix(name);
	if(0 == tmp)return;

	len = openreadclose(name, 0, buf, 0x1000000);
	if(len <= 0)
	{
		say("len=%d, %s\n", len, name);
		return;
	}

	if(0 == tmp[0])return;
	else if(0 == ncmp(tmp, "jpg", 3))loadimgfromjpg(buf, len, &w, &h, &d, &s);
	else if(0 == ncmp(tmp, "png", 3))loadimgfrompng(buf, len, &w, &h, &d, &s);

	src->tex[idx].w = w;
	src->tex[idx].h = h;
}




void lib4d_create(void* addr)
{
	asset_create();

	content_create(addr);

	overview_init(addr);
}
void lib4d_delete()
{
}
