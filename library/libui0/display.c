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




int displayread()
{
	return 0;
}
int displaywrite()
{
	//if(local) {
		windowwrite( &(sc[id]) );
	//}

	//if(net) {
		//netdsp_write();
	//}
	return 0;
}
int displaylist()
{
	return 0;
}
int displaychoose()
{
	return 0;
}
int displaystart(void* buf, void* fmt, int w, int h)
{
	id = 0;
	sc[id].fmt = 0x6267726138383838;	//"bgra8888"
	sc[id].w = 512;
	sc[id].h = 512;
	windowstart( &(sc[id]) );

	return id;
}
int displaystop()
{
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
