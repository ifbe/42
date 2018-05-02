#include "actor.h"




int actoroutput_detail_vbo(struct arena* win)
{
	int j;
	struct relation* rel;
	carvesolid2d_rect(
		win, 0xffffff,
		0.0, 0.0, -0.5,
		0.25, 0.0, 0.0,
		0.0, 0.5, 0.0
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
		carveline2d_arrow(
			win, 0x00ff00,
			-6.0/8, (7-j*2)/16.0, -0.5,
			-2.0/8, (7-j*2)/16.0, -0.5
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
			0.0, 1.0/33, 0.0
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
	int j;
	struct relation* rel;
	drawsolid_rect(
		win, 0xffffff,
		win->width*3/8, win->height*1/4,
		win->width*5/8, win->height*3/4
	);

	j = 0;
	rel = win->irel;
	while(1)
	{
		if(j >= 8)break;
		if(0 == rel)break;

		drawsolid_rect(
			win, 0xffffff,
			0, (win->height)*(j*2+8)/32+2,
			(win->width)/8, (win->height)*(j*2+10)/32-2
		);
		drawline_arrow(
			win, 0x00ff00,
			(win->width)*1/8, (win->height)*(j*2+9)/32,
			(win->width)*3/8, (win->height)*(j*2+9)/32
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

		drawsolid_rect(
			win, 0xffffff,
			(win->width)*7/8, (win->height)*(j*2+8)/32+2,
			win->width, (win->height)*(j*2+10)/32-2
		);
		drawline_arrow(
			win, 0x00ff00,
			(win->width)*5/8, (win->height)*(j*2+9)/32,
			(win->width)*7/8, (win->height)*(j*2+9)/32
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