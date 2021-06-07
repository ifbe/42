#include "libuser.h"
#define OWNBUF buf0
void carveplanet(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void dx11data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);




struct own{
	struct texture tex;
	struct dx11data dx11;
	struct gl41data gl41;
};
static void texball_prep(struct own* my, char* str)
{
	my->tex.data = memorycreate(2048*2048*4, 4);
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




char* texball_hlsl_vs =
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
char* texball_hlsl_ps =
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
static void texball_dx11prep(struct own* my)
{
	struct mysrc* src = &my->dx11.src;

	//shader
	src->vs = texball_hlsl_vs;
	src->fs = texball_hlsl_ps;
	src->shader_enq = 42;

	//texture
	struct texture* tex = &src->tex[0];
	tex->fmt  = hex32('r','g','b','a');
	tex->data = my->tex.data;
	tex->w    = my->tex.w;
	tex->h    = my->tex.h;
	src->tex_enq[0] = 42;

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
	src->vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accy*(accx-1)*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
	src->ibuf_enq = 0;
}
static void texball_dx11draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
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
	carveplanet(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	dx11data_insert(ctx, 's', src, 1);
}




char* texball_glsl_vs =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* texball_glsl_fs =
GLSL_VERSION
"in mediump vec2 uvw;\n"
"out mediump vec4 FragColor;\n"
"uniform sampler2D tex0;\n"
"void main(){\n"
	"FragColor = vec4(texture(tex0, uvw).bgr, 1.0);\n"
	//"FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
"}\n";
static void texball_gl41prep(struct own* my)
{
	struct gl41data* data = &my->gl41;

	//shader
	data->src.vs = texball_glsl_vs;
	data->src.fs = texball_glsl_fs;
	data->src.shader_enq = 42;

	//texture
	struct texture* tex = &data->src.tex[0];
	tex->fmt  = hex32('r','g','b','a');
	tex->data = my->tex.data;
	tex->w    = my->tex.w;
	tex->h    = my->tex.h;
	data->dst.texname[0] = "tex0";
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
	data->src.vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accy*(accx-1)*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
	data->src.ibuf_enq = 0;
}
static void texball_gl41draw(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	//say("c(%f,%f,%f),r(%f,%f,%f),f(%f,%f,%f),t(%f,%f,%f)\n", vc[0],vc[1],vc[2], vr[0],vr[1],vr[2], vf[0],vf[1],vf[2], vu[0],vu[1],vu[2]);
	//gl41line_sphere(ctx, 0xff00ff, vc, vr, vf, vu);
	//gl41line_prism4(ctx, 0xff00ff, vc, vr, vf, vu);

	struct own* my = act->OWNBUF;
	if(0 == my)return;

	struct mysrc* src = &my->gl41.src;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	carveplanet(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static void texball_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	xmax = act->width;
	if(xmax >= ww*2)xmax = ww*2;
	ymax = act->height;
	if(ymax >= hh*2)ymax = hh*2;
	stride = win->stride;
	for(y=0;y<ymax;y++)
	{
		dst = (win->buf) + (cy-hh+y)*stride*4 + (cx-ww)*4;
		src = (act->buf) + 4*y*(act->width);
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->fmt)&0xff))
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
static void texball_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texball_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texball_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void texball_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("texball(%x,%x,%x)\n",win,act,sty);
}
static void texball_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
}




static void texball_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11full_:texball_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41full_:texball_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
static void texball_wrl_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
}
static void texball_wnd(_ent* ent,void* slot, _ent* wnd,void* area)
{
}




static void texball_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41full_:
		texball_wnd(ent,slot, caller,area);
		break;
	default:
		texball_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void texball_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void texball_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void texball_linkup(struct halfrel* self, struct halfrel* peer)
{
	say("@texball_linkup\n");
}




static void texball_search(struct entity* act)
{
}
static void texball_modify(struct entity* act)
{
}
static void texball_delete(struct entity* act)
{
	if(0 == act)return;
}
static void texball_create(struct entity* act, void* str)
{
	if(0 == act)return;

	struct own* my = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == my)return;

	if(0 == str)str = "datafile/jpg/texball-earth.jpg";
	texball_prep(my, str);

	texball_dx11prep(my);
	texball_gl41prep(my);
}




void texball_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('t', 'e', 'x', 'b', 'a', 'l', 'l', 0);

	p->oncreate = (void*)texball_create;
	p->ondelete = (void*)texball_delete;
	p->onsearch = (void*)texball_search;
	p->onmodify = (void*)texball_modify;

	p->onlinkup = (void*)texball_linkup;
	p->ondiscon = (void*)texball_discon;
	p->ontaking = (void*)texball_taking;
	p->ongiving = (void*)texball_giving;
}
