#include "libuser.h"
#define OWNBUF listptr.buf0
void carveplanet_verttexnorm(void*, void*, int accx, int accy, vec3 vc, vec3 vr, vec3 vf, vec3 vu);
void dx11data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
void gl41data_insert(_obj* ctx, int type, struct mysrc* src, int cnt);
#define accx 360
#define accy 180



struct own{
	struct texture albedotex;
	struct texture heighttex;
	vec4 distance_per_value;
	vec4 c_and_len;
	struct dx11data dx11;
	struct gl41data gl41;
};




static void texball_copypath(u8* dst, u8* src)
{
	int j;
	for(j=0;j<128;j++){
		if(src[j] <= 0xa){
			dst[j] = 0;
			break;
		}
		dst[j] = src[j];
	}
}
static void texball_prep(struct own* my, u8* albedo, u8* height)
{
	//albedo
	my->albedotex.data = memoryalloc(8192*4096*4, 4);
	if(0 == my->albedotex.data)return;

	int ret = loadtexfromfile(&my->albedotex, albedo);
	if(ret < 0){
		logtoall("texball: failed to load albedo\n");
	}


	//height
	my->heighttex.data = memoryalloc(4096*2048*4, 4);
	if(0 == my->heighttex.data)return;

	ret = loadtexfromfile(&my->heighttex, height);
	if(ret < 0){
		logtoall("texball: failed to load height\n");
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


	struct vertex* vtx = src->vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_333;
	vtx->vbuf_w = 4*9;
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
	carveplanet_verttexnorm(vbuf, ibuf, accx, accy, vc, vr, vf, vu);
	src->vbuf_enq += 1;
	src->ibuf_enq += 1;

	dx11data_insert(ctx, 's', src, 1);
}



#if 1

char* texball_glsl_vs =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"layout(location = 2)in vec3 normal;\n"
"layout(location = 0)out vec3 vo_gi_pos;\n"
"layout(location = 1)out vec2 vo_gi_uvw;\n"
"layout(location = 2)out vec3 vo_gi_norm;\n"
"void main(){\n"
	"vo_gi_pos = vertex;\n"
	"vo_gi_uvw = texuvw;\n"
	"vo_gi_norm = normal;\n"
"}\n";

char* texball_glsl_gs =
GLSL_VERSION
GLSL_PRECISION
"layout(triangles) in;\n"
"layout(triangle_strip, max_vertices = 12) out;\n"
"layout(location = 0)in vec3 vo_gi_pos[];\n"
"layout(location = 1)in vec2 vo_gi_uvw[];\n"
"layout(location = 2)in vec3 vo_gi_norm[];\n"
"uniform mat4 cammvp;\n"
"uniform vec4 c_and_len;\n"
"uniform vec3 distance_per_value;\n"
"uniform sampler2D heightmap;\n"
"out vec2 uvw;\n"
"void main(){\n"
	"vec2 midTex2 = (vo_gi_uvw[0] + vo_gi_uvw[1]) / 2.0;\n"
	"vec2 midTex0 = (vo_gi_uvw[1] + vo_gi_uvw[2]) / 2.0;\n"
	"vec2 midTex1 = (vo_gi_uvw[2] + vo_gi_uvw[0]) / 2.0;\n"
	"vec3 mid2 = (vo_gi_pos[0] + vo_gi_pos[1]) / 2.0;\n"
	"vec3 mid0 = (vo_gi_pos[1] + vo_gi_pos[2]) / 2.0;\n"
	"vec3 mid1 = (vo_gi_pos[2] + vo_gi_pos[0]) / 2.0;\n"
	//"vec3 midNorm2 = normalize((vo_gi_norm[0] + vo_gi_norm[1]) / 2.0);\n"
	//"vec3 midNorm0 = normalize((vo_gi_norm[1] + vo_gi_norm[2]) / 2.0);\n"
	//"vec3 midNorm1 = normalize((vo_gi_norm[2] + vo_gi_norm[0]) / 2.0);\n"

	"vec3 mid0fix = c_and_len.xyz + normalize(    mid0    -c_and_len.xyz)*(c_and_len.w + (texture(heightmap, midTex0).b-0.5)*distance_per_value.x);\n"
	"vec3 mid1fix = c_and_len.xyz + normalize(    mid1    -c_and_len.xyz)*(c_and_len.w + (texture(heightmap, midTex1).b-0.5)*distance_per_value.x);\n"
	"vec3 mid2fix = c_and_len.xyz + normalize(    mid2    -c_and_len.xyz)*(c_and_len.w + (texture(heightmap, midTex2).b-0.5)*distance_per_value.x);\n"
	"vec3 pos0fix = c_and_len.xyz + normalize(vo_gi_pos[0]-c_and_len.xyz)*(c_and_len.w + (texture(heightmap, midTex0).b-0.5)*distance_per_value.x);\n"
	"vec3 pos1fix = c_and_len.xyz + normalize(vo_gi_pos[1]-c_and_len.xyz)*(c_and_len.w + (texture(heightmap, midTex1).b-0.5)*distance_per_value.x);\n"
	"vec3 pos2fix = c_and_len.xyz + normalize(vo_gi_pos[2]-c_and_len.xyz)*(c_and_len.w + (texture(heightmap, midTex2).b-0.5)*distance_per_value.x);\n"

	"vec4 mid0wvp = cammvp * vec4(mid0fix, 1.0);\n"
	"vec4 mid1wvp = cammvp * vec4(mid1fix, 1.0);\n"
	"vec4 mid2wvp = cammvp * vec4(mid2fix, 1.0);\n"
	"vec4 pos0wvp = cammvp * vec4(pos0fix, 1.0);\n"
	"vec4 pos1wvp = cammvp * vec4(pos1fix, 1.0);\n"
	"vec4 pos2wvp = cammvp * vec4(pos2fix, 1.0);\n"

	"gl_Position = pos0wvp;uvw = vo_gi_uvw[0];EmitVertex();\n"
	"gl_Position = mid1wvp;uvw =      midTex1;EmitVertex();\n"
	"gl_Position = mid2wvp;uvw =      midTex2;EmitVertex();\n"
	"EndPrimitive();\n"

	"gl_Position = mid0wvp;uvw =      midTex0;EmitVertex();\n"
	"gl_Position = pos1wvp;uvw = vo_gi_uvw[1];EmitVertex();\n"
	"gl_Position = mid2wvp;uvw =      midTex2;EmitVertex();\n"
	"EndPrimitive();\n"

	"gl_Position = mid0wvp;uvw =      midTex0;EmitVertex();\n"
	"gl_Position = mid1wvp;uvw =      midTex1;EmitVertex();\n"
	"gl_Position = pos2wvp;uvw = vo_gi_uvw[2];EmitVertex();\n"
	"EndPrimitive();\n"

	"gl_Position = mid0wvp;uvw =      midTex0;EmitVertex();\n"
	"gl_Position = mid1wvp;uvw =      midTex1;EmitVertex();\n"
	"gl_Position = mid2wvp;uvw =      midTex2;EmitVertex();\n"
	"EndPrimitive();\n"
"}\n";

#else

char* texball_glsl_vs =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 vertex;\n"
"layout(location = 1)in vec2 texuvw;\n"
"layout(location = 2)in vec3 normal;\n"
"uniform mat4 cammvp;\n"
"uniform vec4 c_and_len;\n"
"uniform vec3 distance_per_value;\n"
"uniform sampler2D heightmap;\n"
"out vec2 uvw;\n"
"void main(){\n"
	"uvw = texuvw;\n"
	"vec3 tmpxyz = c_and_len.xyz + c_and_len.w*normalize(vertex-c_and_len.xyz);\n"
	"tmpxyz += distance_per_value.x * normalize(normal) * (texture(heightmap, texuvw).b-0.5);\n"
	"gl_Position = cammvp * vec4(tmpxyz, 1.0);\n"
"}\n";
char* texball_glsl_gs = 0;

#endif

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
	data->src.gs = texball_glsl_gs;
	data->src.fs = texball_glsl_fs;
	data->src.shader_enq = 42;

	data->dst.arg[0].name = "distance_per_value";
	data->dst.arg[0].data = my->distance_per_value;
	data->dst.arg[0].fmt = 'v';

	data->dst.arg[1].name = "c_and_len";
	data->dst.arg[1].data = my->c_and_len;
	data->dst.arg[1].fmt = hex16('v','4');

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
	//logtoall("w=%d,h=%d\n",data->src.tex[0].w, data->src.tex[0].h);

	struct vertex* vtx = data->src.vtx;
	vtx->geometry = 3;
	vtx->opaque = 0;

	vtx->vbuf_fmt = vbuffmt_333;
	vtx->vbuf_w = 4*9;
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
static void texball_gl41draw(
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

	gl41line_circle(ctx, 0xffffff, vc,vr,vf);

	struct own* my = act->OWNBUF;
	if(0 == my)return;

	my->c_and_len[0] = vc[0];
	my->c_and_len[1] = vc[1];
	my->c_and_len[2] = vc[2];
	my->c_and_len[3] = vec3_getlen(vr);

	struct mysrc* src = &my->gl41.src;
	if(0 == src)return;

	void* vbuf = src->vtx[0].vbuf;
	void* ibuf = src->vtx[0].ibuf;
	carveplanet_verttexnorm(vbuf, ibuf, accx, accy, vc, vr, vf, vu);

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
	logtoall("texball(%x,%x,%x)\n",win,act,sty);
}
static void texball_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
}




static void texball_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->vfmt){
	case _dx11list_:texball_dx11draw(ent,slot, wor,geom, wnd,area);break;
	case _gl41list_:texball_gl41draw(ent,slot, wor,geom, wnd,area);break;
	}
}
static void texball_read_byworld_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
}
static void texball_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41list_:
		break;
	}
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

	switch(caller->type){
	case _wnd_:
	case _render_:
		texball_read_bywnd(ent,slot, caller,area);
		break;
	default:
		texball_read_byworld_bycam_bywnd(ent,slot, stack,sp);
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
	logtoall("@texball_attach\n");
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
static void texball_create(_obj* act, void* arg, int argc, u8** argv)
{
	if(0 == act)return;

	struct own* my = act->OWNBUF = memoryalloc(0x1000, 0);
	if(0 == my)return;

	my->distance_per_value[0] = 0.0;

	int j;
	u8 albedo[128] = {0};
	u8 height[128] = {0};
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "factor:", 7)){
			decstr2float(argv[j]+7, &my->distance_per_value[0]);
		}
		if(0 == ncmp(argv[j], "albedo:", 7)){
			texball_copypath(albedo, argv[j]+7);
		}
		if(0 == ncmp(argv[j], "height:", 7)){
			texball_copypath(height, argv[j]+7);
		}
	}
	if(0==albedo[0])mysnprintf(albedo, 128, "%s", "datafile/jpg/earth/earth-albedo-4096.jpg");
	if(0==height[0])mysnprintf(height, 128, "%s", "datafile/jpg/earth/earth-height-4096.jpg");
	texball_prep(my, albedo, height);
	texball_dx11prep(my);
	texball_gl41prep(my);
}




void texball_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('t', 'e', 'x', 'b', 'a', 'l', 'l', 0);

	p->oncreate = (void*)texball_create;
	p->ondelete = (void*)texball_delete;
	p->onreader = (void*)texball_search;
	p->onwriter = (void*)texball_modify;

	p->onattach = (void*)texball_attach;
	p->ondetach = (void*)texball_detach;
	p->ontaking = (void*)texball_taking;
	p->ongiving = (void*)texball_giving;
}
