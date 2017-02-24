#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
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
struct screen
{
        u64 buf;
        u64 fmt;
        u64 w;
        u64 h;
	char padding[256 - sizeof(u64)*4];
};
static struct screen* sc;
static int id = 0;




u64 displayread()
{
	return 0;
}
u64 displaywrite()
{
	//if(local) {
		windowwrite( &(sc[id]) );
	//}

	//if(net) {
		//netdsp_write();
	//}
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
u64 displaystart(void* buf, void* fmt, int w, int h)
{
	id = 0;
	sc[id].fmt = 0x6267726138383838;	//"bgra8888"
	sc[id].w = 512;
	sc[id].h = 512;
	windowstart( &(sc[id]) );

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
