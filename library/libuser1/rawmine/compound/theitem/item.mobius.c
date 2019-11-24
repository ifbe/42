#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void mobius_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_vbo3d(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int j;
	float a,c,s;
	float x,y,z,n;
	vec3 tc,tr,tf,tu;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;

	tc[0] = vc[0] + vu[0];
	tc[1] = vc[1] + vu[1];
	tc[2] = vc[2] + vu[2];
	tr[0] = vr[0]*0.75;
	tr[1] = vr[1]*0.75;
	tr[2] = vr[2]*0.75;
	tf[0] = vf[0]*0.75;
	tf[1] = vf[1]*0.75;
	tf[2] = vf[2]*0.75;
	carveline_circle(win, 0xff0000, tc, tr, tf);

	act->x0 = (timeread()/10000) % 360;
	for(j=0;j<180;j++)
	{
		a = j*PI/90;
		c = cosine(a);
		s = sine(a);

		tr[0] = vr[0]*c + vf[0]*s;
		tr[1] = vr[1]*c + vf[1]*s;
		tr[2] = vr[2]*c + vf[2]*s;

		tc[0] = vc[0] + tr[0]*0.75 + vu[0];
		tc[1] = vc[1] + tr[1]*0.75 + vu[1];
		tc[2] = vc[2] + tr[2]*0.75 + vu[2];

		//cos' = -sin, sin' = cos
		x = -s*vr[0] + c*vf[0];
		y = -s*vr[1] + c*vf[1];
		z = -s*vr[2] + c*vf[2];
		n = 1.0/squareroot(x*x + y*y + z*z);
		tf[0] = x*n;
		tf[1] = y*n;
		tf[2] = z*n;

		//
		c = cosine(a/2);
		s = sine(a/2);
		tr[0] = (tr[0]*c + vu[0]*s)*0.25;
		tr[1] = (tr[1]*c + vu[1]*s)*0.25;
		tr[2] = (tr[2]*c + vu[2]*s)*0.25;

		//tu = cross(tr, tf)
		x = tr[1] * tf[2] - tr[2] * tf[1];
		y = tr[2] * tf[0] - tr[0] * tf[2];
		z = tr[0] * tf[1] - tr[1] * tf[0];
		n = 1.0 / squareroot(x*x + y*y + z*z);

		tu[0] = x * n;
		tu[1] = y * n;
		tu[2] = z * n;
/*
		if((act->x0 < 180) && (j == act->x0))
		{
			act->camera.vc[0] = tc[0] + tf[0] + tu[0]*10;
			act->camera.vc[1] = tc[1] + tf[1] + tu[1]*10;
			act->camera.vc[2] = tc[2] + tf[2] + tu[2]*10;

			act->camera.vt[0] = 2*tu[0];
			act->camera.vt[1] = 2*tu[1];
			act->camera.vt[2] = 2*tu[2];
			act->camera.vb[0] =-2*tu[0];
			act->camera.vb[1] =-2*tu[1];
			act->camera.vb[2] =-2*tu[2];

			n = squareroot(tf[0]*tf[0] + tf[1]*tf[1] + tf[2]*tf[2]);
			act->camera.vn[0] = tf[0] / n;
			act->camera.vn[1] = tf[1] / n;
			act->camera.vn[2] = tf[2] / n;

			n = squareroot(tr[0]*tr[0] + tr[1]*tr[1] + tr[2]*tr[2]);
			act->camera.vr[0] = 2*tr[0] / n;
			act->camera.vr[1] = 2*tr[1] / n;
			act->camera.vr[2] = 2*tr[2] / n;
			act->camera.vl[0] =-2*tr[0] / n;
			act->camera.vl[1] =-2*tr[1] / n;
			act->camera.vl[2] =-2*tr[2] / n;

			tc[0] += tu[0]*10;
			tc[1] += tu[1]*10;
			tc[2] += tu[2]*10;
			carvesolid_prism4(win, 0xff0000, tc, tr, tf, tu);
		}
		else if((act->x0 >= 180) && (j+180 == act->x0))
		{
			act->camera.vc[0] = tc[0] + tf[0] - tu[0]*10;
			act->camera.vc[1] = tc[1] + tf[1] - tu[1]*10;
			act->camera.vc[2] = tc[2] + tf[2] - tu[2]*10;

			act->camera.vt[0] =-2*tu[0];
			act->camera.vt[1] =-2*tu[1];
			act->camera.vt[2] =-2*tu[2];
			act->camera.vb[0] = 2*tu[0];
			act->camera.vb[1] = 2*tu[1];
			act->camera.vb[2] = 2*tu[2];

			n = squareroot(tf[0]*tf[0] + tf[1]*tf[1] + tf[2]*tf[2]);
			act->camera.vn[0] = tf[0] / n;
			act->camera.vn[1] = tf[1] / n;
			act->camera.vn[2] = tf[2] / n;

			n = squareroot(tr[0]*tr[0] + tr[1]*tr[1] + tr[2]*tr[2]);
			act->camera.vr[0] =-2*tr[0] / n;
			act->camera.vr[1] =-2*tr[1] / n;
			act->camera.vr[2] =-2*tr[2] / n;
			act->camera.vl[0] = 2*tr[0] / n;
			act->camera.vl[1] = 2*tr[1] / n;
			act->camera.vl[2] = 2*tr[2] / n;

			tc[0] -= tu[0]*10;
			tc[1] -= tu[1]*10;
			tc[2] -= tu[2]*10;
			carvesolid_prism4(win, 0xff0000, tc, tr, tf, tu);
		}
		else carvesolid_prism4(win, 0x808080, tc, tr, tf, tu);
*/
	}
}
static void mobius_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void mobius_draw(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)mobius_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)mobius_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)mobius_draw_html(act, pin, win, sty);
	else if(fmt == _json_)mobius_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)mobius_draw_vbo2d(act, pin, win, sty);
		//else mobius_draw_vbo3d(act, pin, win, sty);
	}
	else mobius_draw_pixel(act, pin, win, sty);
}




static void mobius_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct entity* ctx = buf;
	//say("@drone_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)mobius_draw_vbo3d(act,pin,ctx,sty);
	}
	//mobius_draw(act, pin, win, sty);
}
static void mobius_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void mobius_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void mobius_start(struct halfrel* self, struct halfrel* peer)
{
}




static void mobius_search(struct entity* act)
{
}
static void mobius_modify(struct entity* act)
{
}
static void mobius_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void mobius_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void mobius_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('m', 'o', 'b', 'i', 'u', 's', 0, 0);

	p->oncreate = (void*)mobius_create;
	p->ondelete = (void*)mobius_delete;
	p->onsearch = (void*)mobius_search;
	p->onmodify = (void*)mobius_modify;

	p->onstart = (void*)mobius_start;
	p->onstop  = (void*)mobius_stop;
	p->onread  = (void*)mobius_read;
	p->onwrite = (void*)mobius_write;
}
