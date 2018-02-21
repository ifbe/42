#include "actor.h"
#define _act_ hex32('a','c','t',0)
#define _win_ hex32('w','i','n',0)




static struct arena* arena = 0;
static struct actor* actor = 0;
static struct style* style = 0;
static struct compo* compo = 0;
static int winlen = 0;
static int actlen = 0;
static int stylen = 0;
static int comlen = 0;




void select_2d(struct arena* win, struct style* sty)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;
	drawline_rect(win, 0xff00ff, cx-ww, cy-hh, cx+ww, cy+hh);
}
void select_3d(struct arena* win, struct style* sty)
{
	int cx = sty->i_cx;
	int cy = sty->i_cy;
	int cz = sty->i_cz;
	int ww = sty->i_rx;
	int hh = sty->i_fy;
	int dd = sty->i_uz;

	carveline_prism4(
		win, 0xff0000,
		cx, cy, dd/2,
		ww, 0.0, 0.0,
		0.0, hh, 0.0,
		0.0, 0.0, dd/2
	);
}