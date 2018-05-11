#include "actor.h"




int actoroutput_detail_vbo(struct arena* win)
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
int actoroutput_detail_pixel(struct arena* win)
{
	struct relation* rel;
	int j,m,n;
	n = win->height/32-2;

	//self
	drawsolid_rect(
		win, 0xffffff,
		win->width*3/8+1, win->height*4/16+1,
		win->width*4/8-1, win->height*5/16-1
	);
	drawstring_fit(
		win, 0,
		win->width*3/8, win->height*4/16,
		win->width*4/8, win->height*5/16,
		(void*)&win->tier, 4
	);
	drawsolid_rect(
		win, 0xffffff,
		win->width*4/8+1, win->height*4/16+1,
		win->width*5/8-1, win->height*5/16-1
	);
	drawstring_fit(
		win, 0,
		win->width*4/8, win->height*4/16,
		win->width*5/8, win->height*5/16,
		(void*)&win->type, 4
	);
	drawsolid_rect(
		win, 0xffffff,
		win->width*3/8+1, win->height*5/16+1,
		win->width*5/8-1, win->height*6/16-1
	);
	drawstring_fit(
		win, 0,
		win->width*3/8, win->height*5/16,
		win->width*5/8, win->height*6/16,
		(void*)&win->fmt, 8
	);
	drawsolid_rect(
		win, 0xffffff,
		win->width*3/8+1, win->height*6/16+1,
		win->width*5/8-1, win->height*12/16-1
	);

	j = 0;
	rel = win->irel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;
		m = (win->height)*(j*2+9)/32;

		//ichip
		drawsolid_rect(
			win, 0xffffff,
			0, m-n,
			(win->width)/8, m+n
		);
		drawstring_fit(
			win, 0,
			0, m-n,
			(win->width)/8, m+n,
			(void*)"1234", 0
		);

		//ofoot, ifoot
		drawline_arrow(
			win, 0x00ff00,
			(win->width)*1/8, m,
			(win->width)*3/8, m
		);
		drawascii_fit(
			win, 0x00ff00,
			(win->width)*1/8, m-8,
			(win->width)*2/8, m+8,
			'o'
		);
		drawascii_fit(
			win, 0x00ff00,
			(win->width)*2/8, m-8,
			(win->width)*3/8, m+8,
			'i'
		);

		j++;
		rel = samedstnextsrc(rel);
	}

	j = 0;
	rel = win->orel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;
		m = (win->height)*(j*2+9)/32;

		//ochip
		drawsolid_rect(
			win, 0xffffff,
			(win->width)*7/8, m-n,
			win->width, m+n
		);
		drawstring_fit(
			win, 0,
			(win->width)*7/8, m-n,
			win->width, m+n,
			(void*)"1234", 0
		);

		//ofoot, ifoot
		drawline_arrow(
			win, 0x00ff00,
			(win->width)*5/8, m,
			(win->width)*7/8, m
		);
		drawascii_fit(
			win, 0x00ff00,
			(win->width)*5/8, m-8,
			(win->width)*6/8, m+8,
			'o'
		);
		drawascii_fit(
			win, 0x00ff00,
			(win->width)*6/8, m-8,
			(win->width)*7/8, m+8,
			'i'
		);

		j++;
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int actoroutput_detail(struct arena* win)
{
	if(_vbo_ == win->fmt)actoroutput_detail_vbo(win);
	else actoroutput_detail_pixel(win);
	return 0;
}
int actorinput_detail(struct arena* win, struct event* ev)
{
	return 0;
}