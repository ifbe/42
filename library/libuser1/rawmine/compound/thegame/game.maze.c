#include "libuser.h"
void maze_generate(void* buf, int w, int h);
void maze_solve(void* buf, int w, int h);




#define WIDTH 32
#define HEIGHT 32




static void maze_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8* buf;
	int x,y,w;
	int cx, cy, ww, hh;
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
static void maze_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y,z,w;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	u8* buf = act->buf;

	carvesolid_rect(ctx, 0, vc, vr, vf);
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
				carvesolid_rect(ctx, 0x808080, tc, tr, tf);
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
				carvesolid_rect(ctx, 0xff0000, tc, tr, tf);
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
				carvesolid_rect(ctx, 0x909090, tc, tr, tf);
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
				carvesolid_rect(ctx, 0xff0000, tc, tr, tf);
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
				carvesolid_rect(ctx, 0x707070, tc, tr, tf);
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
				carvesolid_rect(ctx, 0xff0000, tc, tr, tf);
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
				carvesolid_rect(ctx, 0x606060, tc, tr, tf);
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
				carvesolid_rect(ctx, 0xff0000, tc, tr, tf);
			}
		}
	}
}
static void maze_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void maze_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
static void maze_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
static void maze_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
static void maze_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'a':act->ix0 -= 1;break;
			case 'd':act->ix0 += 1;break;
			case 's':act->iy0 -= 1;break;
			case 'w':act->iy0 += 1;break;
		}
	}
}




static void maze_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//world -> this
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		if('v' == len)maze_draw_gl41(act,part, win,geom, wnd,area);
	}
	//maze_draw(act, pin, win, sty);
}
static void maze_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//maze_event(act, pin, win, sty, ev, 0);
}
static void maze_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void maze_start(struct halfrel* self, struct halfrel* peer)
{
}




static void maze_search(struct entity* act)
{
}
static void maze_modify(struct entity* act)
{
}
static void maze_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf)
	{
		memorydelete(act->buf);
		act->buf = 0;
	}
}
static void maze_create(struct entity* act)
{
	if(0 == act)return;
	act->buf = memorycreate(WIDTH*HEIGHT, 0);

	act->ix0 = 31;
	act->iy0 = -31;

	maze_generate(act->buf, WIDTH, HEIGHT);
	maze_solve(act->buf, WIDTH, HEIGHT);
}




void maze_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('m', 'a', 'z', 'e');

	p->oncreate = (void*)maze_create;
	p->ondelete = (void*)maze_delete;
	p->onsearch = (void*)maze_search;
	p->onmodify = (void*)maze_modify;

	p->onstart = (void*)maze_start;
	p->onstop  = (void*)maze_stop;
	p->onread  = (void*)maze_read;
	p->onwrite = (void*)maze_write;
}
