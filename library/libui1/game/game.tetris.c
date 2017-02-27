#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void rect(void*,
	int x1, int y1,
	int x2, int y2,
	u32 bc, u32 fc);
//
u32 getrandom();
int diary(char*,int,char*,...);
void printmemory(char*,int);
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
typedef struct stucture
{
	int x;
	int y;
	int type;
	int direction;
	int x1;
	int y1;
	int x2;
	int y2;
	int x3;
	int y3;
	int x4;
	int y4;
}structure;
static structure that;
//
static int score=0;
static unsigned char* table;




static void cubie(struct window* win, int x,int y,int z)
{
	u32 bodycolor;
	int startx,starty,endx,endy;
	int aa = (win->h)*32/40;

	if(win->w < aa)
	{
		startx = (win->w)*x/32;
		endx = (x+1)*(win->w)/32 -1;
	}
	else
	{
		startx = (win->w-aa)/2 + (x*aa/32);
		endx = (win->w-aa)/2 + ((x+1)*aa/32) -1;
	}

	starty = (win->h)*y/40;
	endy = (y+1)*(win->h)/40 - 1;

	bodycolor=z>0?0xffffffff:0;
	rect(win,
		startx, starty,
		endx, endy,
		bodycolor, 0x44444444
	);
}
static void tetris_read_pixel(struct window* win)
{
	int x,y;
	for(y=0;y<40;y++)
	{
		for(x=0;x<32;x++)
		{
			//say("%d ",table[y*32+x]);
			cubie(win, x,y,table[y*32+x]);
		}
		//say("\n");
	}
	//say("\n");

	//print cubies
	cubie(win, that.x1, that.y1, 1);
	cubie(win, that.x2, that.y2, 1);
	cubie(win, that.x3, that.y3, 1);
	cubie(win, that.x4, that.y4, 1);

	//print score
	//decimal(10,10,score);
}




static int htmlcubie(char* p, int x, int y)
{
	return diary(
		p, 0x1000,
		"<div class=\"rect\" style=\""
		"left:%.2f%;"
		"top:%.2f%;"
		"\">%d</div>",
		x*3.1, y*2.5, table[y*32+x]
	);
}
static void tetris_read_html(struct window* win)
{
	int x,y;
	char* p = (char*)(win->buf);

	*(u32*)p = 0x6c6d7468;
	p += 0x1000;

	p += diary(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"background:#fff;"
		"position:absolute;"
		"width:3.1%;"
		"height:2.5%;"
		"}"
		"</style>"
	);
	for(y=0;y<40;y++)
	{
		for(x=0;x<32;x++)
		{
			if(table[y*32+x] == 0)continue;
			p += htmlcubie(p, x, y);
		}
	}

	p += htmlcubie(p, that.x1, that.y1);
	p += htmlcubie(p, that.x2, that.y2);
	p += htmlcubie(p, that.x3, that.y3);
	p += htmlcubie(p, that.x4, that.y4);
}




static void tetris_read_text(struct window* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	char* p = (char*)(win->buf);

	for(x=0;x<width*height*4;x++)p[x]=0;
	if(height>=40)
	{
		for(y=0;y<40;y++)
		{
			for(x=0;x<32;x++)
			{
				if(table[y*32+x])
				{
					p[(y*width+x)<<2]='#';
				}
			}
		}
		p[(that.x1 + that.y1*width)<<2]='#';
		p[(that.x2 + that.y2*width)<<2]='#';
		p[(that.x3 + that.y3*width)<<2]='#';
		p[(that.x4 + that.y4*width)<<2]='#';
	}
	else
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<32;x++)
			{
				if(table[32*(y+40-height) + x])
				{
					p[(y*width+x)<<2]='#';
				}
			}
		}
		p[(that.x1 + (that.y1-40+height)*width)<<2]='#';
		p[(that.x2 + (that.y2-40+height)*width)<<2]='#';
		p[(that.x3 + (that.y3-40+height)*width)<<2]='#';
		p[(that.x4 + (that.y4-40+height)*width)<<2]='#';
	}
}
static void tetris_read(struct window* win)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		tetris_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		tetris_read_html(win);
	}

	//pixel
	else
	{
		tetris_read_pixel(win);
	}
}






static void generate()
{
	if(that.type==0)
	{
		//	0000
		if(that.direction==1 | that.direction==3)
		{
			that.x1=that.x;
			that.x2=that.x +1;
			that.x3=that.x +2;
			that.x4=that.x +3;
			that.y1=that.y2=that.y3=that.y4=that.y;
		}

		//	0
		//	0
		//	0
		//	0
		if(that.direction==0 | that.direction==2)
		{
			that.x1=that.x2=that.x3=that.x4=that.x;
			that.y1=that.y;
			that.y2=that.y +1;
			that.y3=that.y +2;
			that.y4=that.y +3;
		}
	}
	if(that.type==1)
	{
		//	000
		//	 0
		if(that.direction==0)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x;
			that.y4=that.y +1;
		}

		//	0
		//	00
		//	0
		if(that.direction==1)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y;
		}

		//	 0
		//	000
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x;
			that.y4=that.y -1;
		}

		//	 0
		//	00
		//	 0
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x;
			that.y4=that.y -1;
		}
	}
	if(that.type==2)
	{
		//	0
		//	00
		//	 0
		if(that.direction==0|that.direction==2)
		{	that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x -1;
			that.y3=that.y -1;
			that.x4=that.x;
			that.y4=that.y+1;
		}

		//	 00
		//	00
		if(that.direction==1|that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}
	}
	if(that.type==3)
	{
		//	 0
		//	00
		//	0
		if(that.direction==0|that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}

		//	00
		//	 00
		if(that.direction==1|that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x +1;
			that.y2=that.y;
			that.x3=that.x;
			that.y3=that.y -1;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}
	}
	if(that.type==4)
	{
		//	00
		//	0
		//	0
		if(that.direction==0)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}

		//	0
		//	000
		if(that.direction==1)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}

		//	 0
		//	 0
		//	00
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}

		//	000
		//	  0
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x +1;
			that.y4=that.y +1;
		}
	}
	if(that.type==5)
	{
		//	00
		//	 0
		//	 0
		if(that.direction==0)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x -1;
			that.y4=that.y -1;
		}

		//	000
		//	0
		if(that.direction==1)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x -1;
			that.y4=that.y +1;
		}

		//	0
		//	0
		//	00
		if(that.direction==2)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x;
			that.y2=that.y -1;
			that.x3=that.x;
			that.y3=that.y +1;
			that.x4=that.x +1;
			that.y4=that.y +1;
		}

		//	  0
		//	000
		if(that.direction==3)
		{
			that.x1=that.x;
			that.y1=that.y;
			that.x2=that.x -1;
			that.y2=that.y;
			that.x3=that.x +1;
			that.y3=that.y;
			that.x4=that.x +1;
			that.y4=that.y -1;
		}
	}

	//	00
	//	00
	if(that.type==6)
	{
		that.x1=that.x;
		that.y1=that.y;
		that.x2=that.x +1;
		that.y2=that.y;
		that.x3=that.x;
		that.y3=that.y +1;
		that.x4=that.x +1;
		that.y4=that.y +1;
	}
}




static int check()
{
	unsigned int temp;

	//left
	if(that.x1<0)return 1;
	if(that.x2<0)return 2;
	if(that.x3<0)return 3;
	if(that.x4<0)return 4;

	//right
	if(that.x1>31)return 5;
	if(that.x2>31)return 6;
	if(that.x3>31)return 7;
	if(that.x4>31)return 8;

	//collision
	if(table[32 * that.y1 + that.x1] != 0)return 9;
	if(table[32 * that.y2 + that.x2] != 0)return 10;
	if(table[32 * that.y3 + that.x3] != 0)return 11;
	if(table[32 * that.y4 + that.x4] != 0)return 12;

	//success
	return 0;
}


static void left()
{
	if(that.x1>0&&that.x2>0&&that.x3>0&&that.x4>0)
	{
		that.x --;
		that.x1 --;
		that.x2 --;
		that.x3 --;
		that.x4 --;
		if(check() != 0)
		{
			that.x ++;
			that.x1 ++;
			that.x2 ++;
			that.x3 ++;
			that.x4 ++;
		}
	}
}
static void right()
{
	if(that.x1<31&&that.x2<31&&that.x3<31&&that.x4<31)
	{
		that.x ++;
		that.x1 ++;
		that.x2 ++;
		that.x3 ++;
		that.x4 ++;
		if(check() != 0)
		{
			that.x --;
			that.x1 --;
			that.x2 --;
			that.x3 --;
			that.x4 --;
		}
	}
}
static void up()
{
	that.direction=(that.direction +1)%4;
	generate();
	if(check() != 0)
	{
		that.direction=(that.direction+3)%4;
	}
	generate();
}
static int down()
{
	int x,y,count;

	that.y ++;
	that.y1 ++;
	that.y2 ++;
	that.y3 ++;
	that.y4 ++;
	if(check() < 9)return 0;

	that.y --;
	that.y1 --;
	that.y2 --;
	that.y3 --;
	that.y4 --;

	table[32 * that.y1 + that.x1]=1;
	table[32 * that.y2 + that.x2]=1;
	table[32 * that.y3 + that.x3]=1;
	table[32 * that.y4 + that.x4]=1;

	y=39;
	while(1)
	{
		count=0;
		for(x=0;x<32;x++)
		{
			if(table[32*y+x]>0)count++;
		}

		if(count==32)
		{
			//
			for(x=y*32+31;x>32;x--)
			{
				table[x]=table[x-32];
			}
		}

		else
		{
			y--;
			if(y==0)break;
		}
	}

	//
	that.x=getrandom() %27+1;
	that.y=1;
	that.type=getrandom() % 7;
	that.direction=getrandom() & 0x3;
	generate();
	return 1;
}
static void tetris_write(struct event* ev)
{
	int ret;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 0x2d6d)
	{
		for(ret=0;ret<20;ret++)if(down()==1)return;
	}
	else if(type == 0x64626b)
	{
		if(key==0x25)left();
		else if(key==0x27)right();
		else if(key==0x26)up();
		else if(key==0x28)down();
	}
	else if(type == 0x72616863)
	{
		if(key=='a')left();
		else if(key=='d')right();
		else if(key=='w')up();
		else if(key=='s')down();
		else if(key==' ')
		{
			for(ret=0;ret<20;ret++)if(down()==1)return;
		}
	}
}




static void tetris_list()
{
}
static void tetris_choose()
{
}
static void tetris_start()
{
	int x;
	for(x= 0*32;x<40*32;x++) table[x]=0;
	for(x=40*32;x<41*32;x++) table[x]=1;

	that.x=getrandom() %27 +1;
	that.y=1;
	that.type=5;
	that.direction=2;
	generate();
}
static void tetris_stop()
{
}
void tetris_create(void* base,void* addr)
{
	struct player* p = addr;
	table=(u8*)(addr+0x300000);

	p->type = 0x656d6167;
	p->name = 0x736972746574;

	p->start = (u64)tetris_start;
	p->stop = (u64)tetris_stop;
	p->list = (u64)tetris_list;
	p->choose = (u64)tetris_choose;
	p->read = (u64)tetris_read;
	p->write = (u64)tetris_write;
}
void tetris_delete()
{
}
