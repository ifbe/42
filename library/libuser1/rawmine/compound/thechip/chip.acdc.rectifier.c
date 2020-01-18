#include "libuser.h"
void rectify_chosen(float* v, int* t)
{
	int j;
	float hi,lo;

	//highest
	t[0] = t[1] = 0;
	hi = v[0];
	lo = v[0];
	for(j=1;j<3;j++)
	{
		if(hi < v[j])
		{
			t[0] = j;
			hi = v[j];
		}
		if(lo > v[j])
		{
			t[1] = j;
			lo = v[j];
		}
	}
}




static void rectify_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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
}
static void rectify_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	u64 time;
	int chosen[2];
	int x,y,z,j,rgb;
	float a,s[3];
	vec3 tc,tr,tf,tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	gl41line_prism4(ctx, 0xffffff, vc, vr, vf, vu);

	time = timeread() / 1000;
	a = time * PI / 1000;
	s[0] = sine(a - PI*2.0/3.0);
	s[1] = sine(a);
	s[2] = sine(a + PI*2.0/3.0);
	rectify_chosen(s, chosen);

	//board
	for(j=0;j<3;j++){
		tr[j] = vr[j]/2;
		tf[j] = vf[j]/2;
		tu[j] = vu[j]/16;
	}
	gl41solid_prism4(ctx, 0x444444, vc, tr, tf, tu);

	//3 inputs
	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tu[j] = vu[j]/8;
	}
	for(y=-1;y<2;y++)
	{
		a = s[y+1];
		if(a > 0.0)rgb = 0xff0000;
		else rgb = 0xff;

		for(j=0;j<3;j++)tc[j] = vc[j] - vr[j]/2 + y*vf[j]/2 + a*vu[j]/2;
		gl41solid_sphere(ctx, rgb, tc, tr, tf, tu);
	}
	for(y=-1;y<2;y++)
	{
		for(j=0;j<3;j++){
			tc[j] = vc[j] - vr[j]/2 + y*vf[j]/2 - vu[j]/2;
			tu[j] = tc[j] + vu[j];
		}
		gl41line(ctx, 0xffffff, tc, tu);
	}

	//6 diodes
	for(j=0;j<3;j++){
		tr[j] = vu[j]/8;
		tf[j] = vf[j]/8;
	}
	for(z=-1;z<2;z+=2)
	{
		for(y=-1;y<2;y++)
		{
			//-
			for(j=0;j<3;j++){
				tc[j] = vc[j] + z*vr[j]/16 + y*vf[j]/2 + z*vu[j]/2;
				tu[j] = vr[j] / 32;
			}
			gl41solid_cylinder(ctx, 0x808080, tc, tr, tf, tu);

			//+
			for(j=0;j<3;j++){
				tc[j] -= z*vr[j]/3.6;
				tu[j] = vr[j] / 4;
			}
			gl41solid_cylinder(ctx, 0x404040, tc, tr, tf, tu);

			a = s[y+1];
			rgb = 0xffffff;
			if(z>0)		//if highest
			{
				if(y+1 == chosen[0])rgb = 0xff0000;
			}
			else		//if lowest
			{
				if(y+1 == chosen[1])rgb = 0xff;
			}

			//wire
			for(j=0;j<3;j++){
				tc[j] = vc[j] - vr[j]/2 + y*vf[j]/2 + z*vu[j]/2;
				tu[j] = tc[j] + vr[j];
			}
			gl41line(ctx, rgb, tc, tu);
		}
	}

	//2 outputs
	for(j=0;j<3;j++){
		tr[j] = vr[j]/8;
		tf[j] = vf[j]/8;
		tu[j] = vu[j]/8;
		tc[j] = vc[j] + vr[j]/2 + s[chosen[0]]*vu[j]/2;
	}
	gl41solid_sphere(ctx, 0xff0000, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]/2 + s[chosen[1]]*vu[j]/2;
	gl41solid_sphere(ctx, 0x0000ff, tc, tr, tf, tu);

	for(z=-1;z<2;z+=2)
	{
		for(j=0;j<3;j++){
			tc[j] = vc[j] + vr[j]/2 - vf[j]/2 + z*vu[j]/2;
			tu[j] = tc[j] + vf[j];
		}
		gl41line(ctx, 0xffffff, tc, tu);
	}
}
static void rectify_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rectify_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rectify_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rectify_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void rectify_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, u8* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> rectify
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)rectify_draw_gl41(act,slot, win,geom, wnd,area);
	}
}
static void rectify_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, u8* buf, int len)
{
}
static void rectify_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void rectify_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void rectify_search(struct entity* act, u8* buf)
{
}
static void rectify_modify(struct entity* act, u8* buf)
{
}
static void rectify_delete(struct entity* act, u8* buf)
{
}
static void rectify_create(struct entity* act, u8* buf)
{
}




void rectify_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r','e','c','t','i','f','y',0);

	p->oncreate = (void*)rectify_create;
	p->ondelete = (void*)rectify_delete;
	p->onsearch = (void*)rectify_search;
	p->onmodify = (void*)rectify_modify;

	p->onlinkup = (void*)rectify_linkup;
	p->ondiscon = (void*)rectify_discon;
	p->onread  = (void*)rectify_read;
	p->onwrite = (void*)rectify_write;
}
