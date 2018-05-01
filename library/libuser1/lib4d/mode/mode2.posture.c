#include "actor.h"




int actoroutput_posture(struct arena* win)
{
	if(_vbo_ == win->fmt)
	{
		carvesolid2d_rect(
			win, 0xffff00,
			0.0, 0.0, -0.5,
			0.5, 0.0, 0.0,
			0.0, 0.5, 0.0
		);
	}
	else
	{
		drawsolid_rect(
			win, 0xffff00,
			win->width/4, win->height/4,
			win->width*3/4, win->height*3/4
		);
	}
	return 0;
}