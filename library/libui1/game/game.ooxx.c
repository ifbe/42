#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long 
//
void line(void*,
	int x1,int y1,
	int x2,int y2,
	u32 color);
void circleframe(void*,
	int x, int y,
	int r, u32 color);
void backgroundcolor(void*,
	u32);
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
//
static int turn;
static char data[9];




void ooxx_read(struct window* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	int min = (width<height) ? width:height;

	backgroundcolor(win, 0);
	line(win,
		min/16, min/3,
		min*15/16, min/3,
		0xffffffff);
	line(win,
		min/16, min*2/3,
		min*15/16, min*2/3,
		0xffffffff);
	line(win,
		min/3, min/16,
		min/3, min*15/16,
		0xffffffff);
	line(win,
		min*2/3, min/16,
		min*2/3, min*16/16,
		0xffffffff);

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if(data[3*y + x] == 'o')
			{
				circleframe(win,
					(2*x+1)*min/6,
					(2*y+1)*min/6,
					min/12,
					0xff
				);
			}
			else if(data[3*y + x] == 'x')
			{
				line(win,
					(4*x+1)*min/12, (4*y+1)*min/12,
					(4*x+3)*min/12, (4*y+3)*min/12,
					0xff0000
				);
				line(win,
					(4*x+3)*min/12, (4*y+1)*min/12,
					(4*x+1)*min/12, (4*y+3)*min/12,
					0xff0000
				);
			}
		}//forx
	}//fory
}




void ooxx_write(struct event* ev)
{
	char val;
	int x,y;
/*
	int width = haha->width;
	int height = haha->height;
	int min = (width<height) ? width:height;
*/
	int min = 512;
	if(ev->what == 0x2d6d)
	{
		x=(ev->why) & 0xffff;
		if(x>min)return;

		y=( (ev->why) >> 16 ) & 0xffff;
		if(y>min)return;

		x = x*3/min;
		y = y*3/min;
say("%d,%d\n",x,y);
		if(data[y*3 + x] != 0)return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y*3 + x] = val;
		turn++;
	}
}




static void ooxx_list()
{
}
static void ooxx_choose()
{
}
static void ooxx_start()
{
	int j;

	turn=0;
	for(j=0;j<9;j++)data[j]=0;
}
static void ooxx_stop()
{
}
void ooxx_create(char* base,void* addr)
{
	struct player* p = addr;

	p->type = 0x656d6167;
	p->name = 0x78786f6f;

	p->start = (u64)ooxx_start;
	p->stop = (u64)ooxx_stop;
	p->list = (u64)ooxx_list;
	p->choose = (u64)ooxx_choose;
	p->read = (u64)ooxx_read;
	p->write = (u64)ooxx_write;
}
void ooxx_delete()
{
}
