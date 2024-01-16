#include "libuser.h"
#define A_PEERFOOT whdf.iw0
#define B_PEERFOOT whdf.iwn
#define LVAL listf32.f0
struct joint{
	float here[3];
	int exist;

	float grad[3];
	int sure;
};




static void stick_draw_pixel(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stick_draw_json(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stick_draw_html(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stick_draw_tui(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void stick_draw_cli(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}




void stick_read_force(_obj* ent, _obj* sup, struct joint* jo, int len)
{
	int a = ent->A_PEERFOOT - 'a';
	int b = ent->B_PEERFOOT - 'a';
	logtoall("@stick_read_force: %d,%d\n",a,b);

	float* va = jo[a].here;
	float* vb = jo[b].here;
	gl41line(sup, 0xffffff, va, vb);

	int j;
	vec3 up = {0.0, 0.0, 1.0};
	vec3 vc,vt;
	vec3 vr,vf;

	//vt = normalize(va-vb)
	for(j=0;j<3;j++){
		vc[j] = (va[j]+vb[j])/2;
		vt[j] = va[j]-vb[j];
	}
	vec3_setlen(vt, 1.0);

	//vr = normalize(cross(vt*up))
	vec3_cross(vr, vt, up);
	vec3_setlen(vr, 1.0);

	//vf = normalize(cross(vt*vr))
	vec3_cross(vf, vt, vr);
	vec3_setlen(vf, 1.0);

	//correct length
	vec3_setlen(vr, 10.0);
	vec3_setlen(vf, 10.0);
	vec3_setlen(vt, ent->LVAL/2);
	gl41solid_cylinder(sup, 0x808080, vc,vr,vf,vt);
}




static void stick_read_a(_obj* ent, int key, struct joint* jo,int thisone)
{
	if('V' != key)return;

	int theother = ent->B_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float myx = jo[thisone].here[0];
	float myy = jo[thisone].here[1];
	float myz = jo[thisone].here[2];
	float urx = jo[theother].here[0];
	float ury = jo[theother].here[1];
	float urz = jo[theother].here[2];
	logtoall("@stick_read_a: %d=%f,%f,%f, %d=%f,%f,%f\n",thisone,myx,myy,myz, theother,urx,ury,urz);

	float dx = myx-urx;
	float dy = myy-ury;
	float dz = myz-urz;
	float lenlen = dx*dx + dy*dy + dz*dz - ent->LVAL*ent->LVAL;
	dx = 4*lenlen*(myx - urx);
	dy = 4*lenlen*(myy - ury);
	dz = 4*lenlen*(myz - urz);
	logtoall("derivative: %f,%f,%f\n",dx,dy,dz);
	jo[thisone].grad[0] += dx/10000000;
	jo[thisone].grad[1] += dy/10000000;
	jo[thisone].grad[2] += dz/10000000;
}
static void stick_read_b(_obj* ent, int key, struct joint* jo,int thisone)
{
	if('V' != key)return;

	int theother = ent->A_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float myx = jo[thisone].here[0];
	float myy = jo[thisone].here[1];
	float myz = jo[thisone].here[2];
	float urx = jo[theother].here[0];
	float ury = jo[theother].here[1];
	float urz = jo[theother].here[2];
	logtoall("@stick_read_b: %d=%f,%f,%f, %d=%f,%f,%f\n",thisone,myx,myy,myz, theother,urx,ury,urz);

	float dx = myx-urx;
	float dy = myy-ury;
	float dz = myz-urz;
	float lenlen = dx*dx + dy*dy + dz*dz - ent->LVAL*ent->LVAL;
	dx = 4*lenlen*(myx - urx);
	dy = 4*lenlen*(myy - ury);
	dz = 4*lenlen*(myz - urz);
	logtoall("derivative: %f,%f,%f\n",dx,dy,dz);
	jo[thisone].grad[0] += dx/10000000;
	jo[thisone].grad[1] += dy/10000000;
	jo[thisone].grad[2] += dz/10000000;
}




static void stick_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	switch(stack[sp-1].foottype){
	case 'a':stick_read_a(ent,key, buf,len);break;
	case 'b':stick_read_b(ent,key, buf,len);break;
	case 'f':stick_read_force(ent,stack[sp-8].pchip, buf,len);break;
	}
}
static void stick_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void stick_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void stick_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@stick_attach: %.4s,%.4s\n", &self->foottype, &peer->foottype);
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'a':ent->A_PEERFOOT = peer->foottype;break;
		case 'b':ent->B_PEERFOOT = peer->foottype;break;
	}
}




static void stick_search(_obj* ent)
{
}
static void stick_modify(_obj* ent)
{
}
static void stick_delete(_obj* ent)
{
	if(0 == ent)return;
}
static void stick_create(_obj* ent, void* arg, int argc, u8** argv)
{
	if(0 == ent)return;
	int j;
	float L = 1.0;
	for(j=1;j<argc;j++){
		if(0 == ncmp("l:", argv[j], 2))decstr2float(argv[j]+2, &L);
	}
	logtoall("l=%f\n", L);
	ent->LVAL = L;
}




void stick_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s', 't', 'i', 'c','k', 0, 0, 0);

	p->oncreate = (void*)stick_create;
	p->ondelete = (void*)stick_delete;
	p->onreader = (void*)stick_search;
	p->onwriter = (void*)stick_modify;

	p->onattach = (void*)stick_attach;
	p->ondetach = (void*)stick_detach;
	p->ontaking = (void*)stick_taking;
	p->ongiving = (void*)stick_giving;
}
