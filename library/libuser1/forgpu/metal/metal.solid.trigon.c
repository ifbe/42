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




static char* shadersource =
"#include <metal_matrix>\n"
"using namespace metal;\n"

"struct Uniform {\n"
"	metal::float4x4 wvp;\n"
"};\n"
"struct VertexInput {\n"
"	float3 v [[ attribute(0) ]];\n"
"	float3 n [[ attribute(1) ]];\n"
"	float3 c [[ attribute(2) ]];\n"
"};\n"
"struct VertOutFragIn {\n"
"	float4 v [[ position ]];\n"
"	float3 n;\n"
"	float3 c;\n"
"};\n"

"vertex VertOutFragIn vert(\n"
"	VertexInput in [[ stage_in ]],\n"
"	constant Uniform& uni [[ buffer(1) ]] )\n"
"{\n"
"	VertOutFragIn out;\n"
"	out.v = uni.wvp * float4(in.v, 1.0);\n"
"	out.n = in.n;\n"
"	out.c = in.c;\n"
"	return out;\n"
"}\n"
"fragment float4 frag(VertOutFragIn in [[stage_in]]){\n"
"	return float4(in.c, 1.0);\n"
"}\n";




static int mt20solidtrigon_fill(struct mtsrc* src)
{
	if(0 == src->source){
		src->source = shadersource;
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
		vtx->vbuf_len = 65536*4*9;		//[0,65535]
		vtx->vbuf = memorycreate(vtx->vbuf_len, 0);
		if(0 == vtx->vbuf)return -1;

		vtx->vbuf_w = 4*3*3;
		vtx->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		vtx->vbuf_fmt = vbuffmt_333;
		src->vbuf_enq = 1;
	}

	return 0;
}
int mt20solidtrigon_vars(struct entity* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	if(0 == win)return -1;
	if(0 == win->mtfull_solid)return -2;

	struct mt20data* p = win->mtfull_solid[trigon3d];
	if(0 == p){
		p = win->mtfull_solid[trigon3d] = memorycreate(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ilen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = mt20solidtrigon_fill(&p->src);
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




void mt20solid_triangle(struct entity* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float* vbuf;
	u16* ibuf;
	int vlen = mt20solidtrigon_vars(win, 0, &vbuf, &ibuf, 3, 1);
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




void mt20solid_rect(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = mt20solidtrigon_vars(win, 0, &vbuf, &ibuf, 4, 2);
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




void mt20solid_circle(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = mt20solidtrigon_vars(win, 0, &vbuf, &ibuf, circleacc+1, circleacc);
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



void mt20solid_cask(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = mt20solidtrigon_vars(win, 0, &vbuf, &ibuf, acc * 2, acc * 2);
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
void mt20solid_cylinder(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vec3 tc;
	vec3 tf;
	mt20solid_cask(win, rgb, vc, vr, vf, vu);

	tc[0] = vc[0]+vu[0];
	tc[1] = vc[1]+vu[1];
	tc[2] = vc[2]+vu[2];
	mt20solid_circle(win, rgb, tc, vr, vf);

	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	tf[0] = -vf[0];
	tf[1] = -vf[1];
	tf[2] = -vf[2];
	mt20solid_circle(win, rgb, tc, vr, tf);
}




void mt20solid_prism4(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = mt20solidtrigon_vars(win, 0, &vbuf, &ibuf, 24, 12);
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