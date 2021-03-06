#include "libuser.h"
#define _quat_ hex32('q','u','a','t')
#define _imag_ hex32('i','m','a','g')




static void drone_forgl41_actual(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	float dt;
	vec3 tc,tr,tf,tu;
	vec3 kc,kr,kf,ku;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;


	//debug position
	gl41line(ctx, 0x000000, vc, geom->desire.displace_x);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->desire.displace_v[j];}
	gl41line(ctx, 0x808080, vc, tc);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->desire.displace_a[j];}
	gl41line(ctx, 0xffffff, vc, tc);


	//debug rotation
	float* q = geom->desire.angular_x;
	tr[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	tr[1] = 2.0 * (q[0]*q[1] + q[2]*q[3]);
	tr[2] = 2.0 * (q[0]*q[2] - q[1]*q[3]);
	vec3_setlen(tr, vr[3]);
	tf[0] = 2.0 * (q[0]*q[1] - q[2]*q[3]);
	tf[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	tf[2] = 2.0 * (q[1]*q[2] + q[0]*q[3]);
	vec3_setlen(tf, vf[3]);
	gl41line_rect(ctx, 0x0000ff, vc, tr, tf);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->desire.angular_v[j];}
	gl41line(ctx, 0x00ff00, vc, tc);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->desire.angular_a[j];}
	gl41line(ctx, 0xff0000, vc, tc);


	//board
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/4;
		tr[j] = vr[j] / 4;
		tf[j] = vf[j] / 4;
		tu[j] = vt[j] / 4;
	}
	gl41solid_prism4(ctx, 0xffffff, tc, tr, tf, tu);

	//pie
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/4;
		tr[j] = vr[j] + vf[j];
		tf[j] = (vf[j] - vr[j]) / 16;
	}
	gl41solid_prism4(ctx, 0xfedcba, tc, tr, tf, tu);

	//na
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/4;
		tr[j] = (vr[j] + vf[j]) / 16;
		tf[j] = vf[j] - vr[j];
	}
	gl41solid_prism4(ctx, 0xfedcba, tc, tr, tf, tu);

	//tmp
	dt = timeread() % 1000000;
	for(j=0;j<3;j++){
		kr[j] = vr[j] / 4;
		kf[j] = vf[j] / 4;
		ku[j] = vt[j] / 4;
		tr[j] = vr[j] / 32;
		tf[j] = vf[j] / 32;
		tu[j] = vt[j] / 2;
	}

	//rf, motor1
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] + vf[j] + tu[j];
	gl41solid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] + vf[j] + vt[j];
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '1');
	gl41solid_propeller(ctx, 0xffffff, tc, kr, kf, ku, 1, dt);

	//ln, motor2
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] - vf[j] + tu[j];
	gl41solid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] - vf[j] + vt[j];
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '2');
	gl41solid_propeller(ctx, 0xffffff, tc, kr, kf, ku, 1, dt);

	//lf, motor3
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] + vf[j] + tu[j];
	gl41solid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] + vf[j] + vt[j];
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '3');
	gl41solid_propeller(ctx, 0xffffff, tc, kr, kf, ku, -1, dt);

	//rn, motor4
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] - vf[j] + tu[j];
	gl41solid_cylinder(ctx, 0x765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] - vf[j] + vt[j];
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '4');
	gl41solid_propeller(ctx, 0xffffff, tc, kr, kf, ku, -1, dt);
}
static void drone_forgl41_estimate(
	struct entity* act, struct style* part,
	struct entity* scn, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j;
	float dt;
	vec3 tc,tr,tf,tu;
	vec3 kc,kr,kf,ku;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;


	//board
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/4;
		tr[j] = vr[j] / 4;
		tf[j] = vf[j] / 4;
		tu[j] = vt[j] / 4;
	}
	gl41opaque_prism4(ctx, 0x3fffffff, tc, tr, tf, tu);

	//pie
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/4;
		tr[j] = vr[j] + vf[j];
		tf[j] = (vf[j] - vr[j]) / 16;
	}
	gl41opaque_prism4(ctx, 0x3ffedcba, tc, tr, tf, tu);

	//na
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/4;
		tr[j] = (vr[j] + vf[j]) / 16;
		tf[j] = vf[j] - vr[j];
	}
	gl41opaque_prism4(ctx, 0x3ffedcba, tc, tr, tf, tu);

	//tmp
	dt = timeread() % 1000000;
	for(j=0;j<3;j++){
		kr[j] = vr[j] / 4;
		kf[j] = vf[j] / 4;
		ku[j] = vt[j] / 4;
		tr[j] = vr[j] / 32;
		tf[j] = vf[j] / 32;
		tu[j] = vt[j] / 2;
	}

	//rf, motor1
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] + vf[j] + tu[j];
	gl41opaque_cylinder(ctx, 0x3f765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] + vf[j] + vt[j];
	gl41opaque_propeller(ctx, 0x3fffffff, tc, kr, kf, ku, 1, dt);
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '1');

	//ln, motor2
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] - vf[j] + tu[j];
	gl41opaque_cylinder(ctx, 0x3f765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] - vf[j] + vt[j];
	gl41opaque_propeller(ctx, 0x3fffffff, tc, kr, kf, ku, 1, dt);
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '2');

	//lf, motor3
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] + vf[j] + tu[j];
	gl41opaque_cylinder(ctx, 0x3f765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] - vr[j] + vf[j] + vt[j];
	gl41opaque_propeller(ctx, 0x3fffffff, tc, kr, kf, ku, -1, dt);
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '3');

	//rn, motor4
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] - vf[j] + tu[j];
	gl41opaque_cylinder(ctx, 0x3f765432, tc, tr, tf, tu);
	for(j=0;j<3;j++)tc[j] = vc[j] + vr[j] - vf[j] + vt[j];
	gl41opaque_propeller(ctx, 0x3fffffff, tc, kr, kf, ku, -1, dt);
	gl41ascii_center(ctx, 0xffffff, tc, kr, kf, '4');
}
static void drone_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void drone_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void drone_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void drone_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void drone_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




void drone_write_quaternion(struct entity* act, float* q)
{
	struct relation* rel;
	struct entity* world;
	struct style* sty = 0;
	say("drone_quat: %f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if((_virtual_ == world->type)|(_scene3d_ == world->type)){
			sty = (void*)(rel->srcfoot);
			break;
		}
		rel = samedstnextsrc(rel);
	}
	if(0 == sty)return;
	//say("%f,%f,%f\n",sty->vr[3], sty->vf[3], sty->vt[3]);

	float l = sty->fshape.vr[3];
	sty->fshape.vr[0] = l*(1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0);
	sty->fshape.vr[1] = l*(2.0 * (q[0]*q[1] + q[2]*q[3]));
	sty->fshape.vr[2] = l*(2.0 * (q[0]*q[2] - q[1]*q[3]));
	l = sty->fshape.vf[3];
	sty->fshape.vf[0] = l*(2.0 * (q[0]*q[1] - q[2]*q[3]));
	sty->fshape.vf[1] = l*(1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0);
	sty->fshape.vf[2] = l*(2.0 * (q[1]*q[2] + q[0]*q[3]));
	l = sty->fshape.vt[3];
	sty->fshape.vt[0] = l*(2.0 * (q[0]*q[2] + q[1]*q[3]));
	sty->fshape.vt[1] = l*(2.0 * (q[1]*q[2] - q[0]*q[3]));
	sty->fshape.vt[2] = l*(1.0 - (q[0]*q[0] + q[1]*q[1]) * 2.0);
}
void drone_write_euler(struct entity* act, float* f)
{
	vec4 q;
	float rx = f[0]*PI/360;
	float ry = f[1]*PI/360;
	float rz = f[2]*PI/360;

	float sinx = getsin(rx);
	float siny = getsin(ry);
	float sinz = getsin(rz);
	float cosx = getcos(rx);
	float cosy = getcos(ry);
	float cosz = getcos(rz);

	q[0] = sinx * cosy * cosz - cosx * siny * sinz;
	q[1] = cosx * siny * cosz + sinx * cosy * sinz;
	q[2] = cosx * cosy * sinz - sinx * siny * cosz;
	q[3] = cosx * cosy * cosz + sinx * siny * sinz;

	drone_write_quaternion(act, q);
}




static void drone_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	if(_imag_ == stack[sp-1].flag){
		drone_forgl41_estimate(ent,slot, wor,geom, wnd,area);
	}
	else{
		drone_forgl41_actual(ent,slot, wor,geom, wnd,area);
	}
}




static void drone_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		drone_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void drone_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_quat_ == stack[sp-1].flag)drone_write_quaternion(ent, buf);
	else drone_write_euler(ent, buf);
}
static void drone_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void drone_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void drone_modify(struct entity* act)
{
}
static void drone_search(struct entity* act)
{
}
static void drone_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void drone_create(struct entity* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memorycreate(256, 0);
}




void drone_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('d', 'r', 'o', 'n', 'e', 0, 0, 0);

	p->oncreate = (void*)drone_create;
	p->ondelete = (void*)drone_delete;
	p->onsearch = (void*)drone_search;
	p->onmodify = (void*)drone_modify;

	p->onlinkup = (void*)drone_linkup;
	p->ondiscon = (void*)drone_discon;
	p->ontaking = (void*)drone_taking;
	p->ongiving = (void*)drone_giving;
}
