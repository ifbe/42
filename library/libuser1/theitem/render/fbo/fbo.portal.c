#include "libuser.h"
void matproj_transpose(void* m, struct fstyle* sty);
void loadtexfromfile(struct glsrc* src, int idx, char* name);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


struct portalbuf{
	mat4 mvp;
	vec4 vc;
	vec4 vq;
	u8 data[0];
};
void portal_forwnd(struct glsrc* src)
{
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/portal/vert.glsl", 0, src->vs, 0x1000);
	src->fs = memorycreate(0x1000, 0);
	openreadclose("datafile/shader/portal/frag.glsl", 0, src->fs, 0x1000);
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
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
	struct portalbuf* portal;
	struct glsrc* src;
	if(0 == act)return;

	portal = act->buf0 = memorycreate(0x1000, 0);
	if(0 == portal)return;
	src = (void*)(portal->data);
	portal_forwnd(src);
}




static void portal_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
}
static void portal_draw_gl41_b(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	struct portalbuf* portal;
	vec3 tc,tt;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;

	gl41line_rect(win, 0xffffff, vc, vr, vt);
	tc[0] = vc[0] - vt[0];
	tc[1] = vc[1] - vt[1];
	tc[2] = vc[2] - vt[2];
	gl41solid_rect(win, 0x3f0000, tc, vr, vf);
	tc[0] = vc[0] - vt[0] - vf[0];
	tc[1] = vc[1] - vt[1] - vf[1];
	tc[2] = vc[2] - vt[2] - vf[2];
	tt[0] = vc[0] - vt[0] + vf[0];
	tt[1] = vc[1] - vt[1] + vf[1];
	tt[2] = vc[2] - vt[2] + vf[2];
	gl41line_arrow(win, 0xffffff, tc, tt, vt);

	gl41opaque_circle(win, 0x80ffffff, vc, vr, vt);
	portal = act->buf0;
	if(0 == portal)return;
	gl41line_arrow(win, 0xff0000, portal->vc, portal->vq, vt);
}
static void portal_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct portalbuf* portal;
	struct glsrc* src;
	float (*vbuf)[6];

	vec3 tc,tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;

	gl41line_rect(win, 0xffffff, vc, vr, vt);
	tc[0] = vc[0] - vt[0];
	tc[1] = vc[1] - vt[1];
	tc[2] = vc[2] - vt[2];
	gl41solid_rect(win, 0x00003f, tc, vr, vf);
	tc[0] = vc[0] - vt[0] - vf[0];
	tc[1] = vc[1] - vt[1] - vf[1];
	tc[2] = vc[2] - vt[2] - vf[2];
	tt[0] = vc[0] - vt[0] + vf[0];
	tt[1] = vc[1] - vt[1] + vf[1];
	tt[2] = vc[2] - vt[2] + vf[2];
	gl41line_arrow(win, 0xffffff, tc, tt, vt);

	portal = act->buf0;
	if(0 == portal)return;
	src = (void*)(portal->data);
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
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




void portal_frustum(struct fstyle* frus, struct fstyle* por, struct fstyle* tar, vec3 cam)
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
	struct entity* act, struct fstyle* part,
	struct entity* wrd, struct fstyle* geom,
	struct entity* ctx, struct fstyle* frus,
	struct supply* fbo, struct fstyle* area,
	struct entity* dat, struct fstyle* camf)
{/*
	struct halfrel* self[1];
	struct halfrel* peer[1];
	struct fstyle* obba;
	struct fstyle* obbb;

	struct portalbuf* portal;
	struct glsrc* own;
	struct glsrc* src;

	//
	portal = act->buf0;
	if(0 == portal)return;
	own = (void*)(portal->data);
	if(0 == own)return;

	own->tex[0].glfd = fbo->tex0;
	own->tex[0].name = "tex0";
	own->tex[0].fmt = '!';
	own->tex[0].enq += 1;


	//
	src = fbo->gl_camera;
	if(0 == src)return;

	self[0] = peer[0] = 0;
	portal_search(act, 'a', self, peer);
	if(0 == peer[0])return;
	obba = peer[0]->pfoot;

	self[0] = peer[0] = 0;
	portal_search(act, 'b', self, peer);
	if(0 == peer[0])return;
	obbb = peer[0]->pfoot;

	vec3 cam = {-1000.0, -2500.0, 1000.0};
	portal_frustum(frus, obba, obbb, cam);
	portal->vc[0] = frus->vc[0];
	portal->vc[1] = frus->vc[1];
	portal->vc[2] = frus->vc[2];
	portal->vq[0] = frus->vc[0] + frus->vn[0]*frus->vn[3];
	portal->vq[1] = frus->vc[1] + frus->vn[1]*frus->vn[3];
	portal->vq[2] = frus->vc[2] + frus->vn[2]*frus->vn[3];

	matproj_transpose(portal->mvp, frus);

	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = portal->mvp;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = obba->vc;*/
}




static void portal_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void portal_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void portal_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void portal_linkup(struct halfrel* self, struct halfrel* peer)
{
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
	p->onread  = (void*)portal_read;
	p->onwrite = (void*)portal_write;
}
