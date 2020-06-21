#include "libuser.h"
#define PI 3.1415926535897932384626433832795028841971693993151




static void dna_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dna_draw_gl41(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int z;
	float a,c,s;
	float A,C,S;
	vec3 tc,tr,tf,tu;
	float* vc = sty->fs.vc;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vu = sty->fs.vt;
	gl41line_prism4(win, 0xffffff, vc, vr ,vf, vu);

	tf[0] = vf[0] / 16;
	tf[1] = vf[1] / 16;
	tf[2] = vf[2] / 16;
	tu[0] = vu[0] / 16;
	tu[1] = vu[1] / 16;
	tu[2] = vu[2] / 16;
	for(z=-6;z<=6;z++)
	{
		a = 2*PI*z/6;
		c = cosine(a)/2;
		s = sine(a)/2;

		tc[0] = vc[0] + vr[0]*c + vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s + vu[2]*z/6.0;
		tr[0] = vr[0] / 16;
		tr[1] = vr[1] / 16;
		tr[2] = vr[2] / 16;
		gl41solid_sphere(win, 0xff0000, tc, tr, tf, tu);

		tc[0] = vc[0] - vr[0]*c - vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] - vr[1]*c - vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] - vr[2]*c - vf[2]*s + vu[2]*z/6.0;
		gl41solid_sphere(win, 0x0000ff, tc, tr, tf, tu);
	}

	for(z=-6;z<=5;z++)
	{
		a = 2*PI*z/6;
		c = cosine(a)/2;
		s = sine(a)/2;
		A = 2*PI*(z+1)/6;
		C = cosine(A)/2;
		S = sine(A)/2;

		tc[0] = vc[0] + vr[0]*c + vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] + vr[1]*c + vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] + vr[2]*c + vf[2]*s + vu[2]*z/6.0;
		tr[0] = vc[0] + vr[0]*C + vf[0]*S + vu[0]*(z+1)/6.0;
		tr[1] = vc[1] + vr[1]*C + vf[1]*S + vu[1]*(z+1)/6.0;
		tr[2] = vc[2] + vr[2]*C + vf[2]*S + vu[2]*(z+1)/6.0;
		gl41line(win, 0xff0000, tc, tr);

		tc[0] = vc[0] - vr[0]*c - vf[0]*s + vu[0]*z/6.0;
		tc[1] = vc[1] - vr[1]*c - vf[1]*s + vu[1]*z/6.0;
		tc[2] = vc[2] - vr[2]*c - vf[2]*s + vu[2]*z/6.0;
		tr[0] = vc[0] - vr[0]*C - vf[0]*S + vu[0]*(z+1)/6.0;
		tr[1] = vc[1] - vr[1]*C - vf[1]*S + vu[1]*(z+1)/6.0;
		tr[2] = vc[2] - vr[2]*C - vf[2]*S + vu[2]*(z+1)/6.0;
		gl41line(win, 0x0000ff, tc, tr);
	}
}
static void dna_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dna_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dna_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dna_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void dna_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void dna_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void dna_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void dna_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void dna_search(struct entity* act)
{
}
static void dna_modify(struct entity* act)
{
}
static void dna_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void dna_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void dna_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('d', 'n', 'a', 0);

	p->oncreate = (void*)dna_create;
	p->ondelete = (void*)dna_delete;
	p->onsearch = (void*)dna_search;
	p->onmodify = (void*)dna_modify;

	p->onlinkup = (void*)dna_linkup;
	p->ondiscon = (void*)dna_discon;
	p->ontaking = (void*)dna_taking;
	p->ongiving = (void*)dna_giving;
}
