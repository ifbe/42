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




/*
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
*/
static void hex_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,nx,ny;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;
	drawhyaline_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	ny = hh/8;
	nx = ww/8;
	if(nx > 0x40)nx = 0x40;
	else if(nx > 0x20)nx = 0x20;
	else if(nx > 0x10)nx = 0x10;
	else if(nx > 0x8)nx = 0x8;
	else if(nx > 0x4)nx = 0x4;
	if(printmethod == 0)		//hex
	{
		for(y=0;y<ny;y++)
		{
			for(x=0;x<nx;x++)
			{
				drawbyte(win, 0xffffff,
					cx+16*(x-nx/2), cy+16*(y-ny/2),
					databuf[arenaoffset + y*nx + x]
				);
			}
		}
	}

	else if(printmethod == 1)	//ascii
	{
		for(y=0;y<ny;y++)
		{
			for(x=0;x<nx;x++)
			{
				drawascii(win, 0xffffff,
					cx+16*(x-nx/2), cy+16*(y-ny/2),
					databuf[arenaoffset + y*nx + x]
				);
			}
		}
	}
}
static void hex_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void hex_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
}
static void hex_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void hex_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("hex(%x,%x,%x)\n",win,act,sty);
}
static void hex_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)hex_read_cli(win, act, sty);
	else if(fmt == __tui__)hex_read_tui(win, act, sty);
	else if(fmt == __vbo__)hex_read_vbo(win, act, sty);
	else if(fmt == __html__)hex_read_html(win, act, sty);
	else hex_read_pixel(win, act, sty);
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
		}
		else if(key == 0x4b)	//left
		{
		}
		else if(key == 0x4d)	//right
		{
		}
		else if(key == 0x50)	//down
		{
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
		}
		else if((key>>48) == 'b')		//back
		{
		}
		else
		{
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
