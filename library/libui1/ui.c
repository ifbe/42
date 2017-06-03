#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void lib1d_create(void*);
void lib1d_delete();
void lib2d_create(void*);
void lib2d_delete();
void lib3d_create(void*);
void lib3d_delete();
//
void external_create(void*);
void external_delete();
int external_start();
int external_stop();
int external_list(void*);
int external_choose(void*);
int external_read(void*);
int external_write(void*);
//
void content_create(void*);
void content_delete();
int content_start();
int content_stop();
int content_list(void*);
int content_choose(void*);
int content_read(void*);
int content_write(void*);
//
void say(void*, ...);




struct window
{
	u64 buf1;
	u64 buf2;
	u64 fmt;
	u64 dim;

	u64 w;
	u64 h;
	u64 d;
	u64 t;

	char data[0xc0];
};
static struct window* win = 0;




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void charactercreate(u8* type, u8* addr)
{
	if(type != 0)return;
	if( (type == 0)&&(win != 0) )return;

	//where
	win = (struct window*)(addr+0);

	//lib1d
	lib1d_create(addr);

	//lib2d
	lib2d_create(addr);

	//lib3d
	lib3d_create(addr);

	//special
	external_create(addr);

	//ordinary
	content_create(addr);
}
void characterdelete()
{
	//say("[c,f):deleteing character\n");

	content_delete();
	external_delete();

	lib1d_delete();
	lib2d_delete();
	lib3d_delete();

	win = 0;
}
int characterstart(int j)
{
	content_start();
	return 0;
}
int characterstop()
{
	content_stop();
	return 0;
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int characterlist(u8* p)
{
}
int characterchoose(u8* p)
{
	return 0;
}
void characterread()
{
	int j;
	for(j=0;j<16;j++)
	{
		//error
		if(win[j].fmt == 0)break;

		//voice
		if(win[j].fmt == 0x6563696f76)continue;

		//tray
		//if(win[j].fmt == 0x)external_read();

		//else
		content_read(&win[j]);		
	}
}
void characterwrite(u64* ev)
{
	int ret;
	if(ev[1] == 0x727473)
	{
		ret = content_choose( (void*)(ev[0]) );
		if(ret == 0)say("%s\n", ev[0]);
		return;
	}

	//ext
	//ev[2] = (u64)(&win[2]);
	//external_write(ev);

	//ctx
	ev[2] = (u64)(&win[2]);
	content_write(ev);
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~