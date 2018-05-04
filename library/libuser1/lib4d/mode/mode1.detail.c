#include "actor.h"




int actoroutput_detail_vbo(struct arena* win)
{
	int j;
	struct relation* rel;
	carveline2d_rect(
		win, 0xffffff,
		0.0, 0.0, -0.5,
		0.25, 0.0, 0.0,
		0.0, 0.5, 0.0
	);

	carvesolid2d_rect(
		win, 0xffffff,
		-0.125, 7.0/16, -0.5,
		0.124, 0.0, 0.0,
		0.0, 1.0/17, 0.0
	);
	carvestring2d_center(
		win, 0,
		-0.125, 7.0/16, -0.6,
		0.124, 0.0, 0.0,
		0.0, 1.0/17, 0.0,
		(void*)&win->tier, 4
	);
	carvesolid2d_rect(
		win, 0xffffff,
		0.125, 7.0/16, -0.5,
		0.124, 0.0, 0.0,
		0.0, 1.0/17, 0.0
	);
	carvestring2d_center(
		win, 0,
		0.125, 7.0/16, -0.6,
		0.124, 0.0, 0.0,
		0.0, 1.0/17, 0.0,
		(void*)&win->type, 4
	);
	carvesolid2d_rect(
		win, 0xffffff,
		0.0, 5.0/16, -0.5,
		0.24, 0.0, 0.0,
		0.0, 1.0/17, 0.0
	);
	carvestring2d_center(
		win, 0,
		0.0, 5.0/16, -0.6,
		0.248, 0.0, 0.0,
		0.0, 1.0/17, 0.0,
		(void*)&win->fmt, 8
	);


	j = 0;
	rel = win->irel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		carvesolid2d_rect(
			win, 0xffffff,
			-7.0/8, (7-j*2)/16.0, -0.5,
			2.0/17, 0.0, 0.0,
			0.0, 1.0/17, 0.0
		);
		carvestring2d_center(
			win, 0,
			-7.0/8, (7-j*2)/16.0, -0.6,
			2.0/17, 0.0, 0.0,
			0.0, 1.0/17, 0.0,
			(void*)"123", 0
		);

		carveline2d_arrow(
			win, 0x00ff00,
			-6.0/8, (7-j*2)/16.0, -0.5,
			-2.0/8, (7-j*2)/16.0, -0.5
		);
		carve2d_ascii(
			win, 0x00ff00,
			-9.0/16, (7-j*2)/16.0, -0.5,
			1.0/8, 0.0, 0.0,
			0.0, 1.0/17, 0.0,
			'o'
		);
		carve2d_ascii(
			win, 0x00ff00,
			-5.0/16, (7-j*2)/16.0, -0.5,
			1.0/8, 0.0, 0.0,
			0.0, 1.0/17, 0.0,
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

		carvesolid2d_rect(
			win, 0xffffff,
			7.0/8, (7-j*2)/16.0, -0.5,
			2.0/17, 0.0, 0.0,
			0.0, 1.0/17, 0.0
		);
		carvestring2d_center(
			win, 0,
			7.0/8, (7-j*2)/16.0, -0.6,
			2.0/17, 0.0, 0.0,
			0.0, 1.0/17, 0.0,
			(void*)"123", 0
		);

		carveline2d_arrow(
			win, 0x00ff00,
			2.0/8, (7-j*2)/16.0, -0.5,
			6.0/8, (7-j*2)/16.0, -0.5
		);

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