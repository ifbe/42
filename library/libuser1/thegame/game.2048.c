#include "libuser.h"
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




static void the2048_draw_pixel(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u32 color;
	int x,y,x0,y0,x1,y1;
	int cx, cy, ww, hh;
	u8 (*tab)[4] = (void*)(act->buf) + (act->len)*16;

	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
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
static void the2048_draw_vbo2d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u32 rgb;
	int x,y,w,h;
	u8 (*tab)[4];
	float j,k;
	vec3 tc, tr, tf, tu;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid2d_rect(win, 0x444444, vc, vr, vf);

	if(0 == act->buf)tab = ((void*)act) + 0x100;
	else tab = (void*)(act->buf) + (act->len)*16;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			rgb = color2048[tab[y][x]];
			//say("%x\n", rgb);

			j = (x+x-3) / 4.0;
			k = (3-y-y) / 4.0;
			tc[0] = vc[0] + j*vr[0] + k*vf[0];
			tc[1] = vc[1] + j*vr[1] + k*vf[1];
			tc[2] = vc[2] + j*vr[2] + k*vf[2] - 0.1;
			tr[0] = vr[0] / 4.1;
			tr[1] = vr[1] / 4.1;
			tr[2] = vr[2] / 4.1;
			tf[0] = vf[0] / 4.1;
			tf[1] = vf[1] / 4.1;
			tf[2] = vf[2] / 4.1;
			carvesolid2d_rect(win, rgb, tc, tr, tf);

			tr[0] /= 4;
			tf[1] /= 4;
			tc[2] -= 0.1;
			carve2d_decimal(win, 0, tc, tr, tf, val2048[tab[y][x]]);
		}
	}
}
static void the2048_draw_vbo3d(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u32 rgb;
	int x,y;
	u8 (*tab)[4];
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	carvesolid_rect(win, 0x444444, vc, vr, vf);

	if(0 == act->buf)tab = ((void*)act) + 0x100;
	else tab = (void*)(act->buf) + (act->len)*16;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			rgb = color2048[tab[y][x]];
			//say("%x\n", rgb);

			f[0] = (x+x-3) / 4.0;
			f[1] = (3-y-y) / 4.0;
			f[2] = val2048[tab[y][x]] / 2048.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];

			tr[0] = vr[0]/5;
			tr[1] = vr[1]/5;
			tr[2] = vr[2]/5;
			tf[0] = vf[0]/5;
			tf[1] = vf[1]/5;
			tf[2] = vf[2]/5;
			tu[0] = vu[0]*f[2];
			tu[1] = vu[1]*f[2];
			tu[2] = vu[2]*f[2];
			carvesolid_prism4(win, rgb, tc, tr, tf, tu);

			tc[0] += tu[0] + vu[0]*0.01;
			tc[1] += tu[1] + vu[1]*0.01;
			tc[2] += tu[2] + vu[2]*0.01;
			tr[0] = vr[0]/16;
			tr[1] = vr[1]/16;
			tr[2] = vr[2]/16;
			tf[0] = vf[0]/16;
			tf[1] = vf[1]/16;
			tf[2] = vf[2]/16;
			carvedecimal(win, ~rgb, tc, tr, tf, val2048[tab[y][x]]);
		}
	}
}
static void the2048_draw_json(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
static void the2048_draw_html(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
static void the2048_draw_tui(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
static void the2048_draw_cli(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
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
static void the2048_draw(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)the2048_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)the2048_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)the2048_draw_html(act, pin, win, sty);
	else if(fmt == _json_)the2048_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)the2048_draw_vbo2d(act, pin, win, sty);
		else the2048_draw_vbo3d(act, pin, win, sty);
	}
	else the2048_draw_pixel(act, pin, win, sty);
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
static void the2048_event(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	int k;
	short* s;
	//say("%llx,%llx,%llx\n", act, pin, ev);
	//say("%x,%x,%x\n",ev->why, ev->what, ev->where);

	if(_char_ == ev->what)
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
	else if(_kbd_ == ev->what)
	{
		k = (ev->why)&0xff;
		if(0x48 == k)the2048_move(act, 'f');
		else if(0x4b == k)the2048_move(act, 'l');
		else if(0x4d == k)the2048_move(act, 'r');
		else if(0x50 == k)the2048_move(act, 'n');
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		s = (void*)ev;
		if(s[3] & joyl_left) the2048_move(act, 'l');
		if(s[3] & joyl_right)the2048_move(act, 'r');
		if(s[3] & joyl_down) the2048_move(act, 'n');
		if(s[3] & joyl_up)   the2048_move(act, 'f');
	}
}




static void the2048_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	the2048_draw(act, pin, win, sty);
}
static void the2048_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct pinid* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	the2048_event(act, pin, win, sty, ev, 0);
}
static void the2048_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void the2048_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void the2048_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void the2048_start(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act = (void*)(self->chip);
	void* buf = act->buf;
	new2048(buf);
}
static void the2048_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	act->buf = 0;
}
static void the2048_create(struct actor* act, u8* buf)
{
	int j;
	u8* p;
	if(0 == act)return;

	p = (u8*)act + 0x100;
	for(j=0;j<0x100;j++)p[j] = 0;
	act->buf = p;
	act->len = 0;
}




void the2048_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('2','0','4','8');

	p->oncreate = (void*)the2048_create;
	p->ondelete = (void*)the2048_delete;
	p->onstart  = (void*)the2048_start;
	p->onstop   = (void*)the2048_stop;
	p->oncread  = (void*)the2048_cread;
	p->oncwrite = (void*)the2048_cwrite;
	p->onsread  = (void*)the2048_sread;
	p->onswrite = (void*)the2048_swrite;
}
