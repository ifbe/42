#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//local
int windowcreate();
int windowdelete();
int windowstart(void*);
int windowstop();
int windowlist();
int windowchoose();
int windowread();
int windowwrite(void*);
//
int ncmp(void*, void*, int);
int cmp(void*, void*);
void printmemory(void*, int);
void say(void*, ...);




//
struct window
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;
	char padding[256 - sizeof(u64)*4];
};
static struct window* sc;
static int id = 0;




u64 displayread(struct window* win)
{
	return 0;
}
u64 displaywrite(struct window* win)
{
	if(win->fmt == 0x6c6d7468)
	{
	}
	else
	{
		windowwrite(win);
	}
	return 0;
}
u64 displaylist(u64 dispid, u64 property)
{
	int j;
	if(ncmp("buffer", &property, 8) == 0)return sc[dispid].buf;
	else if(ncmp("format", &property, 8) == 0)return sc[dispid].fmt;
	else if(ncmp("width" , &property, 8) == 0)return sc[dispid].w;
	else if(ncmp("height", &property, 8) == 0)return sc[dispid].h;

	//if(property==unknown)
	for(j=0;j<10;j++)
	{
		if(sc[j].fmt == 0)break;

		say(
		"%llx,%llx,%llx,%llx\n",
		sc[j].buf,
		sc[j].fmt,
		sc[j].w,
		sc[j].h
		);
	}
	return 0;
}
u64 displaychoose(u64 dispid, u64 property, u64 what)
{
	if(ncmp("buffer", &property, 8) == 0)sc[dispid].buf = what;
	else if(ncmp("format", &property, 8) == 0)sc[dispid].fmt = what;
	else if(ncmp("width" , &property, 8) == 0)sc[dispid].w = what;
	else if(ncmp("height", &property, 8) == 0)sc[dispid].h = what;
	else what = 0;	//fail

	return what;
}
u64 displaystart(u64 buf, u64 fmt, u64 w, u64 h)
{
	if(fmt == 0x6c6d7468)
	{
	}
	else
	{
		sc[0].fmt = fmt;
		sc[0].w = w;
		sc[0].h = h;
		windowstart( &(sc[0]) );
	}
	return id;
}
u64 displaystop()
{
	return 0;
}
void displaycreate(u8* type, u8* addr)
{
	int j;
	if(type!=0)return;

	//table where
	sc = (void*)addr;
	for(j=0;j<0x100000;j++)addr[j] = 0;

	//local prepare
	windowcreate();
	say("[c,f):createed display\n");
}
void displaydelete()
{
	//
	say("[c,f):deleteing display\n");

	//1024*1024*4
	windowdelete();
}
