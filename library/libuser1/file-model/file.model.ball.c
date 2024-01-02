#include "libuser.h"
#define OWNBUF listptr.buf0
void carveplanet_verttexnorm(void*, void*, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void dx11data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




struct own{
	struct texture albedotex;
	struct texture heighttex;
	struct dx11data dx11;
	struct gl41data gl41;
};
static void texball_prep(struct own* my, char* albedo, char* height)
{
	//albedo
	my->albedotex.data = memoryalloc(4096*2048*4, 4);
	if(0 == my->albedotex.data)return;

	int ret = loadtexfromfile(&my->albedotex, albedo);
	if(ret < 0){
		say("texball: failed to load albedo\n");
	}


	//height
	my->heighttex.data = memoryalloc(4096*2048*4, 4);
	if(0 == my->heighttex.data)return;

	ret = loadtexfromfile(&my->heighttex, height);
	if(ret < 0){
		say("texball: failed to load height\n");
	}
/*
	int x,y;
	u32* buf = my->albedotex.data;
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
	my->albedotex.w = 2048;
	my->albedotex.h = 2048;
*/
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
	struct texture* albedotex = &src->tex[0];
	albedotex->fmt  = hex32('r','g','b','a');
	albedotex->data = my->albedotex.data;
	albedotex->w    = my->albedotex.w;
	albedotex->h    = my->albedotex.h;
	src->tex_enq[0] = 42;

	struct texture* heighttex = &src->tex[1];
	heighttex->fmt  = hex32('r','g','b','a');
	heighttex->data = my->heighttex.data;
	heighttex->w    = my->heighttex.w;
	heighttex->h    = my->heighttex.h;
	src->tex_enq[1] = 42;


#define accx 64
#define accy 63
	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_333;
	vtx->vbuf_w = 4*9;
	vtx->vbuf_h = accx*accy+(accx-1)*2;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
	src->vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accy*(accx-1)*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memoryalloc(vtx->ibuf_len, 0);
	src->ibuf_enq = 0;
}
static void texball_dx11draw(
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
	carveplanet_verttexnorm(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	dx11data_insert(ctx, 's', src, 1);
}




char* texball_glsl_vs =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"layout(location = 2)in vec3 normal;\n"
"uniform mat4 cammvp;\n"
"uniform sampler2D heightmap;\n"
"out vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"vec3 tmpxyz = vertex + normal*texture(heightmap, texuvw).b/10.0;\n"
	"gl_Position = cammvp * vec4(tmpxyz, 1.0);\n"
"}\n";
char* texball_glsl_fs =
GLSL_VERSION
GLSL_PRECISION
"in vec2 uvw;\n"
"out vec4 FragColor;\n"
"uniform sampler2D albedomap;\n"
"void main(){\n"
	"FragColor = vec4(texture(albedomap, uvw).bgr, 1.0);\n"
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
	struct texture* albedomap = &data->src.tex[0];
	albedomap->fmt  = hex32('r','g','b','a');
	albedomap->data = my->albedotex.data;
	albedomap->w    = my->albedotex.w;
	albedomap->h    = my->albedotex.h;
	data->dst.texname[0] = "albedomap";
	data->src.tex_enq[0] = 42;
	struct texture* heightmap = &data->src.tex[1];
	heightmap->fmt  = hex32('r','g','b','a');
	heightmap->data = my->heighttex.data;
	heightmap->w    = my->heighttex.w;
	heightmap->h    = my->heighttex.h;
	data->dst.texname[1] = "heightmap";
	data->src.tex_enq[1] = 42;
	//say("w=%d,h=%d\n",data->src.tex[0].w, data->src.tex[0].h);

#define accx 64
#define accy 63
	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_333;
	vtx->vbuf_w = 4*9;
	vtx->vbuf_h = accx*accy+(accx-1)*2;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
	data->src.vbuf_enq = 0;

	vtx->ibuf_fmt = 0x222;
	vtx->ibuf_w = 2*3;
	vtx->ibuf_h = accy*(accx-1)*2;
	vtx->ibuf_len = (vtx->ibuf_w) * (vtx->ibuf_h);
	vtx->ibuf = memoryalloc(vtx->ibuf_len, 0);
	data->src.ibuf_enq = 0;
}
static void texball_gl41draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* none)
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
	carveplanet_verttexnorm(vbuf, ibuf, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;
	gl41data_insert(ctx, 's', src, 1);
}




static void texball_draw_pixel(
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
		//say("y=%d,%llx,%llx\n",y,dst,src);
		if('b' == ((win->hfmt)&0xff))
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
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void texball_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void texball_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void texball_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("texball(%x,%x,%x)\n",win,act,sty);
}
static void texball_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
}




static void texball_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->hfmt){
	case _dx11list_:texball_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41list_:texball_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
static void texball_wrl_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void texball_wnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
}




static void texball_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		texball_wnd(ent,slot, caller,area);
		break;
	default:
		texball_wrl_cam_wnd(ent,slot, stack,sp);
		break;
	}
}
static void texball_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void texball_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void texball_attach(struct halfrel* self, struct halfrel* peer)
{
	say("@texball_attach\n");
}




static void texball_search(_obj* act)
{
}
static void texball_modify(_obj* act)
{
}
static void texball_delete(_obj* act)
{
	if(0 == act)return;
}
static void texball_create(_obj* act, void* str)
{
	if(0 == act)return;

	struct own* my = act->OWNBUF = memoryalloc(0x1000, 0);
	if(0 == my)return;

	char* albedo = str;
	if(0 == str)str = "datafile/jpg/texball-earth.jpg";
	char* height = "datafile/jpg/texball-earth-height.jpg";
	texball_prep(my, albedo, height);

	texball_dx11prep(my);
	texball_gl41prep(my);
}




void texball_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('t', 'e', 'x', 'b', 'a', 'l', 'l', 0);

	p->oncreate = (void*)texball_create;
	p->ondelete = (void*)texball_delete;
	p->onreader = (void*)texball_search;
	p->onwriter = (void*)texball_modify;

	p->onattach = (void*)texball_attach;
	p->ondetach = (void*)texball_detach;
	p->ontaking = (void*)texball_taking;
	p->ongiving = (void*)texball_giving;
}
