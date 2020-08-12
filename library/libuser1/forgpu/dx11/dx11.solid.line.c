#include "libuser.h"
#define halfsqrt3 0.8660254037844386
#define acc 32
void carvelineindex(         float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb);
void carvelineindex_bezier(  float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb, vec3 vt);
void carvelineindex_triangle(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb, vec3 vt);
void carvelineindex_rect(    float* vbuf, int vlen, u16* ibuf, int ilen, vec3 vc, vec3 vr, vec3 vf);
void carvelineindex_circle(  float* vbuf, int vlen, u16* ibuf, int ilen, vec3 vc, vec3 vr, vec3 vf);




char dx11solidline_vert[] =
"cbuffer VSConstantBuffer : register(b0){\n"
	"matrix matmvp;\n"
"};\n"
"struct VSin{\n"
	"float3 v : PA;\n"
	"float3 c : PB;\n"
"};\n"
"struct VSout{\n"
	"float4 where : SV_POSITION;\n"
	"float4 color : COLOR;\n"
"};\n"
"VSout main(VSin input){\n"
	"VSout output;\n"
	"output.where = mul(float4(input.v, 1.0), matmvp);\n"
	"output.color = float4(input.c, 1.0);\n"
	"return output;\n"
"}\n";
char dx11solidline_frag[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




static int dx11solidline_fill(struct mysrc* src)
{
	if(0 == src->vs){
		src->vs = dx11solidline_vert;
		src->fs = dx11solidline_frag;
		src->shader_enq = 1;
	}

	struct vertex* vtx = src->vtx;
	vtx->opaque = 0;
	vtx->geometry = 2;

	if(0 == vtx->ibuf){
		vtx->ibuf_len = 0x100000;
		vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
		if(0 == vtx->ibuf)return -1;

		vtx->ibuf_w = 2*2;
		vtx->ibuf_h = 0;	//(src->ibuf_len) / (src->ibuf_w);
		vtx->ibuf_fmt = 0x22;
		src->ibuf_enq = 1;
	}

	if(0 == vtx->vbuf){
		vtx->vbuf_len = 65536*4*6;
		vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
		if(0 == vtx->vbuf)return -2;

		vtx->vbuf_w = 4*3*2;
		vtx->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		vtx->vbuf_fmt = vbuffmt_33;
		src->vbuf_enq = 1;
	}

	return 0;
}
int dx11solidline_vars(struct entity* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	if(0 == win)return -1;
	if(0 == win->dxfull_solid)return -2;

	struct dx11data* p = win->dxfull_solid[line3d];
	if(0 == p){
		p = win->dxfull_solid[line3d] = memorycreate(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ilen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = dx11solidline_fill(&p->src);
		if(ret < 0)return -4;
	}

	vlen = vtx->vbuf_h;
	ilen = vtx->ibuf_h;
	*vbuf = (vtx->vbuf) + (24*vlen);
	*ibuf = (vtx->ibuf) + (4*ilen);
	vtx->vbuf_h += vcnt;
	vtx->ibuf_h += icnt;

	return vlen;
}




void dx11line(struct entity* win, u32 rgb,
	vec3 va, vec3 vb)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11solidline_vars(win, 0, &vbuf, &ibuf, 2, 1);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*2;j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvelineindex(vbuf,vlen, ibuf,0, va,vb);
}




void dx11line_rect(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11solidline_vars(win, 0, &vbuf, &ibuf, 4, 4);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*4;j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvelineindex_rect(vbuf,vlen, ibuf,0, vc,vr,vf);
}
