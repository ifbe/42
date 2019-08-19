#include "libuser.h"
void* allocstyle();
void* allocstyle();
void draw8bit_rect(
	struct actor* win, u32 rgb,
	int x0, int y0, int x1, int y1);




static struct object* obj = 0;
static struct element* ele = 0;
static struct arena* arena = 0;
static struct actor* actor = 0;




void defaultstyle_2d(struct fstyle* sty, int w, int h, int d)
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

	sty->vt[0] = 0;
	sty->vt[1] = 0;
	sty->vt[2] = (w+h)/4;
}
void defaultstyle_2in3(struct fstyle* sty)
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

	sty->vt[0] = 0.0;
	sty->vt[1] = 0.0;
	sty->vt[2] = 1.0;
}
void defaultstyle_3d(struct fstyle* sty, int w, int h, int d)
{
	sty->vr[0] = w;
	sty->vr[1] = 0.0;
	sty->vr[2] = 0.0;

	sty->vf[0] = 0.0;
	sty->vf[1] = h;
	sty->vf[2] = 0.0;

	sty->vt[0] = 0.0;
	sty->vt[1] = 0.0;
	sty->vt[2] = d;

	sty->vc[0] = 0.0;
	sty->vc[1] = 0.0;
	sty->vc[2] = 0.0;
}
int arenaactor(struct actor* win, struct actor* ccc, struct actor* act, struct actor* tmp)
{
	int w,h;
	struct style* sty;
	struct style* pin;
	if(0 == win)return 0;

	sty = allocstyle();
	if(0 == sty)return 0;

	pin = allocstyle();
	if(0 == pin)return 0;

	switch(ccc->fmt)
	{
		case _bg3d_:
		case _fg3d_:
		case _ui3d_:
		{
			if(_vbo_ == win->fmt)defaultstyle_3d(&sty->f, w, h, (w+h)/2);
			else{
				w = win->width;
				h = win->height;
				defaultstyle_2d(&sty->f, w, h, (w+h)/2);
			}
			break;
		}
		case _bg2d_:
		case _fg2d_:
		case _ui2d_:
		{
			if(_vbo_ == win->fmt)defaultstyle_2in3(&sty->f);
			else{
				w = win->width;
				h = win->height;
				defaultstyle_2d(&sty->f, w, h, (w+h)/2);
			}
			break;
		}
	}

	actorcreate(0, act);
	relationcreate(act, pin, _act_, 0, ccc, sty, _win_, 0);
	return 0;
}




void overview_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	struct relation* rel;
	u32 c,cursor;
	int x,y,j,k;
	int x0,y0,x1,y1;
	int cx,cy,ww,hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	cursor = (act->x0) + (act->y0)*16;
/*
	drawline(win, 0x0000ff, 0, h*1/4, w-1, h*1/4);
	drawline(win, 0x00ff00, 0, h*2/4, w-1, h*2/4);
	drawline(win, 0xff0000, 0, h*3/4, w-1, h*3/4);
*/
	//actor
	for(j=0;j<128;j++)
	{
		c = actor[j].type & 0xff;
		if(0 == c)break;

		if(j == cursor)c = 0xffff00ff;
		else if((c >= 'a')&&(c <= 'z'))c = 0xc0808080;
		else c = 0xc0ffffff;

		x = j%16;
		y = j/16;
		x0 = (cx+1)+(x-8)*ww/8;
		y0 = (cy+1)+(y-16)*hh/16;
		x1 = (cx-1)+(x-7)*ww/8;
		y1 = (cy-1)+(y-15)*hh/16;
		drawhyaline_rect(win, c, x0, y0, x1, y1);
		drawstring_fit(win, 0xffffff, x0, y0, x1, y1, (u8*)&actor[j].fmt, 8);
	}

	//arena
	for(j=0;j<128;j++)
	{
		if(0 == arena[j].type)break;

		if(j+128 == cursor)c = 0xffff00ff;
		//else if(win == &arena[j])c = 0xff808080;
		else c = 0x80ffffff;

		x = j%16;
		y = j/16;
		x0 = (cx+1)+(x-8)*ww/8;
		y0 = (cy+1)+(y-8)*hh/16;
		x1 = (cx-1)+(x-7)*ww/8;
		y1 = (cy-1)+(y-7)*hh/16;
		drawhyaline_rect(win, c, x0, y0, x1, y1);
		drawstring_fit(win, 0xffffff, x0, y0, x1, y1, (u8*)&arena[j].fmt, 8);
	}

	//artery
	for(j=0;j<128;j++)
	{
		if(0 == ele[j].type)continue;

		if(j+256 == cursor)c = 0xffff00ff;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		x0 = (cx+1)+(x-8)*ww/8;
		y0 = (cy+1)+(y+0)*hh/16;
		x1 = (cx-1)+(x-7)*ww/8;
		y1 = (cy-1)+(y+1)*hh/16;
		drawhyaline_rect(win, c, x0, y0, x1, y1);
		drawstring_fit(win, 0xffffff, x0, y0, x1, y1, (u8*)&ele[j].type, 8);
	}

	//system
	for(j=0;j<0x1000;j++)
	{
		if(0 == obj[j].type)continue;

		if((j%128)+384 == cursor)c = 0xffff00ff;
		else c = 0x80ffffff;

		x = (j%128)%16;
		y = (j%128)/16;
		x0 = (cx+1)+(x-8)*ww/8;
		y0 = (cy+1)+(y+8)*hh/16;
		x1 = (cx-1)+(x-7)*ww/8;
		y1 = (cy-1)+(y+9)*hh/16;
		drawhyaline_rect(win, c, x0, y0, x1, y1);
		drawstring_fit(win, 0xffffff, x0, y0, x1, y1, (u8*)&obj[j].type, 8);
	}

	//actor.irel
	for(j=0;j<128;j++)
	{
		if(0 == actor[j].fmt)continue;

		rel = actor[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_fd_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct object);
				k %= 128;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-31)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
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
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-15)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-15)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-15)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffc0ff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-31)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-15)*hh/32
				);
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
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+ 1)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+ 1)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+ 1)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffc0ff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-31)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+ 1)*hh/32
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
				k %= 128;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);
				drawline_arrow(win, 0xffc0ff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-31)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			rel = samedstnextsrc(rel);
		}
	}
}
void overview_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
		sty->f.vc[0] = 0.0;
		sty->f.vc[1] = 0.0;
		sty->f.vc[2] = -0.5;
		sty->f.vr[0] = 1.0;
		sty->f.vr[1] = 0.0;
		sty->f.vr[2] = 0.0;
		sty->f.vf[0] = 0.0;
		sty->f.vf[1] = 1.0;
		sty->f.vf[2] = 0.0;
	}
	vc = sty->f.vc;
	vr = sty->f.vr;
	vf = sty->f.vf;
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
			if((k >= 'a')&&(k <= 'z'))fg = 0xc0c0c0;
			else fg = 0x00ffff;
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

		tc[2] += 0.01;
		tr[0] = vr[0] / 32;
		tr[1] = vr[1] / 32;
		tr[2] = vr[2] / 32;
		tf[0] = vf[0] / 32;
		tf[1] = vf[1] / 32;
		tf[2] = vf[2] / 32;
		carvestring2d_center(win, fg, tc, tr, tf, (u8*)&actor[j].fmt, 8);
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
			//if(win == &arena[j])fg = 0xffff00ff;
			//else fg = 0x80ffffff;
			fg = 0x80ffffff;
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

		tc[2] += 0.01;
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

		tc[2] += 0.01;
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

		tc[2] += 0.01;
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
		if(0 == actor[j].fmt)continue;

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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0x0000ff, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%16)*4-29)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0x00ffff, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%16)*4-29)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0xffff00, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%16)*4-29)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0xff0000, tc, tr);
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0x4040ff, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct element);

				r = ((k%16)*4-29)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0x40ffff, tc, tr);
			}
			else if(_win_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)arena;
				k = k / sizeof(struct arena);

				r = ((k%16)*4-29)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0xffff40, tc, tr);
			}
			else if(_act_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)actor;
				k = k / sizeof(struct actor);

				r = ((k%16)*4-29)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0xff4040, tc, tr);
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
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
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-31)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				carveline2d_arrow(win, 0xffc0ff, tc, tr);
			}

			rel = samedstnextsrc(rel);
		}
	}
}
void overview_draw_8bit(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
void overview_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
void overview_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int j,k,x,y;
	int ww = ((win->stride)/2)&0xfffc;
	int hh = (win->height)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);
/*
	for(j=0;j<64;j++)
	{
		if(0 == actor[j].fmt)break;

		if(j == win->forez)k=1;
		else k=2;

		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&actor[j].fmt, 8);
	}
*/
}
void overview_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void overview_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	if(win->fmt == _cli_)overview_draw_cli(act, pin, win, 0);
	else if(win->fmt == _tui_)overview_draw_tui(act, pin, win, 0);
	else if(win->fmt == _vbo_)overview_draw_vbo(act, pin, win, 0);
	else if(win->fmt == _html_)overview_draw_html(act, pin, win, 0);
	else if(win->fmt == _8bit_)overview_draw_8bit(act, pin, win, 0);
	else overview_draw_pixel(act, pin, win, 0);
}




void overview_drag(struct actor* win, int x0, int y0, int x1, int y1)
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
			if((act_d->type) && (act_s->type)){
				relationcreate(
					act_d, 0, _act_, 0,
					act_s, 0, _act_, 0
				);
			}
			else if(0 == act_s->type)actorcreate(act_d->fmt, 0);
			else if(0 == act_d->type)actorcreate(act_s->fmt, 0);
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

			//arenaactor(win, win_s, act_d, 0);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*16];
			if((win_d->type) && (win_s->type)){
				relationcreate(
					win_d, 0, _win_, 0,
					win_s, 0, _win_, 0
				);
			}
			else if(win_s->type){
				if(_win_ == win_s->type)win_d = arenacreate(_coop_, win_s);
			}
			else if(win_d->type){
				if(_win_ == win_s->type)win_s = arenacreate(_coop_, win_d);
			}
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
			relationcreate(act_d, 0, _act_, 0, ele_s, 0, _art_, 0);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*16];
			relationcreate(win_d, 0, _win_, 0, ele_s, 0, _art_, 0);
		}
		else if(y1 < 24)
		{
			ele_d = &ele[x1 + (y1-16)*16];
			relationcreate(ele_d, 0, _art_, 0, ele_s, 0, _art_, 0);
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
			relationcreate(act_d, 0, _act_, 0, obj_s, 0, _fd_, 0);
		}
		else if(y1 < 16)
		{
			win_d = &arena[x1 + (y1-8)*16];
			relationcreate(win_d, 0, _win_, 0, obj_s, 0, _fd_, 0);
		}
		else if(y1 < 24)
		{
			ele_d = &ele[x1 + (y1-16)*16];
			relationcreate(ele_d, 0, _art_, 0, obj_s, 0, _fd_, 0);
		}
		else if(y1 < 32)
		{
		}
	}
}
static int overview_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
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
			x = 4 + 4 * (x - (sty->f.vc[0])) / (sty->f.vr[0]);
			y = 16 + 16 * (y - (sty->f.vc[1])) / (sty->f.vf[1]);
		}
		act->x0 = x;
		act->y0 = y;
/*
		if('-' == k)
		{
			j = win->mouse[id].x0;
			k = win->mouse[id].y0;

			if(0 == sty)
			{
				j = (j*16) / width;
				k = (k*32) / height;
			}
			else
			{
				j = 4 + 4 * (j - (sty->f.vc[0])) / (sty->f.vr[0]);
				k = 16 + 16 * (k - (sty->f.vc[1])) / (sty->f.vf[1]);
			}

			if((j<0)|(j>=16))return 0;
			if((k<0)|(k>=32))return 0;
			if((x<0)|(x>=16))return 0;
			if((y<0)|(y>=32))return 0;

			overview_drag(win, j, k, x, y);
			return 1;
		}
*/
	}
/*
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




static void overview_read(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	overview_draw(act, pin, win, sty);
}
static int overview_write(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return overview_event(act, pin, win, sty, ev, 0);
}
static void overview_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void overview_start(struct halfrel* self, struct halfrel* peer)
{
    say("@overview_start\n");
}




void overview_search(struct actor* act)
{
}
void overview_modify(struct actor* act)
{
}
void overview_delete(struct actor* act)
{
}
void overview_create(struct actor* act, void* str)
{
    say("@overview_create\n");
}




void overview_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 'v', 'e', 'r', 'v', 'i', 'e', 'w');

	p->oncreate = (void*)overview_create;
	p->ondelete = (void*)overview_delete;
	p->onsearch = (void*)overview_search;
	p->onmodify = (void*)overview_modify;

	p->onstart = (void*)overview_start;
	p->onstop  = (void*)overview_stop;
	p->onread  = (void*)overview_read;
	p->onwrite = (void*)overview_write;
}
void overview_init(void* addr)
{
	obj = addr - 0x400000;
	ele = addr - 0x300000;
	arena = addr + 0x000000;
	actor = addr + 0x100000;
}
