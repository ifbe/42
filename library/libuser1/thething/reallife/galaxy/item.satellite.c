#include "libuser.h"
#define longitude fx0
#define latitude fy0
#define altitude fz0
#define CTXBUF buf0
void carveplanet(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(struct entity* ctx, int type, struct glsrc* src, int cnt);




char* satellite_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* satellite_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	//"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";




void satellite_ctxforwnd(struct glsrc* src, char* str)
{
	//shader
	src->vs = satellite_glsl_v;
	src->fs = satellite_glsl_f;
	src->shader_enq = 42;

	//texture
	src->tex[0].fmt = hex32('r','g','b','a');
	src->tex[0].name = "tex0";
	src->tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&src->tex[0], str);
	src->tex_enq[0] = 42;
	//say("w=%d,h=%d\n",src->tex[0].w, src->tex[0].h);

#define accx 64
#define accy 63
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*6;
	vtx->vbuf_h = accx*accy+(accx-1)*2;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accy*(accx-1)*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memorycreate(vtx->ibuf_len, 0);

	src->vbuf_enq = 0;
	src->ibuf_enq = 0;
}
static void satellite_draw_gl41(
	struct entity* act, struct style* part,		//self
	struct entity* win, struct style* geom,		//world,mygeom
	struct entity* ctx, struct style* none)		//gldata
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;

	vec3 tc,tt,tr,tf;
	int j;
	float a0,c0,s0;
	float az,cz,sz;
	a0 = (act->longitude+180) * PI / 180;
	c0 = getcos(a0);
	s0 = getsin(a0);
	az = act->latitude * PI / 180;
	cz = getcos(az);
	sz = getsin(az);
	for(j=0;j<3;j++){
		tt[j] = (vr[j]*c0 + vf[j]*s0)*cz*2 + vt[j]*sz*2;
		tc[j] = vc[j] + tt[j];
		tr[j] = -vr[j]*s0 + vf[j]*c0;
	}
	vec3_cross(tf, tt, tr);
	vec3_setlen(tr, 32);
	vec3_setlen(tf, 32);
	gl41line(ctx, 0xff00ff, vc, tc);
	gl41solid_rect(ctx, 0xffff00, tc, tr, tf);


	struct glsrc* src = act->CTXBUF;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	carveplanet(vbuf, ibuf, vc, vr, vf, vt);

	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static void satellite_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void satellite_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void satellite_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void satellite_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void satellite_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("satellite(%x,%x,%x)\n",win,act,sty);
}
static void satellite_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
}




static void satellite_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack&&('v' == key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		satellite_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void satellite_read_bywnd(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}




static void satellite_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(sp < 2)return;
	struct supply* sup = stack[sp-2].pchip;

	switch(sup->fmt){
	case _gl41fbog_:
	case _gl41wnd0_:
	case _full_:
	case _wnd_:{
		if('v' != key)break;
		satellite_read_bywnd(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	default:{
		satellite_read_bycam(ent,foot, stack,sp, arg,key, buf,len);break;
	}
	}
}
static void satellite_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	float* f = buf;
	say("@satellite_write: %f,%f,%f,%f\n", f[0],f[1],f[2],f[3]);

	ent->longitude= f[0];
	ent->latitude = f[1];
	ent->altitude = f[2];
}
static void satellite_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void satellite_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@satellite_linkup\n");
}




static void satellite_search(struct entity* act)
{
}
static void satellite_modify(struct entity* act)
{
}
static void satellite_delete(struct entity* act)
{
	if(0 == act)return;
}
static void satellite_create(struct entity* act, void* str)
{
	if(0 == act)return;

	act->longitude = 0.0;
	act->latitude = 0.0;
	act->altitude = 0.0;

	void* ctx = act->CTXBUF = memorycreate(0x1000, 0);
	if(0 == ctx)return;

	if(0 == str)str = "datafile/jpg/earth.jpg";
	satellite_ctxforwnd(ctx, str);
}




void satellite_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s','a','t','e','l','l','i', 0);

	p->oncreate = (void*)satellite_create;
	p->ondelete = (void*)satellite_delete;
	p->onsearch = (void*)satellite_search;
	p->onmodify = (void*)satellite_modify;

	p->onlinkup = (void*)satellite_linkup;
	p->ondiscon = (void*)satellite_discon;
	p->ontaking = (void*)satellite_taking;
	p->ongiving = (void*)satellite_giving;
}
