#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void drone_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void drone_draw_vbo(
	struct actor* act, struct style* part,
	struct actor* win, struct style* geom,
	struct actor* wrd, struct style* camg,
	struct actor* ctx, struct style* temp)
{
    vec3 tc,tr,tf,tu;
	vec3 kc,kr,kf,ku;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	carveline_rect(ctx, 0xffffff, vc, vr, vf);

    tu[0] = vt[0] / 64;
    tu[1] = vt[1] / 64;
    tu[2] = vt[2] / 64;

    //center
    tr[0] = vr[0] / 4;
    tr[1] = vr[1] / 4;
    tr[2] = vr[2] / 4;
    tf[0] = vf[0] / 4;
    tf[1] = vf[1] / 4;
    tf[2] = vf[2] / 4;
    carvesolid_prism4(ctx, 0xffffff, vc, tr, tf, tu);

    //pie
    tr[0] = vr[0] + vf[0];
    tr[1] = vr[1] + vf[1];
    tr[2] = vr[2] + vf[2];
    tf[0] = (vf[0] - vr[0]) / 16;
    tf[1] = (vf[1] - vr[1]) / 16;
    tf[2] = (vf[2] - vr[2]) / 16;
    carvesolid_prism4(ctx, 0xfedcba, vc, tr, tf, tu);

    //na
    tr[0] = (vr[0] + vf[0]) / 16;
    tr[1] = (vr[1] + vf[1]) / 16;
    tr[2] = (vr[2] + vf[2]) / 16;
    tf[0] = vf[0] - vr[0];
    tf[1] = vf[1] - vr[1];
    tf[2] = vf[2] - vr[2];
    carvesolid_prism4(ctx, 0xfedcba, vc, tr, tf, tu);


    kr[0] = vr[0] / 4;
    kr[1] = vr[1] / 4;
    kr[2] = vr[2] / 4;
    kf[0] = vf[0] / 4;
    kf[1] = vf[1] / 4;
    kf[2] = vf[2] / 4;
	ku[0] = vt[0] / 4;
	ku[1] = vt[1] / 4;
	ku[2] = vt[2] / 4;

    tr[0] = vr[0] / 32;
    tr[1] = vr[1] / 32;
    tr[2] = vr[2] / 32;
    tf[0] = vf[0] / 32;
    tf[1] = vf[1] / 32;
    tf[2] = vf[2] / 32;
    tu[0] = vt[0] / 32;
    tu[1] = vt[1] / 32;
    tu[2] = vt[2] / 32;

    //rf, motor1
    tc[0] = vc[0] + vr[0] + vf[0];
    tc[1] = vc[1] + vr[1] + vf[1];
    tc[2] = vc[2] + vr[2] + vf[2];
    carvesolid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
    tc[0] = vc[0] + vr[0] + vf[0] + tu[0];
    tc[1] = vc[1] + vr[1] + vf[1] + tu[1];
    tc[2] = vc[2] + vr[2] + vf[2] + tu[2];
	carveascii_center(ctx, 0xffffff, tc, kr, kf, '1');
	carvesolid_propeller(ctx, 0xffffff, tc, kr, kf, ku, 1);

    //ln, motor2
    tc[0] = vc[0] - vr[0] - vf[0];
    tc[1] = vc[1] - vr[1] - vf[1];
    tc[2] = vc[2] - vr[2] - vf[2];
    carvesolid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
    tc[0] = vc[0] - vr[0] - vf[0] + tu[0];
    tc[1] = vc[1] - vr[1] - vf[1] + tu[1];
    tc[2] = vc[2] - vr[2] - vf[2] + tu[2];
	carveascii_center(ctx, 0xffffff, tc, kr, kf, '2');
	carvesolid_propeller(ctx, 0xffffff, tc, kr, kf, ku, 1);

    //lf, motor3
    tc[0] = vc[0] - vr[0] + vf[0];
    tc[1] = vc[1] - vr[1] + vf[1];
    tc[2] = vc[2] - vr[2] + vf[2];
    carvesolid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
    tc[0] = vc[0] - vr[0] + vf[0] + tu[0];
    tc[1] = vc[1] - vr[1] + vf[1] + tu[1];
    tc[2] = vc[2] - vr[2] + vf[2] + tu[2];
	carveascii_center(ctx, 0xffffff, tc, kr, kf, '3');
	carvesolid_propeller(ctx, 0xffffff, tc, kr, kf, ku, -1);

    //rn, motor4
    tc[0] = vc[0] + vr[0] - vf[0];
    tc[1] = vc[1] + vr[1] - vf[1];
    tc[2] = vc[2] + vr[2] - vf[2];
    carvesolid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
    tc[0] = vc[0] + vr[0] - vf[0] + tu[0];
    tc[1] = vc[1] + vr[1] - vf[1] + tu[1];
    tc[2] = vc[2] + vr[2] - vf[2] + tu[2];
	carveascii_center(ctx, 0xffffff, tc, kr, kf, '4');
	carvesolid_propeller(ctx, 0xffffff, tc, kr, kf, ku, -1);
}
static void drone_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void drone_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)drone_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)drone_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)drone_draw_html(act, pin, win, sty);
	else if(fmt == _json_)drone_draw_json(act, pin, win, sty);
	else drone_draw_pixel(act, pin, win, sty);
}
void drone_write_quaternion(struct actor* act, float* f)
{
	struct relation* rel;
	struct actor* world;
	struct fstyle* sty = 0;
	say("%f,%f,%f,%f\n",f[0],f[1],f[2],f[3]);

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			sty = (void*)(rel->srcfoot);
		}
		rel = samedstnextsrc(rel);
	}
	if(0 == sty)return;
	//say("%f,%f,%f\n",sty->vr[3], sty->vf[3], sty->vt[3]);

	sty->vr[0] = 1.0;
	sty->vr[1] = 0.0;
	sty->vr[2] = 0.0;
	quaternion_rotate(sty->vr, f);
	sty->vr[0] *= sty->vr[3];
	sty->vr[1] *= sty->vr[3];
	sty->vr[2] *= sty->vr[3];

	sty->vf[0] = 0.0;
	sty->vf[1] = 1.0;
	sty->vf[2] = 0.0;
	quaternion_rotate(sty->vf, f);
	sty->vf[0] *= sty->vf[3];
	sty->vf[1] *= sty->vf[3];
	sty->vf[2] *= sty->vf[3];

	sty->vt[0] = 0.0;
	sty->vt[1] = 0.0;
	sty->vt[2] = 1.0;
	quaternion_rotate(sty->vt, f);
	sty->vt[0] *= sty->vt[3];
	sty->vt[1] *= sty->vt[3];
	sty->vt[2] *= sty->vt[3];
}
void drone_write_euler(struct actor* act, float* f)
{
	vec4 q;
	float rx = f[0]*PI/360;
	float ry = f[1]*PI/360;
	float rz = f[2]*PI/360;

	float sinx = sine(rx);
	float siny = sine(ry);
	float sinz = sine(rz);
	float cosx = cosine(rx);
	float cosy = cosine(ry);
	float cosz = cosine(rz);

	q[0] = sinx * cosy * cosz - cosx * siny * sinz;
	q[1] = cosx * siny * cosz + sinx * cosy * sinz;
	q[2] = cosx * cosy * sinz - sinx * siny * cosz;
	q[3] = cosx * cosy * cosz + sinx * siny * sinz;

	drone_write_quaternion(act, q);
}




static void drone_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> ctx
	struct actor* ctx;

	//cam -> world
	struct actor* cam;
	struct actor* wrd;struct style* camg;

	//world -> texball
	struct actor* win;struct style* geom;
	struct actor* act;struct style* part;

	if(stack){
		ctx = stack[rsp-3]->pchip;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		drone_draw_vbo(act,part, win,geom, wrd,camg, ctx,0);
	}
}
static void drone_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	drone_write_euler(act, buf);
}
static void drone_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void drone_start(struct halfrel* self, struct halfrel* peer)
{
}




static void drone_modify(struct actor* act)
{
}
static void drone_search(struct actor* act)
{
}
static void drone_delete(struct actor* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void drone_create(struct actor* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void drone_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('d', 'r', 'o', 'n', 'e', 0, 0, 0);

	p->oncreate = (void*)drone_create;
	p->ondelete = (void*)drone_delete;
	p->onsearch = (void*)drone_search;
	p->onmodify = (void*)drone_modify;

	p->onstart = (void*)drone_start;
	p->onstop  = (void*)drone_stop;
	p->onread  = (void*)drone_read;
	p->onwrite = (void*)drone_write;
}
