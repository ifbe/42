#include "libuser.h"
void content_create(void*);
void backuper_create(void*);
void overview_init(void*);
//
void* getsuffix(void* p);
void initutf8(void*);
int openreadclose(void* name, int off, void* buf, int len);
int openwriteclose(void* name, int off, void* buf, int len);
//
void actorcreatefromobj(struct actor* act, u8* buf, int len);
void actorcreatefromstl(struct actor* act, u8* buf, int len);
void actorcreatefromjpg(struct actor* act, u8* buf, int len);
void actorcreatefrompng(struct actor* act, u8* buf, int len);
void actorcreatefrommp3(struct actor* act, u8* buf, int len);
void actorcreatefromogg(struct actor* act, u8* buf, int len);
void actorcreatefromwav(struct actor* act, u8* buf, int len);




static u8* utf8data = 0;
static u16* bgmdata = 0;
static struct fstyle def2d;
static struct fstyle def3d;




void asset_create(void* addr)
{
	int j,k;
	if(0 == utf8data)
	{
		//malloc
		utf8data = memorycreate(0x200000);
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
void actorcreatefromfile(struct actor* act, char* name)
{
	int j,len;
	u8* tmp;
	u8* buf;
	//u8 str[256];
	if(0 == act)return;

	tmp = getsuffix(name);
	if(0 == tmp)return;

	//mysnprintf(str, 256, "%s/%s", "datafile", name);

	buf = act->buf;
	if(0 == buf)buf = memorycreate(0x1000000);

	len = openreadclose(name, 0, buf, 0x1000000);
	if(len <= 0)
	{
		say("len=%d, %s\n", len, name);
		return;
	}

	if(0 == tmp[0])return;
	else if(0 == ncmp(tmp, "jpg", 3))actorcreatefromjpg(act, buf, len);
	else if(0 == ncmp(tmp, "png", 3))actorcreatefrompng(act, buf, len);
	else if(0 == ncmp(tmp, "obj", 3))actorcreatefromobj(act, buf, len);
	else if(0 == ncmp(tmp, "stl", 3))actorcreatefromstl(act, buf, len);
	else if(0 == ncmp(tmp, "mp3", 3))actorcreatefrommp3(act, buf, len);
	else if(0 == ncmp(tmp, "ogg", 3))actorcreatefromogg(act, buf, len);
	else if(0 == ncmp(tmp, "wav", 3))actorcreatefromwav(act, buf, len);
}
void* alloc_winobj(struct arena* win, int type)
{
	int j;
	struct datapair* mod;

	if('s' == type)
	{
		mod = win->gl_solid;

		for(j=solidaid_max;j<128;j++)
		{
			if(mod[j].src.vs)continue;
			if(mod[j].src.vbuf)continue;
			return &mod[j];
		}
	}

	if('o' == type)
	{
		mod = win->gl_opaque;

		for(j=opaqueaid_max;j<128;j++)
		{
			if(mod[j].src.vs)continue;
			if(mod[j].src.vbuf)continue;
			return &mod[j];
		}
	}
	return 0;
}




void* defaultstyle_vbo2d()
{
	return &def2d;
}
void createstyle_vbo2d()
{
	def2d.vc[0] = 0.0;
	def2d.vc[1] = 0.0;
	def2d.vc[2] = 0.0;

	def2d.vr[0] = 1.0;
	def2d.vr[1] = 0.0;
	def2d.vr[2] = 0.0;

	def2d.vf[0] = 0.0;
	def2d.vf[1] = 1.0;
	def2d.vf[2] = 0.0;

	def2d.vt[0] = 0.0;
	def2d.vt[1] = 0.0;
	def2d.vt[2] = 1.0;
}




void* defaultstyle_vbo3d()
{
	return &def3d;
}
void createstyle_vbo3d()
{
	def3d.vc[0] = 0.0;
	def3d.vc[1] = 0.0;
	def3d.vc[2] = 0.0;

	def3d.vr[0] = 512.0;
	def3d.vr[1] = 0.0;
	def3d.vr[2] = 0.0;

	def3d.vf[0] = 0.0;
	def3d.vf[1] = 512.0;
	def3d.vf[2] = 0.0;

	def3d.vt[0] = 0.0;
	def3d.vt[1] = 0.0;
	def3d.vt[2] = 512.0;
}




void lib4d_create(void* addr)
{
	createstyle_vbo2d();
	createstyle_vbo3d();

	asset_create(addr);

	content_create(addr);

	backuper_create(addr);

	overview_init(addr);
}
void lib4d_delete()
{
}
