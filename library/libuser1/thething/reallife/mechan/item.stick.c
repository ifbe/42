#include "libuser.h"
#define A_PEERFOOT iw0
#define B_PEERFOOT iwn
#define LVAL fdata1
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




static void stick_draw_pixel(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_json(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_html(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_tui(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void stick_draw_cli(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}




void stick_read_force(struct entity* ent, struct entity* sup, struct joint* jo, int len)
{
	int a = ent->A_PEERFOOT - 'a';
	int b = ent->B_PEERFOOT - 'a';
	say("@stick_read_force: %d,%d\n",a,b);

	float* va = &jo[a].x;
	float* vb = &jo[b].x;
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




static void stick_read_a(struct entity* ent, int key, struct joint* jo,int thisone)
{
	if('V' != key)return;

	int theother = ent->B_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float myx = jo[thisone].x;
	float myy = jo[thisone].y;
	float myz = jo[thisone].z;
	float urx = jo[theother].x;
	float ury = jo[theother].y;
	float urz = jo[theother].z;
	say("@stick_read_a: %d=%f,%f,%f, %d=%f,%f,%f\n",thisone,myx,myy,myz, theother,urx,ury,urz);

	float dx = myx-urx;
	float dy = myy-ury;
	float dz = myz-urz;
	float lenlen = dx*dx + dy*dy + dz*dz - ent->LVAL*ent->LVAL;
	dx = 4*lenlen*(myx - urx);
	dy = 4*lenlen*(myy - ury);
	dz = 4*lenlen*(myz - urz);
	say("derivative: %f,%f,%f\n",dx,dy,dz);
	jo[thisone].gradx += dx/10000000;
	jo[thisone].grady += dy/10000000;
	jo[thisone].gradz += dz/10000000;
}
static void stick_read_b(struct entity* ent, int key, struct joint* jo,int thisone)
{
	if('V' != key)return;

	int theother = ent->A_PEERFOOT - 'a';
	if(theother < 0)return;
	if(theother > 8)return;

	float myx = jo[thisone].x;
	float myy = jo[thisone].y;
	float myz = jo[thisone].z;
	float urx = jo[theother].x;
	float ury = jo[theother].y;
	float urz = jo[theother].z;
	say("@stick_read_b: %d=%f,%f,%f, %d=%f,%f,%f\n",thisone,myx,myy,myz, theother,urx,ury,urz);

	float dx = myx-urx;
	float dy = myy-ury;
	float dz = myz-urz;
	float lenlen = dx*dx + dy*dy + dz*dz - ent->LVAL*ent->LVAL;
	dx = 4*lenlen*(myx - urx);
	dy = 4*lenlen*(myy - ury);
	dz = 4*lenlen*(myz - urz);
	say("derivative: %f,%f,%f\n",dx,dy,dz);
	jo[thisone].gradx += dx/10000000;
	jo[thisone].grady += dy/10000000;
	jo[thisone].gradz += dz/10000000;
}




static void stick_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(foot){
	case 'a':stick_read_a(ent,key, buf,len);break;
	case 'b':stick_read_b(ent,key, buf,len);break;
	case 'f':stick_read_force(ent,stack[sp-8].pchip, buf,len);break;
	}
}
static void stick_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void stick_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void stick_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@stick_linkup: %.4s,%.4s\n", &self->flag, &peer->flag);
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'a':ent->A_PEERFOOT = peer->flag;break;
		case 'b':ent->B_PEERFOOT = peer->flag;break;
	}
}




static void stick_search(struct entity* ent)
{
}
static void stick_modify(struct entity* ent)
{
}
static void stick_delete(struct entity* ent)
{
	if(0 == ent)return;
}
static void stick_create(struct entity* ent, void* arg, int argc, u8** argv)
{
	if(0 == ent)return;
	int j;
	float L = 1.0;
	for(j=1;j<argc;j++){
		if(0 == ncmp("l:", argv[j], 2))decstr2float(argv[j]+2, &L);
	}
	say("l=%f\n", L);
	ent->LVAL = L;
}




void stick_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 't', 'i', 'c','k', 0, 0, 0);

	p->oncreate = (void*)stick_create;
	p->ondelete = (void*)stick_delete;
	p->onsearch = (void*)stick_search;
	p->onmodify = (void*)stick_modify;

	p->onlinkup = (void*)stick_linkup;
	p->ondiscon = (void*)stick_discon;
	p->ontaking = (void*)stick_taking;
	p->ongiving = (void*)stick_giving;
}
