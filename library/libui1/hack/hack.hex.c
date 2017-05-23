#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void xt100_read(void*);
void background1(void*);
void drawascii(
	void*, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);
void drawbyte(
	void*, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);
void rectbody(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
//
int data2hexstr(u64, u8*);
int cmp(void*, void*);
//
int fmt(void*, int, void*, ...);
void printmemory(void*, int);
void say(void*, ...);




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
//flostarea
static int inputcount = 0;
static u8 hi[0x100];
	//[0,0x1f]:target,value
	//[0x20,0x3f]:base,value
	//[0x40,0x5f]:offset,value
	//[0x60,0x7f]:data,value

//where
static u8* databuf=0;
static u64 windowoffset;
static u64 pointeroffset;





static int printmethod=0;
static int xshift=0;
static int byteperline=0;
static int lineperwindow=0;
static void foreground(struct window* win)
{
	//一整页
	int x,y;
	int height = win->h;

	if(printmethod==0)			//hex
	{
		for(y=0;y<height/16;y++)
		{
			for(x=0;x<byteperline;x++)
			{
				drawbyte(
					win, databuf[windowoffset + y*byteperline + x], 1,
					16*x + xshift, 16*y, 0, 0
				);
			}
		}
	}

	else if(printmethod==1)		//ascii
	{
		for(y=0;y<height/16;y++)
		{
			for(x=0;x<byteperline;x++)
			{
				drawascii(
					win, databuf[windowoffset + y*byteperline + x], 1,
					16*x + xshift, 16*y, 0, 0
				);
			}
		}
	}

	else if(printmethod == 2)	//utf8
	{
	}
}
static void floatarea(struct window* win)
{
	u32* screenbuf;
	int width,height;
	int thisx,thisy;
	int x,y;

	screenbuf = (u32*)(win->buf1);
	width = win->w;
	height = win->h;
	thisx = (pointeroffset % byteperline) << 4;
	thisy = (pointeroffset / byteperline) << 4;

	//byte框
	for(y=thisy;y<thisy+16;y++)
	{
		for(x=thisx;x<thisx+16;x++)
		{
			screenbuf[xshift + y*width + x] = ~screenbuf[xshift + y*width + x];
		}
	}

	//256*128的详情框
	thisx+=16;
	thisy+=16;
	if(thisx > width -xshift -256)thisx -= (256+16);
	if(thisy >= height - 128)thisy -= (128+16);
	rectbody(win,
		xshift + thisx, thisy,
		xshift + thisx+256, thisy+128,
		0xffff
	);

	//
	data2hexstr((u64)databuf, hi + 0x10);
	data2hexstr(windowoffset, hi + 0x30);
	data2hexstr(pointeroffset, hi + 0x50);
	data2hexstr(0, hi + 0x70);

	//target,base,offset,data
	for(y=0;y<8;y++)
	{
		for(x=0;x<32;x++)
		{
			drawascii(
				win, hi[(y*32) + x], 1,
				xshift + thisx + x*8, thisy + y*16, 0, 0
			);
		}
	}
}
static void hex_read_pixel(struct window* win)
{
	if(win->dim == 1)
	{
		printmemory(databuf, 0x200);
		xt100_read(win);
		return;
	}

	background1(win);
	foreground(win);
	floatarea(win);
}
static void hex_read_text(struct window* win)
{
	u8 h,l;
	int x,y;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf1);

	if(printmethod==0)		//hex
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<xshift;x++)
			{
				p[y*width + x] = 0x20;
			}
			data2hexstr((u64)databuf + windowoffset + y*byteperline, p + y*width);

			for(x=0;x<byteperline;x++)
			{
				h = l = databuf[windowoffset + y*byteperline + x];

				h = ( (h>>4)&0xf ) + 0x30;
				if(h>0x39)h += 0x7;
				p[xshift + y*width + x*2] = h;

				l = (l&0xf) + 0x30;
				if(l>0x39)l += 0x7;
				p[xshift + y*width + x*2 + 1] = l;
			}

			x=(width-xshift)&0xfffffffe;
			for(;x<width;x++)
			{
				p[y*width + x] = 0x20;
			}
		}
	}

	else if(printmethod==1)		//ascii
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<xshift;x++)
			{
				p[y*width + x] = 0x20;
			}
			data2hexstr((u64)databuf + windowoffset + y*byteperline, p + y*width);

			for(x=0;x<byteperline;x++)
			{
				h = databuf[windowoffset + y*byteperline + x];
				if( (h>0x20)&&(h<0x80) )
				{
					p[xshift + y*width + x*2] = 0x20;
					p[xshift + y*width + x*2 + 1] = h;
				}
				else
				{
					p[xshift + y*width + x*2] = 0x20;
					p[xshift + y*width + x*2 + 1] = 0x20;
				}
			}

			x=(width-xshift)&0xfffffffe;
			for(;x<width;x++)
			{
				p[y*width + x] = 0x20;
			}
		}
	}
}
static void hex_read_html(struct window* win)
{
	float dx,dy;
	u8 ch;
	int x,y;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf1);
return;
	//prepare
	dx = 80.00 / byteperline;
	x = (pointeroffset % byteperline);
	dy = 80.00 / lineperwindow;
	y = (pointeroffset / byteperline);

	//background
	p += fmt(
		p, 0x1000,
		"<style>"
		".bg{position:absolute;width:%02f%;height:%02f%;left:%02f%;top:%02f%;background:#abcdef;color:#000;}",

		dx, dy,
		dx *x +10.00, dy *y +10.00
	);

	//table
	p += fmt(
		p, 0x1000,
		".tb{position:absolute;left:10%;top:10%;width:80%;height:80%;border-collapse:collapse;table-layout:fixed;}"
		".tb td{border:solid #000 1px;text-align:center;}"
	);

	//foreground
	if(x>byteperline-9)x-=9;
	if(y>lineperwindow-5)y-=5;
	p += fmt(
		p, 0x1000,
		".fg1{position:absolute;width:%02f%;height:%02f%;left:%02f%;top:%02f%;border:1px solid #000;background:#fedcba;color:#000;}"
		".fg2{position:absolute;width:%02f%;height:%02f%;left:%02f%;top:%02f%;border:1px solid #000;background:#fedcba;color:#000;}"
		"</style>",

		dx * 4, dy * 4,
		dx *(x+1) +10.00, dy *(y+1) +10.00,

		dx * 4, dy * 4,
		dx *(x+5) +10.00, dy *(y+1) +10.00
	);

	//bg, table
	p += fmt(
		p, 0x1000,
		"<div class=\"bg\"></div>"
		"<table class=\"tb\">"
	);
	if(printmethod==0)		//hex
	{
		for(y=0;y<lineperwindow;y++)
		{
			p += fmt(p, 0x1000, "<tr>");

			for(x=0;x<byteperline;x++)
			{
				ch = databuf[windowoffset + y*byteperline + x];
				p += fmt(
					p, 0x1000,
					"<td>%02x</td>", ch
				);
			}
		}
	}

	else if(printmethod==1)		//ascii
	{
		for(y=0;y<lineperwindow;y++)
		{
			p += fmt(p, 0x1000, "<tr>");
//windowoffset + y*byteperline

			for(x=0;x<byteperline;x++)
			{
				ch = databuf[windowoffset + y*byteperline + x];
				if((ch > 0x1f) && (ch < 0x7f))
				{
					p += fmt(p, 0x1000, "<td>%c</td>", ch);
				}
				else
				{
					p += fmt(p, 0x1000, "<td>.</td>");
				}
			}
		}
	}
	p += fmt(p, 0x1000, "</table>");

	//fg
	p += fmt(
		p, 0x1000,
		"<div class=\"fg1\">"
		"buf:<br />"
		"area:<br />"
		"pointer:<br />"
		"</div>"
		"<div class=\"fg2\">"
		"%llx<br />"
		"%llx<br />"
		"%llx<br />"
		"</div>",

		(u64)databuf,
		windowoffset,
		pointeroffset
	);
}
static void hex_read(struct window* win)
{
	u64 fmt = win->fmt;
	u64 width = win->w;

	//text
	if(fmt == 0x74786574)
	{
		lineperwindow = win->h;

		if(width >= 0x80)
		{
			byteperline = 0x40;
			xshift = (width - 0x80)/2;
		}
		else if(width >= 0x40)
		{
			byteperline = 0x20;
			xshift = (width - 0x40)/2;
		}
		else if(width >= 0x20)
		{
			byteperline = 0x10;
			xshift = (width - 0x20)/2;
		}
		else
		{
			byteperline = 0;
			xshift = 0;
		}

		hex_read_text(win);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		lineperwindow = 16;
		byteperline = 32;
		xshift = 0;

		hex_read_html(win);
	}

	//pixel
	else
	{
		lineperwindow = (win->h)/16;

		if(width >= 2048)
		{
			byteperline = 0x80;
			xshift = (width - 2048)/2;
		}
		else if(width >= 1024)
		{
			byteperline = 0x40;
			xshift = (width - 1024)/2;
		}
		else if(width >= 512)
		{
			byteperline = 0x20;
			xshift = (width - 512)/2;
		}
		else if(width >= 256)
		{
			byteperline = 0x10;
			xshift = (width - 256)/2;
		}
		else
		{
			byteperline = 0;
			xshift = 0;
		}

		hex_read_pixel(win);
	}
}
static void hex_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;
	//say("%x,%x\n",type,key);

	if(type==0x64626b)		//'kbd'
	{
		if(key==0x25)	//left	0x4b
		{
			if( pointeroffset % byteperline == 0 )
			{
				if(windowoffset > byteperline * lineperwindow)
				{
					windowoffset -= byteperline * lineperwindow;
				}
			}
			else
			{
				pointeroffset--;
			}
		}
		else if(key==0x27)	//right	0x4d
		{
			if( pointeroffset % byteperline == byteperline-1 )
			{
				if(windowoffset < 0x400000 - byteperline*lineperwindow)
				{
					windowoffset += byteperline * lineperwindow;
				}
			}
			else
			{
				pointeroffset++;
			}
		}
		else if(key==0x26)	//up	0x4b
		{
			if( pointeroffset < byteperline )
			{
				if(windowoffset >= byteperline)
				{
					windowoffset -= byteperline;
				}
			}
			else
			{
				pointeroffset -= byteperline;
			}
		}
		else if(key==0x28)	//down	0x4d
		{
			if( pointeroffset >= (lineperwindow-1) * byteperline )
			{
				if(windowoffset < 0x400000 - byteperline)
				{
					windowoffset += byteperline;
				}
			}
			else
			{
				pointeroffset += byteperline;
			}
		}
	}
	else if(type==0x2d6d)
	{
		if((key>>48) == 'f')	//front
		{
			if( pointeroffset < byteperline )
			{
				windowoffset -= byteperline;
			}
			else
			{
				pointeroffset -= byteperline;
			}
		}
		else if((key>>48) == 'b')		//back
		{
			if( pointeroffset < (lineperwindow-1) * byteperline )
			{
				windowoffset += byteperline;
			}
			else
			{
				pointeroffset += byteperline;
			}
		}
		else
		{
			int x=key&0xffff;
			int y=(key>>16)&0xffff;
			pointeroffset = ( (y/16) * byteperline ) + ( (x-xshift) / 16 );

			//浮动框以外的
			//px=x/(1024/0x40);
			//py=y/(640/40);
		}
	}
	else if(type==0x72616863)		//'char'
	{
		if(key==9)					//tab
		{
			printmethod=(printmethod+1)%2;
		}
		else if(key==0x8)			//backspace
		{
			if(inputcount!=0)inputcount--;
			hi[0x80+inputcount]=0;
		}
		else if(key==0xd)			//enter
		{
			if(cmp( hi+0x80 , "addr" ) == 0)
			{
			}
		}
		else
		{
			if(inputcount<128)
			{
				hi[0x80+inputcount]=key;
				inputcount++;
			}
		}
	}
}








static void hex_list()
{
}
static void hex_into()
{
}
static void hex_start()
{
	int j;

	//偏移
	windowoffset = pointeroffset = 0;

	//浮动框
	for(j=0;j<0x100;j++)hi[j]=0;
	*(u64*)hi=0x3a746567726174;
	*(u64*)(hi+0x20)=0x3a65736162;
	*(u64*)(hi+0x40)=0x3a74657366666f;
	*(u64*)(hi+0x60)=0x3a61746164;
}
static void hex_stop()
{
}
void hex_create(void* uibuf,void* addr)
{
	struct player* p = addr;
	databuf = uibuf+0x500000;

	p->type = 0x6c6f6f74;
	p->name = 0x786568;

	p->start = (u64)hex_start;
	p->stop = (u64)hex_stop;
	p->list = (u64)hex_list;
	p->choose = (u64)hex_into;
	p->read = (u64)hex_read;
	p->write = (u64)hex_write;
}
void hex_delete()
{
}
