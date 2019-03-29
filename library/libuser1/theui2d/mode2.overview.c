#include "libuser.h"
void* allocstyle();
void* allocpinid();
void* actorstart(void*, void*, void*, void*, void*, void*);
void* actorstop(void*, void*, void*, void*, void*, void*);
void draw8bit_rect(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1);




static struct object* obj = 0;
static struct element* ele = 0;
static struct arena* arena = 0;
static struct actor* actor = 0;




void defaultstyle_2d(struct style* sty, int w, int h, int d)
{
	sty->vc[0] = w/2;
	sty->vc[1] = h/2;
	sty->vc[2] = 0.0;

	sty->vr[0] = w/2;
	sty->vr[1] = 0;
	sty->vr[2] = 0;

	sty->vf[0] = 0;
	sty->vf[1] = h/2;
	sty->vf[2] = 0;

	sty->vu[0] = 0;
	sty->vu[1] = 0;
	sty->vu[2] = (w+h)/4;
}
void defaultstyle_2in3(struct style* sty)
{
	sty->vc[0] = 0.0;
	sty->vc[1] = 0.0;
	sty->vc[2] = 0.0;

	sty->vr[0] = 1.0;
	sty->vr[1] = 0.0;
	sty->vr[2] = 0.0;

	sty->vf[0] = 0.0;
	sty->vf[1] = 1.0;
	sty->vf[2] = 0.0;

	sty->vu[0] = 0.0;
	sty->vu[1] = 0.0;
	sty->vu[2] = 1.0;
}
void defaultstyle_3d(struct style* sty, struct style* tar)
{
	sty->vr[0] = tar->vr[0];
	sty->vr[1] = tar->vr[1];
	sty->vr[2] = tar->vr[2];

	sty->vf[0] = tar->vf[0];
	sty->vf[1] = tar->vf[1];
	sty->vf[2] = tar->vf[2];

	sty->vu[0] = tar->vu[0];
	sty->vu[1] = tar->vu[1];
	sty->vu[2] = tar->vu[2];

	sty->vc[0] = tar->vc[0] + tar->vr[0] + tar->vf[0];
	sty->vc[1] = tar->vc[1] + tar->vr[1] + tar->vf[1];
	sty->vc[2] = tar->vc[2] + tar->vr[2] + tar->vf[2];
}
int arenaactor(struct arena* ccc, struct actor* act)
{
	int w,h;
	struct style* sty;
	struct pinid* pin;
	struct arena* win;
	struct relation* rel;

	sty = allocstyle();
	if(0 == sty)return 0;

	pin = allocpinid();
	if(0 == pin)return 0;

	win = ccc;
	switch(ccc->fmt)
	{
		case _bg3d_:
		case _fg3d_:
		case _ev3d_:
		case _ui3d_:
		{
			rel = ccc->irel0;
			if(0 == rel)return 0;

			win = (void*)(rel->srcchip);
			defaultstyle_3d(sty, &win->target);

			break;
		}
		case _bg2d_:
		case _fg2d_:
		case _ev2d_:
		case _ui2d_:
		{
			rel = ccc->irel0;
			if(0 == rel)return 0;

			win = (void*)(rel->srcchip);
			defaultstyle_2in3(sty);

			break;
		}
		case _vbo_:
		{
			defaultstyle_3d(sty, &win->target);
			break;
		}
		default:
		{
			w = win->width;
			h = win->height;
			defaultstyle_2d(sty, w, h, (w+h)/2);
		}
	}

	actorcreate(0, act);
	actorstart(act, pin, ccc, sty, win, 0);

	relationcreate(act, pin, _act_, ccc, sty, _win_);
	return 0;
}



/*
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
}*/




void overview_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	struct relation* rel;
	u32 c,cursor;
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
	cursor = (act->x0) + (act->y0)*8;
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

		if(j == cursor)c = 0xffff00ff;
		else if((c >= 'a')&&(c <= 'z'))c = 0x40808080;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(win, c,
			(cx+1)+(x-4)*ww/4, (cy+1)+(y-16)*hh/16,
			(cx-1)+(x-3)*ww/4, (cy-1)+(y-15)*hh/16,
			(u8*)&actor[j].name, 8
		);
	}

	//arena
	for(j=0;j<64;j++)
	{
		if(0 == arena[j].type)break;

		if(j+64 == cursor)c = 0xffff00ff;
		else if(win == &arena[j])c = 0xff808080;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(win, c,
			(cx+1)+(x-4)*ww/4, (cy+1)+(y-8)*hh/16,
			(cx-1)+(x-3)*ww/4, (cy-1)+(y-7)*hh/16,
			(u8*)&arena[j].fmt, 8
		);
	}

	//artery
	for(j=0;j<64;j++)
	{
		if(0 == ele[j].type)continue;

		if(j+128 == cursor)c = 0xffff00ff;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(win, c,
			(cx+1)+(x-4)*ww/4, (cy+1)+(y+0)*hh/16,
			(cx-1)+(x-3)*ww/4, (cy-1)+(y+1)*hh/16,
			(u8*)&ele[j].type, 8
		);
	}

	//system
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;

		if((j%64)+192 == cursor)c = 0xffff00ff;
		else c = 0x80ffffff;

		x = (j%64)%8;
		y = (j%64)/8;
		drawicon_1(win, c,
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
void overview_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 bg,fg,cursor;
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
	cursor = (act->x0) + (act->y0)*16;
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
	for(j=0;j<128;j++)
	{
		k = actor[j].type & 0xff;
		if(0 == k)break;

		if(j == cursor)
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

		x = j % 16;
		y = j / 16;
		r = (x*2-15) / 16.0;
		f = (31-y*2) / 32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0] / 16.3;
		tr[1] = vr[1] / 16.3;
		tr[2] = vr[2] / 16.3;
		tf[0] = vf[0] / 33.0;
		tf[1] = vf[1] / 33.0;
		tf[2] = vf[2] / 33.0;
		carvesolid2d_rect(win, bg, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0] / 32;
		tr[1] = vr[1] / 32;
		tr[2] = vr[2] / 32;
		tf[0] = vf[0] / 32;
		tf[1] = vf[1] / 32;
		tf[2] = vf[2] / 32;
		carvestring2d_center(win, fg, tc, tr, tf, (u8*)&actor[j].name, 8);
	}

	//arena
	for(j=0;j<128;j++)
	{
		k = arena[j].type;
		if(0 == k)break;
		if(j+128 == cursor)
		{
			bg = 0xffffff;
			fg = 0xff00ff;
		}
		else
		{
			bg = 0x404040;
			if(win == &arena[j])fg = 0xffff00ff;
			else fg = 0x80ffffff;
		}

		x = j % 16;
		y = j / 16;
		r = (x*2-15) / 16.0;
		f = (15-y*2) / 32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0] / 16.3;
		tr[1] = vr[1] / 16.3;
		tr[2] = vr[2] / 16.3;
		tf[0] = vf[0] / 33;
		tf[1] = vf[1] / 33;
		tf[2] = vf[2] / 33;
		carvesolid2d_rect(win, bg, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0] / 32;
		tr[1] = vr[1] / 32;
		tr[2] = vr[2] / 32;
		tf[0] = vf[0] / 32;
		tf[1] = vf[1] / 32;
		tf[2] = vf[2] / 32;
		carvestring2d_center(win, fg, tc, tr, tf, (u8*)&arena[j].fmt, 8);
	}

	//artery
	for(j=0;j<128;j++)
	{
		if(0 == ele[j].type)continue;
		if(j+256 == cursor)
		{
			bg = 0xffffff;
			fg = 0xff00ff;
		}
		else
		{
			bg = 0x404040;
			fg = 0x80ffffff;
		}

		x = j % 16;
		y = j / 16;
		r = (x*2-15) / 16.0;
		f = (-1-y*2) / 32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0] / 16.3;
		tr[1] = vr[1] / 16.3;
		tr[2] = vr[2] / 16.3;
		tf[0] = vf[0] / 33.0;
		tf[1] = vf[1] / 33.0;
		tf[2] = vf[2] / 33.0;
		carvesolid2d_rect(win, bg, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0] / 32.0;
		tr[1] = vr[1] / 32.0;
		tr[2] = vr[2] / 32.0;
		tf[0] = vf[0] / 32.0;
		tf[1] = vf[1] / 32.0;
		tf[2] = vf[2] / 32.0;
		carvestring2d_center(win, fg, tc, tr, tf, (u8*)&ele[j].type, 8);
	}

	//system
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;
		if((j%128)+384 == cursor)
		{
			bg = 0xffffff;
			fg = 0xff00ff;
		}
		else
		{
			bg = 0x404040;
			fg = 0x80ffffff;
		}

		x = (j % 128) % 16;
		y = (j % 128) / 16;
		r = (x*2-15) / 16.0;
		f = (-17-y*2) / 32.0;
		tc[0] = vc[0] + vr[0]*r + vf[0]*f;
		tc[1] = vc[1] + vr[1]*r + vf[1]*f;
		tc[2] = vc[2] + vr[2]*r + vf[2]*f;
		tr[0] = vr[0] / 16.3;
		tr[1] = vr[1] / 16.3;
		tr[2] = vr[2] / 16.3;
		tf[0] = vf[0] / 33;
		tf[1] = vf[1] / 33;
		tf[2] = vf[2] / 33;
		carvesolid2d_rect(win, bg, tc, tr, tf);

		tc[2] -= 0.01;
		tr[0] = vr[0] / 32.0;
		tr[1] = vr[1] / 32.0;
		tr[2] = vr[2] / 32.0;
		tf[0] = vf[0] / 32.0;
		tf[1] = vf[1] / 32.0;
		tf[2] = vf[2] / 32.0;
		carvestring2d_center(win, fg, tc, tr, tf, (u8*)&obj[j].type, 8);
	}

	//actor.irel
	for(j=0;j<128;j++)
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
				k %= 128;

				r = ((k%16)*4-29)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%16)*4-29)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%16)*4-29)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%16)*4-29)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//arena.irel
	for(j=0;j<128;j++)
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
				k %= 128;

				r = ((k%16)*4-29)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%16)*4-29)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%16)*4-29)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%16)*4-29)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffc0ff, tc, tr);
			}

			rel = samedstnextsrc(rel);
		}
	}

	//element.irel
	for(j=0;j<128;j++)
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
				k %= 128;

				r = ((k%16)*4-29)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%16)*4-29)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%16)*4-29)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%16)*4-29)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
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
				k %= 128;

				r = ((k%16)*4-29)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%16)*4-29)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xc0ffc0, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = vr[0]*((k%16)*4-29)/32.0;
				f = vf[1]*(15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffffff, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%16)*4-29)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f - 0.01;
				carveline2d_arrow(win, 0xffc0ff, tc, tr);
			}

			rel = samedstnextsrc(rel);
		}
	}
}
void overview_read_8bit(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{/*
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
*/
}
void overview_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
void overview_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j,k,x,y;
	int ww = ((win->stride)/2)&0xfffc;
	int hh = (win->height)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);
/*
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
*/
}
void overview_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void overview_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	if(win->fmt == _cli_)overview_read_cli(win, 0, act, pin);
	else if(win->fmt == _tui_)overview_read_tui(win, 0, act, pin);
	else if(win->fmt == _vbo_)overview_read_vbo(win, 0, act, pin);
	else if(win->fmt == _html_)overview_read_html(win, 0, act, pin);
	else if(win->fmt == _8bit_)overview_read_8bit(win, 0, act, pin);
	else overview_read_pixel(win, 0, act, pin);
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
		act_s = &actor[x0+(y0*16)];

		if((x0==x1)&&(y0==y1))
		{
			if(0 == act_s->type)return;
			actorcreate(0, act_s);
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1+(y1*16)];
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
		win_s = &arena[x0 + (y0-8)*16];

		if((x0==x1)&&(y0==y1))
		{
			y1 = y1-8;
			say("@arena:%d\n", (y1*16)+x1);
			//arenacreate(0,0);
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1 + (y1*16)];
			if(0 == act_d->type)return;

			arenaactor(win_s, act_d);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*16];
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
		ele_s = &ele[x0 + (y0-16)*16];
		if((x0==x1)&&(y0==y1))
		{
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1 + (y1*16)];
			relationcreate(act_d, 0, _act_, ele_s, 0, _art_);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*16];
			relationcreate(win_d, 0, _win_, ele_s, 0, _art_);
		}
		else if(y1 < 24)
		{
			ele_d = &ele[x1 + (y1-16)*16];
			relationcreate(ele_d, 0, _art_, ele_s, 0, _art_);
		}
		else if(y1 < 32)
		{
		}
	}
	else if(y0 < 32)
	{
		j = x0 + (y0-24)*16;
		for(;j<0x1000;j+=64)
		{
			obj_s = &obj[j];
			if(obj_s->type)break;
		}

		if((x0==x1)&&(y0==y1))
		{
		}
		else if(y1 < 8)
		{
			act_d = &actor[x1+(y1*16)];
			relationcreate(act_d, 0, _act_, obj_s, 0, _fd_);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*16];
			relationcreate(win_d, 0, _win_, obj_s, 0, _fd_);
		}
		else if(y1 < 24)
		{
			ele_d = &ele[x1 + (y1-16)*16];
			relationcreate(ele_d, 0, _art_, obj_s, 0, _fd_);
		}
		else if(y1 < 32)
		{
		}
	}
}
static int overview_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	int j, k;
	int x, y, id;
	int width = win->width;
	int height = win->height;

	j = (ev->what)&0xff;
	k = ((ev->what)>>8)&0xff;

	if(j == 'p')
	{
		x = (ev->why)&0xffff;
		y = ((ev->why)>>16)&0xffff;
		id = ((ev->why)>>48)&0xffff;
		if(id > 10)id = 10;

		if(0 == sty)
		{
			x = (x*16) / width;
			y = (y*32) / height;
		}
		else
		{
			x = 4 + 4 * (x - (sty->vc[0])) / (sty->vr[0]);
			y = 16 + 16 * (y - (sty->vc[1])) / (sty->vf[1]);
		}
		act->x0 = x;
		act->y0 = y;

		if('-' == k)
		{
			j = win->input[id].x0;
			k = win->input[id].y0;

			if(0 == sty)
			{
				j = (j*16) / width;
				k = (k*32) / height;
			}
			else
			{
				j = 4 + 4 * (j - (sty->vc[0])) / (sty->vr[0]);
				k = 16 + 16 * (k - (sty->vc[1])) / (sty->vf[1]);
			}

			if((j<0)|(j>=16))return 0;
			if((k<0)|(k>=32))return 0;
			if((x<0)|(x>=16))return 0;
			if((y<0)|(y>=32))return 0;

			overview_drag(win, j, k, x, y);
			return 1;
		}
	}/*
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
			return 1;
		}
		if(t[3] & joyl_right)
		{
			arenanext(win);
			return 1;
		}
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (short*)&ev->why;
		if(t[3] & joyr_down)
		{
			arenalogin(win);
			return 1;
		}
	}*/
	return 1;
}
static void overview_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void overview_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void overview_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void overview_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
    say("@overview_start\n");
}
void overview_delete()
{
}
void overview_create(void* addr)
{
    say("@overview_create\n");
}




void overview_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('o', 'v', 'e', 'r', 'v', 'i', 'e', 'w');

	p->oncreate = (void*)overview_create;
	p->ondelete = (void*)overview_delete;
	p->onstart  = (void*)overview_start;
	p->onstop   = (void*)overview_stop;
	p->onget    = (void*)overview_cread;
	p->onpost   = (void*)overview_cwrite;
	p->onread   = (void*)overview_sread;
	p->onwrite  = (void*)overview_swrite;
}
void overview_init(void* addr)
{
	obj = addr - 0x400000;
	ele = addr - 0x300000;
	arena = addr + 0x000000;
	actor = addr + 0x100000;
}
