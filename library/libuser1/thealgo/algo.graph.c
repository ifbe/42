#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void* samedstnextsrc(void*);
void forcedirected_2d(void*, int, void*, int, void*, int);
void forcedirected_3d(void*, int, void*, int, void*, int);




struct origin
{
	u64 type;
	void* addr;
};
struct pairof
{
	u16 parent;
	u16 child;
};
static struct origin orig[16];
static struct pairof pair[16];
static vec3 vbuf[16];
static vec3 obuf[16];
static int olen = 0;
static int plen = 0;
static int vlen = 0;
static int redo = 1;




int graph_add(u64 type, void* addr)
{
	int j;
	for(j=0;j<vlen;j++)
	{
		if(	(orig[j].type == type) &&
			(orig[j].addr == addr) )
		{return j;}
	}

	j = vlen;
	vlen++;
	olen++;

	orig[j].type = type;
	orig[j].addr = addr;

	vbuf[j][0] = (getrandom() & 0xffff) / 65536.0;
	vbuf[j][1] = (getrandom() & 0xffff) / 65536.0;
	vbuf[j][2] = (getrandom() & 0xffff) / 65536.0;
	return j;
}
void graph_pair(int parent, int child)
{
	pair[plen].parent = parent;
	pair[plen].child = child;
	plen += 1;
}
static void graph_traverse(struct arena* this)
{
	u32 color;
	int j, k;
	struct arena* child;
	struct relation* rel;

	j = graph_add(_win_, this);
	rel = this->irel0;
	while(1)
	{
		if(rel == 0)return;

		child = (void*)(rel->srcchip);
		k = graph_add(_win_, child);

		if(rel->srctype == _win_)graph_traverse(child);
		graph_pair(j, k);

		rel = samedstnextsrc(rel);
	}
}
void butane()
{
	int j;
	static u8 table[13][2] =
	{
		0,1,0,2,0,3,
		0,4,
		4,5,4,6,
		4,7,
		7,8,7,9,
		7,10,
		10,11,10,12,10,13
	};

	for(j=0;j<14;j++)
	{
		vbuf[j][0] = (getrandom() & 0xffff) / 65536.0;
		vbuf[j][1] = (getrandom() & 0xffff) / 65536.0;
		vbuf[j][2] = (getrandom() & 0xffff) / 65536.0;
	}
	vlen = 14;
	olen = 0;

	for(j=0;j<13;j++)
	{
		pair[j].parent = table[j][0];
		pair[j].child = table[j][1];
	}
	plen = 13;
}




static void starry_read_pixel_r(struct arena* win, struct arena* haha,
	int cx, int cy, int ww, int hh, float sa, float da)
{
	int x,y,j,k;
	u32 color;
	float ff;
	struct relation* rel;
	struct relation* tmp;
	struct actor* act;

	k = 0;
	rel = haha->irel0;
	while(1)
	{
		if(rel == 0)break;
		rel = samedstnextsrc(rel);
		k++;
	}
	//say("k=%x\n",k);

	j = 0;
	k *= 2;
	rel = haha->irel0;
	while(1)
	{
		if(rel == 0)break;
//say("%x,%llx\n",rel,rel->srctype);

		ff = (da-sa) / k;
		x = cx + ww * cosine(sa + (2*j+1) * ff);
		y = cy + hh * sine(sa + (2*j+1) * ff);

		if(rel->srctype == _win_)
		{
			color = 0x0000ff;
			starry_read_pixel_r(
				win, (void*)act,
				x, y,
				ww/2, hh/2,
				sa+(2*j)*ff, sa+(2*j+2)*ff
			);
		}
		else color = 0x00ff00;

		act = (void*)(rel->srcchip);
		drawline(
			win, color,
			cx, cy,
			x, y
		);
		drawicon_1(
			win, color,
			x - 32, y - 12,
			x + 32, y + 12,
			(void*)(&act->name), 8
		);

		tmp = samedstnextsrc(rel);
		if(tmp == 0)break;

		j += 1;
		rel = tmp;
	}
}
static void starry_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	drawicon_1(
		win, 0xff,
		cx-32, cy-12,
		cx+32, cy+12,
		(void*)"root", 0
	);
	starry_read_pixel_r(
		win, win,
		cx, cy,
		ww/2, hh/2,
		-PI, PI
	);
}




static void graph_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct arena* aa;
	struct actor* bb;
	void* p;
	int i,j,k;
	int cx = sty->vc[0];
	int cy = sty->vc[1];
	int ww = sty->vr[0];
	int hh = sty->vf[1];

	forcedirected_2d(obuf, vlen, vbuf, vlen, pair, plen);
	vbuf[0][0] = vbuf[0][1] = vbuf[0][2] = 0.0;

	drawsolid_rect(win, 0, cx-ww/2, cy-hh/2, cx+ww/2, cy+hh/2);
	for(i=0;i<plen;i++)
	{
		j = pair[i].parent;
		k = pair[i].child;
		drawline(
			win, 0xffffff,
			cx + ww*(vbuf[j][0]),
			cy - hh*(vbuf[j][1]),
			cx + ww*(vbuf[k][0]),
			cy - hh*(vbuf[k][1])
		);
	}
	for(i=0;i<vlen;i++)
	{
		j = cx + ww*(vbuf[i][0]);
		k = cy - hh*(vbuf[i][1]);

		p = "?";
		if(i < olen)
		{
			if(orig[i].type == _win_)
			{
				aa = orig[i].addr;
				p = &(aa->fmt);
			}
			else if(orig[i].type == _act_)
			{
				bb = orig[i].addr;
				p = &(bb->name);
			}
		}
		drawicon_1(
			win, 0xffffff,
			j-16, k-12,
			j+16, k+12,
			p, 8
		);
	}
}
static void graph_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int i,j,k;
	vec3 vr, vf, vu;

	forcedirected_3d(obuf, vlen, vbuf, vlen, pair, plen);
	vbuf[0][0] = vbuf[0][1] = vbuf[0][2] = 0.0;

	for(i=0;i<plen;i++)
	{
		j = pair[i].parent;
		k = pair[i].child;
		carveline(win, 0xff00, vbuf[j], vbuf[k]);
	}

	vr[0] = vf[1] = vu[2] = 0.01;
	vr[1] = vr[2] = 0.0;
	vf[0] = vr[2] = 0.0;
	vu[0] = vu[1] = 0.0;
	for(j=0;j<vlen;j++)
	{
		carvesolid_prism4(win, 0xffffff, vbuf[j], vr, vf, vu);
	}
}
static void graph_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void graph_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"graph\" style=\"width:50%%;height:100px;float:left;background-color:#c8a7d9;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");

	win->len = len;
}
static void graph_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void graph_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void graph_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(redo == 1)
	{
		redo = vlen = plen = 0;
		graph_traverse(win);
	}
	else if(redo == '1')
	{
		redo = vlen = plen = 0;
		butane();
	}

	if(fmt == _cli_)graph_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)graph_read_tui(win, sty, act, pin);
	else if(fmt == _html_)graph_read_html(win, sty, act, pin);
	else if(fmt == _json_)graph_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)graph_read_vbo(win, sty, act, pin);
	else graph_read_pixel(win, sty, act, pin);
}
static void graph_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	if(ev->what == _char_)
	{
		if(ev->why == 0xd)redo = 1;
		if(ev->why == '1')redo = '1';
	}
}
static void graph_get()
{
}
static void graph_post()
{
}
static void graph_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void graph_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void graph_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void graph_create(struct actor* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = orig;
	if(_copy_ == act->type)act->buf = memorycreate(0x1000);
}




void graph_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('g', 'r', 'a', 'p', 'h', 0, 0, 0);

	p->oncreate = (void*)graph_create;
	p->ondelete = (void*)graph_delete;
	p->onstart  = (void*)graph_start;
	p->onstop   = (void*)graph_stop;
	p->onget    = (void*)graph_get;
	p->onpost   = (void*)graph_post;
	p->onread   = (void*)graph_read;
	p->onwrite  = (void*)graph_write;
}
