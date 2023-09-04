#include "libuser.h"
#define A_PEERFOOT whdf.iw0
#define B_PEERFOOT whdf.iwn
#define KVAL listf32.f0
#define LVAL listf32.f1
struct joint{
	float x;
	float y;
	float z;
	int exist;

	float gradx;
	float grady;
	float gradz;
	int sure;
};




static void spring_draw_pixel(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spring_draw_json(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spring_draw_html(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spring_draw_tui(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void spring_draw_cli(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
}




void spring_read_force(_obj* ent, _obj* sup, struct joint* jo, int len)
{
	int a = ent->A_PEERFOOT - 'a';
	int b = ent->B_PEERFOOT - 'a';
	say("@spring_read_force: %d,%d\n",a,b);

	int j;
	vec3 up = {0.0, 0.0, 1.0};
	vec3 vc,vt;
	vec3 vr,vf;
	float* va = &jo[a].x;
	float* vb = &jo[b].x;

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

	//spring length
	vec3_setlen(vr, 10.0);
	vec3_setlen(vf, 10.0);
	for(j=0;j<3;j++)vt[j] = (va[j]-vb[j])/2;
	gl41line_spring(sup, 0x808080, vc,vr,vf,vt);

	//real length
	vec3_setlen(vt, ent->LVAL/2);
	gl41solid_cylinder(sup, 0xffffff, vc, vr, vf, vt);
}




static void spring_read_a(_obj* ent, int key, struct joint* jo, int thisone)
{
	if('R' != key)return;

	int theother = ent->B_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float myx = jo[thisone].x;
	float myy = jo[thisone].y;
	float myz = jo[thisone].z;
	float urx = jo[theother].x;
	float ury = jo[theother].y;
	float urz = jo[theother].z;
	say("@spring_read_a: %d=%f,%f,%f, %d=%f,%f,%f\n",thisone,myx,myy,myz, theother,urx,ury,urz);

	//(0.5*K*dx^2)' = 0.5*K*(len-L)^2, len=sqrt(Xa*Xa + Xb*Xb + Xc*Xc)
	//derivative(Xa) = K*(Xa-X?) -2L*(2*Xa-2*X?)/(2len)
	float dx = myx-urx;
	float dy = myy-ury;
	float dz = myz-urz;
	float len = (ent->LVAL) / squareroot(dx*dx + dy*dy + dz*dz);
	dx = (ent->KVAL)*(myx-urx)*(1 - len);
	dy = (ent->KVAL)*(myy-ury)*(1 - len);
	dz = (ent->KVAL)*(myz-urz)*(1 - len);
	say("derivative: %f,%f,%f\n",dx,dy,dz);
	jo[thisone].gradx += dx/10000;
	jo[thisone].grady += dy/10000;
	jo[thisone].gradz += dz/10000;
}
static void spring_read_b(_obj* ent, int key, struct joint* jo, int thisone)
{
	if('R' != key)return;

	int theother = ent->A_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float myx = jo[thisone].x;
	float myy = jo[thisone].y;
	float myz = jo[thisone].z;
	float urx = jo[theother].x;
	float ury = jo[theother].y;
	float urz = jo[theother].z;
	say("@spring_read_b: %d=%f,%f,%f, %d=%f,%f,%f\n",thisone,myx,myy,myz, theother,urx,ury,urz);

	float dx = myx-urx;
	float dy = myy-ury;
	float dz = myz-urz;
	float len = (ent->LVAL) / squareroot(dx*dx + dy*dy + dz*dz);
	dx = (ent->KVAL)*(myx-urx)*(1 - len);
	dy = (ent->KVAL)*(myy-ury)*(1 - len);
	dz = (ent->KVAL)*(myz-urz)*(1 - len);
	say("derivative: %f,%f,%f\n",dx,dy,dz);
	jo[thisone].gradx += dx/10000;
	jo[thisone].grady += dy/10000;
	jo[thisone].gradz += dz/10000;
}




static void spring_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//say("@spring_read: %.4s\n", &self->foottype);
	switch(stack[sp-1].foottype){
	case 'a':spring_read_a(ent,key, buf,len);break;
	case 'b':spring_read_b(ent,key, buf,len);break;
	case 'f':spring_read_force(ent,stack[sp-8].pchip, buf,len);break;
	}
}
static void spring_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void spring_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void spring_attach(struct halfrel* self, struct halfrel* peer)
{
	//say("@spring_attach: %.4s,%.4s\n", &self->foottype, &peer->foottype);
	_obj* ent = self->pchip;
	switch(self->foottype){
		case 'a':ent->A_PEERFOOT = peer->foottype;break;
		case 'b':ent->B_PEERFOOT = peer->foottype;break;
	}
}




static void spring_search(_obj* ent)
{
}
static void spring_modify(_obj* ent)
{
}
static void spring_delete(_obj* ent)
{
	if(0 == ent)return;
}
static void spring_create(_obj* ent, void* arg, int argc, u8** argv)
{
	if(0 == ent)return;
	int j;
	float K = 1.0;
	float L = 1.0;
	for(j=1;j<argc;j++){
		if(0 == ncmp("k:", argv[j], 2))decstr2float(argv[j]+2, &K);
		if(0 == ncmp("l:", argv[j], 2))decstr2float(argv[j]+2, &L);
	}
	say("k=%f,l=%f\n", K, L);
	ent->KVAL = K;
	ent->LVAL = L;
}




void spring_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s', 'p', 'r', 'i', 'n','g', 0, 0);

	p->oncreate = (void*)spring_create;
	p->ondelete = (void*)spring_delete;
	p->onreader = (void*)spring_search;
	p->onwriter = (void*)spring_modify;

	p->onattach = (void*)spring_attach;
	p->ondetach = (void*)spring_detach;
	p->ontaking = (void*)spring_taking;
	p->ongiving = (void*)spring_giving;
}
