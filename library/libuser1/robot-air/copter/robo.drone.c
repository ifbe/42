#include "libuser.h"
#define _euler_ hex32('e','u','l','r')
#define _quat_ hex32('q','u','a','t')
#define _real_ hex32('r','e','a','l')
#define _imag_ hex32('i','m','a','g')




static void drone_forgl41_actual(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	float dt;
	vec3 tc,tr,tf,tu;
	vec3 kc,kr,kf,ku;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41line_rect(ctx, 0x0000ff, vc, vr, vf);


	//debug position
	gl41line(ctx, 0x000000, vc, geom->actual.displace_x);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->actual.displace_v[j];}
	gl41line(ctx, 0x808080, vc, tc);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->actual.displace_a[j];}
	gl41line(ctx, 0xffffff, vc, tc);

/*
	//debug rotation
	float* q = geom->actual.angular_x;
	tr[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
	tr[1] = 2.0 * (q[0]*q[1] + q[2]*q[3]);
	tr[2] = 2.0 * (q[0]*q[2] - q[1]*q[3]);
	vec3_setlen(tr, vr[3]);
	tf[0] = 2.0 * (q[0]*q[1] - q[2]*q[3]);
	tf[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
	tf[2] = 2.0 * (q[1]*q[2] + q[0]*q[3]);
	vec3_setlen(tf, vf[3]);
	gl41line_rect(ctx, 0x0000ff, vc, tr, tf);
*/
	for(j=0;j<3;j++){tc[j] = vc[j] + geom->actual.angular_v[j];}
	gl41line(ctx, 0x00ff00, vc, tc);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->actual.angular_a[j];}
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
	dt = timeread_us() % 1000000;
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
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
}
static void drone_forgl41_desire(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* ctx, struct style* area)
{
	int j;
	float dt;
	vec3 tc,tr,tf,tu;
	vec3 kc,kr,kf,ku;
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	if(0){
		gl41opaque_rect(ctx, 0x400000ff, vc, vr, vf);
	}
	else{
		float* q = geom->desire.angular_x;
		tr[0] = 1.0 - (q[1]*q[1] + q[2]*q[2]) * 2.0;
		tr[1] = 2.0 * (q[0]*q[1] + q[2]*q[3]);
		tr[2] = 2.0 * (q[0]*q[2] - q[1]*q[3]);
		vec3_setlen(tr, vr[3]);
		tf[0] = 2.0 * (q[0]*q[1] - q[2]*q[3]);
		tf[1] = 1.0 - (q[0]*q[0] + q[2]*q[2]) * 2.0;
		tf[2] = 2.0 * (q[1]*q[2] + q[0]*q[3]);
		vec3_setlen(tf, vf[3]);
		gl41opaque_rect(ctx, 0x400000ff, vc, tr, tf);
	}

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->desire.angular_v[j];}
	gl41line(ctx, 0x00ff00, vc, tc);

	for(j=0;j<3;j++){tc[j] = vc[j] + geom->desire.angular_a[j];}
	gl41line(ctx, 0xff0000, vc, tc);


/*
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
	dt = timeread_us() % 1000000;
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
*/
}
static void drone_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
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
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}
}
static void drone_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void drone_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void drone_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void drone_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




void drone_write_quaternion(_obj* act, float* q)
{
	struct relation* rel;
	_obj* world;
	struct style* sty = 0;
	//logtoall("drone_quat: %f,%f,%f,%f\n",q[0],q[1],q[2],q[3]);

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
	//logtoall("%f,%f,%f\n",sty->vr[3], sty->vf[3], sty->vt[3]);

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
void drone_write_euler(_obj* act, float* f)
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




static void drone_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor = stack[sp-2].pchip;
	struct style* geom = stack[sp-2].pfoot;
	_obj* wnd = stack[sp-6].pchip;
	struct style* area = stack[sp-6].pfoot;
	drone_forgl41_actual(ent,slot, wor,geom, wnd,area);
	drone_forgl41_estimate(ent,slot, wor,geom, wnd,area);
	drone_forgl41_desire(ent,slot, wor,geom, wnd,area);
}




static void drone_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		drone_read_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void drone_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("ent=%p,stack=%p,buf=%p\n",ent,stack,buf);
	switch(stack[sp-1].foottype){
	case _quat_:
		drone_write_quaternion(ent, buf);
		break;
	case _euler_:
		drone_write_euler(ent, buf);
		break;
	}
}
static void drone_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void drone_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void drone_modify(_obj* act)
{
}
static void drone_search(_obj* act)
{
}
static void drone_delete(_obj* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memoryfree(act->buf);
}
static void drone_create(_obj* act)
{
	if(0 == act)return;
	//if(_orig_ == act->type)act->buf = buffer;
	//if(_copy_ == act->type)act->buf = memoryalloc(256, 0);
}




void drone_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('d', 'r', 'o', 'n', 'e', 0, 0, 0);

	p->oncreate = (void*)drone_create;
	p->ondelete = (void*)drone_delete;
	p->onreader = (void*)drone_search;
	p->onwriter = (void*)drone_modify;

	p->onattach = (void*)drone_attach;
	p->ondetach = (void*)drone_detach;
	p->ontaking = (void*)drone_taking;
	p->ongiving = (void*)drone_giving;
}
