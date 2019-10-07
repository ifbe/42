#include "libuser.h"
void ortho_mvp(mat4 m, struct fstyle* s);
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
};




char* dirlit_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main()\n"
"{\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* dirlit_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uvw;\n"
"layout(location = 0)out mediump vec4 FragColor;\n"
"void main()\n"
"{\n"
	//"FragColor = vec4(uvw, 1.0, 1.0);\n"
	"FragColor = vec4(texture(tex0, uvw).rgb, 1.0);\n"

	//"mediump float n = 1.0;"
	//"mediump float f = 1000.0;"
	//"mediump float d = texture(tex0, uvw).r;"
	//"mediump float c = (2.0 * n) / (f + n - d * (f - n));"
	//"FragColor = vec4(c, c, c, 1.0);\n"
"}\n";




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
	int j;
	struct sunbuf* sun;
	struct glsrc* src;
	if(0 == act)return;

	//buf0
	sun = act->buf0 = memorycreate(0x1000, 0);
	sun->rgb[0] = 1.0;
	sun->rgb[1] = 1.0;
	sun->rgb[2] = 1.0;


	//buf1
	src = act->buf1 = memorycreate(0x200, 0);
	if(0 == src)return;

	//
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = dirlit_glsl_v;
	src->fs = dirlit_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex_name[0] = "tex0";

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
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
	float* vt = sty->f.vt;
	struct datapair* dst;
	struct glsrc* src;
	float (*vbuf)[6];

	dst = (void*)(sty->data[0]);
	if(0 == dst)return;
	src = (void*)(pin->data[0]);
	if(0 == src)return;
	vbuf = (void*)(src->vbuf);
	if(0 == vbuf)return;
	//printmemory(&dst->src, sizeof(struct glsrc));
	//say("haha\n");
	//printmemory(&dst->dst, sizeof(struct gldst));
	//say("shader=%d\n", dst->dst.shader);

	//light ray (for debug)
	carveline_rect(win, 0xffffff, vc, vr, vt);
	for(y=-1.0;y<1.01;y+=0.2)
	{
		for(x=-1.0;x<1.01;x+=0.2)
		{
			ta[0] = vc[0] + x*vr[0] + y*vt[0];
			ta[1] = vc[1] + x*vr[1] + y*vt[1];
			ta[2] = vc[2] + x*vr[2] + y*vt[2];
			tb[0] = ta[0] - vf[0];
			tb[1] = ta[1] - vf[1];
			tb[2] = ta[2] - vf[2];
			carveline(win, 0xffff00, ta, tb);
		}
	}

	//depth fbo (for debug)
	vbuf[0][0] = vc[0] - vr[0] - vt[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vt[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vt[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 0.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vt[0] - vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vt[1] - vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vt[2] - vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 1.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vt[0] - vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vt[1] - vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vt[2] - vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 1.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vt[0] - vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vt[1] - vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vt[2] - vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 1.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vt[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vt[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vt[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 0.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vt[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vt[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vt[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 0.0;
	vbuf[5][5] = 0.0;

	src->vbuf_enq += 1;
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
	x = s->vf[0];
	y = s->vf[1];
	z = s->vf[2];
	n = squareroot(x*x + y*y + z*z);
	d->vf[0] = x / n;
	d->vf[1] = y / n;
	d->vf[2] = z / n;
	//d->vf[3] = 2000.0;

	d->vn[0] = d->vf[0];
	d->vn[1] = d->vf[1];
	d->vn[2] = d->vf[2];
	//d->vn[3] = 1.0;
}
static void dirlight_matrix(
	struct actor* act, struct style* pin,
	struct actor* fbo, struct style* sty)
{
	struct halfrel* self;
	struct halfrel* peer;
	struct fstyle* obb;

	struct sunbuf* sun;
	struct glsrc* src;

if(1){
	src = act->buf1;
	say("fbo=%x, tex=%x\n", fbo->fbo, fbo->tex0);
	src->tex_data[0] = fbo->tex0;
	src->tex_fmt[0] = '!';
	src->tex_enq[0] += 1;
}
	//locate world
	dirlight_search(act, 0, &self, &peer);
	obb = peer->pfoot;

	//write context
	sun = act->buf0;
	dirlight_sty2cam(&pin->f, obb);
	ortho_mvp(sun->mvp, &pin->f);
	mat4_transpose(sun->mvp);

	src = fbo->gl_camera;
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
	struct glsrc* src = win->gl_light;
	struct sunbuf* sun = act->buf0;

	src->arg_fmt[0] = 'm';
	src->arg_name[0] = "sunmvp";
	src->arg_data[0] = sun->mvp;

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
			case _gl41fbod_:
			case _gl41fboc_:
			case _gl41fbog_:
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
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	if(0 == act)return;
	if(0 == pin)return;

	pin->data[0] = (u64)(act->buf1);
	say("@dirlit_start:%llx, %llx\n", pin->data[0], pin->data[1]);
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
