#include "actor.h"
int openreadclose(void*, void*, u64, u64);
int openwriteclose(void*, void*, u64, u64);




static u8* stlbuf;
static int stllen;
static float left,right,front,back,bottom,upper;




static void stl_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	int j, ret;
	float* p;
	float cx = (float)(sty->cx) / 65536.0 - 0.5;
	float cy = (float)(sty->cy) / 65536.0 - 0.5;
	float w = (float)(sty->wantw) / 65536.0;
	float h = (float)(sty->wanth) / 65536.0;
	float sx,sy,sz,sw,sh,sd;

	u32 pcount = win->vertexcount;
	u32 ncount = win->normalcount;
	u32 ccount = win->colorcount;
	u32 tcount = win->texturecount;
	u32 icount = win->tricount;

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x000000 + (pcount*12);
	float* normal = buf + 0x200000 + (ncount*12);
	float* color  = buf + 0x400000 + (ccount*12);
	u16* index =    buf + 0xc00000 + (icount*2);

	ret = *(u32*)(stlbuf+80);
	win->vertexcount += 3*ret;
	win->normalcount += 3*ret;
	win->colorcount += 3*ret;
	win->tricount += 3*ret;

	sx = (left+right)/2;
	sy = (back+front)/2;
	sw = right-left;
	sh = front-back;
	ret = ret%(0x100000/36);
	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;

		vertex[j*9 + 0] = cx + (p[3]-sx)/sw*w;
		vertex[j*9 + 1] = cy + (p[4]-sy)/sw*w;
		vertex[j*9 + 2] = (p[5]-bottom)/sw*w;
		vertex[j*9 + 3] = cx + (p[6]-sx)/sw*w;
		vertex[j*9 + 4] = cy + (p[7]-sy)/sw*w;
		vertex[j*9 + 5] = (p[8]-bottom)/sw*w;
		vertex[j*9 + 6] = cx + (p[9]-sx)/sw*w;
		vertex[j*9 + 7] = cy + (p[10]-sy)/sw*w;
		vertex[j*9 + 8] = (p[11]-bottom)/sw*w;

		normal[j*9 + 0] = p[0];
		normal[j*9 + 1] = p[1];
		normal[j*9 + 2] = p[2];
		normal[j*9 + 3] = p[0];
		normal[j*9 + 4] = p[1];
		normal[j*9 + 5] = p[2];
		normal[j*9 + 6] = p[0];
		normal[j*9 + 7] = p[1];
		normal[j*9 + 8] = p[2];

		color[j*9 + 0] = 1.0;
		color[j*9 + 1] = 1.0;
		color[j*9 + 2] = 1.0;
		color[j*9 + 3] = 1.0;
		color[j*9 + 4] = 1.0;
		color[j*9 + 5] = 1.0;
		color[j*9 + 6] = 1.0;
		color[j*9 + 7] = 1.0;
		color[j*9 + 8] = 1.0;

		index[j*3 + 0] = pcount + j*3 + 0;
		index[j*3 + 1] = pcount + j*3 + 1;
		index[j*3 + 2] = pcount + j*3 + 2;
	}
}
static void stl_read_tui(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_cli(struct arena* win, struct actor* act, struct style* sty)
{
	say("stl(%x,%x,%x)\n",win,act,sty);
}
static void stl_read(struct arena* win, struct actor* act, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == __cli__)stl_read_cli(win, act, sty);
	else if(fmt == __tui__)stl_read_tui(win, act, sty);
	else if(fmt == __vbo__)stl_read_vbo(win, act, sty);
	else if(fmt == __html__)stl_read_html(win, act, sty);
	else stl_read_pixel(win, act, sty);
}




static void stl_write(struct event* ev)
{
	u64 type = ev->what;
	u64 key = ev->why;

	if(type == 0x2d70)
	{
		int x = key&0xffff;
		int y = (key>>16)&0xffff;
	}
}




static void stl_list()
{
}
static void stl_change()
{
}
static void stl_start()
{
	float* p;
	int j,ret;
	stlbuf = (void*)startmemory(0x800000);
	stllen = openreadclose("42.stl", stlbuf, 0, 0x800000);
	say("len=%x, count=%x, ", stllen, *(u32*)(stlbuf+80));

	left = back = bottom = 100000.0;
	right = front = upper = -100000.0;

	ret = *(u32*)(stlbuf+80);
	ret = ret%(0x100000/36);
	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;

		if(p[3] < left)left = p[3];
		if(p[3] > right)right = p[3];
		if(p[4] < back)back = p[4];
		if(p[4] > front)front = p[4];
		if(p[5] < bottom)bottom = p[5];
		if(p[5] > upper)upper = p[5];

		if(p[6] < left)left = p[6];
		if(p[6] > right)right = p[6];
		if(p[7] < back)back = p[7];
		if(p[7] > front)front = p[7];
		if(p[8] < bottom)bottom = p[8];
		if(p[8] > upper)upper = p[8];

		if(p[9] < left)left = p[9];
		if(p[9] > right)right = p[9];
		if(p[10] < back)back = p[10];
		if(p[10] > front)front = p[10];
		if(p[11] < bottom)bottom = p[11];
		if(p[11] > upper)upper = p[11];
	}
	say("%f,%f,%f,%f,%f,%f\n",left,right,back,front,bottom,upper);
}
static void stl_stop()
{
}
void stl_create(void* base,void* addr)
{
	struct actor* p = addr;
	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex32('s', 't', 'l', 0);

	p->start = (void*)stl_start;
	p->stop = (void*)stl_stop;
	p->list = (void*)stl_list;
	p->choose = (void*)stl_change;
	p->read = (void*)stl_read;
	p->write = (void*)stl_write;
}
void stl_delete()
{
}
