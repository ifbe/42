#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int websocket_write(u64, u8*, int);
int rdp_write(u64, u8*, int);
int vnc_write(u64, u8*, int);
int count_strlen(void*);
//
int readsocket(int, void*, int, int);
int writesocket(int, void*, int, int);
//
void printmemory(void*, int);
void say(void*, ...);




struct window
{
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;
};
static struct window* win=0;
static int theone=0;




int netwinread()
{
	return 0;
}
int netwinwrite(struct window* win)
{
	void* p;
	int j;
	if(theone == 0)return 0;

	p = (void*)win->buf;
	j = count_strlen(p);
	j = websocket_write(theone, p, j);
	if(j <= 0)theone = 0;

	return 0;
}
int netwinlist()
{
	return 0;
}
int netwinchoose(int j)
{
	return 0;
}
int netwinstop()
{
	return 0;
}
int netwinstart(int j)
{
	theone = j;
	return 0;
}
int netwindelete()
{
	return 0;
}
int netwincreate(void* uibase, void* uithis)
{
	win = uithis;
	win->buf = (u64)malloc(0x100000);
	win->fmt = 0x6c6d7468;
	win->w = 512;
	win->h = 512;
	return 0;
}
