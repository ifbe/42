#include "actor.h"




static u8* stlbuf;
static int stllen;




static void stl_read_html(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
}
static void stl_read_vbo(struct arena* win, struct actor* act, struct style* sty)
{
	float* p;
	int j;
	int ret = *(u32*)(stlbuf+80);
	//say("%x\n",ret);

	u32 pcount = win->info[8];
	u32 ncount = win->info[9];
	u32 ccount = win->info[10];
	u32 icount = win->info[14];

	void* buf = (void*)(win->buf);
	float* vertex = buf + 0x800000 + (pcount*12);
	float* normal = buf + 0x900000 + (ncount*12);
	float* color  = buf + 0xa00000 + (ccount*12);
	u16* index =    buf + 0xe00000 + (icount*2);

	win->info[8] += 3*ret;
	win->info[9] += 3*ret;
	win->info[10] += 3*ret;
	win->info[14] += 3*ret;

	ret = ret%(0x100000/36);
	for(j=0;j<ret;j++)
	{
		p = (void*)stlbuf + 84 + j*50;
		vertex[j*9 + 0] = p[3];
		vertex[j*9 + 1] = p[4];
		vertex[j*9 + 2] = p[5];
		vertex[j*9 + 3] = p[6];
		vertex[j*9 + 4] = p[7];
		vertex[j*9 + 5] = p[8];
		vertex[j*9 + 6] = p[9];
		vertex[j*9 + 7] = p[10];
		vertex[j*9 + 8] = p[11];

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
	if(type == 0x72616863)
	{
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
	stlbuf = (void*)startmemory(0x800000);
	stllen = readfile("42.stl", stlbuf, 0, 0x800000);
	say("%x: %x", stllen, *(u32*)(stlbuf+80));
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
