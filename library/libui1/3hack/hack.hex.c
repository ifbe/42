#include "actor.h"
//void background1(void*);




//flostarea
static int inputcount = 0;
static u8 hi[0x100];
	//[0,0x1f]:target,value
	//[0x20,0x3f]:base,value
	//[0x40,0x5f]:offset,value
	//[0x60,0x7f]:data,value

//where
static u8* databuf=0;
static u64 arenaoffset;
static u64 pointeroffset;





static int printmethod=0;
static int xshift=0;
static int byteperline=0;
static int lineperarena=0;
static void showdata(struct arena* win)
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
				drawbyte(win, 0, 16*x + xshift, 16*y,
					databuf[arenaoffset + y*byteperline + x]
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
				drawascii(win, 0, 16*x + xshift, 16*y,
					databuf[arenaoffset + y*byteperline + x]
				);
			}
		}
	}

	else if(printmethod == 2)	//utf8
	{
	}
}
static void floatarea(struct arena* win)
{
	u32* screenbuf;
	int width,height;
	int thisx,thisy;
	int x,y;

	screenbuf = (u32*)(win->buf);
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
	drawsolid_rect(win, 0xffff,
		xshift + thisx, thisy,
		xshift + thisx+256, thisy+128
	);

	//
	data2hexstr((u64)databuf, hi + 0x10);
	data2hexstr(arenaoffset, hi + 0x30);
	data2hexstr(pointeroffset, hi + 0x50);
	data2hexstr(0, hi + 0x70);

	//target,base,offset,data
	for(y=0;y<8;y++)
	{
		for(x=0;x<32;x++)
		{
			drawascii(win, 0,
				xshift + thisx + x*8, thisy + y*16,
				hi[(y*32) + x]
			);
		}
	}
}
static void hex_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	//background1(win);
	showdata(win);
	floatarea(win);
}
static void hex_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	float dx,dy;
	u8 ch;
	int x,y;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);
return;
	//prepare
	dx = 80.00 / byteperline;
	x = (pointeroffset % byteperline);
	dy = 80.00 / lineperarena;
	y = (pointeroffset / byteperline);

	//background
	p += mysnprintf(
		p, 0x1000,
		"<style>"
		".bg{position:absolute;width:%02f%;height:%02f%;left:%02f%;top:%02f%;background:#abcdef;color:#000;}",

		dx, dy,
		dx *x +10.00, dy *y +10.00
	);

	//table
	p += mysnprintf(
		p, 0x1000,
		".tb{position:absolute;left:10%;top:10%;width:80%;height:80%;border-collapse:collapse;table-layout:fixed;}"
		".tb td{border:solid #000 1px;text-align:center;}"
	);

	//foreground
	if(x>byteperline-9)x-=9;
	if(y>lineperarena-5)y-=5;
	p += mysnprintf(
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
	p += mysnprintf(
		p, 0x1000,
		"<div class=\"bg\"></div>"
		"<table class=\"tb\">"
	);
	if(printmethod==0)		//hex
	{
		for(y=0;y<lineperarena;y++)
		{
			p += mysnprintf(p, 0x1000, "<tr>");

			for(x=0;x<byteperline;x++)
			{
				ch = databuf[arenaoffset + y*byteperline + x];
				p += mysnprintf(
					p, 0x1000,
					"<td>%02x</td>", ch
				);
			}
		}
	}

	else if(printmethod==1)		//ascii
	{
		for(y=0;y<lineperarena;y++)
		{
			p += mysnprintf(p, 0x1000, "<tr>");
//arenaoffset + y*byteperline

			for(x=0;x<byteperline;x++)
			{
				ch = databuf[arenaoffset + y*byteperline + x];
				if((ch > 0x1f) && (ch < 0x7f))
				{
					p += mysnprintf(p, 0x1000, "<td>%c</td>", ch);
				}
				else
				{
					p += mysnprintf(p, 0x1000, "<td>.</td>");
				}
			}
		}
	}
	p += mysnprintf(p, 0x1000, "</table>");

	//fg
	p += mysnprintf(
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
		arenaoffset,
		pointeroffset
	);
}
static void hex_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void hex_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	u8 h,l;
	int x,y;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);

	if(printmethod==0)		//hex
	{
		for(y=0;y<height;y++)
		{
			for(x=0;x<xshift;x++)
			{
				p[y*width + x] = 0x20;
			}
			data2hexstr((u64)databuf + arenaoffset + y*byteperline, p + y*width);

			for(x=0;x<byteperline;x++)
			{
				h = l = databuf[arenaoffset + y*byteperline + x];

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
			data2hexstr((u64)databuf + arenaoffset + y*byteperline, p + y*width);

			for(x=0;x<byteperline;x++)
			{
				h = databuf[arenaoffset + y*byteperline + x];
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
static void hex_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("hex(%x,%x,%x)\n",win,act,sty);
}
static void hex_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;
	u64 width = win->w;

	if(fmt == __cli__)
	{
		hex_read_cli(win, act, sty);
	}
	else if(fmt == __tui__)
	{
		lineperarena = win->h;

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

		hex_read_tui(win, act, sty);
	}
	else if(fmt == __vbo__)
	{
		hex_read_vbo(win, act, sty);
	}
	else if(fmt == __html__)
	{
		lineperarena = 16;
		byteperline = 32;
		xshift = 0;

		hex_read_html(win, act, sty);
	}
	else
	{
		lineperarena = (win->h)/16;

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

		hex_read_pixel(win, act, sty);
	}
}
static void hex_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;
	//say("%x,%x\n",type,key);

	if(type == __kbd__)	//'kbd'
	{
		if(key == 0x48)	//up
		{
			if( pointeroffset < byteperline )
			{
				if(arenaoffset >= byteperline)
				{
					arenaoffset -= byteperline;
				}
			}
			else
			{
				pointeroffset -= byteperline;
			}
		}
		else if(key == 0x4b)	//left
		{
			if( pointeroffset % byteperline == 0 )
			{
				if(arenaoffset > byteperline * lineperarena)
				{
					arenaoffset -= byteperline * lineperarena;
				}
			}
			else
			{
				pointeroffset--;
			}
		}
		else if(key == 0x4d)	//right
		{
			if( pointeroffset % byteperline == byteperline-1 )
			{
				if(arenaoffset < 0x400000 - byteperline*lineperarena)
				{
					arenaoffset += byteperline * lineperarena;
				}
			}
			else
			{
				pointeroffset++;
			}
		}
		else if(key == 0x50)	//down
		{
			if( pointeroffset >= (lineperarena-1) * byteperline )
			{
				if(arenaoffset < 0x400000 - byteperline)
				{
					arenaoffset += byteperline;
				}
			}
			else
			{
				pointeroffset += byteperline;
			}
		}
	}
	else if(type == __char__)
	{
		if(key == 9)		//tab
		{
			printmethod=(printmethod+1)%2;
		}
		else if(key == 0x8)			//backspace
		{
			if(inputcount!=0)inputcount--;
			hi[0x80+inputcount]=0;
		}
		else if(key == 0xd)			//enter
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
	else if(type == 0x2b70)
	{
		if((key>>48) == 'f')	//front
		{
			if( pointeroffset < byteperline )
			{
				if(arenaoffset >= byteperline)arenaoffset -= byteperline;
			}
			else
			{
				if(pointeroffset >= byteperline)pointeroffset -= byteperline;
			}
		}
		else if((key>>48) == 'b')		//back
		{
			if( pointeroffset < (lineperarena-1) * byteperline )
			{
				arenaoffset += byteperline;
			}
			else
			{
				pointeroffset += byteperline;
			}
		}
		else
		{
			int x = key&0xffff;
			int y = (key>>16)&0xffff;
			pointeroffset =
				( (byteperline*x) >> 16 )+
				( (lineperarena*y) >> 16 )*byteperline;

			//浮动框以外的
			//px=x/(1024/0x40);
			//py=y/(640/40);
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
	arenaoffset = pointeroffset = 0;

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
	struct actor* p = addr;
	databuf = uibuf;

	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex32('h', 'e', 'x', 0);

	p->start = (void*)hex_start;
	p->stop = (void*)hex_stop;
	p->list = (void*)hex_list;
	p->choose = (void*)hex_into;
	p->read = (void*)hex_read;
	p->write = (void*)hex_write;
}
void hex_delete()
{
}
