#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void line(void*,
	int x1,int y1,int x2,int y2, u32 color);
void circlebody(void*,
	int x, int y, int r, u32 color);
void backgroundcolor(void*,
	u32);
//
u32 getrandom();
void say(char*,...);




struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
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
//
static int turn;
static int px,py;
//
static char* data;




static void weiqi_read_html(struct window* win)
{
}
static void weiqi_read_text(struct window* win)
{
	int x,y,j,k,ret,color;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf1);

	//
	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			//position
			ret = y*width + x*2;
			ret <<= 2;

			//color
			if( (px == x) && (py == y) )color = 7;
			else if(data[(y*19) + x] == 'b')color = 4;
			else if(data[(y*19) + x] == 'w')color = 1;
			else continue;

			//
			p[ret] = 0x20;
			p[ret + 3] = color;
			p[ret + 4] = 0x20;
			p[ret + 7] = color;
		}
	}
}
static void weiqi_read_pixel(struct window* win)
{
	u32 color;
	int x,y,half;
	int cx = (win->w)/2;
	int cy = (win->h)/2;

	if(cy > cx)half = cx/20;
	else half = cy/20;

	if( ((win->fmt)&0xffffffff) == 0x61626772)
	{
		color = 0x256f8d;
	}
	else
	{
		color = 0x8d6f25;
	}
	backgroundcolor(win, color);

	//heng
	for(y=-9;y<10;y++)
	{
		line(win,
			cx - half*2*9,	cy + half*2*y,
			cx + half*2*9,	cy + half*2*y,	0);
	}

	//shu
	for(x=-9;x<10;x++)
	{
		line(win,
			cx + half*2*x,	cy - half*2*9,
			cx + half*2*x,	cy + half*2*9,	0);
	}

	//dian
	for(y = cy - half*2*6; y <= cy + half*2*6; y += half*2*6)
	{
		for(x = cx - half*2*6; x <= cx + half*2*6; x += half*2*6)
		{
			circlebody(win,
				x, y,
				half/4, 0
			);
		}
	}

	//zi
	for(y=-9;y<=9;y++)
	{
		for(x=-9;x<=9;x++)
		{
			if(data[(y+9)*19 + x+9] == 'b')color = 0;
			else if(data[(y+9)*19 + x+9] == 'w')color = 0xffffffff;
			else continue;

			circlebody(win,
				cx + half*2*x, cy + half*2*y,
				half, color
			);
		}
	}
}
static void weiqi_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		weiqi_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		weiqi_read_html(win);
	}

	//pixel
	else
	{
		weiqi_read_pixel(win);
	}
}




void weiqi_write(struct event* ev)
{
	char val;
	int x,y;
	u64 what = ev->what;
	u64 key = ev->why;

	if(what == 0x64626b)
	{
		if(key == 0x25)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x26)   //up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x27)   //right
		{
			if(px>=18)return;
			px++;
		}
		else if(key == 0x28)   //down
		{
			if(py>=18)return;
			py++;
		}
	}
	else if(what == 0x72616863)
	{
		if(key == 0x20)
		{
			if((turn&1)==0)data[(py*19)+px] = 'b';
			else data[(py*19)+px] = 'w';
			turn++;
		}
	}
	else if(what == 0x2d70)
	{
		x=key & 0xffff;
		y=(key >> 16) & 0xffff;
		//say("%d,%d\n",x,y);

		x = (x*19)>>16;
		y = (y*19)>>16;
		//say("%d,%d\n",x,y);

		if(x<0)return;
		if(x>18)return;
		if(y<0)return;
		if(y>18)return;

		if((turn&0x1) == 0)data[(y*19) + x] = 'b';
		else data[(y*19) + x] = 'w';
		turn++;
	}
}




static void weiqi_list()
{
}
static void weiqi_choose()
{
}
static void weiqi_start()
{
	int x,y;

	turn=0;
	px=py=0;

	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			data[y*19 + x] = 0;
		}
	}
}
static void weiqi_stop()
{
}
void weiqi_create(void* base, void* addr)
{
	struct player* p = addr;
	data = base+0x300000;

	p->type = 0x656d6167;
	p->name = 0x6971696577;

	p->start = (u64)weiqi_start;
	p->stop = (u64)weiqi_stop;
	p->list = (u64)weiqi_list;
	p->choose = (u64)weiqi_choose;
	p->read = (u64)weiqi_read;
	p->write = (u64)weiqi_write;
}
void weiqi_delete()
{
}
