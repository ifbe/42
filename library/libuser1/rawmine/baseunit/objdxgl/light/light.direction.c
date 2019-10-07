#include "libuser.h"
void ortho_mvp(mat4 m, struct fstyle* s);
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
};




static void dirlight_search(struct actor* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
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
static void dirlight_modify(struct actor* act)
{
}
static void dirlight_delete(struct actor* act)
{
}
static void dirlight_create(struct actor* act, void* str)
{
	act->data0 = 0xffffff;
	act->buf = memorycreate(0x1000, 0);
}




static void dirlight_draw_pixel(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dirlight_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	float x,y;
	vec3 ta, tb;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	for(y=-1.0;y<1.01;y+=0.1)
	{
		for(x=-1.0;x<1.01;x+=0.1)
		{
			ta[0] = vc[0] + x*vr[0] + y*vu[0];
			ta[1] = vc[1] + x*vr[1] + y*vu[1];
			ta[2] = vc[2] + x*vr[2] + y*vu[2];
			tb[0] = ta[0] - vf[0];
			tb[1] = ta[1] - vf[1];
			tb[2] = ta[2] - vf[2];
			carveline(win, 0xffff00, ta, tb);
		}
	}
}
static void dirlight_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dirlight_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dirlight_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dirlight_draw_cli(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void dirlight_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;
	if(fmt == _cli_)dirlight_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)dirlight_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)dirlight_draw_html(act, pin, win, sty);
	else if(fmt == _json_)dirlight_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)dirlight_draw_vbo(act, pin, win, sty);
	else dirlight_draw_pixel(act, pin, win, sty);
}




void dirlight_sty2cam(struct fstyle* d, struct fstyle* s)
{
	float a,b,c;
	float x,y,z,n;
	d->vc[0] = s->vc[0];
	d->vc[1] = s->vc[1];
	d->vc[2] = s->vc[2];


	//left, right
	x = s->vr[0];
	y = s->vr[1];
	z = s->vr[2];
	n = squareroot(x*x + y*y + z*z);
	d->vr[0] = x / n;
	d->vr[1] = y / n;
	d->vr[2] = z / n;
	d->vr[3] = n;
	d->vl[0] = - d->vr[0];
	d->vl[1] = - d->vr[1];
	d->vl[2] = - d->vr[2];
	d->vl[3] = -n;


	//bottom, top
	x = s->vt[0];
	y = s->vt[1];
	z = s->vt[2];
	n = squareroot(x*x + y*y + z*z);
	d->vt[0] = x / n;
	d->vt[1] = y / n;
	d->vt[2] = z / n;
	d->vt[3] = n;
	d->vb[0] = - d->vt[0];
	d->vb[1] = - d->vt[1];
	d->vb[2] = - d->vt[2];
	d->vb[3] = -n;


	//near, front
	a = s->vf[0] - s->vn[0];
	b = s->vf[1] - s->vn[1];
	c = s->vf[2] - s->vn[2];

	x = s->vn[0];
	y = s->vn[1];
	z = s->vn[2];
	n = squareroot(x*x + y*y + z*z);
	d->vn[0] = x / n;
	d->vn[1] = y / n;
	d->vn[2] = z / n;
	if(a*x + b*y + c*z < 0)n = -n;
	d->vn[3] = n;

	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	if(a*x + b*y + c*z < 0)n = -n;
	d->vf[3] = n;

	//printstyle(&act->camera);
}
static void dirlight_matrix(
	struct actor* act, struct style* pin,
	struct actor* ctx, struct style* sty)
{
	struct fstyle* obb;
	struct halfrel* self;
	struct halfrel* peer;
	//say("@orthcam_matrix:%llx,%llx,%llx,%llx\n", ctx->gl_camera, ctx->gl_light, ctx->gl_solid, ctx->gl_opaque);

	struct sunbuf* sun = act->buf;
	struct glsrc* src = ctx->gl_camera;

	dirlight_search(act, 0, &self, &peer);
	obb = peer->pfoot;

	dirlight_sty2cam(&pin->f, obb);
	ortho_mvp(sun->mvp, &pin->f);
	mat4_transpose(sun->mvp);

	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "cammvp";
	src->arg_data[0] = sun->mvp;

	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "camxyz";
	src->arg_data[1] = obb->vc;
}




void dirlight_light(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u32 rgb = act->data0;
	struct glsrc* src = win->gl_light;
	struct sunbuf* sun = act->buf;


	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "sunmvp";
	src->arg_data[0] = sun->mvp;


	sun->rgb[2] = (rgb&0xff) / 255.0;
	sun->rgb[1] = ((rgb >>  8)&0xff) / 255.0;
	sun->rgb[0] = ((rgb >> 16)&0xff) / 255.0;
	src->arg_fmt[1] = 'v';
	src->arg_name[1] = "sunrgb";
	src->arg_data[1] = sun->rgb;
}




static void dirlight_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;

	if(ctx){
		switch(ctx->type){
			case _gl41data_:{
				dirlight_light(act,pin,ctx,sty);
				dirlight_draw_vbo(act,pin,ctx,sty);
			}
		}
	}
	else{
		switch(win->type){
			case _gl41view_:
			case _gl41wnd0_:dirlight_matrix(act, pin, win, sty);
		}
	}
}
static void dirlight_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void dirlight_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void dirlight_start(struct halfrel* self, struct halfrel* peer)
{
}




void dirlight_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('d','i','r','l', 'i', 't', 0, 0);

	p->oncreate = (void*)dirlight_create;
	p->ondelete = (void*)dirlight_delete;
	p->onsearch = (void*)dirlight_search;
	p->onmodify = (void*)dirlight_modify;

	p->onstart = (void*)dirlight_start;
	p->onstop  = (void*)dirlight_stop;
	p->onread  = (void*)dirlight_read;
	p->onwrite = (void*)dirlight_write;
}
