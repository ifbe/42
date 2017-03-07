#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void printdecimal(void*, 
	int x, int y, int size, int data, u32 fg, u32 bg);
void rect(void*,
	int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolr);
//
int data2decstr(u64 data,u8* string);
u32 getrandom();
//
int printmemory(void*, int);
int fmt(void*, int, void*, ...);
int say(void*, ...);




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
	u64 buf;
	u64 fmt;
	u64 w;
	u64 h;

	u8 data[0xe0];
};
struct event
{
	u64 why;
	u64 what;
	u64 where;
	u64 when;
};
//
static int num;
static void* buffer;




u32 the2048_color(int val)
{
	switch(val)
	{
		case    0:	return 0x555555;
		case    2:	return 0xfffff0;
		case    4:	return 0xffffc0;
		case    8:	return 0x995000;
		case   16:	return 0xc05000;
		case   32:	return 0xb03000;
		case   64:	return 0xff0000;
		case  128:	return 0xffffa0;
		case  256:	return 0xffff80;
		case  512:	return 0xffff00;
		case 1024:	return 0xffffb0;
		case 2048:	return 0x233333;
		case 4096:	return 0x783d72;
		case 8192:	return 0xd73762;
	}
	return 0;
}
static void cubie(struct window* win, int x,int y,int z)
{
	int min;
	int color;
	int count;
	if(win->w < win->h)min = win->w;
	else min = win->h;

	//
	if(z<16)count=0;
	else if(z<128)count=1;
	else if(z<1024)count=2;
	else if(z<16384)count=3;
	else count=4;

	//
	color = the2048_color(z);
	if( ( (win->fmt)&0xffffffff) == 0x61626772)	//bgra->rgba
	{
		color	= 0xff000000
			+ ((color&0xff)<<16)
			+ (color&0xff00)
			+ ((color&0xff0000)>>16);
	}

	rect(
		win,
		x*min/4,
		y*min/4,
		((x+1)*min/4) - 1,
		((y+1)*min/4) - 1,
		color,
		0
	);

	if(z==0)return;
	printdecimal(win,
		x*(min/4)+min/10-count*16,
		y*(min/4)+min/20,
		4,
		z,
		0,
		0
	);
}
static void the2048_read_pixel(struct window* win)
{
	int x,y;
	int (*table)[4] = buffer + num*16*4;

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			cubie(win, x, y, table[y][x]);
		}
	}
}
static void the2048_read_text(struct window* win)
{
	int x,y,j,k,ret;
	u8 src[10];

	int w = win->w;
	int h = win->h;
	u8* p = (u8*)(win->buf);
	int (*table)[4] = buffer + num*16*4;

	for(x=0;x<w*h*4;x++)p[x]=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			data2decstr(table[y][x], src);
			ret = w*(4*y+1) + 8*x + 2;
			ret <<= 2;

			//color
			for(j=-1;j<=1;j++)
			{
				for(k=-2;k<=3;k++)
				{
					p[ret + (j*w*4) +(k*4) +3] = 4;
				}
			}


			//number
			j=k=0;
			for(j=0;j<10;j++)
			{
				if(src[j] == 0)break;

				p[ret + k] = src[j];
				k += 4;
			}
		}
	}
}
static void the2048_read_html(struct window* win)
{
	int x,y;
	u32 color;
	u8* p = (u8*)(win->buf);
	int (*table)[4] = buffer + num*16*4;

	*(u32*)p = 0x6c6d7468;
	p += 0x1000;

	p += fmt(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"color:#000;"
		"width:25%;"
		"height:25%;"
		"}"
		"</style>"
	);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x] == 0)continue;

			color = the2048_color(table[y][x]);
			p += fmt(
				p, 0x1000,
				"<div class=\"rect\" style=\""
				"left:%d%;"
				"top:%d%;"
				"background:#%06x;"
				"\">%d</div>",
				25*x, 25*y,
				color, table[y][x]
			);
		}
	}
}
static void the2048_read(struct window* win)
{
	u64 fmt = win->fmt;
	//say("(@2048.read)fmt=%x\n",temp);

	//text
	if(fmt == 0x74786574)
	{
		the2048_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		the2048_read_html(win);
	}

	//pixel
	else
	{
		the2048_read_pixel(win);
	}
}




//斗兽场......
static void left2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	//4对祭品
	for(y=0;y<4;y++)
	{
		//前三个会去作死
		for(x=0;x<3;x++)
		{
		//前三个里面没死的才会去作死
		if(table[y][x]!=0)
		{
			//开始作死
			dst=x+1;
			for(;dst<4;dst++)
			{
				//空位置就找下一个
				if(table[y][dst] == 0)continue;

				//跟自己不一样，没法欺负就跑
				if(table[y][dst] != table[y][x])break;

				//找到相同的只能活一个，依次排队准备下次
				table[y][x]=2*table[y][x];	//吃掉别人
				table[y][dst]=0;		//扔尸体
				x=dst;		//别
				break;
			}
		}//if
		}//forx

		//活着的排好队
		dst=0;
		for(x=0;x<4;x++)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[y][dst]=temp;
			dst++;
		}
		}
	}//fory
}
static void right2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	for(y=0;y<4;y++)
	{
		for(x=3;x>0;x--)
		{
		if(table[y][x]!=0)
		{
			dst=x-1;
			for(;dst>=0;dst--)
			{
				if( table[y][dst] == 0 )continue;
				if( table[y][dst] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[y][dst]=0;
				x=dst;
				break;
			}
		}//if
		}//forx

		dst=3;
		for(x=3;x>=0;x--)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[y][dst]=temp;
			dst--;
		}
		}
	}//fory
}
static void up2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	for(x=0;x<4;x++)
	{
		for(y=0;y<3;y++)
		{
		if(table[y][x]!=0)
		{
			dst=y+1;
			for(;dst<4;dst++)
			{
				if( table[dst][x] == 0 )continue;
				if( table[dst][x] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[dst][x]=0;
				y=dst;
				break;
			}
		}//if
		}//fory

		dst=0;
		for(y=0;y<4;y++)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[dst][x]=temp;
			dst++;
		}
		}
	}//forx
}
static void down2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	for(x=0;x<4;x++)
	{
		for(y=3;y>0;y--)
		{
		if(table[y][x]!=0)
		{
			dst=y-1;
			for(;dst>=0;dst--)
			{
				if( table[dst][x] == 0 )continue;
				if( table[dst][x] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[dst][x]=0;
				y=dst;
				break;
			}
		}//if
		}//forx

		dst=3;
		for(y=3;y>=0;y--)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[dst][x]=temp;
			dst--;
		}
		}
	}//fory
}
static void new2048()
{
	int x,y;
	int who,temp;
	int (*table)[4] = buffer + num*16*4;

	//how many blank cubie
	who=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x]==0)who++;
		}
	}
	if(who==0)return;

	//where
	temp = getrandom();
	if(temp<0) temp = -temp;
	who = temp % who;

	//value
	temp = getrandom()&0x3;
	if(temp>=2)temp=4;
	else temp=2;

	//put
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			//here=0 : i "can" put here
			if(table[y][x]==0)
			{
				//who=0 : i "want" put here
				if(who!=0)who--;
				else
				{
					table[y][x]=temp;
					return;
				}
			}//if(empty)
		}//for(x)
	}//for(y)
}
static void the2048_write(struct event* ev)
{
	//kbd
	u8 ch;
	int j;
	int* p;
	int* q;

	//
	if(ev->what == 0x64626b)
	{
		ch = (ev->why)&0xff;
		if( (ch>=0x25) && (ch<=0x28) )
		{
			//
			p = buffer + 64*num;
			num = (num+1)%0x1000;
			q = buffer + 64*num;
			for(j=0;j<16;j++)q[j] = p[j];

			//
			if(ch == 0x25)left2048();
			else if(ch == 0x26)up2048();
			else if(ch == 0x27)right2048();
			else if(ch == 0x28)down2048();

			//new number?
			new2048();
		}
	}
	else if(ev->what == 0x72616863)
	{
		if(ch == 0x8)
		{
			if(num>0)num--;
		}
	}
}








static void the2048_list()
{
}
static void the2048_choose()
{
}
static void the2048_start()
{
	int j;
	u8* p = buffer;
	for(j=0;j<0x4000;j++)p[j] = 0;

	//
	num = 0;
	new2048();
}
static void the2048_stop()
{
}
void the2048_create(void* base, void* addr)
{
	struct player* p = addr;
	buffer = base + 0x300000;

	p->type = 0x656d6167;
	p->name = 0x38343032;
	p->start = (u64)the2048_start;
	p->stop = (u64)the2048_stop;
	p->list = (u64)the2048_list;
	p->choose = (u64)the2048_choose;
	p->read = (u64)the2048_read;
	p->write = (u64)the2048_write;
}
void the2048_delete()
{
}
