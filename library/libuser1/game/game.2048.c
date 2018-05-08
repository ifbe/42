#include "actor.h"
//
void new2048(void*);
void left2048(void*);
void right2048(void*);
void up2048(void*);
void down2048(void*);




static u8 len2048[17] = {
0, 1, 1, 1,
2, 2, 2, 3,
3, 3, 4, 4,
4, 4, 5, 5, 5
};
static u32 val2048[17] = {
   0,    2,    4,    8,
  16,   32,   64,  128,
 256,  512, 1024, 2048,
4096, 8192,16384,32768,65536
};
static u32 color2048[17] = {
0x444444, 0xfffff0, 0xffffc0, 0x995000,
0xc05000, 0xb03000, 0xff0000, 0xffffa0,
0xffff80, 0xffff00, 0xffffb0, 0x233333,
0x783d72, 0xd73762
};




static void the2048_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 color;
	int x,y,x0,y0,x1,y1;
	int cx, cy, ww, hh;
	u8 (*tab)[4] = (void*)(act->buf) + (act->len)*16;

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

	//cubies
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			//color
			color = color2048[tab[y][x]];
			if( ( (win->fmt)&0xffffff) == 0x626772)	//bgra->rgba
			{
				color = 0xff000000
					+ ((color&0xff)<<16)
					+ (color&0xff00)
					+ ((color&0xff0000)>>16);
			}

			//cubie
			x0 = (cx+1) + (x-2)*ww/2;
			y0 = (cy+1) + (y-2)*hh/2;
			x1 = (cx-1) + (x-1)*ww/2;
			y1 = (cy-1) + (y-1)*hh/2;
			drawsolid_rect(win, color, x0, y0, x1, y1);

			if(len2048[tab[y][x]] == 0)continue;
			drawdec_fit(
				win, 0,
				x0, y0*3/4+y1/4,
				x1, y0/4+y1*3/4,
				val2048[tab[y][x]]
			);
		}
	}
}
static void the2048_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 (*tab)[4];
	u32 color;
	int x,y;
	float xxx, yyy, zzz;
	int cx = sty->cx;
	int cy = sty->cy;
	int cz = sty->cz;
	int ww = sty->rx;
	int hh = sty->fy;
	int dd = sty->uz;

	if(0 == act->buf)tab = ((void*)act) + 0x100;
	else tab = (void*)(act->buf) + (act->len)*16;

	carvesolid_rect(
		win, 0x444444,
		cx, cy, 0.0,
		ww, 0.0, 0.0,
		0.0, hh, 0.0
	);
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			color = color2048[tab[y][x]];
			//say("%x\n",color);

			xxx = cx + (x+x-3)*ww/4;
			yyy = cy - (y+y-3)*hh/4;
			zzz = val2048[tab[y][x]];
			zzz = zzz * dd / 1000.0;

			carvesolid_prism4(
				win, color,
				xxx, yyy, zzz,
				ww/8, 0.0, 0.0,
				0.0, hh/8, 0.0,
				0.0, 0.0, zzz
			);
			carvedecimal(
				win, ~color,
				xxx, yyy, (zzz*2.0)+(dd/100.0),
				ww/16, 0.0, 0.0,
				0.0, hh/16, 0.0,
				val2048[tab[y][x]]
			);
		}
	}
}
static void the2048_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int len = win->len;
	u8* buf = win->buf;
	u8 (*tab)[4] = (void*)(act->buf) + (act->len)*16;

	len += mysnprintf(buf+len, 0x100000-len, "{\"2048\" : ");
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			len += mysnprintf(buf+len, 0x100000-len, "\"%d\",", tab[y][x]);
		}//forx
	}//fory
	len += mysnprintf(buf+len, 0x100000-len, "}\n");

	win->len = len;
}
static void the2048_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8 (*tab)[4] = (void*)(act->buf) + (act->len)*16;

	//<head>
	htmlprintf(win, 1,
		".b2048{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".g2048{width:24.8%%;height:24.8%%;margin:0.1%%;float:left;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"b2048\">\n");
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"g2048\" style=\"background-color:#%06x\">%d</div>\n",
				color2048[tab[y][x]], val2048[tab[y][x]]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void the2048_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8 (*tab)[4] = (void*)(act->buf) + (act->len)*16;

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			gentui_rect(win, 4, x*8, y*4, x*8+5, y*4+2);
			gentui_decstr(win, 7, x*8+2, y*4+1, val2048[tab[y][x]]);
		}
	}
}
static void the2048_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8 (*tab)[4] = (void*)(act->buf) + (act->len)*16;

	say("2048(%x,%x,%x,%x)\n", win, act, sty, pin);
	say("%d	%d	%d	%d\n",
		val2048[tab[0][0]],
		val2048[tab[0][1]],
		val2048[tab[0][2]],
		val2048[tab[0][3]]
	);
	say("%d	%d	%d	%d\n",
		val2048[tab[1][0]],
		val2048[tab[1][1]],
		val2048[tab[1][2]],
		val2048[tab[1][3]]
	);
	say("%d	%d	%d	%d\n",
		val2048[tab[2][0]],
		val2048[tab[2][1]],
		val2048[tab[2][2]],
		val2048[tab[2][3]]
	);
	say("%d	%d	%d	%d\n",
		val2048[tab[3][0]],
		val2048[tab[3][1]],
		val2048[tab[3][2]],
		val2048[tab[3][3]]
	);
}
static void the2048_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)the2048_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)the2048_read_tui(win, sty, act, pin);
	else if(fmt == _html_)the2048_read_html(win, sty, act, pin);
	else if(fmt == _json_)the2048_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)the2048_read_vbo(win, sty, act, pin);
	else the2048_read_pixel(win, sty, act, pin);
}




static void the2048_move(struct actor* act, int op)
{
	int j;
	u8* p;
	u8* q;

	p = (void*)(act->buf) + 16*(act->len);
	(act->len) = ((act->len)+1)%4;
	q = (void*)(act->buf) + 16*(act->len);
	for(j=0;j<16;j++)q[j] = p[j];

	if('f' == op)up2048(q);
	else if('l' == op)left2048(q);
	else if('r' == op)right2048(q);
	else if('n' == op)down2048(q);

	new2048(q);
}
static void the2048_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int k;
	//say("%llx,%llx,%llx\n", act, pin, ev);
	//say("%x,%x,%x\n",ev->why, ev->what, ev->where);

	if(ev->what == _char_)
	{
		k = ev->why;
		if(k == 0x8)
		{
			act->len = ((act->len)+15)%16;
			return;
		}

		k = (k>>16)&0xff;
		if(0x41 == k)the2048_move(act, 'f');
		else if(0x42 == k)the2048_move(act, 'n');
		else if(0x43 == k)the2048_move(act, 'r');
		else if(0x44 == k)the2048_move(act, 'l');
	}
	else if(ev->what == _kbd_)
	{
		k = (ev->why)&0xff;
		if(0x48 == k)the2048_move(act, 'f');
		else if(0x4b == k)the2048_move(act, 'l');
		else if(0x4d == k)the2048_move(act, 'r');
		else if(0x50 == k)the2048_move(act, 'n');
	}
	else if(ev->what == _joy_)
	{
		k = ((ev->why)>>32)&0xffff;
		if(_df_ == k)the2048_move(act, 'f');
		else if(_dl_ == k)the2048_move(act, 'l');
		else if(_dr_ == k)the2048_move(act, 'r');
		else if(_dn_ == k)the2048_move(act, 'n');
	}
}
static void the2048_stop(struct actor* act, struct pinid* pin)
{
}
static void the2048_start(struct actor* act, struct pinid* pin)
{
	int j;
	if(0 == act->len)new2048((act->buf));
}
static void the2048_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	act->buf = 0;
}
static void the2048_create(struct actor* act, u8* buf)
{
	u8* p;
	int j,k;
	if(0 == act)return;
	act->buf = ((void*)act) + 0x100;
	act->len = 0;

	if(0 == buf)return;
	p = act->buf;
	k = 0;
	for(j=0;j<16;j++)p[j] = 0;
	for(j=0;j<0x100;j++)
	{
		if((buf[j]>=0x30)&&(buf[j]<=0x39))
		{
			//say("%c", buf[j]);
			p[k] = buf[j]-0x30;
			k++;
		}
		if(k >= 16)break;
	}
	say("\n");
	act->len = 16;
}
static void the2048_list(u8* buf)
{
}
static void the2048_choose(u8* buf)
{
}




void the2048_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex32('2','0','4','8');
	p->irel = 0;
	p->orel = 0;

	p->oncreate = (void*)the2048_create;
	p->ondelete = (void*)the2048_delete;
	p->onstart  = (void*)the2048_start;
	p->onstop   = (void*)the2048_stop;
	p->onlist   = (void*)the2048_list;
	p->onchoose = (void*)the2048_choose;
	p->onread   = (void*)the2048_read;
	p->onwrite  = (void*)the2048_write;
}
