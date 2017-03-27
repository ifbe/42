#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//voice
int voicecreate(void*, void*);
int voicedelete();
int voicestart(int);
int voicestop();
int voicelist();
int voicechoose();
int voiceread();
int voicewrite(void*);
//network
int netwincreate(void*, void*);
int netwindelete();
int netwinstart(int);
int netwinstop();
int netwinlist();
int netwinchoose();
int netwinread();
int netwinwrite(void*);
//local
int windowcreate(void*, void*);
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
static struct window* ui;
static int id = 0;




u64 displayread()
{
	return 0;
}
u64 displaywrite()
{
	int j;
	for(j=0;j<16;j++)
	{
		if(ui[j].fmt == 0)break;
		if(ui[j].fmt == 0x696c63)continue;
		if(ui[j].fmt == 0x6563696f76)continue;

		if(ui[j].fmt == 0x6c6d7468)netwinwrite(&ui[j]);
		else windowwrite(&ui[j]);
	}
	return 0;
}
u64 displaylist(u64 dispid, u64 property)
{
	int j;
	if(ncmp("buffer", &property, 8) == 0)return ui[dispid].buf;
	else if(ncmp("format", &property, 8) == 0)return ui[dispid].fmt;
	else if(ncmp("width" , &property, 8) == 0)return ui[dispid].w;
	else if(ncmp("height", &property, 8) == 0)return ui[dispid].h;

	//if(property==unknown)
	for(j=0;j<10;j++)
	{
		if(ui[j].fmt == 0)break;

		say(
		"%llx,%llx,%llx,%llx\n",
		ui[j].buf,
		ui[j].fmt,
		ui[j].w,
		ui[j].h
		);
	}
	return 0;
}
u64 displaychoose(u64 dispid, u64 property, u64 what)
{
	if(ncmp("buffer", &property, 8) == 0)ui[dispid].buf = what;
	else if(ncmp("format", &property, 8) == 0)ui[dispid].fmt = what;
	else if(ncmp("width" , &property, 8) == 0)ui[dispid].w = what;
	else if(ncmp("height", &property, 8) == 0)ui[dispid].h = what;
	else what = 0;	//fail

	return what;
}
int displaystart(int type, int j)
{
	if(type == 0)
	{
		ui[id].buf = 0;
		ui[id].fmt = 0x6267726138383838;
		ui[id].w = 512;
		ui[id].h = 512;
		windowstart( &(ui[id]) );
		return id;
	}
	else if(type == 0x5357)
	{
		netwinstart(j);
	}
	return 0;
}
int displaystop()
{
	return 0;
}
void displaycreate(u8* type, u8* addr)
{
	int j;
	if(type!=0)return;

	//table where
	ui = (void*)addr;
	for(j=0;j<0x100000;j++)addr[j] = 0;

	//ws,rdp,vnc...
	netwincreate(addr, addr);

	//voice
	voicecreate(addr, addr+0x100);

	//local prepare
	windowcreate(addr, addr+0x200);
	id = 2;

	//
	//say("[c,f):createed display\n");
}
void displaydelete()
{
	//
	//say("[c,f):deleteing display\n");

	//1024*1024*4
	windowdelete();

	//
	netwindelete();
}
