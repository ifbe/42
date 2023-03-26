#include "libuser.h"
#define OWNBUF listptr.buf0
#define _cam_ hex32('c','a','m',0)
int copyfourcc(void*,void*);
void yuyv_to_rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3);
void yuyv_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void uyvy_to_yuvx(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void bggr_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void rggb_to_rgba(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void yuvx_to_ascii(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void rgbx_to_ascii(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth);
void dx11data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);




struct own{
	void* inbuf;
	int infmt;
	int inw;
	int inh;

	struct dx11data dx11;
	struct gl41data gl41;
};




char* video_hlsl_vs =
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
char* video_hlsl_rgbx =
"Texture2D    y8u8v8 : register(t0);\n"
"SamplerState status : register(s0);\n"
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	float2 uvw = input.color;\n"
"	float3 rgb = y8u8v8.Sample(status, uvw);\n"
	"float b = yuv.b;\n"
	"float g = yuv.g;\n"
	"float r = yuv.r;\n"
"	return float4(r, g, b, 1.0);\n"
"}";
char* video_hlsl_yuvx =
"Texture2D    y8u8v8 : register(t0);\n"
"SamplerState status : register(s0);\n"
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	float2 uvw = input.color;\n"
"	float3 yuv = y8u8v8.Sample(status, uvw);\n"
	"float y = yuv.b;\n"
	"float u = yuv.g - 0.5;\n"
	"float v = yuv.r - 0.5;\n"
	"float r = y + 1.402*v;\n"
	"float g = y - 0.34414*u - 0.71414*v;\n"
	"float b = y + 1.1772*u;\n"
"	return float4(r, g, b, 1.0);\n"
"}";
static void video_dx11prep(struct own* my)
{
	struct mysrc* src = &my->dx11.src;

	if(_yuvx_ == my->infmt){
		//shader
		src->vs = video_hlsl_vs;
		src->fs = video_hlsl_yuvx;
		src->shader_enq = 42;

		//texture
		src->tex[0].data = my->inbuf;
		src->tex[0].fmt = _rgba_;
	}
	else{
		//shader
		src->vs = video_hlsl_vs;
		src->fs = video_hlsl_rgbx;
		src->shader_enq = 42;

		//texture
		src->tex[0].data = my->inbuf;
		src->tex[0].fmt = _rgba_;
	}

	//vertex
	struct vertex* vtx = &src->vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
}
void video_dx11draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;

	struct own* own = act->OWNBUF;
	if(0 == own)return;
	struct mysrc* data = &own->dx11.src;
	if(0 == data)return;
	float (*vbuf)[6] = data->vtx[0].vbuf;
	if(0 == vbuf)return;

	vbuf[0][0] = vc[0] - vr[0] - vf[0];
	vbuf[0][1] = vc[1] - vr[1] - vf[1];
	vbuf[0][2] = vc[2] - vr[2] - vf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + vf[0];
	vbuf[1][1] = vc[1] + vr[1] + vf[1];
	vbuf[1][2] = vc[2] + vr[2] + vf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + vf[0];
	vbuf[2][1] = vc[1] - vr[1] + vf[1];
	vbuf[2][2] = vc[2] - vr[2] + vf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + vf[0];
	vbuf[3][1] = vc[1] + vr[1] + vf[1];
	vbuf[3][2] = vc[2] + vr[2] + vf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - vf[0];
	vbuf[4][1] = vc[1] - vr[1] - vf[1];
	vbuf[4][2] = vc[2] - vr[2] - vf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - vf[0];
	vbuf[5][1] = vc[1] + vr[1] - vf[1];
	vbuf[5][2] = vc[2] + vr[2] - vf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	data->tex[0].w = 640;
	data->tex[0].h = 480;
	data->tex_enq[0] += 1;

	data->vbuf_enq += 1;
	dx11data_insert(ctx, 's', data, 1);
}





char* video_glsl_vs =
GLSL_VERSION
"layout(location = 0)in mediump vec3 vertex;\n"
"layout(location = 1)in mediump vec2 texuvw;\n"
"uniform mat4 cammvp;\n"
"out mediump vec2 uv;\n"
"void main(){\n"
	"uv = texuvw;\n"
	"gl_Position = cammvp * vec4(vertex, 1.0);\n"
"}\n";
char* video_glsl_rgba =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uv;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"mediump vec3 yuv = texture(tex0, vec2(uv.x, 1.0-uv.y)).rgb;\n"
	"mediump float r = yuv.r;\n"
	"mediump float g = yuv.g;\n"
	"mediump float b = yuv.b;\n"
	"mediump vec3 rgb = vec3(r,g,b);\n"
	//"mat3 correct = mat3(1.831268, -0.453759, -0.377514, -0.451451, 1.634110, -0.182660, 0.026111, -0.919596, 1.893486);\n"
	//"rgb = correct*rgb;\n"
	"FragColor = vec4(rgb, 1.0);\n"
"}\n";
char* video_glsl_yuvx =
GLSL_VERSION
"uniform sampler2D tex0;\n"
"in mediump vec2 uv;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"mediump vec3 yuv = texture(tex0, uv).rgb;\n"
	"mediump float y = yuv.r;\n"
	"mediump float u = yuv.g - 0.5;\n"
	"mediump float v = yuv.b - 0.5;\n"
	"mediump float r = y + 1.402*v;\n"
	"mediump float g = y - 0.34414*u - 0.71414*v;\n"
	"mediump float b = y + 1.1772*u;\n"
	"FragColor = vec4(r, g, b, 1.0);\n"
"}\n";
static void video_gl41prep(struct own* my)
{
	struct gl41data* data = &my->gl41;
/*
	//dual plane yuv
	if(){
		//shader
		data->src.vs = video_glsl_vs;
		data->src.fs = video_glsl_yuv420p;
		data->src.shader_enq = 42;

		//texture
		data->src.tex[0].data = my->outbuf;		//y plane
		data->src.tex[0].fmt = _rgba_;
		data->src.tex[1].data = my->outbuf;		//uv plane
		data->src.tex[1].fmt = _rgba_;
	}
*/
	if(_yuvx_ == my->infmt){
		//shader
		data->src.vs = video_glsl_vs;
		data->src.fs = video_glsl_yuvx;
		data->src.shader_enq = 42;

		//texture
		data->src.tex[0].data = my->inbuf;
		data->src.tex[0].fmt = _rgba_;
	}
	else{
		//shader
		data->src.vs = video_glsl_vs;
		data->src.fs = video_glsl_rgba;
		data->src.shader_enq = 42;

		//texture
		data->src.tex[0].data = my->inbuf;
		data->src.tex[0].fmt = _rgba_;
	}

	//vertex
	struct vertex* vtx = &data->src.vtx[0];
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_33;
	vtx->vbuf_w = 6*4;
	vtx->vbuf_h = 6;
	vtx->vbuf_len = (vtx->vbuf_w) * (vtx->vbuf_h);
	vtx->vbuf = memorycreate(vtx->vbuf_len, 0);

	//
	data->dst.texname[0] = "tex0";
}
void video_gl41draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;
	gl41line_rect(ctx, 0xffffff, vc,vr,vf);

	struct own* own = act->OWNBUF;
	if(0 == own)return;
	struct mysrc* data = &own->gl41.src;
	if(0 == data)return;
	float (*vbuf)[6] = data->vtx[0].vbuf;
	if(0 == vbuf)return;

	vec3 tf = {vf[0], vf[1], vf[2]};
	float f = (float)own->inh / own->inw;
	vec3_setlen(tf, vec3_getlen(vr)*f);

	vbuf[0][0] = vc[0] - vr[0] - tf[0];
	vbuf[0][1] = vc[1] - vr[1] - tf[1];
	vbuf[0][2] = vc[2] - vr[2] - tf[2];
	vbuf[0][3] = 0.0;
	vbuf[0][4] = 1.0;
	vbuf[0][5] = 0.0;

	vbuf[1][0] = vc[0] + vr[0] + tf[0];
	vbuf[1][1] = vc[1] + vr[1] + tf[1];
	vbuf[1][2] = vc[2] + vr[2] + tf[2];
	vbuf[1][3] = 1.0;
	vbuf[1][4] = 0.0;
	vbuf[1][5] = 0.0;

	vbuf[2][0] = vc[0] - vr[0] + tf[0];
	vbuf[2][1] = vc[1] - vr[1] + tf[1];
	vbuf[2][2] = vc[2] - vr[2] + tf[2];
	vbuf[2][3] = 0.0;
	vbuf[2][4] = 0.0;
	vbuf[2][5] = 0.0;

	vbuf[3][0] = vc[0] + vr[0] + tf[0];
	vbuf[3][1] = vc[1] + vr[1] + tf[1];
	vbuf[3][2] = vc[2] + vr[2] + tf[2];
	vbuf[3][3] = 1.0;
	vbuf[3][4] = 0.0;
	vbuf[3][5] = 0.0;

	vbuf[4][0] = vc[0] - vr[0] - tf[0];
	vbuf[4][1] = vc[1] - vr[1] - tf[1];
	vbuf[4][2] = vc[2] - vr[2] - tf[2];
	vbuf[4][3] = 0.0;
	vbuf[4][4] = 1.0;
	vbuf[4][5] = 0.0;

	vbuf[5][0] = vc[0] + vr[0] - tf[0];
	vbuf[5][1] = vc[1] + vr[1] - tf[1];
	vbuf[5][2] = vc[2] + vr[2] - tf[2];
	vbuf[5][3] = 1.0;
	vbuf[5][4] = 1.0;
	vbuf[5][5] = 0.0;

	//yuvx4yuyv(data->tex[0].data, 1024*1024*4, srcbuf, 640*480*2);
	//say("infmt=%.4s,outfmt=%.4s\n", &own->infmt, &own->outfmt);
	data->tex[0].data = own->inbuf;
	data->tex[0].w = own->inw;
	data->tex[0].h = own->inh;
	data->tex_enq[0] += 1;

	data->vbuf_enq += 1;
	gl41data_insert(ctx, 's', data, 1);
}




void video_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	struct own* own;
	u8* dst;
	int cx, cy, ww, hh, j;
	int w = win->whdf.fbwidth;
	int h = win->whdf.height;
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

	own = act->OWNBUF;
	if(0 == own)return;

	dst = (u8*)(win->rgbanode.buf);
	if(0 == dst)return;

	yuyv_to_rgba(
		  own->inbuf, 0, 640, 480,     0,     0,     0,     0,
		  dst, 0,   w,   h, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
void video_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
void video_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	//<head>
	htmlprintf(win, 1, ".video{width:50%%;height:100px;float:left;background-color:#1984ea;}\n");

	//<body>
	htmlprintf(win, 2, "<div class=\"video\">\n");
}
void video_draw_tui(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty)
{
	struct own* own = ent->OWNBUF;
	if(0 == own->inbuf)return;
	switch(own->infmt){
	case _yuvx_:
		yuvx_to_ascii(own->inbuf, 0, 640, 480, win->tuitext.buf, 0, win->whdf.width, win->whdf.height);
		break;
	case _rgba_:
		rgbx_to_ascii(own->inbuf, 0, 640, 480, win->tuitext.buf, 0, win->whdf.width, win->whdf.height);
		break;
	}
}
void video_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
void video_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev)
{
}




static void video_read_bycam(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	if(0 == stack)return;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->hfmt){
	case _dx11list_:video_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41list_:video_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
static void video_taking(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;
	if(0 == stack)return;

	switch(caller->hfmt){
	case _cli_:
		break;
	case _tui_:
		video_draw_tui(ent,0, caller, area);
		break;
	case _rgba_:
		video_draw_pixel(ent,0, caller, area);
		break;
	default:
		video_read_bycam(ent,foot, stack,sp);
	}
}
static void video_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@video_write.yuv: %p,%x,%p,%x\n", arg, key, buf, len);

	struct own* own = ent->OWNBUF;
	if(0 == own)return;

	own->inbuf = buf;

	say("video parsearg:\n");
	if(_kv88_ == key){
		int j;
		struct kv88* kv = arg;
		for(j=0;j<2;j++){
			say("%llx=%llx\n", kv[j].key, kv[j].val);
		}
	}
}
static void video_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void video_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void video_search(_obj* act)
{
}
static void video_modify(_obj* act)
{
}
static void video_delete(_obj* act)
{
	if(0 == act)return;
}
static void video_create(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;

	struct own* own = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == own)return;

	int j;
	u32 width = 640;
	u32 height = 480;
	u64 fmt = _yuvx_;
	for(j=0;j<argc;j++){
		say("%d:%.4s\n", j, argv[j]);
		if(0 == ncmp(argv[j], "format:", 7)){
			copyfourcc(&fmt, argv[j]+7);
		}
		if(0 == ncmp(argv[j], "width:", 6)){
			decstr2u32(argv[j]+6, &width);
		}
		if(0 == ncmp(argv[j], "height:", 7)){
			decstr2u32(argv[j]+7, &height);
		}
	}
	own->inw = width;
	own->inh = height;
	own->infmt = fmt;
	own->inbuf = 0;

	video_dx11prep(own);
	video_gl41prep(own);
}




void video_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('v', 'i', 'd', 'e', 'o', 0, 0, 0);

	p->oncreate = (void*)video_create;
	p->ondelete = (void*)video_delete;
	p->onreader = (void*)video_search;
	p->onwriter = (void*)video_modify;

	p->onattach = (void*)video_attach;
	p->ondetach = (void*)video_detach;
	p->ontaking = (void*)video_taking;
	p->ongiving = (void*)video_giving;
}
