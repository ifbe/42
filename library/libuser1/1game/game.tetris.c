#include "actor.h"




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




static void cubie(struct arena* win, int z,
	int x1, int y1, int x2, int y2)
{
	u32 bodycolor = z>0?0xffffff:0;

	drawsolid_rect(win, bodycolor, x1, y1, x2, y2);
	drawline_rect(win, 0x444444, x1, y1, x2, y2);
}
static void tetris_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x10000 / 32;
	int h = (win->h) * (sty->wanth) / 0x10000 / 40;
	for(y=0;y<40;y++)
	{
		for(x=0;x<32;x++)
		{
			//say("%d ",table[y*32+x]);
			cubie(win, table[y*32+x],
				cx+(x-16)*w, cy+(y-20)*h,
				cx+(x-15)*w, cy+(y-19)*h);
		}
		//say("\n");
	}
	//say("\n");

	//print cubies
	cubie(win, 1,
		cx+(that.x1-16)*w, cy+(that.y1-20)*h,
		cx+(that.x1-15)*w, cy+(that.y1-19)*h);
	cubie(win, 1,
		cx+(that.x2-16)*w, cy+(that.y2-20)*h,
		cx+(that.x2-15)*w, cy+(that.y2-19)*h);
	cubie(win, 1,
		cx+(that.x3-16)*w, cy+(that.y3-20)*h,
		cx+(that.x3-15)*w, cy+(that.y3-19)*h);
	cubie(win, 1,
		cx+(that.x4-16)*w, cy+(that.y4-20)*h,
		cx+(that.x4-15)*w, cy+(that.y4-19)*h);

	//print score
	//decimal(10,10,score);
}




static int htmlcubie(char* p, int x, int y)
{
	return mysnprintf(
		p, 0x1000,
		"<div class=\"rect\" style=\""
		"left:%.2f%;"
		"top:%.2f%;"
		"\">%d</div>",
		x*3.1, y*2.5, table[y*32+x]
	);
}
static void tetris_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	char* p = (char*)(win->buf);

	p += mysnprintf(
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




static void tetris_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;

	carvesolid_prism4(
		win, 0xffffff,
		cx, cy, 0.0,
		w/16, 0.0, 0.0,
		0.0, h/16, 0.0,
		0.0, 0.0, w/16
	);
}
static void tetris_read_tui(struct arena* win, struct actor* act, struct style* sty)
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
static void tetris_read_cli()
{
}
static void tetris_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)tetris_read_cli();
	else if(fmt == __tui__)tetris_read_tui(win, act, sty);
	else if(fmt == __vbo__)tetris_read_vbo(win, act, sty);
	else if(fmt == __html__)tetris_read_html(win, act, sty);
	else tetris_read_pixel(win, act, sty);
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

	if(type == 0x2d70)
	{
		for(ret=0;ret<20;ret++)if(down()==1)return;
	}
	else if(type == __kbd__)
	{
		if(key==0x48)up();
		else if(key==0x4b)left();
		else if(key==0x4d)right();
		else if(key==0x50)down();
	}
	else if(type == __char__)
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
	struct actor* p = addr;
	table=(u8*)(addr+0x300000);

	p->type = hex32('g', 'a', 'm', 'e');
	p->name = hex64('t', 'e', 't', 'r', 'i', 's', 0, 0);
	p->irel = 0;
	p->orel = 0;

	p->start = (void*)tetris_start;
	p->stop = (void*)tetris_stop;
	p->list = (void*)tetris_list;
	p->choose = (void*)tetris_choose;
	p->read = (void*)tetris_read;
	p->write = (void*)tetris_write;
}
void tetris_delete()
{
}
