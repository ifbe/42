#include "libuser.h"




static int orthcam_draw(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty)
{/*
	vec3 tc,tf;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	tc[0] = vc[0] - vf[0]/2;
	tc[1] = vc[1] - vf[1]/2;
	tc[2] = vc[2] - vf[2]/2;
	tf[0] = vf[0] / 2;
	tf[1] = vf[1] / 2;
	tf[2] = vf[2] / 2;
	carvesolid_prism4(win, 0x800000, tc, vr, vu, tf);

	sty->vn[0] = sty->vf[0];
	sty->vn[1] = sty->vf[1];
	sty->vn[2] = sty->vf[2];
	sty->vl[0] = -sty->vr[0];
	sty->vl[1] = -sty->vr[1];
	sty->vl[2] = -sty->vr[2];
	sty->vb[0] = -sty->vu[0];
	sty->vb[1] = -sty->vu[1];
	sty->vb[2] = -sty->vu[2];
	carvefrustum(win, sty);
*/
	return 0;
}
static int orthcam_event(
	struct actor* act, struct style* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	say("orthcam_event@%llx:%x,%x\n", act, ev->why, ev->what);
	return 1;
}




static void ortho_v(mat4 m, struct fstyle* s)
{
	float cx = s->vc[0];
	float cy = s->vc[1];
	float cz = s->vc[2];

	m[0][0] = s->vr[0];
	m[0][1] = s->vr[1];
	m[0][2] = s->vr[2];
	m[0][3] = -(cx*m[0][0] + cy*m[0][1] + cz*m[0][2]);

	m[1][0] = s->vf[0];
	m[1][1] = s->vf[1];
	m[1][2] = s->vf[2];
	m[1][3] = -(cx*m[1][0] + cy*m[1][1] + cz*m[1][2]);

	m[2][0] =-s->vt[0];
	m[2][1] =-s->vt[1];
	m[2][2] =-s->vt[2];
	m[2][3] = cx*m[2][0] + cy*m[2][1] + cz*m[2][2];

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
static void ortho_p(mat4 m, struct fstyle* s)
{
	float l =-1.0;
	float r = 1.0;
	float b =-1.0;
	float t = 1.0;
	float n =-1.0;
	float f = 1.0;

	m[0][0] = 2.0 / (r-l);
	m[0][1] = 0.0;
	m[0][2] = 0.0;
	m[0][3] = (l+r) / (l-r);

	m[1][0] = 0.0;
	m[1][1] = 2.0 / (t-b);
	m[1][2] = 0.0;
	m[1][3] = (b+t) / (b-t);

	m[2][0] = 0.0;
	m[2][1] = 0.0;
	m[2][2] = 2.0 / (f-n);
	m[2][3] = (n+f) / (n-f);

	m[3][0] = 0.0;
	m[3][1] = 0.0;
	m[3][2] = 0.0;
	m[3][3] = 1.0;
}
static void ortho_mvp(mat4 m, struct fstyle* s)
{
	mat4 t;
	ortho_p(m, s);

	//ortho_o(t, s);
	//mat4_multiply(m, t);

	//ortho_p(t, s);
	//mat4_multiply(m, t);

	ortho_v(t, s);
	mat4_multiply(m, t);
}
static void orthcam_matrix(
	struct actor* act, struct style* pin,
	u8* buf, int len)
{
	struct relation* rel;
	struct arena* r;
	struct fstyle* s;
	//say("orthcam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		if(hex32('g','e','o','m') == rel->dstflag){
			s = (void*)(rel->srcfoot);
			r = (void*)(rel->srcchip);
			break;
		}
		rel = samedstnextsrc(rel);
	}
	if(0 == s)return;


	int j;
	float* m = act->buf;
	ortho_mvp((void*)m, s);
	mat4_transpose((void*)m);


	u64* p = (void*)buf;
	struct glsrc* src = (void*)(buf+0x20);

	p[0] = (u64)src;
	p[1] = (u64)r->gl_light;
	p[2] = (u64)r->gl_solid;
	p[3] = (u64)r->gl_opaque;


	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = m;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = s->vc;
}




static void orthcam_sread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);

	if(_cam_ == self->flag)orthcam_matrix(act, pin, buf, len);
	else orthcam_draw(act, pin, win, sty);
}
static int orthcam_swrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct arena* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	return orthcam_event(act, pin, win, sty, ev, 0);
}
static void orthcam_cread(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void orthcam_cwrite(struct halfrel* self, struct halfrel* peer, u8* buf, int len)
{
}
static void orthcam_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void orthcam_start(struct halfrel* self, struct halfrel* peer)
{
    say("@orthcam_start\n");
}
static void orthcam_delete()
{
}
static void orthcam_create(struct actor* act, void* arg)
{
    say("@orthcam_create\n");
	act->buf = memorycreate(64);
}




void orthcam_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('o', 'r', 't', 'h', 'c', 'a', 'm', 0);

	p->oncreate = (void*)orthcam_create;
	p->ondelete = (void*)orthcam_delete;
	p->onstart  = (void*)orthcam_start;
	p->onstop   = (void*)orthcam_stop;
	p->oncread  = (void*)orthcam_cread;
	p->oncwrite = (void*)orthcam_cwrite;
	p->onsread  = (void*)orthcam_sread;
	p->onswrite = (void*)orthcam_swrite;
}
