#include "libuser.h"
#define acc 18




char dx11solidpoint_vert[] =
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
char dx11solidpoint_frag[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




static int dx11solidpoint_fill(struct mysrc* src)
{
	if(0 == src->vs){
		src->vs = dx11solidpoint_vert;
		src->fs = dx11solidpoint_frag;
		src->shader_enq = 1;
	}

	struct vertex* vtx = src->vtx;
	vtx->opaque = 0;
	vtx->geometry = 1;

	if(0 == vtx->vbuf){
		vtx->vbuf_len = 0x100000;
		vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
		if(0 == vtx->vbuf)return -1;

		vtx->vbuf_w = 4*3*2;
		vtx->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		vtx->vbuf_fmt = vbuffmt_33;
		src->vbuf_enq = 1;
	}

	return 0;
}
int dx11solidpoint_vars(struct entity* win, int unused, float** vbuf, int vcnt)
{
	if(0 == win)return -1;
	if(0 == win->dxfull_solid)return -2;

	struct dx11data* p = win->dxfull_solid[point3d];
	if(0 == p){
		p = win->dxfull_solid[point3d] = memorycreate(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = dx11solidpoint_fill(&p->src);
		if(ret < 0)return -4;
	}

	vlen = vtx->vbuf_h;
	*vbuf = (vtx->vbuf) + (24*vlen);
	vtx->vbuf_h += vcnt;

	return vlen;
}




void dx11point(struct entity* win, u32 rgb, vec3 vc)
{
	float* vbuf;
	dx11solidpoint_vars(win, 0, &vbuf, 1);

	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	vbuf[0] = vc[0];
	vbuf[1] = vc[1];
	vbuf[2] = vc[2];
	vbuf[3] = rr;
	vbuf[4] = gg;
	vbuf[5] = bb;
}
