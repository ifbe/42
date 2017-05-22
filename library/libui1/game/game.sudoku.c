#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void drawdecimal(
	void*, int data, int size,
	int x, int y, u32 fg, u32 bg);
void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
void rect(void*,
	int x1, int y1,
	int x2, int y2,
	u32 body, u32 frame);
void xt100_read(void*);
//
int data2decstr(u64 data,u8* string);
u32 getrandom();
//
int printmemory(char*,int);
int say(char*,...);




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
static int px,py;
static char table[9][9];
//
static char* buffer;




//
int sudoku_possiable(int px, int py, char* p)
{
	int x,y,ret;
	int basex, basey;
	for(x=0;x<10;x++)p[x] = x;

	//heng
	for(x=0;x<9;x++)
	{
		ret = table[py][x];
		if( (ret >= 1) && (ret <= 9) )
		{
			p[ret] = 0;
		}
	}

	//shu
	for(y=0;y<9;y++)
	{
		ret = table[y][px];
		if( (ret >= 1) && (ret <= 9) )
		{
			p[ret] = 0;
		}
	}

	//quan
	basex = px - (px%3);
	basey = py - (py%3);
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ret = table[basey+y][basex+x];
			if( (ret >= 1) && (ret <= 9) )
			{
				p[ret] = 0;
			}
		}
	}

//say("%d,%d:	",px,py);
	//sort
	ret=0;
	for(x=1;x<10;x++)
	{
		if(p[x] == 0)continue;

		p[ret] = p[x];
//say("%d ",p[ret]);

		ret++;
	}
//say("\n");

	p[ret] = 0;
	return ret;
}
int sudoku_random(char* p,int count)
{
	int k,ret;
	if(count == 0)return 0;

	k = getrandom()%count;
	ret = p[k];
	if(count > 1)
	{
		p[k] = p[count-1];
		p[count-1] = 0;
	}

	return ret;
}
void sudoku_solve()
{
	int x,y,t,ret;
	int mode,count,timeout;

/*
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			ret = sudoku_possiable(x,y);
			table[y][x] = sudoku_random(possiable);
		}
	}
*/
	t = 0;		//第几个了
	mode = 0;	//进入还是回头
	timeout = 0;
	while(timeout < 9999)
	{
		y = t/9;
		x = t%9;

		//这是头一次进
		if(mode == 0)
		{
			count = sudoku_possiable(x, y, buffer + 10*t);
		}

		//这一次是回头
		else
		{
			for(count=0;count<10;count++)
			{
				if(buffer[10*t + count] == 0)break;
			}
		}

		//只能回头
		if(count == 0)
		{
			if(t == 0)break;	//失败退出

			table[y][x] = 0;
			t--;
			mode = 1;
		}

		//继续回头
		else if( (count == 1) && (mode != 0) )
		{
			if(t == 0)break;	//失败退出

			table[y][x] = 0;
			t--;
			mode = 1;
		}

		//选一个
		else
		{
			ret = sudoku_random(buffer+10*t, count);
			table[y][x] = ret;

			t++;
			mode = 0;

			if(t >= 81)break;	//成功退出
		}

		timeout++;
	}
}




static void sudoku_read_text(struct window* win)
{
	int x,y,j,k,ret,color;
	int width = win->w;
	int height = win->h;
	char* p = (char*)(win->buf1);

	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(table[y][x] == 0)continue;

			//position
			ret = (3*y+1)*width + 6*x + 2;
			ret <<= 2;

			//color
			if( (px == x)&&(py == y) )color = 1;
			else if( ((x>2)&&(x<6)) && ((y<3)|(y>5)) )color = 2;
			else if( ((y>2)&&(y<6)) && ((x<3)|(x>5)) )color = 2;
			else color = 4;
			for(j=-1;j<=1;j++)
			{
				for(k=-2;k<=3;k++)
				{
					p[ret +(j*width*4) +(k*4) +3] = color;
				}
			}

			//data
			p[ret] = table[y][x] + 0x30;
		}
	}
}
static void sudoku_read_html(struct window* win)
{
}
static void sudoku_read_pixel(struct window* win)
{
	int x,y;
	int w = win->w;
	int h = win->h;
	if(win->dim == 1)
	{
		for(y=0;y<9;y++)
		{
			for(x=0;x<9;x++)
			{
				if(table[y][x] == 0)continue;
				say("%d	",table[y][x]);
			}
			say("\n");
		}
		say("\n");
		xt100_read(win);
		return;
	}

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			rect(win,
				x*w/9,	 y*h/9,
				(x+1)*w/9, (y+1)*h/9,
				0x888888,	  0
			);

			if(table[y][x] != 0)
			{
				drawdecimal(
					win, table[y][x], 4,
					x*w/9, y*h/9, 0, 0
				);
			}
		}
	}
	rectbody(win,
		0, (h/3)-2,
		w, (h/3)+2,
		0
	);
	rectbody(win,
		0, (h*2/3)-2,
		w, (h*2/3)+2,
		0
	);
	rectbody(win,
		(w/3)-2, 0,
		(w/3)+2, h,
		0
	);
	rectbody(win,
		(w*2/3)-2, 0,
		(w*2/3)+2, h,
		0
	);
}
static void sudoku_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		sudoku_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		sudoku_read_html(win);
	}

	//pixel
	else
	{
		sudoku_read_pixel(win);
	}
}
static void sudoku_write(struct event* ev)
{
	u64 what = ev->what;
	u64 key = ev->why;
	if(what == 0x64626b)
	{
		if(key == 0x25)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x26)	//up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x27)	//right
		{
			if(px<0)return;
			if(px>=8)return;
			px++;
		}
		else if(key == 0x28)	//down
		{
			if(py<0)return;
			if(py>=8)return;
			py++;
		}
	}
}








static void sudoku_list()
{
}
static void sudoku_choose()
{
}
static void sudoku_start()
{
	int x,y;
	px = py = 0;

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			table[y][x] = 0;
		}
	}
	sudoku_solve();
}
static void sudoku_stop()
{
}
void sudoku_create(void* base,void* addr)
{
	struct player* p = addr;
	buffer = base+0x300000;

	p->type = 0x656d6167;
	p->name = 0x756b6f647573;

	p->start = (u64)sudoku_start;
	p->stop = (u64)sudoku_stop;
	p->list = (u64)sudoku_list;
	p->choose = (u64)sudoku_choose;
	p->read = (u64)sudoku_read;
	p->write = (u64)sudoku_write;
}
void sudoku_delete()
{
}
