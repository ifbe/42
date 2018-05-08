#include "actor.h"
void xiangqi_generate(char (*data)[9]);
void xiangqi_move(char (*data)[9], int* turn, int px, int py, int x, int y);




static char data[10][9];
static int px, py, qx, qy, turn;




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

	}
	return "";
}
void xiangqi_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 black, brown, red;
	u32 chesscolor, fontcolor, temp;
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->cx;
		cy = sty->cy;
		ww = sty->rx;
		hh = sty->fy;
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	black = 0;
	if(0x626772 == (win->fmt&0xffffff))
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
		drawline(
			win, 0,
			cx - ww*8/9,
			cy + hh*(2*y+1)/10,
			cx + ww*8/9,
			cy + hh*(2*y+1)/10
		);
	}

	//shu
	for(x=-4;x<5;x++)
	{
		drawline(win, 0,
			cx + x*ww*2/9,	cy - hh*9/10,
			cx + x*ww*2/9,	cy - hh/10);
		drawline(win, 0,
			cx + x*ww*2/9,	cy + hh*9/10,
			cx + x*ww*2/9,	cy + hh/10);
	}

	//pie,na
	drawline(win, 0,
		cx - ww*2/9,
		cy - hh*9/10,
		cx + ww*2/9,
		cy - hh*5/10
	);
	drawline(win, 0,
		cx + ww*2/9,
		cy - hh*9/10,
		cx - ww*2/9,
		cy - hh*5/10
	);
	drawline(win, 0,
		cx - ww*2/9,
		cy + hh*9/10,
		cx + ww*2/9,
		cy + hh*5/10
	);
	drawline(win, 0,
		cx + ww*2/9,
		cy + hh*9/10,
		cx - ww*2/9,
		cy + hh*5/10
	);

	//chess
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			if(*(u8*)pin == 'r')temp = data[y][x];
			else temp = data[9-y][8-x];

			//empty
			if(temp < 'A')continue;

			//>0x41
			else if(temp <= 'Z')fontcolor = black;

			//>0x61
			else if(temp <= 'z')fontcolor = red;

			if( (px == x)&&(py == y) )chesscolor = 0xabcdef;
			else chesscolor = brown;

			drawsolid_circle(
				win, chesscolor,
				cx + (2*x-8)*ww/9,
				cy + (2*y-9)*hh/10,
				hh/10
			);

			drawutf8_fit(win, fontcolor,
				cx + (2*x-9)*ww/9,
				cy + (2*y-10)*hh/10,
				cx + (2*x-7)*ww/9,
				cy + (2*y-8)*hh/10,
				char2hanzi(temp), 0
			);
		}//forx
	}//fory
}
static void xiangqi_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u32 chesscolor, fontcolor, temp;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	carvesolid_rect(
		win, 0x8d6f25,
		cx, cy, cz,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
	for(y=-5;y<5;y++)
	{
		carveline(
			win, 0x222222,
			cx-(ww*8/9), cy+(2*y+1)*hh/10, 0.0,
			cx+(ww*8/9), cy+(2*y+1)*hh/10, 0.0
		);
	}
	for(x=-4;x<5;x++)
	{
		carveline(
			win, 0x222222,
			cx+x*ww*2/9, cy-hh*1/10, 0.0,
			cx+x*ww*2/9, cy-hh*9/10, 0.0
		);
		carveline(
			win, 0x222222,
			cx+x*ww*2/9, cy+hh*1/10, 0.0,
			cx+x*ww*2/9, cy+hh*9/10, 0.0
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
				win, 0xf9d65b,
				cx+(ww/9)*(2*x-8), cy+(hh/10)*(2*y-9), ww/20,
				ww/9, 0.0, 0.0,
				0.0, 0.0, ww/20
			);
			carveutf8(
				win, fontcolor,
				cx+(ww/9)*(2*x-8), cy+(hh/10)*(2*y-9), ww*2/19,
				ww/18, 0.0, 0.0,
				0.0, hh/20, 0.0,
				(u8*)char2hanzi(data[y][x]), 0
			);
		}
	}
}
static void xiangqi_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,c;
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(buf+len, 0x100000-len, "{\"xiangqi\" : ");
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			c = data[y][x];
			if(0 == c)c = '0';
			len += mysnprintf(buf+len, 0x100000-len, "\"%c\",", c);
		}//forx
	}//fory
	len += mysnprintf(buf+len, 0x100000-len, "}\n");

	win->len = len;
}
static void xiangqi_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;

	//<head>
	htmlprintf(win, 1,
		".xqbg{width:50%%;height:50%%;float:left;background-color:#8d6f25;text-align:center;}\n"
		".xqfg{width:11%%;height:10%%;float:left;background-color:#efcdab;border-radius:50%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"xqbg\">\n");
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"xqfg\">%s</div>\n",
				char2hanzi(data[y][x])
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void xiangqi_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,color;
	int width = win->stride;
	int height = win->height;
	u8* buf = (u8*)(win->buf);
	u8* q;

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
static void xiangqi_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void xiangqi_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)xiangqi_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)xiangqi_read_tui(win, sty, act, pin);
	else if(fmt == _html_)xiangqi_read_html(win, sty, act, pin);
	else if(fmt == _json_)xiangqi_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)xiangqi_read_vbo(win, sty, act, pin);
	else xiangqi_read_pixel(win, sty, act, pin);
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
void xiangqi_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
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
static void xiangqi_stop(struct actor* act, struct pinid* pin)
{
}
static void xiangqi_start(struct actor* act, struct pinid* pin)
{
	int j;
	char* p = (char*)data;

	px = py = -1;
	qx = qy = 0;
	turn = 0;

	for(j=0;j<90;j++)
	{
		if(0 != p[j])break;
	}
	if(j >= 90)xiangqi_generate(data);
}
static void xiangqi_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	if((_COPY_ == act->type)&&(0 != act->buf))
	{
		memorydelete(act->buf);
	}
	act->buf = 0;
}
static void xiangqi_create(struct actor* act, u8* buf)
{
	u8* p;
	int j,k;
	if(0 == act)return;
	else if(_orig_ == act->type)act->buf = data;
	else if(_copy_ == act->type)act->buf = memorycreate(10*9);

	if(0 == buf)return;
	p = act->buf;
	k = 0;
	for(j=0;j<16;j++)p[j] = 0;
	for(j=0;j<0x100;j++)
	{
		if(buf[j] >= 0x80)
		{
			//say("%.3s", buf+j);
			if(0 == ncmp(buf+j, "车", 3))p[k] = 'a';
			else if(0 == ncmp(buf+j, "马", 3))p[k] = 'b';
			else if(0 == ncmp(buf+j, "象", 3))p[k] = 'c';
			else if(0 == ncmp(buf+j, "士", 3))p[k] = 'd';
			else if(0 == ncmp(buf+j, "将", 3))p[k] = 'e';
			else if(0 == ncmp(buf+j, "卒", 3))p[k] = 's';
			else if(0 == ncmp(buf+j, "炮", 3))p[k] = 'z';
			else if(0 == ncmp(buf+j, "车", 3))p[k] = 'A';
			else if(0 == ncmp(buf+j, "马", 3))p[k] = 'B';
			else if(0 == ncmp(buf+j, "相", 3))p[k] = 'C';
			else if(0 == ncmp(buf+j, "仕", 3))p[k] = 'D';
			else if(0 == ncmp(buf+j, "帅", 3))p[k] = 'E';
			else if(0 == ncmp(buf+j, "兵", 3))p[k] = 'S';
			else if(0 == ncmp(buf+j, "炮", 3))p[k] = 'Z';

			j += 2;
			k++;
		}
		else if(buf[j] >= 0x30)
		{
			//say("%c", buf[j]);
			p[k] = 0;
			k++;
		}
		if(k >= 90)break;
	}
	say("\n");
}




void xiangqi_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('x', 'i', 'a', 'n', 'g', 'q', 'i', 0);
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)xiangqi_create;
	p->ondelete = (void*)xiangqi_delete;
	p->onstart  = (void*)xiangqi_start;
	p->onstop   = (void*)xiangqi_stop;
	p->onlist   = (void*)xiangqi_list;
	p->onchoose = (void*)xiangqi_choose;
	p->onread   = (void*)xiangqi_read;
	p->onwrite  = (void*)xiangqi_write;
}
