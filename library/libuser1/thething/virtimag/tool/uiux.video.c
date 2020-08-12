#include "libuser.h"
#define OWNBUF buf0
#define _cam_ hex32('c','a','m',0)
#define _yuv_ hex32('y','u','v',0)
void yuyv2rgba(
	u8* src, int s1, int w0, int h0, int x0, int y0, int x1, int y1,
	u8* dst, int s2, int w1, int h1, int x2, int y2, int x3, int y3
);
void dx11data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(struct entity* ctx, int type, struct mysrc* src, int cnt);


struct own{
	void* yuyv;
	void* y8u8v8a8;
	int width;
	int height;

	struct dx11data dx11;
	struct gl41data gl41;
};
void video_prep(struct own* my)
{
	my->y8u8v8a8 = memorycreate(1024*1024*4, 0);
}
void video_update(
	u8* dstbuf, int dstlen,
	u8* srcbuf, int srclen)
{
	int x,y;
	u8* dst;
	u8* src;
	if(0 == srcbuf)return;
	if(0 == dstbuf)return;

	for(y=0;y<480;y++)
	{
		dst = dstbuf + (y*640*4);
		src = srcbuf + (y*640*2);
		for(x=0;x<640;x+=2)
		{
			dst[4*x + 0] = src[2*x + 0];
			dst[4*x + 1] = src[2*x + 1];
			dst[4*x + 2] = src[2*x + 3];

			dst[4*x + 4] = src[2*x + 2];
			dst[4*x + 5] = src[2*x + 1];
			dst[4*x + 6] = src[2*x + 3];
		}
	}
}




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
char* video_hlsl_ps =
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

	//shader
	src->vs = video_hlsl_vs;
	src->fs = video_hlsl_ps;
	src->shader_enq = 42;

	//texture
	src->tex[0].data = my->y8u8v8a8;
	src->tex[0].fmt = hex32('r','g','b','a');

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
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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

	//video_update(data->tex[0].data, 1024*1024*4, srcbuf, 640*480*2);
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
char* video_glsl_fs =
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

	//shader
	data->src.vs = video_glsl_vs;
	data->src.fs = video_glsl_fs;
	data->src.shader_enq = 42;

	//texture
	data->src.tex[0].data = my->y8u8v8a8;
	data->src.tex[0].fmt = hex32('r','g','b','a');

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
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->fshape.vc;
	float* vr = geom->fshape.vr;
	float* vf = geom->fshape.vf;
	float* vt = geom->fshape.vt;

	struct own* own = act->OWNBUF;
	if(0 == own)return;
	struct mysrc* data = &own->gl41.src;
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

	//video_update(data->tex[0].data, 1024*1024*4, srcbuf, 640*480*2);
	data->tex[0].w = 640;
	data->tex[0].h = 480;
	data->tex_enq[0] += 1;

	data->vbuf_enq += 1;
	gl41data_insert(ctx, 's', data, 1);
}




void video_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	struct own* own;
	u8* dst;
	int cx, cy, ww, hh, j;
	int w = win->fbwidth;
	int h = win->height;
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

	own = act->OWNBUF;
	if(0 == own)return;

	dst = (u8*)(win->rgbabuf);
	if(0 == dst)return;

	yuyv2rgba(
		  own->yuyv, 0, 640, 480,     0,     0,     0,     0,
		  dst, 0,   w,   h, cx-ww, cy-hh, cx+ww, cy+hh
	);
}
void video_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void video_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	//<head>
	htmlprintf(win, 1, ".video{width:50%%;height:100px;float:left;background-color:#1984ea;}\n");

	//<body>
	htmlprintf(win, 2, "<div class=\"video\">\n");
}
void video_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void video_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
void video_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev)
{
}




static void video_read_bycam(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if( 0 == stack)return;
	if('v'!= key)return;

	slot = stack[sp-1].pfoot;
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11full_:video_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41full_:video_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
static void video_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* wnd = stack[sp-2].pchip;
	struct style* sty = stack[sp-2].pfoot;
	if(_rgba_ == wnd->fmt){
		video_draw_pixel(ent,0, wnd, sty);
	}
	else{
		video_read_bycam(ent,foot, stack,sp, arg,key);
	}
}
static void video_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct own* own = ent->OWNBUF;
	if(0 == own)return;

	if(_yuv_ == foot){
		//say("@video_write.yuv: %llx,%x,%llx,%x\n", arg, key, buf, len);
		own->yuyv = buf;
		if(own->y8u8v8a8)video_update(own->y8u8v8a8, 1024*1024*4, buf, 640*480*2);
	}
}
static void video_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void video_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void video_search(struct entity* act)
{
}
static void video_modify(struct entity* act)
{
}
static void video_delete(struct entity* act)
{
	if(0 == act)return;
}
static void video_create(struct entity* act)
{
	if(0 == act)return;

	struct own* own = act->OWNBUF = memorycreate(0x1000, 0);
	if(0 == own)return;

	video_prep(own);
	video_dx11prep(own);
	video_gl41prep(own);
}




void video_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('v', 'i', 'd', 'e', 'o', 0, 0, 0);

	p->oncreate = (void*)video_create;
	p->ondelete = (void*)video_delete;
	p->onsearch = (void*)video_search;
	p->onmodify = (void*)video_modify;

	p->onlinkup = (void*)video_linkup;
	p->ondiscon = (void*)video_discon;
	p->ontaking = (void*)video_taking;
	p->ongiving = (void*)video_giving;
}
