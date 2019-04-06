#include "libuser.h"
void maze_generate(void* buf, int w, int h);
void maze_solve(void* buf, int w, int h);




#define WIDTH 32
#define HEIGHT 32




static void maze_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u8* buf;
	int x,y,w;
	int cx, cy, ww, hh;
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

	buf = act->buf;
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			w = buf[WIDTH*y+x];
			if((w&1) == 1)	//left
			{
				drawline(
					win, 0xff0000,
					(cx-ww) + (2*x+0)*ww/WIDTH,
					(cy-hh) + (2*y+0)*hh/HEIGHT,
					(cx-ww) + (2*x+0)*ww/WIDTH,
					(cy-hh) + (2*y+2)*hh/HEIGHT
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+0)*ww/WIDTH,
					(cy-hh) + (2*y+1)*hh/HEIGHT,
					(cx-ww) + (2*x+1)*ww/WIDTH,
					(cy-hh) + (2*y+1)*hh/HEIGHT
				);
			}

			if((w&2) == 2)	//right
			{
				drawline(
					win, 0x00ffff,
					(cx-ww-1) + (2*x+2)*ww/WIDTH,
					(cy-hh)   + (2*y+0)*hh/HEIGHT,
					(cx-ww-1) + (2*x+2)*ww/WIDTH,
					(cy-hh)   + (2*y+2)*hh/HEIGHT
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+1)*ww/WIDTH,
					(cy-hh) + (2*y+1)*hh/HEIGHT,
					(cx-ww) + (2*x+2)*ww/WIDTH,
					(cy-hh) + (2*y+1)*hh/HEIGHT
				);
			}

			if((w&4) == 4)	//up
			{
				drawline(
					win, 0x0000ff,
					(cx-ww) + (2*x+0)*ww/WIDTH,
					(cy-hh) + (2*y+0)*hh/HEIGHT,
					(cx-ww) + (2*x+2)*ww/WIDTH,
					(cy-hh) + (2*y+0)*hh/HEIGHT
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+1)*ww/WIDTH,
					(cy-hh) + (2*y+0)*hh/HEIGHT,
					(cx-ww) + (2*x+1)*ww/WIDTH,
					(cy-hh) + (2*y+1)*hh/HEIGHT
				);
			}

			if((w&8) == 8)	//down
			{
				drawline(
					win, 0xffff00,
					(cx-ww)   + (2*x+0)*ww/WIDTH,
					(cy-hh-1) + (2*y+2)*hh/HEIGHT,
					(cx-ww)   + (2*x+2)*ww/WIDTH,
					(cy-hh-1) + (2*y+2)*hh/HEIGHT
				);
			}
			else if((w&0x80) == 0x80)
			{
				drawline(
					win, 0x00ff00,
					(cx-ww) + (2*x+1)*ww/WIDTH,
					(cy-hh) + (2*y+1)*hh/HEIGHT,
					(cx-ww) + (2*x+1)*ww/WIDTH,
					(cy-hh) + (2*y+2)*hh/HEIGHT
				);
				/*
				drawsolid_rect(
					win, 0x00ff00,
					(cx-ww) + (4*x+1)*ww/WIDTH/2,
					(cy-hh) + (4*y+1)*hh/HEIGHT/2,
					(cx-ww) + (4*x+3)*ww/WIDTH/2,
					(cy-hh) + (4*y+3)*hh/HEIGHT/2
				);
				*/
			}
		}
	}
}
static void maze_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,z,w;
	vec3 tc, tr, tf, tu, f;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	u8* buf = act->buf;

	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			w = buf[WIDTH*y+x];
			if((w&1) == 1)	//left
			{
				f[0] = (2.0*x+0.0)/WIDTH - 1.0;
				f[1] = (2.0*y+0)/HEIGHT - 1.0;
				f[1] = (2.0*y+2)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vc[0] + f[0]*vr[0] + f[2]*vf[0];
				tr[1] = vc[1] + f[0]*vr[1] + f[2]*vf[1];
				tr[2] = vc[2] + f[0]*vr[2] + f[2]*vf[2];
				carveline2d(win, 0x808080, tc, tr);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid2d_rect(win, 0xff0000, tc, tr, tf);
			}

			if((w&2) == 2)	//right
			{
				f[0] = (2.0*x+2.0)/WIDTH - 1.0;
				f[1] = (2.0*y+0)/HEIGHT - 1.0;
				f[2] = (2.0*y+2)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vc[0] + f[0]*vr[0] + f[2]*vf[0];
				tr[1] = vc[1] + f[0]*vr[1] + f[2]*vf[1];
				tr[2] = vc[2] + f[0]*vr[2] + f[2]*vf[2];
				carveline2d(win, 0x909090, tc, tr);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid_rect(win, 0xff0000, tc, tr, tf);
			}

			if((w&4) == 4)	//down	//careful,different
			{
				f[0] = (2.0*x+0.0)/WIDTH - 1.0;
				f[1] = (2.0*x+2.0)/WIDTH - 1.0;
				f[2] = (2.0*y+2.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[2]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[2]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[2]*vf[2];
				tr[0] = vc[0] + f[1]*vr[0] + f[2]*vf[0];
				tr[1] = vc[1] + f[1]*vr[1] + f[2]*vf[1];
				tr[2] = vc[2] + f[1]*vr[2] + f[2]*vf[2];
				carveline2d(win, 0x707070, tc, tr);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid_rect(win, 0xff0000, tc, tr, tf);
			}

			if((w&8) == 8)	//up	//careful,different
			{
				f[0] = (2.0*x+0.0)/WIDTH - 1.0;
				f[1] = (2.0*x+2.0)/WIDTH - 1.0;
				f[2] = (2.0*y+2.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[2]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[2]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[2]*vf[2];
				tr[0] = vc[0] + f[1]*vr[0] + f[2]*vf[0];
				tr[1] = vc[1] + f[1]*vr[1] + f[2]*vf[1];
				tr[2] = vc[2] + f[1]*vr[2] + f[2]*vf[2];
				carveline2d(win, 0x606060, tc, tr);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid2d_rect(win, 0xff0000, tc, tr, tf);
			}
		}
	}
}
static void maze_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,z,w;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	u8* buf = act->buf;

	carvesolid_rect(win, 0, vc, vr, vf);
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			w = buf[WIDTH*y+x];
			if((w&1) == 1)	//left
			{
				f[0] = (2.0*x+0.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				f[2] = 1.0 / (WIDTH+HEIGHT);
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
				tr[0] = vf[0] / HEIGHT;
				tr[1] = vf[1] / HEIGHT;
				tr[2] = vf[2] / HEIGHT;
				tf[0] = vu[0] / (WIDTH+HEIGHT);
				tf[1] = vu[1] / (WIDTH+HEIGHT);
				tf[2] = vu[2] / (WIDTH+HEIGHT);
				carvesolid_rect(win, 0x808080, tc, tr, tf);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid_rect(win, 0xff0000, tc, tr, tf);
			}

			if((w&2) == 2)	//right
			{
				f[0] = (2.0*x+2.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				f[2] = 1.0 / (WIDTH+HEIGHT);
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
				tr[0] = -vf[0] / HEIGHT;
				tr[1] = -vf[1] / HEIGHT;
				tr[2] = -vf[2] / HEIGHT;
				tf[0] = vu[0] / (WIDTH+HEIGHT);
				tf[1] = vu[1] / (WIDTH+HEIGHT);
				tf[2] = vu[2] / (WIDTH+HEIGHT);
				carvesolid_rect(win, 0x909090, tc, tr, tf);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid_rect(win, 0xff0000, tc, tr, tf);
			}

			if((w&4) == 4)	//down	//careful,different
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+0.0)/HEIGHT - 1.0;
				f[2] = 1.0 / (WIDTH+HEIGHT);
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
				tr[0] = -vr[0] / WIDTH;
				tr[1] = -vr[1] / WIDTH;
				tr[2] = -vr[2] / WIDTH;
				tf[0] = vu[0] / (WIDTH+HEIGHT);
				tf[1] = vu[1] / (WIDTH+HEIGHT);
				tf[2] = vu[2] / (WIDTH+HEIGHT);
				carvesolid_rect(win, 0x707070, tc, tr, tf);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid_rect(win, 0xff0000, tc, tr, tf);
			}

			if((w&8) == 8)	//up	//careful,different
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+2.0)/HEIGHT - 1.0;
				f[2] = 1.0 / (WIDTH+HEIGHT);
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
				tr[0] = vr[0] / WIDTH;
				tr[1] = vr[1] / WIDTH;
				tr[2] = vr[2] / WIDTH;
				tf[0] = vu[0] / (WIDTH+HEIGHT);
				tf[1] = vu[1] / (WIDTH+HEIGHT);
				tf[2] = vu[2] / (WIDTH+HEIGHT);
				carvesolid_rect(win, 0x606060, tc, tr, tf);
			}
			else if((w&0x80) == 0x80)
			{
				f[0] = (2.0*x+1.0)/WIDTH - 1.0;
				f[1] = (2.0*y+1.0)/HEIGHT - 1.0;
				tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
				tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
				tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
				tr[0] = vr[0] / WIDTH/2;
				tr[1] = vr[1] / WIDTH/2;
				tr[2] = vr[2] / WIDTH/2;
				tf[0] = vf[0] / HEIGHT/2;
				tf[1] = vf[1] / HEIGHT/2;
				tf[2] = vf[2] / HEIGHT/2;
				carvesolid_rect(win, 0xff0000, tc, tr, tf);
			}
		}
	}


	x = act->x0;
	y = act->y0;

	act->camera.vc[0] = vc[0] + vr[0]*x/32 + vf[0]*y/32;
	act->camera.vc[1] = vc[1] + vr[1]*x/32 + vf[1]*y/32;
	act->camera.vc[2] = vc[2] + vr[2]*x/32 + vf[2]*y/32 + 10.0;
	act->camera.vn[0] = 0.0;
	act->camera.vn[1] = 1.0;
	act->camera.vn[2] = 0.0;

	act->camera.vl[0] =-1.0;
	act->camera.vl[1] = 0.0;
	act->camera.vl[2] = 0.0;
	act->camera.vr[0] = 1.0;
	act->camera.vr[1] = 0.0;
	act->camera.vr[2] = 0.0;

	act->camera.vb[0] = 0.0;
	act->camera.vb[1] = 0.0;
	act->camera.vb[2] =-1.0;
	act->camera.vu[0] = 0.0;
	act->camera.vu[1] = 0.0;
	act->camera.vu[2] = 1.0;

}
static void maze_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void maze_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int len = win->len;
	u8* buf = win->buf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"maze\" style=\"WIDTH:50%%;HEIGHT:100px;float:left;background-color:#444444;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");
	win->len = len;
}
static void maze_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,stride;
	u8* p;
	u8* buf = act->buf;
	u8* out = win->buf;

	stride = win->stride;
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			p = out + (y*stride*4) + (x*8);
			if(buf[WIDTH*y+x] != 0)mysnprintf(p, 4, "⬛");
			else mysnprintf(p, 4, "  ");
		}
	}
}
static void maze_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	u8* buf = act->buf;
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			if(buf[WIDTH*y+x] != 0)say("⬛");	//██
			else say("  ");
		}
		say("\n");
	}
	say("\n\n\n\n");
}
static void maze_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)maze_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)maze_read_tui(win, sty, act, pin);
	else if(fmt == _html_)maze_read_html(win, sty, act, pin);
	else if(fmt == _json_)maze_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)maze_read_vbo2d(win, sty, act, pin);
		else maze_read_vbo3d(win, sty, act, pin);
	}
	else maze_read_pixel(win, sty, act, pin);
}
static void maze_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'a':act->x0 -= 1;break;
			case 'd':act->x0 += 1;break;
			case 's':act->y0 -= 1;break;
			case 'w':act->y0 += 1;break;
		}
	}
}
static void maze_cread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void maze_cwrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void maze_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void maze_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
	u8* buf = leaf->buf;
	maze_generate(buf, WIDTH, HEIGHT);
	maze_solve(buf, WIDTH, HEIGHT);
}
static void maze_delete(struct actor* act)
{
	if(0 == act)return;
	if(act->buf)
	{
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void maze_create(struct actor* act)
{
	if(0 == act)return;
	act->buf = memorycreate(WIDTH*HEIGHT);

	act->x0 = 31;
	act->y0 = -31;
}




void maze_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex32('m', 'a', 'z', 'e');

	p->oncreate = (void*)maze_create;
	p->ondelete = (void*)maze_delete;
	p->onstart  = (void*)maze_start;
	p->onstop   = (void*)maze_stop;
	p->onget    = (void*)maze_cread;
	p->onpost   = (void*)maze_cwrite;
	p->onread   = (void*)maze_sread;
	p->onwrite  = (void*)maze_swrite;
}
