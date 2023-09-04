#include "libuser.h"




void dbgpid_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41opaque_rect(ctx, 0x40ffd010, vc, vr, vf);

	struct style* sty = act->priv_ptr;
	if(0 == sty)return;

	int j;
	vec3 tc,tr,tf,tt;
	for(j=0;j<3;j++){
		tc[j] = vc[j]-vr[j]+vf[j]*7/8;
		tr[j] = vr[j]/20;
		tf[j] = vf[j]/20;
	}
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_x[0]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]*2/3+vf[j]*7/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_x[1]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]/3+vf[j]*7/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_x[2]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]+vf[j]*6/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_v[0]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]*2/3+vf[j]*6/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_v[1]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]/3+vf[j]*6/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_v[2]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]+vf[j]*5/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_a[0]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]*2/3+vf[j]*5/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_a[1]);
	for(j=0;j<3;j++)tc[j] = vc[j]-vr[j]/3+vf[j]*5/8;
	gl41float(ctx,0xff0000, tc,tr,tf, sty->actual.displace_a[2]);

	for(j=0;j<3;j++)tc[j] = vc[j]+vf[j]*7/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_x[0]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j]/3+vf[j]*7/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_x[1]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j]*2/3+vf[j]*7/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_x[2]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vf[j]*6/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_v[0]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j]/3+vf[j]*6/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_v[1]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j]*2/3+vf[j]*6/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_v[2]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vf[j]*5/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_a[0]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j]/3+vf[j]*5/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_a[1]);
	for(j=0;j<3;j++)tc[j] = vc[j]+vr[j]*2/3+vf[j]*5/8;
	gl41float(ctx,0x0000ff, tc,tr,tf, sty->desire.displace_a[2]);
}
static void dbgpid_read_byworld_bywnd(_obj* ent,struct style* slot, _syn* stack,int sp)
{
	//say("@%s\n",__func__);
	if(0 == stack)return;

	_obj* wor = stack[sp-2].pchip;
	struct style* geom = stack[sp-2].pfoot;
	_obj* wnd = stack[sp-4].pchip;
	struct style* area = stack[sp-4].pfoot;

	float x = (geom->fs.vc[0] + geom->fs.vq[0])/2;
	float y = (geom->fs.vc[1] + geom->fs.vq[1])/2;
	float r = (geom->fs.vq[0] - geom->fs.vc[0])/2;
	float f = (geom->fs.vq[1] - geom->fs.vc[1])/2;

	struct fstyle fs;
	fs.vc[0] =   x;fs.vc[1] =   y;fs.vc[2] = 0.0;
	fs.vr[0] =   r;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] =   f;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 0.0;
	dbgpid_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
}




static void dbgpid_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* caller = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _virtual_:
		dbgpid_read_byworld_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void dbgpid_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void dbgpid_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void dbgpid_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void dbgpid_reader(_obj* act)
{
}
static void dbgpid_writer(_obj* ent,void* foot, void* arg,int key, void* buf,int len)
{
	//say("%s: %p, %p\n",__func__, ent, buf);
	ent->priv_ptr = buf;
}
static void dbgpid_delete(_obj* act)
{
}
static void dbgpid_create(_obj* act, u8* arg, int argc, u8** argv)
{
	act->priv_ptr = 0;
}




void dbgpid_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('d', 'b', 'g', 'p', 'i', 'd', 0, 0);

	p->oncreate = (void*)dbgpid_create;
	p->ondelete = (void*)dbgpid_delete;
	p->onreader = (void*)dbgpid_reader;
	p->onwriter = (void*)dbgpid_writer;

	p->onattach = (void*)dbgpid_attach;
	p->ondetach = (void*)dbgpid_detach;
	p->ontaking = (void*)dbgpid_taking;
	p->ongiving = (void*)dbgpid_giving;
}
