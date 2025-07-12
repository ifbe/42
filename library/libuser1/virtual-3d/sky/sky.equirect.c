#include "libuser.h"
#define OWNBUF listptr.buf0
void carveplanet_verttex(void*, void*, int accx, int accy, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void dx11data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
#define accx 180
#define accy 180



struct own{
	struct texture tex;
	struct dx11data dx11;
	struct gl41data gl41;
};
static void equirect_prep(struct own* my, char* str)
{
	my->tex.data = memoryalloc(4096*2048*4, 4);
	if(0 == my->tex.data)return;

	int j = loadtexfromfile(&my->tex, str);
	if(j >= 0)return;

	int x,y;
	u32* buf = my->tex.data;
	for(y=0;y<1024;y++){
		for(x=0;x<2048;x++){
			buf[y*2048+x] = ((y&0x3f)<<18) | ((x&0x3f)<<10);
		}
	}
	for(y=1024;y<2048;y++){
		for(x=0;x<2048;x++){
			buf[y*2048+x] = ((y&0x3f)<<10) | ((x&0x3f)<<2);
		}
	}
	my->tex.w = 2048;
	my->tex.h = 2048;
}




char* equirect_hlsl_vs =
"cbuffer VSConstantBuffer : register(b0){\n"
	"matrix matmvp;\n"
"};\n"
"struct VSin{\n"
	"float3 v : PA;\n"
	"float3 t : PB;\n"
"};\n"
"struct VSout{\n"
	"float4 where : SV_POSITION;\n"
	"float4 color : COLOR;\n"
"};\n"
"VSout main(VSin input){\n"
	"VSout output;\n"
	"output.where = mul(float4(input.v, 1.0), matmvp);\n"
	"output.color = float4(input.t, 1.0);\n"
	"return output;\n"
"}\n";
char* equirect_hlsl_ps =
"Texture2D    b8g8r8 : register(t0);\n"
"SamplerState status : register(s0);\n"
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	float2 uvw = input.color;\n"
"	float3 bgr = b8g8r8.Sample(status, uvw);\n"
"	return float4(bgr, 1.0);\n"
"}";
static void equirect_dx11prep(struct own* my)
{
	struct mysrc* src = &my->dx11.src;

	//shader
	src->vs = equirect_hlsl_vs;
	src->fs = equirect_hlsl_ps;
	src->shader_enq = 42;

	//texture
	struct texture* tex = &src->tex[0];
	tex->fmt  = hex32('r','g','b','a');
	tex->data = my->tex.data;
	tex->w    = my->tex.w;
	tex->h    = my->tex.h;
	src->tex_enq[0] = 42;

	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*6;
	vtx->vbuf_h = (accx+1)*(accy+1);
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
	src->vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accx*(accy-2)*2 + accx*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memoryalloc(vtx->ibuf_len, 0);
	src->ibuf_enq = 0;
}
static void equirect_dx11draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* none)
{
	struct own* my = act->OWNBUF;
	if(0 == my)return;

	struct mysrc* src = &my->dx11.src;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	carveplanet_verttex(vbuf, ibuf, accx, accy, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	dx11data_insert(ctx, 's', src, 1);
}




char* equirect_glsl_vs =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"uniform int camtype;\n"
"uniform mat4 cammv_;\n"
"uniform mat4 cammvp;\n"
"out vec2 uvw;\n"
"const float MYPI=3.1415926589793;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"switch(camtype){\n"
	"case 20:\n"
	"vec4 tmp = cammv_ * vec4(vertex, 1.0);\n"
	"float tx = tmp.x;\n"
	"float ty = tmp.y;\n"
	"float tz = tmp.z;\n"
	"float sq2 = length(vec2(tx, ty));\n"
	"float sq3 = length(vec2(sq2, tz));\n"
	"float tu = -atan(-tx, ty)/MYPI;\n"
	"float tv = atan(tz, sq2)*2/MYPI;\n"
	"float tw = (1000000-sq3) / (1000000-100);\n"
	"gl_Position = vec4(tu, tv, tw, 1.0);\n"
	"break;\n"
	"default:gl_Position = cammvp * vec4(vertex, 1.0);\n"
	"}\n"
"}\n";
char* equirect_glsl_fs =
GLSL_VERSION
GLSL_PRECISION
"in vec2 uvw;\n"
"out vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	//"FragColor = vec4(uvw, 1.0, 1.0);\n"
"}\n";
static void equirect_gl41prep(struct own* my)
{
	struct gl41data* data = &my->gl41;

	//shader
	data->src.vs = equirect_glsl_vs;
	data->src.fs = equirect_glsl_fs;
	data->src.shader_enq = 42;

	//texture
	struct texture* tex = &data->src.tex[0];
	tex->fmt  = hex32('r','g','b','a');
	tex->data = my->tex.data;
	tex->w    = my->tex.w;
	tex->h    = my->tex.h;
	data->dst.texname[0] = "tex0";
	data->src.tex_enq[0] = 42;
	//logtoall("w=%d,h=%d\n",data->src.tex[0].w, data->src.tex[0].h);

	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 4*6;
	vtx->vbuf_h = (accx+1)*(accy+1);
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
	data->src.vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accx*(accy-2)*2 + accx*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memoryalloc(vtx->ibuf_len, 0);
	data->src.ibuf_enq = 0;
}
static void equirect_gl41draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* none)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	//logtoall("c(%f,%f,%f),r(%f,%f,%f),f(%f,%f,%f),t(%f,%f,%f)\n", vc[0],vc[1],vc[2], vr[0],vr[1],vr[2], vf[0],vf[1],vf[2], vu[0],vu[1],vu[2]);
	//gl41line_sphere(ctx, 0xff00ff, vc, vr, vf, vu);
	//gl41line_prism4(ctx, 0xff00ff, vc, vr, vf, vu);

	struct own* my = act->OWNBUF;
	if(0 == my)return;

	struct mysrc* src = &my->gl41.src;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	carveplanet_verttex(vbuf, ibuf, accx, accy, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static void equirect_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{/*
	u32 tmp;
	u32* dst;
	u32* src;
	int x,y,xmax,ymax,stride;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	xmax = act->whdf.width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->whdf.height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf) + 4*y*(act->whdf.width);
		//logtoall("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->type)&0xff))
		{
			for(x=0;x<xmax;x++)dst[x] = src[x];
		}
		else
		{
			for(x=0;x<xmax;x++)
			{
				tmp = src[x];
				dst[x] = 0xff000000 | (tmp&0xff00) | ((tmp>>16)&0xff) | ((tmp&0xff)<<16);
			}
		}
	}*/
}
static void equirect_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void equirect_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void equirect_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void equirect_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	logtoall("equirect(%x,%x,%x)\n",win,act,sty);
}
static void equirect_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
}




static void equirect_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->vfmt){
	case _dx11list_:equirect_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41list_:equirect_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
static void equirect_wrl_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void equirect_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
	case _bgra8888_:
		break;
	case _gl41list_:
		break;
	}
}




static void equirect_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		equirect_read_bywnd(ent,slot, caller,area);
		break;
	default:
		equirect_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
static void equirect_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void equirect_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void equirect_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@equirect_attach\n");
}




static void equirect_search(_obj* act)
{
}
static void equirect_modify(_obj* act)
{
}
static void equirect_delete(_obj* act)
{
	if(0 == act)return;
}
static void equirect_create(_obj* act, void* str)
{
	if(0 == act)return;

	struct own* my = act->OWNBUF = memoryalloc(0x1000, 0);
	if(0 == my)return;

	if(0 == str)str = "datafile/jpg/texball-earth.jpg";
	equirect_prep(my, str);

	equirect_dx11prep(my);
	equirect_gl41prep(my);
}




void equirect_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('e', 'q', 'u', 'i', 'r', 'e', 'c', 't');

	p->oncreate = (void*)equirect_create;
	p->ondelete = (void*)equirect_delete;
	p->onreader = (void*)equirect_search;
	p->onwriter = (void*)equirect_modify;

	p->onattach = (void*)equirect_attach;
	p->ondetach = (void*)equirect_detach;
	p->ontaking = (void*)equirect_taking;
	p->ongiving = (void*)equirect_giving;
}
