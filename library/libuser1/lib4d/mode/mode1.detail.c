#include "actor.h"




int actoroutput_detail_vbo(struct arena* win, struct style* sty)
{
	vec3 vc;
	vec3 vr;
	vec3 vf;
	int j;
	struct relation* rel;

	vc[0] = 0.0;
	vc[1] = 0.0;
	vc[2] = -0.5;
	vr[0] = 0.25;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 0.5;
	vf[2] = 0.0;
	carveline2d_rect(win, 0xffffff, vc, vr, vf);

	vr[0] = 0.124;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 1.0/17;
	vf[2] = 0.0;

	vc[0] = -0.125;
	vc[1] = 7.0/16;
	vc[2] = -0.5;
	carvesolid2d_rect(win, 0xffffff, vc, vr, vf);
	vc[2] = -0.6;
	carvestring2d_center(win, 0, vc, vr, vf, (void*)&win->tier, 4);

	vc[0] = 0.125;
	vc[1] = 7.0/16;
	vc[2] = -0.5;
	carvesolid2d_rect(win, 0xffffff, vc, vr, vf);
	vc[2] = -0.6;
	carvestring2d_center(win, 0, vc, vr, vf, (void*)&win->type, 4);

	vc[0] = 0.0;
	vc[1] = 5.0/16;
	vc[2] = -0.5;
	carvesolid2d_rect(win, 0xffffff, vc, vr, vf);
	vc[2] = -0.6;
	carvestring2d_center(win, 0, vc, vr, vf, (void*)&win->fmt, 8);


	j = 0;
	rel = win->irel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		vr[0] = 2.0/17;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 1.0/17;
		vf[2] = 0.0;

		vc[0] = -7.0/8;
		vc[1] = (7-j*2)/16.0;
		vc[2] = -0.5;
		carvesolid2d_rect(win, 0xffffff, vc, vr, vf);
		vc[2] = -0.6;
		carvestring2d_center(win, 0, vc, vr, vf, (void*)"123", 0);

		vc[0] = -9.0/16;
		vc[1] = (7-j*2)/16.0;
		vc[2] = -0.5;
		vr[0] = 1.0/8;
		vf[1] = 1.0/17;
		carve2d_ascii(win, 0x00ff00, vc, vr, vf, 'o');
		vc[0] = -5.0/16;
		carve2d_ascii(win, 0x00ff00, vc, vr, vf, 'i');

		vc[0] = -6.0/8;
		vc[1] = (7-j*2)/16.0;
		vc[2] = -0.5;
		vr[0] = -2.0/8;
		vr[1] = (7-j*2)/16.0;
		vr[2] = -0.5;
		carveline2d_arrow(win, 0x00ff00, vc, vr);

		j++;
		rel = samedstnextsrc(rel);
	}

	j = 0;
	rel = win->orel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		vr[0] = 2.0/17;
		vr[1] = 0.0;
		vr[2] = 0.0;
		vf[0] = 0.0;
		vf[1] = 1.0/17;
		vf[2] = 0.0;

		vc[0] = 7.0/8;
		vc[1] = (7-j*2)/16.0;
		vc[2] = -0.5;
		carvesolid2d_rect(win, 0xffffff, vc, vr, vf);
		vc[2] = -0.6;
		carvestring2d_center(win, 0, vc, vr, vf, (void*)"123", 0);

		vc[0] = 7.0/16;
		vc[1] = (7-j*2)/16.0;
		vc[2] = -0.5;
		vr[0] = 1.0/8;
		vf[1] = 1.0/17;
		carve2d_ascii(win, 0x00ff00, vc, vr, vf, 'o');
		vc[0] = 11.0/16;
		carve2d_ascii(win, 0x00ff00, vc, vr, vf, 'i');

		vc[0] = 2.0/8;
		vc[1] = (7-j*2)/16.0;
		vc[2] = -0.5;
		vr[0] = 6.0/8;
		vr[1] = (7-j*2)/16.0;
		vr[2] = -0.5;
		carveline2d_arrow(win, 0x00ff00, vc, vr);

		j++;
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int actoroutput_detail_pixel(struct arena* win, struct style* sty)
{
	struct relation* rel;
	int j,m,n;
	int w = win->width;
	int h = win->height;
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

	drawsolid_rect(win, 0xffffff,
		cx+1-ww/2, cy+1-hh*8/16,
		cx-1     , cy-1-hh*7/16
	);
	drawstring_fit(win, 0x000000,
		cx+1-ww/2, cy+1-hh*8/16,
		cx-1     , cy-1-hh*7/16,
		(void*)&win->tier, 4
	);
	drawsolid_rect(win, 0xffffff,
		cx+1     , cy+1-hh*8/16,
		cx-1+ww/2, cy-1-hh*7/16
	);
	drawstring_fit(win, 0x000000,
		cx+1     , cy+1-hh*8/16,
		cx-1+ww/2, cy-1-hh*7/16,
		(void*)&win->type, 4
	);
	drawsolid_rect(win, 0xffffff,
		cx+1-ww/2, cy+1-hh*7/16,
		cx-1+ww/2, cy-1-hh*6/16
	);
	drawstring_fit(win, 0x000000,
		cx+1-ww/2, cy+1-hh*7/16,
		cx-1+ww/2, cy-1-hh*6/16,
		(void*)&win->fmt, 8
	);
	drawsolid_rect(win, 0xffffff,
		cx+1-ww/2, cy+1-hh*6/16,
		cx-1+ww/2, cy-1+hh/2
	);
/*
	//self
	drawsolid_rect(win, 0xffffff, w*1/4+1, h*4/16+1, w*2/4-1, h*5/16-1);
	drawstring_fit(win, 0, w*1/4, h*4/16, w*2/4, h*5/16, (void*)&win->tier, 4);
	drawsolid_rect(win, 0xffffff, w*2/4+1, h*4/16+1, w*3/4-1, h*5/16-1);
	drawstring_fit(win, 0, w*2/4, h*4/16, w*3/4, h*5/16, (void*)&win->type, 4);
	drawsolid_rect(win, 0xffffff, w*1/4+1, h*5/16+1, w*3/4-1, h*6/16-1);
	drawstring_fit(win, 0, w*1/4, h*5/16, w*3/4, h*6/16, (void*)&win->fmt, 8);
	drawsolid_rect(win, 0xffffff, w*1/4+1, h*6/16+1, w*3/4-1, h*12/16-1);
*/
	n = win->height/32-2;

	j = 0;
	rel = win->irel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;
		m = h*(j*2+9)/32;

		//ichip
		drawsolid_rect(win, 0xffffff, 0, m-n, w/8, m+n);
		drawstring_fit(win, 0, 0, m-n, w/8, m+n, (void*)"1234", 0);

		//ofoot, ifoot
		drawline_arrow(win, 0x00ff00, w*2/16, m, w*4/16, m);
		drawascii_fit(win, 0x00ff00, w*2/16, m-8, w*3/16, m+8, 'o');
		drawascii_fit(win, 0x00ff00, w*3/16, m-8, w*4/16, m+8, 'i');

		j++;
		rel = samedstnextsrc(rel);
	}

	j = 0;
	rel = win->orel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;
		m = h*(j*2+9)/32;

		//ochip
		drawsolid_rect(win, 0xffffff, w*7/8, m-n, w, m+n);
		drawstring_fit(win, 0, w*7/8, m-n, w, m+n, (void*)"1234", 0);

		//ofoot, ifoot
		drawline_arrow(win, 0x00ff00, w*12/16, m, w*14/16, m);
		drawascii_fit(win, 0x00ff00, w*12/16, m-8, w*13/16, m+8, 'o');
		drawascii_fit(win, 0x00ff00, w*13/16, m-8, w*14/16, m+8, 'i');

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
int actorinput_detail(struct arena* win, struct event* ev)
{
	return 0;
}