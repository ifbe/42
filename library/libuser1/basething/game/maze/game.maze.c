#include "libuser.h"
void maze_generate(void* buf, int w, int h);
void maze_solve(void* buf, int w, int h);




#define WIDTH 32
#define HEIGHT 32




static void maze_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u8* buf;
	int x,y,w;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	buf = act->listptr.buf0;
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
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,y,z,w;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	u8* buf = act->listptr.buf0;

	gl41solid_rect(ctx, 0, vc, vr, vf);
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
				gl41solid_rect(ctx, 0x808080, tc, tr, tf);
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
				gl41solid_rect(ctx, 0xff0000, tc, tr, tf);
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
				gl41solid_rect(ctx, 0x909090, tc, tr, tf);
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
				gl41solid_rect(ctx, 0xff0000, tc, tr, tf);
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
				gl41solid_rect(ctx, 0x707070, tc, tr, tf);
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
				gl41solid_rect(ctx, 0xff0000, tc, tr, tf);
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
				gl41solid_rect(ctx, 0x606060, tc, tr, tf);
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
				gl41solid_rect(ctx, 0xff0000, tc, tr, tf);
			}
		}
	}
}
static void maze_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void maze_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void maze_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y,width;
	u8* p;
	u8* buf = act->listptr.buf0;
	u8* out = win->tuitext.buf;

	width = win->whdf.width;
	for(y=0;y<HEIGHT;y++)
	{
		for(x=0;x<WIDTH;x++)
		{
			p = out + (y*width*4) + (x*8);
			if(buf[WIDTH*y+x] != 0)mysnprintf(p, 4, "⬛");
			else mysnprintf(p, 4, "  ");
		}
	}
}
static void maze_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;
	u8* buf = act->listptr.buf0;
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	if(_char_ == ev->what)
	{
		switch(ev->why)
		{
			case 'a':act->whdf.ix0 -= 1;break;
			case 'd':act->whdf.ix0 += 1;break;
			case 's':act->whdf.iy0 -= 1;break;
			case 'w':act->whdf.iy0 += 1;break;
		}
	}
}




static void maze_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	maze_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void maze_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller = stack[sp-2].pchip;
	//struct style* area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		maze_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void maze_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void maze_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void maze_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void maze_search(_obj* act)
{
}
static void maze_modify(_obj* act)
{
}
static void maze_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void maze_create(_obj* act)
{
	if(0 == act)return;
	act->listptr.buf0 = memorycreate(WIDTH*HEIGHT, 0);

	act->whdf.ix0 = 31;
	act->whdf.iy0 = -31;

	maze_generate(act->listptr.buf0, WIDTH, HEIGHT);
	maze_solve(act->listptr.buf0, WIDTH, HEIGHT);
}




void maze_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex32('m', 'a', 'z', 'e');

	p->oncreate = (void*)maze_create;
	p->ondelete = (void*)maze_delete;
	p->onreader = (void*)maze_search;
	p->onwriter = (void*)maze_modify;

	p->onattach = (void*)maze_attach;
	p->ondetach = (void*)maze_detach;
	p->ontaking = (void*)maze_taking;
	p->ongiving = (void*)maze_giving;
}
