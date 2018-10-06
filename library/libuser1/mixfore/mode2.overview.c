#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define _json_ hex32('j','s','o','n')
#define _xml_ hex32('x','m','l',0)
#define _art_ hex32('a','r','t',0)
#define _fd_ hex32('f','d',0,0)
int arenaactor(struct arena* win, struct actor* act);
void draw8bit_rect(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1);




static struct object* obj = 0;
static struct element* ele = 0;
static struct arena* arena = 0;
static struct actor* actor = 0;




void overview_create(void* addr)
{
	obj = addr - 0x400000;
	ele = addr - 0x300000;
	arena = addr + 0x000000;
	actor = addr + 0x100000;
}
void overview_delete()
{
}




int arenalogin(struct arena* win)
{
	struct actor* act;
	int j = win->forez;
	if(j<0)return 0;

	act = &actor[j];
	if(0 == act->type)return 0;

	arenaactor(win, act);
	return 0;
}
int arenaprev(struct arena* win)
{
	if(win->forez > 0)win->forez -= 1;
	return 0;
}
int arenanext(struct arena* win)
{
	int j = win->forez + 1;
	if(0 != actor[j].type)win->forez += 1;
	return 0;
}




void overview_read_pixel(struct arena* win, struct style* sty)
{
	struct relation* rel;
	u32 c;
	int x,y,j,k;
	int cx,cy,ww,hh;
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
/*
	drawline(win, 0x0000ff, 0, h*1/4, w-1, h*1/4);
	drawline(win, 0x00ff00, 0, h*2/4, w-1, h*2/4);
	drawline(win, 0xff0000, 0, h*3/4, w-1, h*3/4);
*/
	//actor
	for(j=0;j<64;j++)
	{
		c = actor[j].type & 0xff;
		if(0 == c)break;
		else if(j == win->forez)c = 0xffff00ff;
		else if((c >= 'a')&&(c <= 'z'))c = 0x40808080;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(
			win, c,
			(cx+1)+(x-4)*ww/4, (cy+1)+(y-16)*hh/16,
			(cx-1)+(x-3)*ww/4, (cy-1)+(y-15)*hh/16,
			(u8*)&actor[j].name, 8
		);
	}

	//arena
	for(j=0;j<64;j++)
	{
		c = arena[j].type;
		if(0 == c)break;
		else if(win == &arena[j])c = 0xffff00ff;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(
			win, c,
			(cx+1)+(x-4)*ww/4, (cy+1)+(y-8)*hh/16,
			(cx-1)+(x-3)*ww/4, (cy-1)+(y-7)*hh/16,
			(u8*)&arena[j].fmt, 8
		);
	}

	//artery
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)continue;
		x = j%8;
		y = j/8;
		drawicon_1(
			win, 0x80ffffff,
			(cx+1)+(x-4)*ww/4, (cy+1)+(y+0)*hh/16,
			(cx-1)+(x-3)*ww/4, (cy-1)+(y+1)*hh/16,
			(u8*)&ele[j].type, 8
		);
	}

	//system
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;
		x = (j%64)%8;
		y = (j%64)/8;
		drawicon_1(
			win, 0x80ffffff,
			(cx+1)+(x-4)*ww/4, (cy+1)+(y+8)*hh/16,
			(cx-1)+(x-3)*ww/4, (cy-1)+(y+9)*hh/16,
			(u8*)&obj[j].type, 8
		);
	}

	//actor.irel
	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)continue;

		rel = actor[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+17)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-31)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+ 1)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-31)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-15)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-31)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-31)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-31)*hh/32
				);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//arena.irel
	for(j=0;j<64;j++)
	{
		if(0 == arena[j].type)break;

		rel = arena[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+17)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-15)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+ 1)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-15)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-15)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-15)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffc0ff,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-31)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)-15)*hh/32
				);
			}

			rel = samedstnextsrc(rel);
		}
	}

	//element.irel
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)break;

		rel = ele[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+17)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+ 1)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+ 1)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+ 1)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-15)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+ 1)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffc0ff,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-31)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+ 1)*hh/32
				);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//object.irel
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)break;

		rel = obj[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+17)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+17)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)+ 1)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+17)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-15)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+17)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffc0ff,
					cx+(4*(k%8)-13)*ww/16,
					cy+(2*(k/8)-31)*hh/32,
					cx+(4*(j%8)-15)*ww/16,
					cy+(2*(j/8)+17)*hh/32
				);
			}
			rel = samedstnextsrc(rel);
		}
	}
}
void overview_read_vbo(struct arena* win, struct style* sty)
{
	u32 bg,fg;
	float r,f;
	int x,y,j,k;
	struct relation* rel;
	float* vc;
	float* vr;
	float* vf;
	vec3 tc;
	vec3 tr;
	vec3 tf;
	struct style tmp;
	if(0 == sty)
	{
		sty = &tmp;
		sty->vc[0] = 0.0;
		sty->vc[1] = 0.0;
		sty->vc[2] = -0.5;
		sty->vr[0] = 1.0;
		sty->vr[1] = 0.0;
		sty->vr[2] = 0.0;
		sty->vf[0] = 0.0;
		sty->vf[1] = 1.0;
		sty->vf[2] = 0.0;
	}
	vc = sty->vc;
	vr = sty->vr;
	vf = sty->vf;
/*
	tc[0] = -1.0;
	tc[1] = 0.5;
	tc[2] = 0.0;
	tr[0] = 1.0;
	tr[1] = 0.5;
	tr[2] = 0.0;
	carveline2d(win, 0x0000ff, tc, tr);
	tc[0] = -1.0;
	tc[1] = 0.0;
	tc[2] = 0.0;
	tr[0] = 1.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	carveline2d(win, 0x00ff00, tc, tr);
	tc[0] = -1.0;
	tc[1] = -0.5;
	tc[2] = 0.0;
	tr[0] = 1.0;
	tr[1] = -0.5;
	tr[2] = 0.0;
	carveline2d(win, 0xff0000, tc, tr);
*/
	//actor
	for(j=0;j<64;j++)
	{
		k = actor[j].type & 0xff;
		if(0 == k)break;
		else if(j == win->forez)
		{
			bg = 0xffffff;
			fg = 0xff00ff;
		}
		else
		{
			bg = 0x404040;
			if((k >= 'a')&&(k <= 'z'))fg = 0x808080;
			else fg = 0xffffff;
		}

		x = j%8;
		y = j/8;
		r = (x*2-7)/8.0;
		f = (31-y*2)/32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0]*4.0/33;
		tr[1] = vr[1]*4.0/33;
		tr[2] = vr[2]*4.0/33;
		tf[0] = vf[0]*1.0/34;
		tf[1] = vf[1]*1.0/34;
		tf[2] = vf[2]*1.0/34;
		carvesolid2d_rect(win, bg, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0]*1.0/16;
		tr[1] = vr[1]*1.0/16;
		tr[2] = vr[2]*1.0/16;
		tf[0] = vf[0]*1.0/32;
		tf[1] = vf[1]*1.0/32;
		tf[2] = vf[2]*1.0/32;
		carvestring2d_center(win, fg, tc, tr, tf, (u8*)&actor[j].name, 8);
	}

	//arena
	for(j=0;j<64;j++)
	{
		k = arena[j].type;
		if(0 == k)break;
		else if(win == &arena[j])fg = 0xffff00ff;
		else fg = 0x80ffffff;

		x = j%8;
		y = j/8;
		r = (x*2-7)/8.0;
		f = (15-y*2)/32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0]*4.0/33;
		tr[1] = vr[1]*4.0/33;
		tr[2] = vr[2]*4.0/33;
		tf[0] = vf[0]*1.0/34;
		tf[1] = vf[1]*1.0/34;
		tf[2] = vf[2]*1.0/34;
		carvesolid2d_rect(win, 0x404040, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0]*1.0/16;
		tr[1] = vr[1]*1.0/16;
		tr[2] = vr[2]*1.0/16;
		tf[0] = vf[0]*1.0/32;
		tf[1] = vf[1]*1.0/32;
		tf[2] = vf[2]*1.0/32;
		carvestring2d_center(win, fg, tc, tr, tf, (u8*)&arena[j].fmt, 8);
	}

	//artery
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)continue;
		x = j%8;
		y = j/8;
		r = (x*2-7)/8.0;
		f = (-1-y*2)/32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0]*4.0/33;
		tr[1] = vr[1]*4.0/33;
		tr[2] = vr[2]*4.0/33;
		tf[0] = vf[0]*1.0/34;
		tf[1] = vf[1]*1.0/34;
		tf[2] = vf[2]*1.0/34;
		carvesolid2d_rect(win, 0x404040, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0]*1.0/16;
		tr[1] = vr[1]*1.0/16;
		tr[2] = vr[2]*1.0/16;
		tf[0] = vf[0]*1.0/32;
		tf[1] = vf[1]*1.0/32;
		tf[2] = vf[2]*1.0/32;
		carvestring2d_center(win, 0xffffff, tc, tr, tf, (u8*)&ele[j].type, 8);
	}

	//system
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;
		x = (j%64)%8;
		y = (j%64)/8;
		r = (x*2-7)/8.0;
		f = (-17-y*2)/32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0]*4.0/33;
		tr[1] = vr[1]*4.0/33;
		tr[2] = vr[2]*4.0/33;
		tf[0] = vf[0]*1.0/34;
		tf[1] = vf[1]*1.0/34;
		tf[2] = vf[2]*1.0/34;
		carvesolid2d_rect(win, 0x404040, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0]*1.0/16;
		tr[1] = vr[1]*1.0/16;
		tr[2] = vr[2]*1.0/16;
		tf[0] = vf[0]*1.0/32;
		tf[1] = vf[1]*1.0/32;
		tf[2] = vf[2]*1.0/32;
		carvestring2d_center(win, 0xffffff, tc, tr, tf, (u8*)&obj[j].type, 8);
	}

	//actor.irel
	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)continue;

		rel = actor[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;

				r = ((k%8)*4-13)/16.0;
				f = (-17-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (31-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%8)*4-13)/16.0;
				f = (-1-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (31-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%8)*4-13)/16.0;
				f = (15-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (31-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%8)*4-13)/16.0;
				f = (31-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (31-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//arena.irel
	for(j=0;j<64;j++)
	{
		if(0 == arena[j].type)break;

		rel = arena[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;

				r = ((k%8)*4-13)/16.0;
				f = (-17-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (15-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%8)*4-13)/16.0;
				f = (-1-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (15-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%8)*4-13)/16.0;
				f = (15-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (15-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%8)*4-13)/16.0;
				f = (31-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (15-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffc0ff, tc, tr);
			}

			rel = samedstnextsrc(rel);
		}
	}

	//element.irel
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)break;

		rel = ele[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;

				r = ((k%8)*4-13)/16.0;
				f = (-17-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-1-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%8)*4-13)/16.0;
				f = (-1-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-1-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%8)*4-13)/16.0;
				f = (15-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-1-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%8)*4-13)/16.0;
				f = (31-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-1-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffc0ff, tc, tr);
			}

			rel = samedstnextsrc(rel);
		}
	}

	//object.irel
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)break;

		rel = obj[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 64;

				r = ((k%8)*4-13)/16.0;
				f = (-17-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-17-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%8)*4-13)/16.0;
				f = (-1-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-17-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = vr[0]*((k%8)*4-13)/16.0;
				f = vf[1]*(15-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-17-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%8)*4-13)/16.0;
				f = (31-(k/8)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%8)*4-15)/16.0;
				f = (-17-(j/8)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffc0ff, tc, tr);
			}

			rel = samedstnextsrc(rel);
		}
	}
}
void overview_read_8bit(struct arena* win, struct style* sty)
{
	int x,y;
	int j,c;
	for(j=0;j<64;j++)
	{
		if(j == win->forez)c = 0x80;
		else c = 0x42;

		x = j%4;
		y = j/4;
		draw8bit_rect(
			win, c,
			 81+x*40, 61+y*10,
			118+x*40, 68+y*10
		);
	}
}
void overview_read_html(struct arena* win, struct style* sty)
{
}
void overview_read_tui(struct arena* win, struct style* sty)
{
	int j,k,x,y;
	int ww = ((win->stride)/2)&0xfffc;
	int hh = (win->height)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);

	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;

		if(j == win->forez)k=1;
		else k=2;

		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&actor[j].name, 8);
	}
}
void overview_read_cli(struct arena* win, struct style* sty)
{
}
void actoroutput_overview(struct arena* win, struct style* sty)
{
	if(win->fmt == _cli_)overview_read_cli(win, sty);
	else if(win->fmt == _tui_)overview_read_tui(win, sty);
	else if(win->fmt == _vbo_)overview_read_vbo(win, sty);
	else if(win->fmt == _html_)overview_read_html(win, sty);
	else if(win->fmt == _8bit_)overview_read_8bit(win, sty);
	else overview_read_pixel(win, sty);
}




void overview_drag(struct arena* win, int x0, int y0, int x1, int y1)
{
	int j;
	struct object* obj_s;
	struct object* obj_d;
	struct element* ele_s;
	struct element* ele_d;
	struct arena* win_s;
	struct arena* win_d;
	struct actor* act_s;
	struct actor* act_d;
	say("(%d,%d)->(%d,%d)\n", x0, y0, x1, y1);

	if(y0 < 8)
	{
		act_s = &actor[x0+(y0*8)];

		if((x0==x1)&&(y0==y1))
		{
			if(0 == act_s->type)return;
			actorcreate(0, act_s);
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1+(y1*8)];
			if((act_d->type) && (act_s->type))relationcreate(act_d, 0, _act_, act_s, 0, _act_);
			else if(0 == act_s->type)actorcreate(act_d->name, 0);
			else if(0 == act_d->type)actorcreate(act_s->name, 0);
		}
		else if(y1 < 16)
		{
		}
		else if(y1 < 24)
		{
		}
		else if(y1 < 32)
		{
		}
	}
	else if(y0 < 16)
	{
		win_s = &arena[x0 + (y0-8)*8];

		if((x0==x1)&&(y0==y1))
		{
			y1 = y1-8;
			say("@arena:%d\n", (y1*8)+x1);
			//arenacreate(0,0);
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1 + (y1*8)];
			if(0 == act_d->type)return;

			arenaactor(win_s, act_d);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*8];
			if((win_d->type) && (win_s->type))relationcreate(win_d, 0, _win_, win_s, 0, _win_);
			else if(win_d->type)win_d = arenacreate(_coop_, 0);
			else if(win_s->type)win_s = arenacreate(_coop_, 0);
		}
		else if(y1 < 24)
		{
		}
		else if(y1 < 32)
		{
		}
	}
	else if(y0 < 24)
	{
		ele_s = &ele[x0 + (y0-16)*8];
		if((x0==x1)&&(y0==y1))
		{
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1 + (y1*8)];
			relationcreate(act_d, 0, _act_, ele_s, 0, _art_);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*8];
			relationcreate(win_d, 0, _win_, ele_s, 0, _art_);
		}
		else if(y1 < 24)
		{
		}
		else if(y1 < 32)
		{
		}
	}
	else if(y0 < 32)
	{
		j = x0 + (y0-24)*8;
		for(;j<0x1000;j+=64)
		{
			obj_s = &obj[x0+(y0*8)];
			if(obj_s->type)break;
		}

		if((x0==x1)&&(y0==y1))
		{
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1+(y1*8)];
			relationcreate(act_d, 0, _act_, obj_s, 0, _fd_);
		}
		else if(y1 < 16)
		{
		}
		else if(y1 < 24)
		{
		}
		else if(y1 < 32)
		{
		}
	}
}
void actorinput_overview(struct arena* win, struct style* sty, struct event* ev)
{
	short* t;
	struct actor* act;
	int j, k;
	int x, y, id;
	int width = win->width;
	int height = win->height;

	j = (ev->what)&0xff;
	k = ((ev->what)>>8)&0xff;

	if(j == 'p')
	{
		x = (ev->why)&0xffff;
		x = (x*8) / width;
		y = ((ev->why)>>16)&0xffff;
		y = (y*32) / height;
		id = ((ev->why)>>48)&0xffff;
		if(id > 10)id = 10;

		if('@' == k)
		{
			if(y < 8)win->forez = x + (y*8);
		}
		else if('+' == k)
		{
			if(y < 8)win->forez = x + (y*8);
		}
		else if('-' == k)
		{
			j = win->input[id].x0;
			j = (j*8) / width;
			k = win->input[id].y0;
			k = (k*32) / height;

			if((j<0)|(j>=8))return;
			if((k<0)|(k>=32))return;
			if((x<0)|(x>=8))return;
			if((y<0)|(y>=32))return;

			overview_drag(win, j, k, x, y);
		}
	}
	else if(_char_ == ev->what)
	{
		if((0xd == ev->why)|(0xa == ev->why))
		{
			arenalogin(win);
		}
		else if(0x435b1b == ev->why)
		{
			arenanext(win);
		}
		else if(0x445b1b == ev->why)
		{
			arenaprev(win);
		}
	}
	else if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)
		{
			arenaprev(win);
		}
		else if(0x4d == ev->why)
		{
			arenanext(win);
		}
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (short*)&ev->why;
		if(t[3] & joyl_left)
		{
			arenaprev(win);
			return;
		}
		if(t[3] & joyl_right)
		{
			arenanext(win);
			return;
		}
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (short*)&ev->why;
		if(t[3] & joyr_down)
		{
			arenalogin(win);
			return;
		}
	}
}
