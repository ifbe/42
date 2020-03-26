#include "libuser.h"
#define _fbo_ hex32('f','b','o',0)
void matorth_transpose(mat4 m, struct fstyle* s);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);


#define FBOBUF buf0
#define LITBUF buf1
#define CTXBUF buf2
#define OWNBUF buf3
struct sunbuf{
	mat4 mvp;
	vec4 rgb;
	u32 u_rgb;
	u32 glfd;
};




char* dirlit_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* dirlit_glsl_f =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"uniform sampler2D suntex;\n"
"in mediump vec2 uvw;\n"
"layout(location = 0)out mediump vec4 FragColor;\n"
"void main(){\n"
	"mediump float d = 100.0 * texture(suntex, uvw).r;"
	"FragColor = vec4(d, d, d, 1.0);\n"
"}\n";




static void dirlight_forfbo_update(
	struct entity* act, struct style* slot,
	struct entity* wrd, struct style* geom,
	struct supply* fbo, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	struct glsrc* src = act->FBOBUF;
	if(0 == src)return;

	src->arg[0].fmt = 'm';
	src->arg[0].name = "cammvp";
	src->arg[0].data = sun->mvp;
	src->arg[1].fmt = 'v';
	src->arg[1].name = "camxyz";
	src->arg[1].data = &geom->frus.vc;
	fbo->gl_camera[0] = act->FBOBUF;
}
static void dirlight_forfbo_prepare(struct glsrc* src)
{
}




static void dirlight_frustum(struct fstyle* d, struct fstyle* s)
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
	d->vf[3] = n;

	d->vn[0] = d->vf[0];
	d->vn[1] = d->vf[1];
	d->vn[2] = d->vf[2];
	d->vn[3] = 1.0;
}
static void dirlight_forwnd_light_update(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* wnd, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	dirlight_frustum(&geom->frus, &geom->fs);
	matorth_transpose(sun->mvp, &geom->frus);

	struct glsrc* src = act->LITBUF;
	if(0 == src)return;

	src->arg[0].fmt = 'm';
	src->arg[0].name = "sunmvp";
	src->arg[0].data = sun->mvp;

	src->arg[1].fmt = 'v';
	src->arg[1].name = "sunrgb";
	src->arg[1].data = sun->rgb;

	src->arg[2].fmt = 'v';
	src->arg[2].name = "sundir";
	src->arg[2].data = &geom->frus.vf;

	src->tex[0].glfd = sun->glfd;
	src->tex[0].name = "suntex";
	src->tex[0].fmt = '!';
	src->tex[0].enq += 1;

	wnd->gl_light[0] = act->LITBUF;
}
static void dirlight_forwnd_light_prepare(struct glsrc* src)
{
	src->routine_name = "passtype";
	src->routine_detail = "dirlight";
}




static void dirlight_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	struct sunbuf* sun;
	struct rgbbuf* tmp;
	struct glsrc* src;
	float (*vbuf)[6];

	int x,y,j;
	vec3 ta, tb;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vt);

	//light ray (for debug)
	sun = act->OWNBUF;
	for(y=-1;y<2;y+=2){
	for(x=-10;x<11;x++){
		for(j=0;j<3;j++){
			ta[j] = vc[j] + x*vr[j]/10.0 + y*vt[j];
			tb[j] = ta[j] + vf[j];
		}
		gl41line(ctx, sun->u_rgb, ta, tb);
	}
	}
	for(x=-1;x<2;x+=2){
	for(y=-10;y<11;y++){
		for(j=0;j<3;j++){
			ta[j] = vc[j] + x*vr[j] + y*vt[j]/10.0;
			tb[j] = ta[j] + vf[j];
		}
		gl41line(ctx, sun->u_rgb, ta, tb);
	}
	}

	//depth fbo (for debug)
	src = act->CTXBUF;
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
	gl41data_insert(ctx, 's', act->CTXBUF, 1);
}
static void dirlight_forwnd_vertex_update(struct entity* act, struct style* slot, struct supply* fbo, struct style* area)
{
	struct sunbuf* sun = act->OWNBUF;
	if(0 == sun)return;

	sun->glfd = fbo->tex0;
}
static void dirlight_forwnd_vertex_prepare(struct glsrc* src)
{
	src->geometry = 3;
	src->method = 'v';

	//
	src->vs = dirlit_glsl_v;
	src->fs = dirlit_glsl_f;
	src->shader_enq = 42;

	//vertex
	src->vbuf_fmt = vbuffmt_33;
	src->vbuf_w = 6*4;
	src->vbuf_h = 6;
	src->vbuf_len = (src->vbuf_w) * (src->vbuf_h);
	src->vbuf = memorycreate(src->vbuf_len, 0);
}




static void dirlight_read_bycam(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;
//world -> dirlight
	struct entity* win;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		win = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len){
			dirlight_forwnd_light_update(act,slot, win,geom, wnd,area);
			dirlight_draw_gl41(act,slot, win,geom, wnd,area);
		}
		if('?' == len){
			int ret;
			struct halfrel* rel[2];
			ret = relationsearch(act, _fbo_, &rel[0], &rel[1]);
			if(ret <= 0)return;

			struct supply* fbo = rel[1]->pchip;
			struct style* rect = rel[1]->pfoot;
			dirlight_forfbo_update(act,slot, win,geom, fbo,rect);
			relationwrite(act,_fbo_, stack,rsp, buf,len);

			dirlight_forwnd_vertex_update(act,slot, fbo,rect);
		}
	}
}
static void dirlight_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dirlight_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dirlight_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dirlight_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dirlight_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void dirlight_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	dirlight_read_bycam(self,peer, stack,rsp, buf,len);
}
static void dirlight_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void dirlight_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void dirlight_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void dirlight_search(struct entity* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	struct entity* world;
	struct fstyle* obb = 0;
	//say("freecam@%llx,%llx,%llx,%d\n",act,pin,buf,len);

	rel = act->irel0;
	while(1){
		if(0 == rel)return;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return;
		}
		rel = samedstnextsrc(rel);
	}
}
static void dirlight_modify(struct entity* act)
{
}
static void dirlight_delete(struct entity* act)
{
}
static void dirlight_create(struct entity* act, void* str)
{
	struct sunbuf* sun;
	if(0 == act)return;

	sun = act->OWNBUF = memorycreate(0x400, 0);
	sun->u_rgb = 0xffff00;
	sun->rgb[0] = ((sun->u_rgb >>16) & 0xff) / 255.0;
	sun->rgb[1] = ((sun->u_rgb >> 8) & 0xff) / 255.0;
	sun->rgb[2] = ((sun->u_rgb >> 0) & 0xff) / 255.0;

	act->FBOBUF = memorycreate(0x400, 0);
	dirlight_forfbo_prepare(act->FBOBUF);

	act->LITBUF = memorycreate(0x400, 0);
	dirlight_forwnd_light_prepare(act->LITBUF);

	act->CTXBUF = memorycreate(0x400, 0);
	dirlight_forwnd_vertex_prepare(act->CTXBUF);
}




void dirlight_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('d','i','r','l', 'i', 't', 0, 0);

	p->oncreate = (void*)dirlight_create;
	p->ondelete = (void*)dirlight_delete;
	p->onsearch = (void*)dirlight_search;
	p->onmodify = (void*)dirlight_modify;

	p->onlinkup = (void*)dirlight_linkup;
	p->ondiscon = (void*)dirlight_discon;
	p->onread  = (void*)dirlight_read;
	p->onwrite = (void*)dirlight_write;
}
