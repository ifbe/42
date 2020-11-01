#include "libuser.h"
#define _fboa_ hex32('f','b','o','a')
#define _fbob_ hex32('f','b','o','b')
#define Aside buf0
#define Bside buf1
void matproj_transpose(void* m, struct fstyle* sty);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);


struct portalbuf{
	mat4 mvp;
	vec4 vc;
	struct entity* world;
	struct style* geom;
	struct supply* fbo;
	struct style* rect;
	struct gl41data forwnd;
	struct gl41data forfbo;
};
void portal_forwnd(struct mysrc* src)
{
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
void portal_forwnd_update(struct entity* act, struct portalbuf* p, struct supply* fbo, struct style* area)
{
	struct gl41data* data = &p->forwnd;
	if(0 == data)return;

	data->dst.texname[0] = "tex0";
	data->src.tex[0].glfd = fbo->tex0;
	data->src.tex[0].fmt = '!';
	data->src.tex_enq[0] += 1;
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
	struct portalbuf* aa;
	struct portalbuf* bb;
	if(0 == act)return;

	//aside
	aa = act->Aside = memorycreate(0x1000, 0);
	if(0 == aa)return;
	portal_forwnd(&aa->forwnd.src);

	//bside
	bb = act->Bside = memorycreate(0x1000, 0);
	if(0 == bb)return;
	portal_forwnd(&bb->forwnd.src);
}




static void portal_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void portal_draw_gl41(
	struct entity* act, struct portalbuf* portal,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	vec3 tc,tt;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	gl41line_rect(ctx, 0xffffff, vc, vr, vt);
	tc[0] = vc[0] - vt[0];
	tc[1] = vc[1] - vt[1];
	tc[2] = vc[2] - vt[2];
	gl41solid_rect(ctx, 0x00003f, tc, vr, vf);
	tc[0] = vc[0] - vt[0] - vf[0];
	tc[1] = vc[1] - vt[1] - vf[1];
	tc[2] = vc[2] - vt[2] - vf[2];
	tt[0] = vc[0] - vt[0] + vf[0];
	tt[1] = vc[1] - vt[1] + vf[1];
	tt[2] = vc[2] - vt[2] + vf[2];
	gl41line_arrow(ctx, 0xffffff, tc, tt, vt);


	if(0 == portal)return;
	struct mysrc* src = &portal->forwnd.src;
	if(0 == src)return;
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




void portal_frustum(struct fstyle* por, struct fstyle* tar, struct fstyle* frus, vec3 cam)
{
	float x0,y0,z0,t0;
	float delta;

//-------------q---------------
	//portal.n
	x0 = por->vf[0];
	y0 = por->vf[1];
	z0 = por->vf[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	//op*cos(on,op): t = op * portal.n
	t0= (por->vc[0] - cam[0])*x0
	  + (por->vc[1] - cam[1])*y0
	  + (por->vc[2] - cam[2])*z0;

	//foot of a perpendicular: q = p - t*portal.n
	frus->vq[0] = cam[0] + t0*x0;
	frus->vq[1] = cam[1] + t0*y0;
	frus->vq[2] = cam[2] + t0*z0;

	frus->vc[0] = tar->vc[0];
	frus->vc[1] = tar->vc[1];
	frus->vc[2] = tar->vc[2];

	//length(frus.n) > length(camera to portal)
	frus->vn[3] = t0;
	frus->vf[3] = 1e20;
	//say("q,c = (%f,%f,%f), (%f,%f,%f)\n", frus->vq[0], frus->vq[1], frus->vq[2], frus->vc[0], frus->vc[1], frus->vc[2]);


//----------------n,f----------------
	//dir*len: fbo.n = t*portal.n + offset
	x0 = tar->vf[0];
	y0 = tar->vf[1];
	z0 = tar->vf[2];
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

/*	say("n,f,c = (%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n",
		frus->vn[0], frus->vn[1], frus->vn[2],
		frus->vf[0], frus->vf[1], frus->vf[2],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/


//--------------l,r--------------------
	//nr(portal) = norm(portal.r)
	x0 = por->vr[0];
	y0 = por->vr[1];
	z0 = por->vr[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	//l.len = (l-q) * nr
	t0= (por->vc[0] - por->vr[0] - frus->vq[0]) * x0
	  + (por->vc[1] - por->vr[1] - frus->vq[1]) * y0
	  + (por->vc[2] - por->vr[2] - frus->vq[2]) * z0;
	frus->vl[3] = t0;

	//r.len = (r-q) * nr
	t0= (por->vc[0] + por->vr[0] - frus->vq[0]) * x0
	  + (por->vc[1] + por->vr[1] - frus->vq[1]) * y0
	  + (por->vc[2] + por->vr[2] - frus->vq[2]) * z0;
	frus->vr[3] = t0;

	//delta z
	delta = (frus->vq[0] - por->vc[0]) * x0
	      + (frus->vq[1] - por->vc[1]) * y0
		  + (frus->vq[2] - por->vc[2]) * z0;

	//nr(target) = norm(target.r)
	x0 = tar->vr[0];
	y0 = tar->vr[1];
	z0 = tar->vr[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	frus->vl[0] = -x0;
	frus->vl[1] = -y0;
	frus->vl[2] = -z0;

	frus->vr[0] = x0;
	frus->vr[1] = y0;
	frus->vr[2] = z0;

	frus->vc[0] += x0 * delta;
	frus->vc[1] += y0 * delta;
	frus->vc[2] += z0 * delta;

/*	say("l,r,c = (%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n",
		frus->vl[0], frus->vl[1], frus->vl[2],
		frus->vr[0], frus->vr[1], frus->vr[2],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/


//----------------b,t-----------------
	//nt(portal) = norm(portal.t)
	x0 = por->vt[0];
	y0 = por->vt[1];
	z0 = por->vt[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	//b.len =  = (b-q) * nt
	t0= (por->vc[0] - por->vt[0] - frus->vq[0]) * x0
	  + (por->vc[1] - por->vt[1] - frus->vq[1]) * y0
	  + (por->vc[2] - por->vt[2] - frus->vq[2]) * z0;
	frus->vb[3] = t0;

	//t.len = (u-q) * nt
	t0= (por->vc[0] + por->vt[0] - frus->vq[0]) * x0
	  + (por->vc[1] + por->vt[1] - frus->vq[1]) * y0
	  + (por->vc[2] + por->vt[2] - frus->vq[2]) * z0;
	frus->vt[3] = t0;

	//delta z
	delta = (frus->vq[0] - por->vc[0]) * x0
	      + (frus->vq[1] - por->vc[1]) * y0
		  + (frus->vq[2] - por->vc[2]) * z0;

	//nt(target) = norm(target.r)
	x0 = tar->vt[0];
	y0 = tar->vt[1];
	z0 = tar->vt[2];
	t0 = squareroot(x0*x0 + y0*y0 + z0*z0);
	x0 /= t0;
	y0 /= t0;
	z0 /= t0;

	frus->vb[0] = -x0;
	frus->vb[1] = -y0;
	frus->vb[2] = -z0;

	frus->vt[0] = x0;
	frus->vt[1] = y0;
	frus->vt[2] = z0;

	frus->vc[0] += x0 * delta;
	frus->vc[1] += y0 * delta;
	frus->vc[2] += z0 * delta;

/*	say("b,t,c = (%f,%f,%f), (%f,%f,%f), (%f,%f,%f)\n",
		frus->vb[0], frus->vb[1], frus->vb[2],
		frus->vt[0], frus->vt[1], frus->vt[2],
		frus->vc[0], frus->vc[1], frus->vc[2]);*/


//--------------larger vn--------------
	frus->vn[3] *= 1.001;
/*	say("portal_frustum: (%f,%f), (%f,%f), (%f,%f)\n",
		frus->vn[3], frus->vf[3], frus->vl[3], frus->vr[3], frus->vb[3], frus->vt[3]);*/
}
static void portal_matrix(
	struct entity* act, float* eye,
	struct portalbuf* this, struct portalbuf* that)
{
	struct fstyle* peershap = &that->geom->fshape;

	struct fstyle* selfshap = &this->geom->fshape;
	struct fstyle* frus = &this->geom->frustum;
	portal_frustum(selfshap, peershap, frus, eye);
	matproj_transpose(this->mvp, frus);


	struct gl41data* data = &this->forfbo;
	data->dst.arg[0].fmt = 'm';
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].data = this->mvp;
	data->dst.arg[1].fmt = 'v';
	data->dst.arg[1].name = "camxyz";
	data->dst.arg[1].data = frus->vc;
	this->fbo->glfull_camera[0] = &this->forfbo;
}




static void portal_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;
	say("@portal_read:%.4s,%llx\n", &foot, stack[sp-2].pfoot);

	struct entity* wor;struct style* geom;
	struct entity* dup;struct style* camg;
	struct entity* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	dup = stack[sp-3].pchip;camg = stack[sp-3].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	if('v' == key){
		if('a' == stack[sp-1].flag){
			portal_draw_gl41(ent,ent->Aside, wor,geom, wnd,area);
		}
		if('b' == stack[sp-1].flag){
			portal_draw_gl41(ent,ent->Bside, wor,geom, wnd,area);
		}
	}
	if('?' == key){
		if('a' == stack[sp-1].flag){
			struct portalbuf* p = ent->Aside;
			portal_matrix(ent, camg->frus.vc, ent->Aside, ent->Bside);
			give_data_into_peer(ent,_fboa_, stack,sp, 0,0, 0,0);
			portal_forwnd_update(ent, p, p->fbo, p->rect);
		}
		if('b' == stack[sp-1].flag){
			struct portalbuf* p = ent->Bside;
			portal_matrix(ent, camg->frus.vc, ent->Bside, ent->Aside);
			give_data_into_peer(ent,_fbob_, stack,sp, 0,0, 0,0);
			portal_forwnd_update(ent, p, p->fbo, p->rect);
		}
/*
		//update matrix for fbo
		struct supply* fbo = rel[1]->pchip;
		struct style* rect = rel[1]->pfoot;
		water_forfbo_update(ent,slot, wor,geom, dup,camg, fbo,rect);

		//wnd.data -> fbo.texture
		give_data_into_peer(ent,_fbo_, stack,sp, 0,0, 0,0);

		//fbo.texture -> my.data -> wnd.data
		water_forwnd_update(ent,slot, fbo,rect);*/
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
	struct entity* ent;
	struct portalbuf* p;

	ent = self->pchip;
	if(0==ent)return;

	switch(self->flag){
	case 'a':{
		p = ent->Aside;
		p->world = peer->pchip;
		p->geom = peer->pfoot;
		break;
	}
	case 'b':{
		p = ent->Bside;
		p->world = peer->pchip;
		p->geom = peer->pfoot;
		break;
	}
	case _fboa_:{
		p = ent->Aside;
		p->fbo = peer->pchip;
		p->rect = peer->pfoot;
		break;
	}
	case _fbob_:{
		p = ent->Bside;
		p->fbo = peer->pchip;
		p->rect = peer->pfoot;
		break;
	}
	}//switch
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
