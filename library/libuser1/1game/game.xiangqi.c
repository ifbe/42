#include "actor.h"
void xiangqi_move(char (*data)[9], int* turn, int px, int py, int x, int y);




static char data[10][9];
static int turn;
static int px,py,qx,qy;




void* char2hanzi(int val)
{
	switch(val)
	{
		case 'a':return "车";
		case 'b':return "马";
		case 'c':return "象";
		case 'd':return "士";
		case 'e':return "将";
		case 's':return "卒";
		case 'z':return "炮";

		case 'A':return "车";
		case 'B':return "马";
		case 'C':return "相";
		case 'D':return "仕";
		case 'E':return "帅";
		case 'S':return "兵";
		case 'Z':return "炮";

		default:return 0;
	}
}
static int htmlcircle(char* p, int x, int y)
{
	u32 textcolor;
	char* hanzi;
	char ch;

	ch = data[y][x];
	if( (ch>='a') && (ch<='z') )
	{
		textcolor=0;
	}
	else if( (ch >='A') && (ch <= 'Z') )
	{
		textcolor=0xff0000;
	}
	else return 0;

	hanzi = char2hanzi(data[y][x]);
	return mysnprintf(
		p, 0x1000,
		"<div class=\"circle\" style=\""
		"left:%d%;"
		"top:%d%;"
		"background:#ffff00;"
		"color:#%06x;"
		"\"><br>%s</div>",
		x*11, y*10,
		textcolor, hanzi
	);
}
static void xiangqi_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	char* p = (char*)(win->buf);
	return;

	p += mysnprintf(
		p, 0x1000,
		"<style type=\"text/css\">"
		".circle{"
		"position:absolute;"
		"border-radius:50%;"
		"width:10%;"
		"height:10%;"
		"text-align:center;"
		"}"
		"</style>"
	);
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			p += htmlcircle(p, x, y);
		}//forx
	}//fory
}




void xiangqi_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 black, brown, red;
	u32 chesscolor, fontcolor, temp;
	int x,y,half;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	//
	black=0;
	if( ((win->fmt)&0xffffffff) == 0x61626772)
	{
		temp = 0x256f8d;
		red = 0xff;
		brown = 0x36878d;
	}
	else
	{
		temp = 0x8d6f25;
		red = 0xff0000;
		brown = 0x8d8736;
	}
	drawsolid_rect(win, temp, cx-ww, cy-hh, cx+ww, cy+hh);

	//heng
	for(y=-5;y<5;y++)
	{
		drawline(win, 0,
			cx - half*8,	cy + half*(2*y+1),
			cx + half*8,	cy + half*(2*y+1));
	}

	//shu
	for(x=-4;x<5;x++)
	{
		drawline(win, 0,
			cx + x*half*2,	cy - half*9,
			cx + x*half*2,	cy - half*1);
		drawline(win, 0,
			cx + x*half*2,	cy + half*9,
			cx + x*half*2,	cy + half*1);
	}

	//pie,na
	drawline(win, 0,
		cx - half*2,	cy - half*9,
		cx + half*2,	cy - half*5);
	drawline(win, 0,
		cx + half*2,	cy - half*9,
		cx - half*2,	cy - half*5);
	drawline(win, 0,
		cx - half*2,	cy + half*9,
		cx + half*2,	cy + half*5);
	drawline(win, 0,
		cx + half*2,	cy + half*9,
		cx - half*2,	cy + half*5);

	//chess
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			//empty
			if(data[y][x] < 'A')continue;

			//>0x41
			else if(data[y][x] <= 'Z')fontcolor = black;

			//>0x61
			else if(data[y][x] <= 'z')fontcolor = red;

			if( (px == x)&&(py == y) )chesscolor = 0xabcdef;
			else chesscolor = brown;

			drawsolid_circle(win, chesscolor,
				cx + (2*x-8)*half, cy + (2*y-9)*half, half);
/*
			drawascii(win, fontcolor,
				cx + (2*x-8)*half - (half/8*8/2),
				cy + (2*y-9)*half - (half/8*16/2),
				data[y][x]
			);
*/
			drawutf8(win, fontcolor,
				cx + (2*x-8)*half - (half/8*8/2),
				cy + (2*y-9)*half - (half/8*16/2),
				char2hanzi(data[y][x]), 0
			);
		}//forx
	}//fory
}
static void xiangqi_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	u32 chesscolor, fontcolor, temp;
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	//heng
	for(y=-5;y<5;y++)
	{
		carveline(win, 0x444444,
			cx-(ww*4/9), cy+(2*y+1)*hh/20, 0.0,
			cx+(ww*4/9), cy+(2*y+1)*hh/20, 0.0
		);
	}

	//shu
	for(x=-4;x<5;x++)
	{
		carveline(win, 0x444444,
			cx+x*ww/9, cy-hh*1/20, 0.0,
			cx+x*ww/9, cy-hh*9/20, 0.0
		);
		carveline(win, 0x444444,
			cx+x*ww/9, cy+hh*1/20, 0.0,
			cx+x*ww/9, cy+hh*9/20, 0.0
		);
	}

	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			//empty
			if(data[y][x] < 'A')continue;

			//>0x41
			else if(data[y][x] <= 'Z')fontcolor = 0;

			//>0x61
			else if(data[y][x] <= 'z')fontcolor = 0xff0000;

			carvesolid_cylinder(
				win, 0x8d6f25,
				cx+(ww/18)*(2*x-8),
				cy+(hh/20)*(2*y-9),
				ww/40,
				ww/18,
				0.0,
				0.0,
				0.0,
				0.0,
				ww/40
			);
			carveutf8(
				win, fontcolor,
				cx+(ww/18)*(2*x-8),
				cy+(hh/20)*(2*y-9),
				ww/19,
				ww/36,
				0.0,
				0.0,
				0.0,
				hh/40,
				0.0,
				(u8*)char2hanzi(data[y][x]), 0
			);
		}
	}
}
static void xiangqi_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,color;
	int width = win->w;
	int height = win->h;
	u8* buf = (u8*)(win->buf);
	u8* q;

	//
	for(x=0;x<width*height*4;x++)buf[x] = 0;
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			q = char2hanzi(data[y][x]);
			if(q == 0)
			{
				if(x != qx)continue;
				if(y != qy)continue;
			}

			//color
			if( (px==x)&& (py==y) )color = 5;
			else if( (qx==x)&& (qy==y) )color = 2;
			else if(data[y][x] >= 'a')color = 1;
			else color = 4;
			gentui_rect(win, color, x*6, y*3, x*6+5, y*3+2);

			//character
			gentui_utf8(win, 0, x*6+2, y*3+1, char2hanzi(data[y][x]), 0);
		}
	}
}
static void xiangqi_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void xiangqi_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)xiangqi_read_cli(win, act, sty);
	else if(fmt == _tui_)xiangqi_read_tui(win, act, sty);
	else if(fmt == _vbo_)xiangqi_read_vbo(win, act, sty);
	else if(fmt == _html_)xiangqi_read_html(win, act, sty);
	else xiangqi_read_pixel(win, act, sty);
}




int xiangqi_pickup(int x, int y)
{
	if( (x==px) && (y==py) )
	{
		px = py = -1;
		return 1;
	}

	//chess choosing
	if( (px<0) | (py<0) | (data[py][px] == 0) )
	{
		if( (data[y][x]>='a') && (data[y][x]<='z') && ((turn&1) == 0) )
		{
			px = x;
			py = y;
		}
		if( (data[y][x]>='A') && (data[y][x]<='Z') && ((turn&1) == 1) )
		{
			px = x;
			py = y;
		}
		return 2;
	}

	return 0;
}
void xiangqi_write(struct event* ev)
{
	int x, y, ret;
	u64 key = ev->why;
	u64 what = ev->what;

	if(what == _kbd_)
	{
		if(key == 0x48)	//up
		{
			if(qy<1)return;
			qy--;
		}
		else if(key == 0x4b)	//left
		{
			if(qx<1)return;
			qx--;
		}
		else if(key == 0x4d)	//right
		{
			if(qx<0)return;
			if(qx>=8)return;
			qx++;
		}
		else if(key == 0x50)	//down
		{
			if(qy<0)return;
			if(qy>=9)return;
			qy++;
		}
	}

	else if(what == _char_)
	{
		if(key == 0x20)
		{
			ret = xiangqi_pickup(qx, qy);
			if(ret > 0)return;

			//move?
			xiangqi_move(data, &turn, px, py, qx, qy);
		}
		else if(key == 0x415b1b)
		{
			if(qy<1)return;	//up
			qy--;
		}
		else if(key == 0x425b1b)	//down
		{
			if(qy<0)return;
			if(qy>=9)return;
			qy++;
		}
		else if(key == 0x435b1b)	//right
		{
			if(qx<0)return;
			if(qx>=8)return;
			qx++;
		}
		else if(key == 0x445b1b)	//left
		{
			if(qx<1)return;
			qx--;
		}
	}

	else if(what == 0x2b70)
	{
		//x = key & 0xffff;
		//y = (key >> 16) & 0xffff;
		//say("%d,%d => ",x,y);

		//x = (x*9)>>16;
		//y = (y*10)>>16;
		//say("%d,%d\n",x,y);

		x = (key & 0xffff) / 6;
		y = ((key >> 16) & 0xffff) / 3;

		if(x < 0)return;
		if(x > 8)return;
		if(y < 0)return;
		if(y > 9)return;

		//pick?
		ret = xiangqi_pickup(x, y);
		if(ret > 0)return;

		//move?
		xiangqi_move(data, &turn, px, py, x, y);
		px = py = -1;
	}
}




static void xiangqi_list()
{
}
static void xiangqi_choose()
{
}
static void xiangqi_start()
{
	int j;
	char* p=(char*)data;

	px = py = -1;
	qx = qy = 0;
	turn = 0;
	for(j=0;j<90;j++)p[j]=0;




	for(j=0;j<=4;j++)
	{
		//(black)JU,MA,XIANG,SHI,JIANG
		data[0][ j ]='A' + j;
		data[0][8-j]='A' + j;
		
		//(red)ju,ma,xiang,shi,jiang
		data[9][ j ]='a' + j;
		data[9][8-j]='a' + j;
	}
	for(j=0;j<5;j++)
	{
		//(red)SOLDIER
		data[3][j*2]='S';

		//(black)soldier
		data[6][j*2]='s';
	}

	//(red)PAO
	data[2][1]='Z';
	data[2][7]='Z';

	//(black)pao
	data[7][1]='z';
	data[7][7]='z';
}
static void xiangqi_stop()
{
}
void xiangqi_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex64('x', 'i', 'a', 'n', 'g', 'q', 'i', 0);
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)xiangqi_start;
	act->stop = (void*)xiangqi_stop;
	act->list = (void*)xiangqi_list;
	act->choose = (void*)xiangqi_choose;
	act->read = (void*)xiangqi_read;
	act->write = (void*)xiangqi_write;
}
void xiangqi_delete()
{
}
