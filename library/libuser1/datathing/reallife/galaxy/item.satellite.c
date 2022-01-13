#include "libuser.h"
#define longitude whdf.fx0
#define latitude whdf.fy0
#define altitude whdf.fz0
#define CTXBUF listptr.buf0
void carveplanet(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




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




void satellite_ctxforwnd(struct gl41data* data, char* str)
{
	//shader
	data->src.vs = satellite_glsl_v;
	data->src.fs = satellite_glsl_f;
	data->src.shader_enq = 42;

	//texture
	data->dst.texname[0] = "tex0";
	data->src.tex[0].fmt = hex32('r','g','b','a');
	data->src.tex[0].data = memorycreate(2048*2048*4, 0);
	loadtexfromfile(&data->src.tex[0], str);
	data->src.tex_enq[0] = 42;
	//say("w=%d,h=%d\n",data->src.tex[0].w, data->src.tex[0].h);

#define accx 64
#define accy 63
	struct vertex* vtx = data->src.vtx;
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

	data->src.vbuf_enq = 0;
	data->src.ibuf_enq = 0;
}
static void satellite_draw_gl41(
	_obj* act, struct style* part,		//self
	_obj* win, struct style* geom,		//world,mygeom
	_obj* ctx, struct style* none)		//gldata
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


	struct mysrc* src = act->CTXBUF;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	carveplanet(vbuf, ibuf, vc, vr, vf, vt);

	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static void satellite_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void satellite_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("satellite(%x,%x,%x)\n",win,act,sty);
}
static void satellite_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
}




static void satellite_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	satellite_draw_gl41(ent,slot, wor,geom, wnd,area);
}
static void satellite_wnd(_obj* ent,void* foot, _syn* stack,int sp)
{
}




static void satellite_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		satellite_wnd(ent,foot, stack,sp);
		break;
	default:
		satellite_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void satellite_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	float* f = buf;
	say("@satellite_write: %f,%f,%f,%f\n", f[0],f[1],f[2],f[3]);

	ent->longitude= f[0];
	ent->latitude = f[1];
	ent->altitude = f[2];
}
static void satellite_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void satellite_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@satellite_attach\n");
}




static void satellite_search(_obj* act)
{
}
static void satellite_modify(_obj* act)
{
}
static void satellite_delete(_obj* act)
{
	if(0 == act)return;
}
static void satellite_create(_obj* act, void* str)
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




void satellite_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('s','a','t','e','l','l','i', 0);

	p->oncreate = (void*)satellite_create;
	p->ondelete = (void*)satellite_delete;
	p->onsearch = (void*)satellite_search;
	p->onmodify = (void*)satellite_modify;

	p->onattach = (void*)satellite_attach;
	p->ondetach = (void*)satellite_detach;
	p->ontaking = (void*)satellite_taking;
	p->ongiving = (void*)satellite_giving;
}
