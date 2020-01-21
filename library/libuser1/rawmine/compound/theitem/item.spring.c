#include "libuser.h"
#define A_PEERFOOT iw0
#define B_PEERFOOT iwn
#define KVAL fdata0
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




static void spring_draw_pixel(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spring_draw_json(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spring_draw_html(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spring_draw_tui(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void spring_draw_cli(
	struct entity* ent, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void spring_draw_gl41(
	struct entity* ent, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
}
static void spring_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> terminal
	struct entity* scn;struct style* geom;
	struct entity* ent;struct style* slot;

	if(stack && ('v' == len)){
		ent = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		spring_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}




static void spring_read_a(struct halfrel* self, struct halfrel* peer, void* arg, int idx, struct joint* jo, int thisone)
{
	if(idx != 'R')return;

	struct entity* ent = self->pchip;
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
static void spring_read_b(struct halfrel* self, struct halfrel* peer, void* arg, int idx, struct joint* jo, int thisone)
{
	if(idx != 'R')return;

	struct entity* ent = self->pchip;
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




static void spring_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	switch(self->flag){
	case 'a':spring_read_a(self,peer, arg,idx, buf,len);break;
	case 'b':spring_read_b(self,peer, arg,idx, buf,len);break;
	default: spring_read_bycam(self,peer, arg,idx, buf,len);
	}
}
static void spring_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void spring_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void spring_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	switch(self->flag){
		case 'a':ent->A_PEERFOOT = peer->flag;break;
		case 'b':ent->B_PEERFOOT = peer->flag;break;
	}
}




static void spring_search(struct entity* ent)
{
}
static void spring_modify(struct entity* ent)
{
}
static void spring_delete(struct entity* ent)
{
	if(0 == ent)return;
}
static void spring_create(struct entity* ent, void* arg, int argc, u8** argv)
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




void spring_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'p', 'r', 'i', 'n','g', 0, 0);

	p->oncreate = (void*)spring_create;
	p->ondelete = (void*)spring_delete;
	p->onsearch = (void*)spring_search;
	p->onmodify = (void*)spring_modify;

	p->onlinkup = (void*)spring_linkup;
	p->ondiscon = (void*)spring_discon;
	p->onread  = (void*)spring_read;
	p->onwrite = (void*)spring_write;
}
