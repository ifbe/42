#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void drawascii(
	void*, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);
void rect(void*,
	int x0, int y0,
	int x1, int y1,
	u32 bc, u32 fc);
//
u32 getrandom();
void say(char*,...);




struct player
{
	u64 type;
	u64 name;
	u64 start;
	u64 stop;
	u64 list;
	u64 choose;
	u64 read;
	u64 write;

	u8 data[0xc0];
};
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
	
	u8 data[0xc0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};

//max=32768
static int arealeft = 16384;
static int areatop = 32768;
static int arearight = 49152;
static int areabottom = 49152;
static char table[8][8] = {
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z',' ',' ',' ',' ',0x8,0xd,
	'0','1','2','3','4','5','6','7',
	'8','9',' ',' ',' ',' ',' ',' ',
	'+','-','*','/',' ',' ',' ',' ',
	'=',' ',' ',' ',' ',' ',' ',' '
};




static void virtkbd_read(struct window* win)
{
	int x,y;
	int left,top,right,bottom;
	int width = win->w;
	int height = win->h;

	//[a,z]
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			left = (arealeft + x*(arearight-arealeft)/8)*width/65536;
			top = (areatop + y*(areabottom-areatop)/8)*height/65536;
			right = (arealeft + (x+1)*(arearight-arealeft)/8)*width/65536;
			bottom = (areatop + (y+1)*(areabottom-areatop)/8)*height/65536;
			//say("====%d,%d,%d,%d\n",left,top,right,bottom);

			rect(win,
				left, top,
				right, bottom,
				0xff00ff, 0
			);

			drawascii(
				win, table[y][x], 1,
				left, top, 0xffffffff, 0
			);
		}
	}
}
static int virtkbd_write(struct event* ev)
{
	int x,y;
	say("%x,%x\n",x,y);

	if(ev->what == 0x2d70)
	{
		x = (ev->why)&0xffff;
		if(x < arealeft)return 1;
		if(x > arearight)return 1;

		y = ((ev->why)>>16)&0xffff;
		if(y < areatop)return 1;
		if(y > areabottom)return 1;

		x = (x-arealeft)*8/32768;
		y = (y-areatop)*8/16384;

		ev->what = 0x72616863;
		ev->why = table[y][x];
	}

	return 1;
}




static void virtkbd_into()
{
}
static void virtkbd_list()
{
}
static void virtkbd_start()
{
}
static void virtkbd_stop()
{
}
void virtkbd_create(void* base,void* addr)
{
	struct player* p = addr;

	p->type = 0;
	p->name = 0x64626b74726976;
	p->start = (u64)virtkbd_start;
	p->stop = (u64)virtkbd_stop;
	p->list = (u64)virtkbd_list;
	p->choose = (u64)virtkbd_into;
	p->read = (u64)virtkbd_read;
	p->write = (u64)virtkbd_write;
}
void virtkbd_delete()
{
}
