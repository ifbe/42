#include "libuser.h"
#define acc 16
#define circleacc 32
#define sphereaccx 16
#define sphereaccy (sphereaccx|0x1)
void carvetrigonindex_triangle_v3n3x3(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 v0,vec3 v1,vec3 v2);
void carvetrigonindex_rect_v3n3x3(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_circle_v3n3x3(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_cask_v3n3x3(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_prism4_v3n3x3(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_sphere_v3n3x3(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);




char dx11solidtrigon_vert[] =
"cbuffer VSConstantBuffer : register(b0){\n"
	"matrix matmvp;\n"
"};\n"
"struct VSin{\n"
	"float3 v : PA;\n"
	"float3 n : PB;\n"
	"float3 c : PC;\n"
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
char dx11solidtrigon_frag[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




static int dx11solidtrigon_fill(struct mysrc* src)
{
	if(0 == src->vs){
		src->vs = dx11solidtrigon_vert;
		src->fs = dx11solidtrigon_frag;
		src->shader_enq = 1;
	}

	struct vertex* vtx = src->vtx;
	vtx->opaque = 0;
	vtx->geometry = 3;

	if(0 == vtx->ibuf){
		vtx->ibuf_len = 0x100000;
		vtx->ibuf = memorycreate(vtx->ibuf_len, 0);
		if(0 == vtx->ibuf)return -2;

		vtx->ibuf_w = 2*3;
		vtx->ibuf_h = 0;	//(src->ibuf_len) / (src->ibuf_w);
		vtx->ibuf_fmt = 0x222;
		src->ibuf_enq = 1;
	}

	if(0 == vtx->vbuf){
		vtx->vbuf_len = 65536*4*9;		//65535*4*9
		vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
		if(0 == vtx->vbuf)return -1;

		vtx->vbuf_w = 4*3*3;
		vtx->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		vtx->vbuf_fmt = vbuffmt_333;
		src->vbuf_enq = 1;
	}

	return 0;
}
int dx11solidtrigon_vars(struct entity* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	if(0 == win)return -1;
	if(0 == win->dxfull_solid)return -2;

	struct dx11data* p = win->dxfull_solid[trigon3d];
	if(0 == p){
		p = win->dxfull_solid[trigon3d] = memorycreate(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ilen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = dx11solidtrigon_fill(&p->src);
		if(ret < 0)return -4;
	}

	vlen = vtx->vbuf_h;
	ilen = vtx->ibuf_h;
	*vbuf = (vtx->vbuf) + (36*vlen);
	*ibuf = (vtx->ibuf) + (6*ilen);
	vtx->vbuf_h += vcnt;
	vtx->ibuf_h += icnt;

	return vlen;
}




void dx11solid_triangle(struct entity* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11solidtrigon_vars(win, 0, &vbuf, &ibuf, 3, 1);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*3;j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvetrigonindex_triangle_v3n3x3(vbuf,vlen, ibuf,0, v0,v1,v2);
}




void dx11solid_rect(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11solidtrigon_vars(win, 0, &vbuf, &ibuf, 4, 2);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*4;j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvetrigonindex_rect_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void dx11solid_circle(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11solidtrigon_vars(win, 0, &vbuf, &ibuf, circleacc+1, circleacc);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*(circleacc+1);j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvetrigonindex_circle_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf);
}



void dx11solid_cask(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11solidtrigon_vars(win, 0, &vbuf, &ibuf, acc * 2, acc * 2);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*(acc*2);j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvetrigonindex_cask_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void dx11solid_cylinder(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vec3 tc;
	vec3 tf;
	dx11solid_cask(win, rgb, vc, vr, vf, vu);

	tc[0] = vc[0]+vu[0];
	tc[1] = vc[1]+vu[1];
	tc[2] = vc[2]+vu[2];
	dx11solid_circle(win, rgb, tc, vr, vf);

	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	tf[0] = -vf[0];
	tf[1] = -vf[1];
	tf[2] = -vf[2];
	dx11solid_circle(win, rgb, tc, vr, tf);
}




void dx11solid_prism4(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = dx11solidtrigon_vars(win, 0, &vbuf, &ibuf, 24, 12);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*24;j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvetrigonindex_prism4_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
