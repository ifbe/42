#include "libuser.h"
#define mybuf buf0
void world2clip_projz0z1_transpose(mat4 mat, struct fstyle* frus);
void world2clip_projznzp_transpose(mat4 mat, struct fstyle* frus);
void gl41data_addcam(struct entity* wnd, struct gl41data* data);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);


struct portalbuf{
	mat4 wvp;
	vec4 vc;
	struct entity* peer_portal;
	struct portalbuf* peer_buf;

	struct entity* selfworld_body;
	struct style* selfworld_geom;
	struct entity* peerworld_body;
	struct style* peerworld_geom;

	struct gl41data fbo;
	struct gl41data mesh;
};




static void portal_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void portal_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void portal_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void portal_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void portal_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




//want(frus) know(cam, selfgeom,peergeom)
void portal_frustum(struct fstyle* frus, float* cam, struct fstyle* selfgeom, struct fstyle* peergeom)
{
	float x0,y0,z0,t0;
	float delta;
	//say("self@(%f,%f,%f), peer@(%f,%f,%f)\n",selfgeom->vc[0],selfgeom->vc[1],selfgeom->vc[2],peergeom->vc[0],peergeom->vc[1],peergeom->vc[2]);

//-------------q---------------
	//portal.n
	x0 = peergeom->vf[0];
	y0 = peergeom->vf[1];
	z0 = peergeom->vf[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	//len(eye to q) = vec(eye to center) * portal.n
	t0= (peergeom->vc[0] - cam[0])*x0
	  + (peergeom->vc[1] - cam[1])*y0
	  + (peergeom->vc[2] - cam[2])*z0;

	//foot of a perpendicular: q = eye + len(eye to q)*portal.n
	frus->vq[0] = cam[0] + t0*x0;
	frus->vq[1] = cam[1] + t0*y0;
	frus->vq[2] = cam[2] + t0*z0;

	frus->vc[0] = selfgeom->vc[0];
	frus->vc[1] = selfgeom->vc[1];
	frus->vc[2] = selfgeom->vc[2];

	//say("q,c = (%f,%f,%f), (%f,%f,%f)\n", frus->vq[0], frus->vq[1], frus->vq[2], frus->vc[0], frus->vc[1], frus->vc[2]);


//----------------n,f----------------
	frus->vn[3] = t0;
	frus->vf[3] = 1000000000.0;

	//normalize(self.vf) = t*portal.n + offset
	x0 = selfgeom->vf[0];
	y0 = selfgeom->vf[1];
	z0 = selfgeom->vf[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	frus->vn[0] = x0;
	frus->vn[1] = y0;
	frus->vn[2] = z0;

	frus->vf[0] = x0;
	frus->vf[1] = y0;
	frus->vf[2] = z0;

	frus->vc[0] -= x0 * (frus->vn[3]);
	frus->vc[1] -= y0 * (frus->vn[3]);
	frus->vc[2] -= z0 * (frus->vn[3]);

	frus->vn[3] *= 1.001;

/*	say("n,f,c = (%f,%f,%f,%f), (%f,%f,%f,%f), (%f,%f,%f)\n",
		frus->vn[0], frus->vn[1], frus->vn[2], frus->vn[3],
		frus->vf[0], frus->vf[1], frus->vf[2], frus->vf[3],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/


//--------------l,r--------------------
	//nr(portal) = norm(portal.r)
	x0 = peergeom->vr[0];
	y0 = peergeom->vr[1];
	z0 = peergeom->vr[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	//l.len = (l-q) * nr
	t0= (peergeom->vc[0] - peergeom->vr[0] - frus->vq[0]) * x0
	  + (peergeom->vc[1] - peergeom->vr[1] - frus->vq[1]) * y0
	  + (peergeom->vc[2] - peergeom->vr[2] - frus->vq[2]) * z0;
	frus->vl[3] = t0;

	//r.len = (r-q) * nr
	t0= (peergeom->vc[0] + peergeom->vr[0] - frus->vq[0]) * x0
	  + (peergeom->vc[1] + peergeom->vr[1] - frus->vq[1]) * y0
	  + (peergeom->vc[2] + peergeom->vr[2] - frus->vq[2]) * z0;
	frus->vr[3] = t0;

	//delta in right direction
	delta = (frus->vq[0] - peergeom->vc[0]) * x0
	      + (frus->vq[1] - peergeom->vc[1]) * y0
		  + (frus->vq[2] - peergeom->vc[2]) * z0;

	//nr(target) = norm(target.r)
	x0 = selfgeom->vr[0];
	y0 = selfgeom->vr[1];
	z0 = selfgeom->vr[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	frus->vl[0] = x0;
	frus->vl[1] = y0;
	frus->vl[2] = z0;

	frus->vr[0] = x0;
	frus->vr[1] = y0;
	frus->vr[2] = z0;

	frus->vc[0] += x0 * delta;
	frus->vc[1] += y0 * delta;
	frus->vc[2] += z0 * delta;

/*	say("l,r,c = (%f,%f,%f,%f), (%f,%f,%f,%f), (%f,%f,%f)\n",
		frus->vl[0], frus->vl[1], frus->vl[2], frus->vl[3],
		frus->vr[0], frus->vr[1], frus->vr[2], frus->vr[3],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/


//----------------b,t-----------------
	//nt(portal) = norm(portal.t)
	x0 = peergeom->vt[0];
	y0 = peergeom->vt[1];
	z0 = peergeom->vt[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	//b.len =  = (b-q) * nt
	t0= (peergeom->vc[0] - peergeom->vt[0] - frus->vq[0]) * x0
	  + (peergeom->vc[1] - peergeom->vt[1] - frus->vq[1]) * y0
	  + (peergeom->vc[2] - peergeom->vt[2] - frus->vq[2]) * z0;
	frus->vb[3] = t0;

	//t.len = (u-q) * nt
	t0= (peergeom->vc[0] + peergeom->vt[0] - frus->vq[0]) * x0
	  + (peergeom->vc[1] + peergeom->vt[1] - frus->vq[1]) * y0
	  + (peergeom->vc[2] + peergeom->vt[2] - frus->vq[2]) * z0;
	frus->vt[3] = t0;

	//delta in front direction
	delta = (frus->vq[0] - peergeom->vc[0]) * x0
	      + (frus->vq[1] - peergeom->vc[1]) * y0
		  + (frus->vq[2] - peergeom->vc[2]) * z0;

	//nt(target) = norm(target.t)
	x0 = selfgeom->vt[0];
	y0 = selfgeom->vt[1];
	z0 = selfgeom->vt[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	frus->vb[0] = x0;
	frus->vb[1] = y0;
	frus->vb[2] = z0;

	frus->vt[0] = x0;
	frus->vt[1] = y0;
	frus->vt[2] = z0;

	frus->vc[0] += x0 * delta;
	frus->vc[1] += y0 * delta;
	frus->vc[2] += z0 * delta;

/*	say("b,t,c = (%f,%f,%f,%f), (%f,%f,%f,%f), (%f,%f,%f)\n",
		frus->vb[0], frus->vb[1], frus->vb[2], frus->vb[3],
		frus->vt[0], frus->vt[1], frus->vt[2], frus->vt[3],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/
}




void portal_fbo_update(
	struct entity* act, struct style* part,
	struct entity* wrd, struct style* geom,
	struct entity* wrl, struct style* camg,
	struct supply* wnd, struct style* area)
{
	struct portalbuf* ptr = act->mybuf;
	if(0 == ptr)return;
	struct gl41data* data = &ptr->fbo;
	if(0 == data)return;

	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = ptr->wvp;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = geom->frus.vc;
	gl41data_addcam((void*)wnd, data);
}
void portal_fbo_prepare(struct gl41data* data)
{
	struct mysrc* src = &data->src;

	src->tex[0].w = 1024;
	src->tex[0].h = 1024;
	src->tex[0].fmt = 0;
	src->tex[0].glfd = 0;

	src->type = 'c';
	src->target_enq = 42;
}




static void portal_mesh_update(
	struct entity* act, struct portalbuf* portal,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tr,tf,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	gl41line_rect(ctx, 0xffffff, vc, vr, vt);
	tc[0] = vc[0] - vf[0]/2 - vt[0];
	tc[1] = vc[1] - vf[1]/2 - vt[1];
	tc[2] = vc[2] - vf[2]/2 - vt[2];
	tf[0] = vf[0] / 2;
	tf[1] = vf[1] / 2;
	tf[2] = vf[2] / 2;
	gl41line_rect(ctx, 0xffffff, tc, vr, tf);
	tc[0] = vc[0] - vt[0] - vf[0];
	tc[1] = vc[1] - vt[1] - vf[1];
	tc[2] = vc[2] - vt[2] - vf[2];
	tt[0] = vc[0] - vt[0];
	tt[1] = vc[1] - vt[1];
	tt[2] = vc[2] - vt[2];
	gl41line_arrow(ctx, 0xffffff, tc, tt, vt);


	struct portalbuf* selfptr = act->mybuf;
	if(0 == selfptr)return;
	struct portalbuf* peerptr = selfptr->peer_buf;
	if(0 == peerptr)return;
//.texture
	struct mysrc* src = &selfptr->mesh.src;
	if(0 == src)return;
	src->tex[0].glfd = peerptr->fbo.dst.tex[0];
	src->tex[0].fmt = '!';
	src->tex_enq[0] += 1;
	peerptr->mesh.dst.texname[0] = "tex0";

//.vertex
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vt[0];
	vbuf[0][1] = vc[1] - vr[1] - vt[1];
	vbuf[0][2] = vc[2] - vr[2] - vt[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vt[0];
	vbuf[1][1] = vc[1] + vr[1] + vt[1];
	vbuf[1][2] = vc[2] + vr[2] + vt[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vt[0];
	vbuf[2][1] = vc[1] - vr[1] + vt[1];
	vbuf[2][2] = vc[2] - vr[2] + vt[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vt[0];
	vbuf[3][1] = vc[1] + vr[1] + vt[1];
	vbuf[3][2] = vc[2] + vr[2] + vt[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vt[0];
	vbuf[4][1] = vc[1] - vr[1] - vt[1];
	vbuf[4][2] = vc[2] - vr[2] - vt[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vt[0];
	vbuf[5][1] = vc[1] + vr[1] - vt[1];
	vbuf[5][2] = vc[2] + vr[2] - vt[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}
static void portal_mesh_prepare(struct gl41data* data)
{
	struct mysrc* src = &data->src;

	//
	src->vs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/portal/vert.glsl", 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/portal/frag.glsl", 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//vertex
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
}




static void portal_wrl_cam_wnd(_ent* ent,void* foot, _syn* stack,int sp)
{
	if(0 == stack)return;
	//say("@portal_read:%p,%p\n", ent,foot);

	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;

	struct portalbuf* selfptr = ent->mybuf;
	struct portalbuf* peerptr = selfptr->peer_buf;
	selfptr->peerworld_body = peerptr->selfworld_body;
	selfptr->peerworld_geom = peerptr->selfworld_geom;
	//say("selfptr=%p,peerptr=%p\n",selfptr,peerptr);

	struct fstyle* selfshap = &geom->fs;
	struct fstyle* peershap = &peerptr->selfworld_geom->fshape;
	//say("selfgeom=%p,peergeom=%p\n",selfshap,peershap);

	//
	portal_frustum(&geom->frus, camg->frus.vc, selfshap, peershap);
	if(_gl41list_ == wnd->fmt)world2clip_projznzp_transpose(selfptr->wvp, &geom->frus);
	else world2clip_projz0z1_transpose(selfptr->wvp, &geom->frus);

	//create or update fbo
	portal_fbo_update(ent,foot, wor,geom, dup,camg, (void*)wnd,area);

	//geom
	portal_mesh_update(ent,foot, wor,geom, wnd,area);
}




static void portal_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		portal_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void portal_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void portal_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void portal_linkup(struct halfrel* self, struct halfrel* peer)
{
	struct entity* ent = self->pchip;
	if(0 == ent)return;

	struct portalbuf* ptr = ent->mybuf;
	if(0 == ptr)return;

	struct entity* peer_portal;
	switch(self->flag){
	case hex32('p','e','e','r'):
		peer_portal = peer->pchip;
		if(0 == peer_portal)break;

		ptr->peer_portal = peer_portal;
		ptr->peer_buf = peer_portal->mybuf;
		break;
	case hex32('s','k','i','n'):
		ptr->selfworld_body = peer->pchip;
		ptr->selfworld_geom = peer->pfoot;
		break;
	}//switch
}




static void portal_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		if(foot == rel->dstflag){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void portal_modify(struct entity* act)
{
}
static void portal_delete(struct entity* act)
{
}
static void portal_create(struct entity* act, void* str)
{
	struct portalbuf* aa = act->mybuf = memorycreate(0x10000, 0);
	if(0 == aa)return;

	portal_fbo_prepare(&aa->fbo);

	portal_mesh_prepare(&aa->mesh);
}




void portal_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'o', 'r', 't', 'a', 'l', 0, 0);

	p->oncreate = (void*)portal_create;
	p->ondelete = (void*)portal_delete;
	p->onsearch = (void*)portal_search;
	p->onmodify = (void*)portal_modify;

	p->onlinkup = (void*)portal_linkup;
	p->ondiscon = (void*)portal_discon;
	p->ontaking = (void*)portal_taking;
	p->ongiving = (void*)portal_giving;
}
