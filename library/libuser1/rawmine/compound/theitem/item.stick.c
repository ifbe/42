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




static void stick_draw_gl41(
	struct entity* ent, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
}
static void stick_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
		stick_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}
void stick_read_force(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, struct joint* jo, int len)
{
	struct entity* ent = self->pchip;
	struct entity* sup = peer->pchip;
	int a = ent->A_PEERFOOT - 'a';
	int b = ent->B_PEERFOOT - 'a';
	say("@stick_read_force: %d,%d\n",a,b);

	gl41line(sup, 0xffffff, &jo[a].x, &jo[b].x);
}




static void stick_read_a(struct halfrel* self, struct halfrel* peer, void* arg, int idx, struct joint* jo, int thisone)
{
	if(idx != 'V')return;

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
static void stick_read_b(struct halfrel* self, struct halfrel* peer, void* arg, int idx, struct joint* jo, int thisone)
{
	if(idx != 'V')return;

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




static void stick_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	switch(self->flag){
	case 'a':stick_read_a(self,peer, arg,idx, buf,len);break;
	case 'b':stick_read_b(self,peer, arg,idx, buf,len);break;
	case 'f':stick_read_force(self,peer, arg,idx, buf,len);break;
	default: stick_read_bycam(self,peer, arg,idx, buf,len);
	}
}
static void stick_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
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
	p->onread  = (void*)stick_read;
	p->onwrite = (void*)stick_write;
}
