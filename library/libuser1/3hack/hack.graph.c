#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
void* samepinnextchip(void*);
void forcedirected_2d(void*, int, void*, int, void*, int);
void forcedirected_3d(void*, int, void*, int, void*, int);




struct origin
{
	u64 type;
	void* addr;
};
struct vertex
{
	float x;
	float y;
	float z;
	float w;
};
struct pairof
{
	u16 parent;
	u16 child;
};
static struct origin orig[16];
static struct pairof pair[16];
static struct vertex vbuf[16];
static struct vertex obuf[16];
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

	vbuf[j].x = (getrandom() & 0xffff) / 65536.0;
	vbuf[j].y = (getrandom() & 0xffff) / 65536.0;
	vbuf[j].z = (getrandom() & 0xffff) / 65536.0;
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
	rel = this->irel;
	while(1)
	{
		if(rel == 0)return;

		child = (void*)(rel->selfchip);
		k = graph_add(_win_, child);

		if(rel->selftype == _win_)graph_traverse(child);
		graph_pair(j, k);

		rel = samepinnextchip(rel);
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
		vbuf[j].x = (getrandom() & 0xffff) / 65536.0;
		vbuf[j].y = (getrandom() & 0xffff) / 65536.0;
		vbuf[j].z = (getrandom() & 0xffff) / 65536.0;
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
	rel = haha->irel;
	while(1)
	{
		if(rel == 0)break;
		rel = samepinnextchip(rel);
		k++;
	}
	//say("k=%x\n",k);

	j = 0;
	k *= 2;
	rel = haha->irel;
	while(1)
	{
		if(rel == 0)break;
//say("%x,%llx\n",rel,rel->selftype);

		ff = (da-sa) / k;
		x = cx + ww * cosine(sa + (2*j+1) * ff);
		y = cy + hh * sine(sa + (2*j+1) * ff);

		if(rel->selftype == _win_)
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

		act = (void*)(rel->selfchip);
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

		tmp = samepinnextchip(rel);
		if(tmp == 0)break;

		j += 1;
		rel = tmp;
	}
}
static void starry_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int ww = (win->w) * (sty->wantw) / 0x20000;
	int hh = (win->h) * (sty->wanth) / 0x20000;

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




static void graph_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int i,j,k;

	forcedirected_3d(obuf, vlen, vbuf, vlen, pair, plen);
	vbuf[0].x = vbuf[0].y = vbuf[0].z = 0.0;

	for(i=0;i<plen;i++)
	{
		j = pair[i].parent;
		k = pair[i].child;
		carveline(
			win, 0xff00,
			vbuf[j].x, vbuf[j].y, vbuf[j].z,
			vbuf[k].x, vbuf[k].y, vbuf[k].z
		);
	}
	for(j=0;j<vlen;j++)
	{
		carvesolid_prism4(
			win, 0xffffff,
			vbuf[j].x, vbuf[j].y, vbuf[j].z,
			0.01, 0.0, 0.0,
			0.0, 0.01, 0.0,
			0.0, 0.0, 0.01
		);
	}
}
static void graph_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	int i,j,k;
	int cx = (sty->cx) * (win->w) / 0x10000;
	int cy = (sty->cy) * (win->h) / 0x10000;
	int ww = (sty->wantw) * (win->w) / 0x10000;
	int hh = (sty->wanth) * (win->h) / 0x10000;
	void* p;
	struct arena* aa;
	struct actor* bb;

	forcedirected_2d(obuf, vlen, vbuf, vlen, pair, plen);
	vbuf[0].x = vbuf[0].y = vbuf[0].z = 0.0;

	drawsolid_rect(win, 0, cx-ww/2, cy-hh/2, cx+ww/2, cy+hh/2);
	for(i=0;i<plen;i++)
	{
		j = pair[i].parent;
		k = pair[i].child;
		drawline(
			win, 0xffffff,
			cx + ww*(vbuf[j].x),
			cy - hh*(vbuf[j].y),
			cx + ww*(vbuf[k].x),
			cy - hh*(vbuf[k].y)
		);
	}
	for(i=0;i<vlen;i++)
	{
		j = cx + ww*(vbuf[i].x);
		k = cy - hh*(vbuf[i].y);

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
static void graph_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
}
static void graph_read(struct arena* win, struct actor* act, struct style* sty)
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

	if(fmt == _cli_)graph_read_cli(win, act, sty);
	else if(fmt == _tui_)graph_read_tui(win, act, sty);
	else if(fmt == _html_)graph_read_html(win, act, sty);
	else if(fmt == _vbo_)graph_read_vbo(win, act, sty);
	else graph_read_pixel(win, act, sty);
}
static void graph_write(struct event* ev)
{
	if(ev->what == _char_)
	{
		if(ev->why == 0xd)redo = 1;
		if(ev->why == '1')redo = '1';
	}
}




static void graph_list()
{
}
static void graph_change()
{
}
static void graph_start()
{
}
static void graph_stop()
{
}
void graph_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('h', 'a', 'c', 'k');
	p->name = hex64('g', 'r', 'a', 'p', 'h', 0, 0, 0);

	p->start = (void*)graph_start;
	p->stop = (void*)graph_stop;
	p->list = (void*)graph_list;
	p->choose = (void*)graph_change;
	p->read = (void*)graph_read;
	p->write = (void*)graph_write;
}
void graph_delete()
{
}
