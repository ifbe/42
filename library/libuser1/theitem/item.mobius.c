#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void mobius_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mobius_read_vbo2d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mobius_read_vbo3d(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int j;
	float a,c,s;
	float x,y,z,n;
	vec3 tc,tr,tf,tu;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;

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

		if((act->x0 < 180) && (j == act->x0))
		{
			act->camera.vc[0] = tc[0] + tf[0] + tu[0]*10;
			act->camera.vc[1] = tc[1] + tf[1] + tu[1]*10;
			act->camera.vc[2] = tc[2] + tf[2] + tu[2]*10;

			act->camera.vu[0] = 2*tu[0];
			act->camera.vu[1] = 2*tu[1];
			act->camera.vu[2] = 2*tu[2];
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

			act->camera.vu[0] =-2*tu[0];
			act->camera.vu[1] =-2*tu[1];
			act->camera.vu[2] =-2*tu[2];
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
	}
}
static void mobius_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mobius_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mobius_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mobius_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void mobius_sread(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)mobius_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)mobius_read_tui(win, sty, act, pin);
	else if(fmt == _html_)mobius_read_html(win, sty, act, pin);
	else if(fmt == _json_)mobius_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)
	{
		if(_2d_ == win->vfmt)mobius_read_vbo2d(win, sty, act, pin);
		else mobius_read_vbo3d(win, sty, act, pin);
	}
	else mobius_read_pixel(win, sty, act, pin);
}
static void mobius_swrite(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
}
static void mobius_cread()
{
}
static void mobius_cwrite()
{
}
static void mobius_stop(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void mobius_start(
	struct actor* leaf, struct pinid* lf,
	struct arena* twig, struct style* tf,
    struct arena* root, struct style* rf)
{
}
static void mobius_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void mobius_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256);
}




void mobius_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('m', 'o', 'b', 'i', 'u', 's', 0, 0);

	p->oncreate = (void*)mobius_create;
	p->ondelete = (void*)mobius_delete;
	p->onstart  = (void*)mobius_start;
	p->onstop   = (void*)mobius_stop;
	p->onget    = (void*)mobius_cread;
	p->onpost   = (void*)mobius_cwrite;
	p->onread   = (void*)mobius_sread;
	p->onwrite  = (void*)mobius_swrite;
}
