#include "libuser.h"
void fixmatrix(void* m, struct fstyle* sty);
void loadtexfromfile(struct glsrc* src, int idx, char* name);
struct portalbuf{
	mat4 mvp;
	u8 data[0];
};




static void portal_search(struct actor* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct actor* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_world3d_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void portal_modify(struct actor* act)
{
}
static void portal_delete(struct actor* act)
{
}
static void portal_create(struct actor* act, void* str)
{
	struct portalbuf* portal;
	struct glsrc* src;
	if(0 == act)return;

	portal = act->buf0 = memorycreate(0x1000, 0);
	if(0 == portal)return;


	//
	src = (void*)(portal->data);
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




static void portal_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
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
static void portal_draw_vbo_b(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	vec3 tc;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;
	carveline_rect(win, 0x3f0000, vc, vr, vt);
	carveopaque_circle(win, 0x80ffffff, vc, vr, vt);

	tc[0] = vc[0] - vt[0];
	tc[1] = vc[1] - vt[1];
	tc[2] = vc[2] - vt[2];
	carvesolid_rect(win, 0x3f0000, tc, vr, vf);
}
static void portal_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	struct portalbuf* portal;
	struct glsrc* src;
	float (*vbuf)[6];

	vec3 tc;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vt = sty->f.vt;

	carveline_rect(win, 0x00003f, vc, vr, vt);
	tc[0] = vc[0] - vt[0];
	tc[1] = vc[1] - vt[1];
	tc[2] = vc[2] - vt[2];
	carvesolid_rect(win, 0x00003f, tc, vr, vf);

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
}
static void portal_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void portal_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)portal_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)portal_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)portal_draw_html(act, pin, win, sty);
	else if(fmt == _json_)portal_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)portal_draw_vbo(act, pin, win, sty);
	else portal_draw_pixel(act, pin, win, sty);
}




void portal_frustum(struct fstyle* frus, struct fstyle* obb, vec3 cam)
{
	float x,y,z,t;
	frus->vc[0] = 1000.0;
	frus->vc[1] = 0.0;
	frus->vc[2] = 1000.0;

	frus->vl[0] = -1.0;
	frus->vl[1] = 0.0;
	frus->vl[2] = 0.0;
	frus->vl[3] = -1.0;

	frus->vr[0] = 1.0;
	frus->vr[1] = 0.0;
	frus->vr[2] = 0.0;
	frus->vr[3] = 1.0;

	frus->vb[0] = 0.0;
	frus->vb[1] = 0.0;
	frus->vb[2] = -1.0;
	frus->vb[3] = -1.0;

	frus->vt[0] = 0.0;
	frus->vt[1] = 0.0;
	frus->vt[2] = 1.0;
	frus->vt[3] = 1.0;

	frus->vn[0] = 0.0;
	frus->vn[1] = 1.0;
	frus->vn[2] = 0.0;
	frus->vn[3] = 1.0;

	frus->vf[0] = 0.0;
	frus->vf[1] = 1.0;
	frus->vf[2] = 0.0;
	frus->vf[3] = 100000000.0;
}
static void portal_matrix(
	struct actor* act, struct fstyle* frus,
	struct actor* fbo, struct fstyle* area)
{
	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;

	struct portalbuf* portal;
	struct glsrc* own;
	struct glsrc* src;

	//
	portal = act->buf0;
	if(0 == portal)return;
	own = (void*)(portal->data);
	if(0 == own)return;

	own->tex_data[0] = fbo->tex0;
	own->tex_name[0] = "tex0";
	own->tex_fmt[0] = '!';
	own->tex_enq[0] += 1;


	//
	src = fbo->gl_camera;
	if(0 == src)return;

	portal_search(act, 0, &self, &peer);
	obb = peer->pfoot;

	vec3 cam = {2000.0, -2000.0, 2000.0};
	portal_frustum(frus, obb, cam);
	fixmatrix(portal->mvp, frus);
	mat4_transpose(portal->mvp);

	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = portal->mvp;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = obb->vc;
}




static void portal_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	if(ctx){
		if(_gl41data_ == ctx->type){
			if('a' == self->flag)portal_draw_vbo(act,pin,ctx,sty);
			if('b' == self->flag)portal_draw_vbo_b(act,pin,ctx,sty);
		}
	}
	else{
		switch(win->type){
			case _gl41view_:
			case _gl41fbod_:
			case _gl41fboc_:
			case _gl41fbog_:
			case _gl41wnd0_:portal_matrix(act, &pin->fs, win, &sty->fs);
		}
	}
}
static void portal_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void portal_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void portal_start(struct halfrel* self, struct halfrel* peer)
{
	struct actor* act;
	struct style* pin;
	struct portalbuf* portal;

	act = (void*)(self->chip);
	if(0 == act)return;
	pin = (void*)(self->foot);
	if(0 == pin)return;

	if(hex32('m','v','p',0) == self->flag){
		say("portal_start: mvp\n");
		return;
	}
	if('a' == self->flag){
		portal = act->buf0;
		pin->data[0] = (u64)(portal->data);
	}
}




void portal_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('p', 'o', 'r', 't', 'a', 'l', 0, 0);

	p->oncreate = (void*)portal_create;
	p->ondelete = (void*)portal_delete;
	p->onsearch = (void*)portal_search;
	p->onmodify = (void*)portal_modify;

	p->onstart = (void*)portal_start;
	p->onstop  = (void*)portal_stop;
	p->onread  = (void*)portal_read;
	p->onwrite = (void*)portal_write;
}
