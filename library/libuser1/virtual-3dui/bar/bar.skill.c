#include "libuser.h"
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




char* skillbar_glsl_v =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"out mediump vec2 uvw;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";

char* skillbar_glsl_f =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"mediump vec3 bgr = texture(tex0, uvw).bgr;\n"
	"FragColor = vec4(bgr, 1.0);\n"
"}\n";

static void vertex_gen(float (*vbuf)[6], float x, float y, vec3 vc, vec3 vr, vec3 vf)
{
	int t = 0;
	vbuf[t+0][0] = vc[0] - vr[0] - vf[0];
	vbuf[t+0][1] = vc[1] - vr[1] - vf[1];
	vbuf[t+0][2] = vc[2] - vr[2] - vf[2];
	vbuf[t+0][3] = (x+0)/16.0;
	vbuf[t+0][4] = (y+1)/16.0;
	vbuf[t+0][5] = 0.0;

	vbuf[t+1][0] = vc[0] + vr[0] + vf[0];
	vbuf[t+1][1] = vc[1] + vr[1] + vf[1];
	vbuf[t+1][2] = vc[2] + vr[2] + vf[2];
	vbuf[t+1][3] = (x+1)/16.0;
	vbuf[t+1][4] = (y+0)/16.0;
	vbuf[t+1][5] = 0.0;

	vbuf[t+2][0] = vc[0] - vr[0] + vf[0];
	vbuf[t+2][1] = vc[1] - vr[1] + vf[1];
	vbuf[t+2][2] = vc[2] - vr[2] + vf[2];
	vbuf[t+2][3] = (x+0)/16.0;
	vbuf[t+2][4] = (y+0)/16.0;
	vbuf[t+2][5] = 0.0;

	vbuf[t+3][0] = vc[0] + vr[0] + vf[0];
	vbuf[t+3][1] = vc[1] + vr[1] + vf[1];
	vbuf[t+3][2] = vc[2] + vr[2] + vf[2];
	vbuf[t+3][3] = (x+1)/16.0;
	vbuf[t+3][4] = (y+0)/16.0;
	vbuf[t+3][5] = 0.0;

	vbuf[t+4][0] = vc[0] - vr[0] - vf[0];
	vbuf[t+4][1] = vc[1] - vr[1] - vf[1];
	vbuf[t+4][2] = vc[2] - vr[2] - vf[2];
	vbuf[t+4][3] = (x+0)/16.0;
	vbuf[t+4][4] = (y+1)/16.0;
	vbuf[t+4][5] = 0.0;

	vbuf[t+5][0] = vc[0] + vr[0] - vf[0];
	vbuf[t+5][1] = vc[1] + vr[1] - vf[1];
	vbuf[t+5][2] = vc[2] + vr[2] - vf[2];
	vbuf[t+5][3] = (x+1)/16.0;
	vbuf[t+5][4] = (y+1)/16.0;
	vbuf[t+5][5] = 0.0;
}
void skillbar_ctxforwnd(struct gl41data* data, char* str)
{
	//shader
	data->src.vs = skillbar_glsl_v;
	data->src.fs = skillbar_glsl_f;
	data->src.shader_enq = 42;

	//texture
	data->dst.texname[0] = "tex0";
	data->src.tex[0].fmt = hex32('r','g','b','a');
	data->src.tex[0].data = memoryalloc(2048*2048*4, 0);
	loadtexfromfile(&data->src.tex[0], str);
	data->src.tex_enq[0] = 42;

	//vertex
	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 0;
	vtx->vbuf_len = (vtx->vbuf_w) * 6*16*16;
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
	data->src.vbuf_enq = 42;
}




static void skillbar_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void skillbar_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y,k,t;
	vec3 tc, tr, tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(wnd, 0xff00ff, vc, vr, vf);

	struct mysrc* src = act->listptr.buf0;
	if(0 == src)return;
	float (*vbuf)[6] = src->vtx[0].vbuf;
	if(0 == vbuf)return;
	tr[0] = vr[0]/30;
	tr[1] = vr[1]/30;
	tr[2] = vr[2]/30;
	tf[0] = vf[0]/30;
	tf[1] = vf[1]/30;
	tf[2] = vf[2]/30;
	k = t = 0;
	for(y=0;y<2;y++){
		for(x=-12;x<12;x++){
			if(0 == x)continue;

			tc[0] = vc[0] + vr[0]*(2*x+1)/28.0 + vf[0]*(2*y-23)/24.0;
			tc[1] = vc[1] + vr[1]*(2*x+1)/28.0 + vf[1]*(2*y-23)/24.0;
			tc[2] = vc[2] + vr[2]*(2*x+1)/28.0 + vf[2]*(2*y-23)/24.0;
			gl41line_rectselect(wnd, 0xff00ff, tc, tr, tf);

			vertex_gen(&vbuf[t+0], k%16, k/16, tc, tr, tf);
			k ++;
			t += 6;
		}
	}
	for(x=0;x<2;x++){
	for(y=0;y<12;y++){
			tc[0] = vc[0] + vr[0]*(2*x+21)/24.0 + vf[0]*(2*y-11)/24.0;
			tc[1] = vc[1] + vr[1]*(2*x+21)/24.0 + vf[1]*(2*y-11)/24.0;
			tc[2] = vc[2] + vr[2]*(2*x+21)/24.0 + vf[2]*(2*y-11)/24.0;
			gl41line_rectselect(wnd, 0xff00ff, tc, tr, tf);

			vertex_gen(&vbuf[t+0], k%16, k/16, tc, tr, tf);
			k ++;
			t += 6;
		}
	}

	src->vtx[0].vbuf_h = 6*12*6;
	src->vbuf_enq += 1;
	gl41data_insert(wnd, 's', src, 1);
}
static void skillbar_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void skillbar_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void skillbar_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void skillbar_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("skillbar(%x,%x,%x)\n",win,act,sty);
}




static void skillbar_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	skillbar_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void skillbar_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;

	switch(wnd->type){
	case _wnd_:
		break;
	default:
		skillbar_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void skillbar_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void skillbar_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void skillbar_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void skillbar_search(_obj* act)
{
}
static void skillbar_modify(_obj* act)
{
}
static void skillbar_delete(_obj* act)
{
	if(0 == act)return;
	memoryfree(act->listptr.buf0);
	act->listptr.buf0 = 0;
}
static void skillbar_create(_obj* act, void* str)
{
	if(0 == act)return;

	struct gl41data* data = act->listptr.buf0 = memoryalloc(0x1000, 0);
	if(0 == str)str = "datafile/jpg/cartoon.jpg";
	skillbar_ctxforwnd(data, str);
}




void skillbar_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('s', 'k', 'i', 'l', 'l', 0, 0, 0);

	p->oncreate = (void*)skillbar_create;
	p->ondelete = (void*)skillbar_delete;
	p->onreader = (void*)skillbar_search;
	p->onwriter = (void*)skillbar_modify;

	p->onattach = (void*)skillbar_attach;
	p->ondetach = (void*)skillbar_detach;
	p->ontaking = (void*)skillbar_taking;
	p->ongiving = (void*)skillbar_giving;
}
