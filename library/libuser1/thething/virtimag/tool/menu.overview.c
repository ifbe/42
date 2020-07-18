#include "libuser.h"
void gl41data_before(struct entity* wnd);
void gl41data_after(struct entity* wnd);
void gl41data_01cam(struct entity* wnd);
void gl41data_convert(struct entity* wnd, struct style* area, struct event* ev, vec3 v);
void draw8bit_rect(
	struct entity* win, u32 rgb,
	int x0, int y0, int x1, int y1);




static struct origin* ori = 0;
static struct worker* wrk = 0;
static struct device* dev = 0;
static struct driver* dri = 0;
static struct sysobj* obj = 0;
static struct artery* ele = 0;
static struct supply* supply = 0;
static struct entity* entity = 0;




void overview_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	struct relation* rel;
	u32 c,cursor;
	int x,y,j,k;
	int x0,y0,x1,y1;
	int cx,cy,ww,hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	cursor = (act->ix0) + (act->iy0)*16;
/*
	drawline(win, 0x0000ff, 0, h*1/4, w-1, h*1/4);
	drawline(win, 0x00ff00, 0, h*2/4, w-1, h*2/4);
	drawline(win, 0xff0000, 0, h*3/4, w-1, h*3/4);
*/
	//entity
	for(j=0;j<128;j++)
	{
		c = entity[j].type & 0xff;
		if(0 == c)break;

		if(j == cursor)c = 0xffff00ff;
		else c = 0x80ffffff;

		x = j%16;
		y = j/16;
		x0 = (cx+1)+(x-8)*ww/8;
		y0 = (cy+1)+(y-16)*hh/16;
		x1 = (cx-1)+(x-7)*ww/8;
		y1 = (cy-1)+(y-15)*hh/16;
		drawopaque_rect(win, c, x0, y0, x1, y1);
		drawstring_fit(win, 0xffffff, x0, y0, x1, y1, (u8*)&entity[j].fmt, 8);
	}

	//supply
	for(j=0;j<128;j++)
	{
		if(0 == supply[j].type)break;

		if(j+128 == cursor)c = 0xffff00ff;
		//else if(win == &supply[j])c = 0xff808080;
		else c = 0x80ffffff;

		x = j%16;
		y = j/16;
		x0 = (cx+1)+(x-8)*ww/8;
		y0 = (cy+1)+(y-8)*hh/16;
		x1 = (cx-1)+(x-7)*ww/8;
		y1 = (cy-1)+(y-7)*hh/16;
		drawopaque_rect(win, c, x0, y0, x1, y1);
		drawstring_fit(win, 0xffffff, x0, y0, x1, y1, (u8*)&supply[j].fmt, 8);
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
		drawopaque_rect(win, c, x0, y0, x1, y1);
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
		drawopaque_rect(win, c, x0, y0, x1, y1);
		drawstring_fit(win, 0xffffff, x0, y0, x1, y1, (u8*)&obj[j].type, 8);
	}

	//entity.irel
	for(j=0;j<128;j++)
	{
		if(0 == entity[j].fmt)continue;

		rel = entity[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-31)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-31)*hh/32
				);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//supply.irel
	for(j=0;j<128;j++)
	{
		if(0 == supply[j].type)break;

		rel = supply[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-15)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-15)*hh/32
				);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)-15)*hh/32
				);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);
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
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+ 1)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+ 1)*hh/32
				);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+ 1)*hh/32
				);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);
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
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+17)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);
				drawline_arrow(win, 0xc0ffc0,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)+ 1)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);
				drawline_arrow(win, 0xffffff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-15)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);
				drawline_arrow(win, 0xffc0ff,
					cx+(4*(k%16)-29)*ww/32, cy+(2*(k/16)-31)*hh/32,
					cx+(4*(j%16)-31)*ww/32, cy+(2*(j/16)+17)*hh/32
				);
			}
			rel = samedstnextsrc(rel);
		}
	}
}
void overview_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	u32 bg,fg,cursor;
	float r,f,t;
	int x,y,j,k;
	struct relation* rel;
	vec3 tc;
	vec3 tr;
	vec3 tf;

	float* vc;
	float* vr;
	float* vf;
	float* vt;
	struct style tmp;
	if(0 == geom)
	{
		geom = &tmp;
		geom->fshape.vc[0] = 0.0;
		geom->fshape.vc[1] = 0.0;
		geom->fshape.vc[2] = -0.5;
		geom->fshape.vr[0] = 1.0;
		geom->fshape.vr[1] = 0.0;
		geom->fshape.vr[2] = 0.0;
		geom->fshape.vf[0] = 0.0;
		geom->fshape.vf[1] = 1.0;
		geom->fshape.vf[2] = 0.0;
		geom->fshape.vt[0] = 0.0;
		geom->fshape.vt[1] = 0.0;
		geom->fshape.vt[2] = 1.0;
	}
	vc = geom->fshape.vc;
	vr = geom->fshape.vr;
	vf = geom->fshape.vf;
	vt = geom->fshape.vt;
	cursor = (act->ix0) + (act->iy0)*16;

/*
	tc[0] = -1.0;
	tc[1] = 0.5;
	tc[2] = 0.0;
	tr[0] = 1.0;
	tr[1] = 0.5;
	tr[2] = 0.0;
	gl41line2d(ctx, 0x0000ff, tc, tr);
	tc[0] = -1.0;
	tc[1] = 0.0;
	tc[2] = 0.0;
	tr[0] = 1.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	gl41line2d(ctx, 0x00ff00, tc, tr);
	tc[0] = -1.0;
	tc[1] = -0.5;
	tc[2] = 0.0;
	tr[0] = 1.0;
	tr[1] = -0.5;
	tr[2] = 0.0;
	gl41line2d(ctx, 0xff0000, tc, tr);
*/
	//entity
	for(j=0;j<128;j++)
	{
		k = entity[j].fmt;
		if(0 == k)break;

		if(j == cursor)
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
		gl41solid_rect(ctx, bg, tc, tr, tf);
		tc[0] += vt[0]*0.01;
		tc[1] += vt[1]*0.01;
		tc[2] += vt[2]*0.01;
		gl41string_center(ctx, fg, tc, tr, tf, (u8*)&entity[j].fmt, 8);
	}

	//supply
	for(j=0;j<128;j++)
	{
		k = supply[j].type;
		if(0 == k)break;
		if(j+128 == cursor)
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
		gl41solid_rect(ctx, bg, tc, tr, tf);
		tc[0] += vt[0]*0.01;
		tc[1] += vt[1]*0.01;
		tc[2] += vt[2]*0.01;
		gl41string_center(ctx, fg, tc, tr, tf, (u8*)&supply[j].fmt, 8);
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
		gl41solid_rect(ctx, bg, tc, tr, tf);
		tc[0] += vt[0]*0.01;
		tc[1] += vt[1]*0.01;
		tc[2] += vt[2]*0.01;
		gl41string_center(ctx, fg, tc, tr, tf, (u8*)&ele[j].type, 8);
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
		gl41solid_rect(ctx, bg, tc, tr, tf);
		tc[0] += vt[0]*0.01;
		tc[1] += vt[1]*0.01;
		tc[2] += vt[2]*0.01;
		gl41string_center(ctx, fg, tc, tr, tf, (u8*)&obj[j].type, 8);
	}

	//entity.irel
	for(j=0;j<128;j++)
	{
		if(0 == entity[j].fmt)continue;

		rel = entity[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;

				r = ((k%16)*4-30)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0x0000ff, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);

				r = ((k%16)*4-30)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0x00ffff, tc, tr);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);

				r = ((k%16)*4-30)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xffff00, tc, tr);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);

				r = ((k%16)*4-30)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (31-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xff0000, tc, tr);
			}
			rel = samedstnextsrc(rel);
		}
	}

	//supply.irel
	for(j=0;j<128;j++)
	{
		if(0 == supply[j].type)break;

		rel = supply[j].irel0;
		while(1)
		{
			if(0 == rel)break;
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;

				r = ((k%16)*4-30)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0x4040ff, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);

				r = ((k%16)*4-30)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0x40ffff, tc, tr);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);

				r = ((k%16)*4-30)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xffff40, tc, tr);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);

				r = ((k%16)*4-30)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (15-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xff4040, tc, tr);
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
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;

				r = ((k%16)*4-30)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);

				r = ((k%16)*4-30)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xc0ffc0, tc, tr);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);

				r = ((k%16)*4-30)/32.0;
				f = (15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xffffff, tc, tr);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);

				r = ((k%16)*4-30)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-1-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xffc0ff, tc, tr);
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
			if(_sys_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)obj;
				k = k / sizeof(struct sysobj);
				k %= 128;

				r = ((k%16)*4-30)/32.0;
				f = (-17-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xc0ffc0, tc, tr);
			}
			else if(_art_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)ele;
				k = k / sizeof(struct artery);

				r = ((k%16)*4-30)/32.0;
				f = (-1-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xc0ffc0, tc, tr);
			}
			else if(_sup_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)supply;
				k = k / sizeof(struct supply);

				r = vr[0]*((k%16)*4-30)/32.0;
				f = vf[1]*(15-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xffffff, tc, tr);
			}
			else if(_ent_ == rel->srctype)
			{
				k = (void*)(rel->srcchip) - (void*)entity;
				k = k / sizeof(struct entity);

				r = ((k%16)*4-30)/32.0;
				f = (31-(k/16)*2)/32.0;
				tc[0] = vc[0] + vr[0]*r + vf[0]*f;
				tc[1] = vc[1] + vr[1]*r + vf[1]*f;
				tc[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;

				r = ((j%16)*4-30)/32.0;
				f = (-17-(j/16)*2)/32.0;
				tr[0] = vc[0] + vr[0]*r + vf[0]*f;
				tr[1] = vc[1] + vr[1]*r + vf[1]*f;
				tr[2] = vc[2] + vr[2]*r + vf[2]*f + 0.01;
				gl41line_shorter(ctx, 0xffc0ff, tc, tr);
			}

			rel = samedstnextsrc(rel);
		}
	}
}
void overview_draw_8bit(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void overview_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j,k,x,y;
	int ww = ((win->width)/2)&0xfffc;
	int hh = (win->height)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);
/*
	for(j=0;j<64;j++)
	{
		if(0 == entity[j].fmt)break;

		if(j == win->forez)k=1;
		else k=2;

		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&entity[j].fmt, 8);
	}
*/
}
void overview_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




void overview_drag(struct entity* win, int x0, int y0, int x1, int y1)
{
	int j;
	struct sysobj* obj_s;
	struct sysobj* obj_d;
	struct artery* ele_s;
	struct artery* ele_d;
	struct supply* win_s;
	struct supply* win_d;
	struct entity* act_s;
	struct entity* act_d;
	say("(%d,%d)->(%d,%d)\n", x0, y0, x1, y1);

	if(y0 < 8)
	{
		act_s = &entity[x0+(y0*16)];

		if((x0==x1)&&(y0==y1))
		{
			if(0 == act_s->type)return;
			entitycreate(0, act_s, 0, 0);
		}
		else if(y1 < 8)
		{
			act_d = &entity[x1+(y1*16)];
			if((act_d->type) && (act_s->type)){
				relationcreate(
					act_d, 0, _ent_, 0,
					act_s, 0, _ent_, 0
				);
			}
			else if(0 == act_s->type)entitycreate(act_d->fmt, 0, 0, 0);
			else if(0 == act_d->type)entitycreate(act_s->fmt, 0, 0, 0);
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
		win_s = &supply[x0 + (y0-8)*16];

		if((x0==x1)&&(y0==y1))
		{
			y1 = y1-8;
			say("@supply:%d\n", (y1*16)+x1);
			//supplycreate(0,0);
		}
		else if(y1 < 8)
		{
			act_d = &entity[x1 + (y1*16)];
			if(0 == act_d->type)return;

			//supplyentity(win, win_s, act_d, 0);
		}
		else if(y1 < 16)
		{
			win_d = &supply[x1 + (y1-8)*16];
			if((win_d->type) && (win_s->type)){
				relationcreate(
					win_d, 0, _sup_, 0,
					win_s, 0, _sup_, 0
				);
			}
			else if(win_s->type){
				if(_sup_ == win_s->type)win_d = supplycreate(_gl41coop_, win_s, 0, 0);
			}
			else if(win_d->type){
				if(_sup_ == win_s->type)win_s = supplycreate(_gl41coop_, win_d, 0, 0);
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
			act_d = &entity[x1 + (y1*16)];
			relationcreate(act_d, 0, _ent_, 0, ele_s, 0, _art_, 0);
		}
		else if(y1 < 16)
		{
			win_d = &supply[x1 + (y1-8)*16];
			relationcreate(win_d, 0, _sup_, 0, ele_s, 0, _art_, 0);
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
			act_d = &entity[x1+(y1*16)];
			relationcreate(act_d, 0, _ent_, 0, obj_s, 0, _sys_, 0);
		}
		else if(y1 < 16)
		{
			win_d = &supply[x1 + (y1-8)*16];
			relationcreate(win_d, 0, _sup_, 0, obj_s, 0, _sys_, 0);
		}
		else if(y1 < 24)
		{
			ele_d = &ele[x1 + (y1-16)*16];
			relationcreate(ele_d, 0, _art_, 0, obj_s, 0, _sys_, 0);
		}
		else if(y1 < 32)
		{
		}
	}
}
static int overview_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
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
			x = 4 + 4 * (x - (sty->fs.vc[0])) / (sty->fs.vr[0]);
			y = 16 + 16 * (y - (sty->fs.vc[1])) / (sty->fs.vf[1]);
		}
		act->ix0 = x;
		act->iy0 = y;
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
				j = 4 + 4 * (j - (sty->fs.vc[0])) / (sty->fs.vr[0]);
				k = 16 + 16 * (k - (sty->fs.vc[1])) / (sty->fs.vf[1]);
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
			supplylogin(win);
		}
		else if(0x435b1b == ev->why)
		{
			supplynext(win);
		}
		else if(0x445b1b == ev->why)
		{
			supplyprev(win);
		}
	}
	else if(_kbd_ == ev->what)
	{
		if(0x4b == ev->why)
		{
			supplyprev(win);
		}
		else if(0x4d == ev->why)
		{
			supplynext(win);
		}
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		t = (short*)&ev->why;
		if(t[3] & joyl_left)
		{
			supplyprev(win);
			return 1;
		}
		if(t[3] & joyl_right)
		{
			supplynext(win);
			return 1;
		}
	}
	else if(joy_right == (ev->what & joy_mask))
	{
		t = (short*)&ev->why;
		if(t[3] & joyr_down)
		{
			supplylogin(win);
			return 1;
		}
	}*/
	return 1;
}




static void overview_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		overview_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void overview_read_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;
	gl41data_before(wnd);
	overview_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);

	gl41data_01cam(wnd);
}
static void overview_write_bywnd(_ent* ent,struct style* slot, _ent* wnd,struct style* area, struct event* ev)
{
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		//say("%f,%f\n",xyz[0],xyz[1]);

		ent->ix0 = (int)(16*xyz[0]);
		ent->iy0 = (int)(32*(1.0-xyz[1]));
		say("%d,%d\n",ent->ix0,ent->iy0);

		if(0x2d70 == ev->what){
		}
	}
}




static int overview_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41full_:{
		if('v' != key)break;
		overview_read_bywnd(ent,slot, wnd,area);break;
	}
	default:overview_read_bycam(ent,foot, stack,sp, arg,key);break;
	}
	return 0;
}
static int overview_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//struct entity* ent = stack[sp-1].pchip;
	struct style* slot = stack[sp-1].pfoot;
	struct entity* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->fmt){
	case _gl41full_:overview_write_bywnd(ent,slot, wnd,area, buf);break;
	}
	return 0;
}
static void overview_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void overview_linkup(struct halfrel* self, struct halfrel* peer)
{
    say("@overview_linkup\n");
}




void overview_search(struct entity* act)
{
}
void overview_modify(struct entity* act)
{
}
void overview_delete(struct entity* act)
{
}
void overview_create(struct entity* act, void* str)
{
    say("@overview_create\n");
}




void overview_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 'v', 'e', 'r', 'v', 'i', 'e', 'w');

	p->oncreate = (void*)overview_create;
	p->ondelete = (void*)overview_delete;
	p->onsearch = (void*)overview_search;
	p->onmodify = (void*)overview_modify;

	p->onlinkup = (void*)overview_linkup;
	p->ondiscon = (void*)overview_discon;
	p->ontaking = (void*)overview_taking;
	p->ongiving = (void*)overview_giving;
}
void overview_init(void* addr)
{
	addr -= 0xe00000;

	ori = addr + 0x000000;
	wrk = addr + 0x200000;

	dev = addr + 0x400000;
	dri = addr + 0x600000;

	obj = addr + 0x800000;
	ele = addr + 0xa00000;

	supply = addr + 0xc00000;
	entity = addr + 0xe00000;
}
