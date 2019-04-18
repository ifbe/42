#include "libuser.h"




void drawarena(struct arena* win, int val, int x0, int y0, int xn, int yn)
{
	void* buf;
	int x,y;
	int xa,ya,xb,yb;
	drawline_rect(win, 0xc0c0c0, x0, y0, xn, yn);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			xa = ((4-x)*x0 + (x+0)*xn)/4;
			ya = ((4-y)*y0 + (y+0)*yn)/4;
			xb = ((3-x)*x0 + (x+1)*xn)/4;
			yb = ((3-y)*y0 + (y+1)*yn)/4;
			drawsolid_rect(win, 0x404040, xa+2, ya+2, xb-2, yb-2);

			if(y)continue;
			switch(x)
			{
				case 0:buf = &win->tier;break;
				case 1:buf = &win->type;break;
				case 2:buf = &win->fmt; break;
				case 3:buf = &win->vfmt;break;
			}
			drawstring_fit(win, 0xffffff, xa, ya, xb, yb, buf, 8);
		}
	}
}
void carvearena(struct arena* win, int val, vec3 vc, vec3 vr, vec3 vf)
{
	int x,y;
	void* buf;
	vec3 tc,tr,tf,tu;
	carveline2d_rect(win, 0xffffff, vc, vr, vf);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			tc[0] = vc[0] + (x-1.5)*vr[0]/2 + (1.5-y)*vf[0]/2;
			tc[1] = vc[1] + (x-1.5)*vr[1]/2 + (1.5-y)*vf[1]/2;
			tc[2] = -0.7;
			tr[0] = vr[0]/4.1;
			tr[1] = 0.0;
			tr[2] = 0.0;
			tf[0] = 0.0;
			tf[1] = vf[1]/4.1;
			tf[2] = 0.0;
			carvesolid2d_rect(win, 0xffffff, tc, tr, tf);

			if(y)continue;
			switch(x)
			{
				case 0:buf = &win->tier;break;
				case 1:buf = &win->type;break;
				case 2:buf = &win->fmt; break;
				case 3:buf = &win->vfmt;break;
			}

			tc[2] = -0.8;
			tr[0] = vr[0]/4;
			tr[1] = 0.0;
			tr[2] = 0.0;
			tf[0] = 0.0;
			tf[1] = vf[1]/4;
			tf[2] = 0.0;
			carvestring2d_center(win, 0x000000, tc, tr, tf, buf, 8);
		}
	}
}




int actoroutput_detail_vbo(struct arena* win, struct style* sty)
{
	int j;
	struct relation* rel;
	struct actor* act;
	vec3 tc, tr, tf;

	float* vc;
	float* vr;
	float* vf;
	struct style tmp;

	//1. prep
	if(0 == sty)
	{
		sty = &tmp;
		sty->vc[0] = 0.0;
		sty->vc[1] = 0.0;
		sty->vc[2] = -0.9;
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

	//2. body
	tr[0] = vr[0]/2;
	tr[1] = vr[1]/2;
	tr[2] = vr[2]/2;
	tf[0] = vf[0]/2;
	tf[1] = vf[1]/2;
	tf[2] = vf[2]/2;
	carvearena(win, 0, vc, tr, tf);

	//3. irel
	j = 0;
	rel = win->irel0;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		//item
		tr[0] = 2.0/17;
		tr[1] = 0.0;
		tr[2] = 0.0;
		tf[0] = 0.0;
		tf[1] = 1.0/17;
		tf[2] = 0.0;
		tc[0] = -7.0/8;
		tc[1] = (7-j*2)/16.0;
		tc[2] = -0.5;
		carvesolid2d_rect(win, 0xffffff, tc, tr, tf);

		//name
		tc[2] = -0.6;
		if(_act_ == rel->srctype)
		{
			act = (void*)(rel->srcchip);
			carvestring2d_center(win, 0, tc, tr, tf, (void*)(&act->fmt), 0);
		}

		//arrow
		tc[0] = -6.0/8;
		tc[1] = (7-j*2)/16.0;
		tc[2] = -0.5;
		tr[0] = -4.0/8;
		tr[1] = (7-j*2)/16.0;
		tr[2] = -0.5;
		carveline2d_arrow(win, 0x00ff00, tc, tr);

		//foot
		tc[0] = -23.0/32;
		tc[1] = (7-j*2)/16.0;
		tc[2] = -0.5;
		tr[0] = 1.0/16;
		tr[1] = 0;
		tr[2] = 0;
		tf[0] = 0;
		tf[1] = 1.0/16;
		tf[2] = 0;
		carve2d_ascii(win, 0x00ff00, tc, tr, tf, 'o');
		tc[0] = -19.0/32;
		carve2d_ascii(win, 0x00ff00, tc, tr, tf, 'i');

		j++;
		rel = samedstnextsrc(rel);
	}

	//4. orel
	j = 0;
	rel = win->orel0;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		//item
		tr[0] = 2.0/17;
		tr[1] = 0.0;
		tr[2] = 0.0;
		tf[0] = 0.0;
		tf[1] = 1.0/17;
		tf[2] = 0.0;
		tc[0] = 7.0/8;
		tc[1] = (7-j*2)/16.0;
		tc[2] = -0.5;
		carvesolid2d_rect(win, 0xffffff, tc, tr, tf);

		//name
		tc[2] = -0.6;
		if(_act_ == rel->dsttype)
		{
			act = (void*)(rel->dstchip);
			carvestring2d_center(win, 0, tc, tr, tf, (void*)(&act->fmt), 0);
		}

		//arrow
		tc[0] = 4.0/8;
		tc[1] = (7-j*2)/16.0;
		tc[2] = -0.5;
		tr[0] = 6.0/8;
		tr[1] = (7-j*2)/16.0;
		tr[2] = -0.5;
		carveline2d_arrow(win, 0x00ff00, tc, tr);

		//foot
		tc[0] = 19.0/32;
		tc[1] = (7-j*2)/16.0;
		tc[2] = -0.5;
		tr[0] = 1.0/16;
		tr[1] = 0;
		tr[2] = 0;
		tf[0] = 0;
		tf[1] = 1.0/16;
		tf[2] = 0;
		carve2d_ascii(win, 0x00ff00, tc, tr, tf, 'o');
		tc[0] = 23.0/32;
		carve2d_ascii(win, 0x00ff00, tc, tr, tf, 'i');

		j++;
		rel = samesrcnextdst(rel);
	}

	return 0;
}
int actoroutput_detail_pixel(struct arena* win, struct style* sty)
{
	struct relation* rel;
	struct actor* act;
	int cx,cy,ww,hh, j;

	//1. prep
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

	//2. body
	drawarena(win, 0, cx-ww/2, cy-hh/2, cx+ww/2, cy+hh/2);

	//3. irel
	j = 0;
	rel = win->irel0;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		//ichip
		drawsolid_rect(win, 0xffffff, cx-ww+2, cy+hh*(j-4)/8+2, cx-ww*3/4-2, cy+hh*(j-3)/8-2);
		if(_act_ == rel->srctype)
		{
			act = (void*)(rel->srcchip);
			drawstring_fit(win, 0x000000, cx-ww, cy+hh*(j-4)/8, cx-ww*3/4, cy+hh*(j-3)/8, (void*)(&act->fmt), 0);
		}

		//ofoot, ifoot
		drawline_arrow(win,0x00ff00, cx-ww*6/8, cy+hh*(j+j-7)/16, cx-ww*4/8, cy+hh*(j+j-7)/16);
		drawascii_fit(win, 0x00ff00, cx-ww*6/8, cy+hh*(j+j-8)/16, cx-ww*5/8, cy+hh*(j+j-6)/16, 'o');
		drawascii_fit(win, 0x00ff00, cx-ww*5/8, cy+hh*(j+j-8)/16, cx-ww*4/8, cy+hh*(j+j-6)/16, 'i');

		j++;
		rel = samedstnextsrc(rel);
	}

	//4. orel
	j = 0;
	rel = win->orel0;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		//ochip
		drawsolid_rect(win, 0xffffff, cx+ww*3/4+2, cy+hh*(j-4)/8+2, cx+ww-2, cy+hh*(j-3)/8-2);
		if(_act_ == rel->dsttype)
		{
			act = (void*)(rel->dstchip);
			drawstring_fit(win, 0x000000, cx+ww*3/4, cy+hh*(j-4)/8, cx+ww, cy+hh*(j-3)/8, (void*)(&act->fmt), 0);
		}

		//ofoot, ifoot
		drawline_arrow(win,0x00ff00, cx+ww*4/8, cy+hh*(j+j-7)/16, cx+ww*6/8, cy+hh*(j+j-7)/16);
		drawascii_fit(win, 0x00ff00, cx+ww*4/8, cy+hh*(j+j-8)/16, cx+ww*5/8, cy+hh*(j+j-6)/16, 'o');
		drawascii_fit(win, 0x00ff00, cx+ww*5/8, cy+hh*(j+j-8)/16, cx+ww*6/8, cy+hh*(j+j-6)/16, 'i');

		j++;
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int actoroutput_detail(struct arena* win, struct style* sty)
{
	if(_vbo_ == win->fmt)actoroutput_detail_vbo(win, sty);
	else actoroutput_detail_pixel(win, sty);
	return 0;
}




int actorinput_detail(struct arena* win, struct style* sty, struct event* ev)
{
	return 0;
}
