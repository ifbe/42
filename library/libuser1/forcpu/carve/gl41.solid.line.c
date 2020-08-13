#include "libuser.h"
#define halfsqrt3 0.8660254037844386
#define acc 32
void carvelineindex(         float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb);
void carvelineindex_bezier(  float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb, vec3 vt);
void carvelineindex_triangle(float* vbuf, int vlen, u16* ibuf, int ilen, vec3 va, vec3 vb, vec3 vt);
void carvelineindex_rect(    float* vbuf, int vlen, u16* ibuf, int ilen, vec3 vc, vec3 vr, vec3 vf);
void carvelineindex_circle(  float* vbuf, int vlen, u16* ibuf, int ilen, vec3 vc, vec3 vr, vec3 vf);




static char dx11solidline_vert[] =
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
static char dx11solidline_frag[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




static char gl41solidline_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

static char gl41solidline_frag[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";




static int line3d_fill(struct entity* win, struct mysrc* src)
{
	if(0 == src->vs){
		switch(win->fmt){
		case _gl41full_:
			src->vs = gl41solidline_vert;
			src->fs = gl41solidline_frag;
			break;
		case _dx11full_:
			src->vs = dx11solidline_vert;
			src->fs = dx11solidline_frag;
			break;
		default:return -3;
		}
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
int line3d_vars(struct entity* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	if(0 == win)return -1;
	if(0 == win->glfull_solid)return -2;

	struct gl41data* p = win->glfull_solid[line3d];
	if(0 == p){
		p = win->glfull_solid[line3d] = memorycreate(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ilen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = line3d_fill(win, &p->src);
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




void gl41line(struct entity* win, u32 rgb,
	vec3 va, vec3 vb)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 2, 1);
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
void gl41line_shorter(struct entity* win, u32 rgb,
	vec3 va, vec3 vb)
{
	vec3 t, ta, tb;
	t[0] = (va[0] + vb[0])/2;
	t[1] = (va[1] + vb[1])/2;
	t[2] = (va[2] + vb[2])/2;

	ta[0] = t[0] + (va[0] - t[0])*0.95;
	ta[1] = t[1] + (va[1] - t[1])*0.95;
	ta[2] = t[2] + (va[2] - t[2])*0.95;

	tb[0] = t[0] + (vb[0] - t[0])*0.95;
	tb[1] = t[1] + (vb[1] - t[1])*0.95;
	tb[2] = t[2] + (vb[2] - t[2])*0.95;

	gl41line(win, rgb, ta, tb);
}




void carveline_arrow(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 va, vec3 vb, vec3 vn)
{
	vbuf[ 0] = va[0];
	vbuf[ 1] = va[1];
	vbuf[ 2] = va[2];

	vbuf[ 6] = vb[0];
	vbuf[ 7] = vb[1];
	vbuf[ 8] = vb[2];

	//tx = va - vb, ty = rotate(tx,30)
	vec3 tx, ty;
	ty[0] = tx[0] = va[0] - vb[0];
	ty[1] = tx[1] = va[1] - vb[1];
	ty[2] = tx[2] = va[2] - vb[2];
	quaternion_operation(ty, vn, PI/2);

	vbuf[12] = vb[0] + (halfsqrt3 * tx[0] + 0.5 * ty[0]) / 8;
	vbuf[13] = vb[1] + (halfsqrt3 * tx[1] + 0.5 * ty[1]) / 8;
	vbuf[14] = vb[2] + (halfsqrt3 * tx[2] + 0.5 * ty[2]) / 8;

	vbuf[18] = vb[0] + (halfsqrt3 * tx[0] - 0.5 * ty[0]) / 8;
	vbuf[19] = vb[1] + (halfsqrt3 * tx[1] - 0.5 * ty[1]) / 8;
	vbuf[20] = vb[2] + (halfsqrt3 * tx[2] - 0.5 * ty[2]) / 8;

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen+1;
	ibuf[5] = vlen+3;
}
void gl41line_arrow(struct entity* win, u32 rgb,
	vec3 va, vec3 vb, vec3 vn)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 4, 3);
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
	carveline_arrow(vbuf,vlen, ibuf,0, va,vb,vn);
}




void gl41line_bezier(struct entity* win, u32 rgb,
	vec3 va, vec3 vb, vec3 vt)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, acc + 1, acc);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(acc+1);j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvelineindex_bezier(vbuf,vlen, ibuf,0, va,vb,vt);
}




#define SPRACC (acc*32)
void carveline_spring(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt)
{
	int j,k;
	float a,c,s;
	for(k=0;k<=SPRACC;k++)
	{
		a = k * 32*tau / SPRACC;
		c = getcos(a);
		s = getsin(a);
		for(j=0;j<3;j++)vbuf[6*k +j] = vc[j] +vr[j]*c +vf[j]*s +vt[j]*(2.0*k/SPRACC-1);

		if(k >= SPRACC)break;
		ibuf[2*k + 0] = vlen + k;
		ibuf[2*k + 1] = vlen + k+1;
	}
}
void gl41line_spring(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, SPRACC + 1, SPRACC);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(SPRACC+1);j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carveline_spring(vbuf,vlen, ibuf,0, vc,vr,vf,vt);
}




void carveline_yshape(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 v0, vec3 v1, vec3 v2)
{
	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];

	vbuf[ 6] = v1[0];
	vbuf[ 7] = v1[1];
	vbuf[ 8] = v1[2];

	vbuf[12] = v2[0];
	vbuf[13] = v2[1];
	vbuf[14] = v2[2];

	vbuf[18] = (v0[0]+v1[0]+v2[0])/3;
	vbuf[19] = (v0[1]+v1[1]+v2[1])/3;
	vbuf[20] = (v0[2]+v1[2]+v2[2])/3;

	ibuf[0] = vlen+0;
	ibuf[1] = vlen+3;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+3;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
}
void gl41line_yshape(struct entity* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 4, 3);
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
	carveline_yshape(vbuf,vlen, ibuf,0, v0,v1,v2);
}




void gl41line_triangle(struct entity* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 3, 3);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*3;j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvelineindex_triangle(vbuf,vlen, ibuf,0, v0,v1,v2);
}




void gl41line_rect(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 4, 4);
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




void carveline_rectround(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf)
{
	int j;
	for(j=0;j<3;j++){
		//lb
		vbuf[ 0+j] = vc[j] -vr[j]     -vf[j]*0.9;
		vbuf[ 6+j] = vc[j] -vr[j]*0.9 -vf[j];
		//rb
		vbuf[12+j] = vc[j] +vr[j]*0.9 -vf[j];
		vbuf[18+j] = vc[j] +vr[j]     -vf[j]*0.9;
		//rt
		vbuf[24+j] = vc[j] +vr[j]     +vf[j]*0.9;
		vbuf[30+j] = vc[j] +vr[j]*0.9 +vf[j];
		//lt
		vbuf[36+j] = vc[j] -vr[j]*0.9 +vf[j];
		vbuf[42+j] = vc[j] -vr[j]     +vf[j]*0.9;
	}

	ibuf[ 0] = vlen;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+1;
	ibuf[ 3] = vlen+2;
	ibuf[ 4] = vlen+2;
	ibuf[ 5] = vlen+3;
	ibuf[ 6] = vlen+3;
	ibuf[ 7] = vlen+4;
	ibuf[ 8] = vlen+4;
	ibuf[ 9] = vlen+5;
	ibuf[10] = vlen+5;
	ibuf[11] = vlen+6;
	ibuf[12] = vlen+6;
	ibuf[13] = vlen+7;
	ibuf[14] = vlen+7;
	ibuf[15] = vlen+0;
}
void gl41line_rectround(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 8, 8);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*8;j+=6){
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}
	carveline_rectround(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void carveline_rectselect(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf)
{
	//left bot
	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];

	vbuf[ 6] = vbuf[0] + vf[0]/2;
	vbuf[ 7] = vbuf[1] + vf[1]/2;
	vbuf[ 8] = vbuf[2] + vf[2]/2;

	vbuf[12] = vbuf[0] + vr[0]/2;
	vbuf[13] = vbuf[1] + vr[1]/2;
	vbuf[14] = vbuf[2] + vr[2]/2;

	//right bot
	vbuf[18] = vc[0] + vr[0] - vf[0];
	vbuf[19] = vc[1] + vr[1] - vf[1];
	vbuf[20] = vc[2] + vr[2] - vf[2];

	vbuf[24] = vbuf[18] + vf[0]/2;
	vbuf[25] = vbuf[19] + vf[1]/2;
	vbuf[26] = vbuf[20] + vf[2]/2;

	vbuf[30] = vbuf[18] - vr[0]/2;
	vbuf[31] = vbuf[19] - vr[1]/2;
	vbuf[32] = vbuf[20] - vr[2]/2;

	//left top
	vbuf[36] = vc[0] - vr[0] + vf[0];
	vbuf[37] = vc[1] - vr[1] + vf[1];
	vbuf[38] = vc[2] - vr[2] + vf[2];

	vbuf[42] = vbuf[36] - vf[0]/2;
	vbuf[43] = vbuf[37] - vf[1]/2;
	vbuf[44] = vbuf[38] - vf[2]/2;

	vbuf[48] = vbuf[36] + vr[0]/2;
	vbuf[49] = vbuf[37] + vr[1]/2;
	vbuf[50] = vbuf[38] + vr[2]/2;

	//right top
	vbuf[54] = vc[0] + vr[0] + vf[0];
	vbuf[55] = vc[1] + vr[1] + vf[1];
	vbuf[56] = vc[2] + vr[2] + vf[2];

	vbuf[60] = vbuf[54] - vf[0]/2;
	vbuf[61] = vbuf[55] - vf[1]/2;
	vbuf[62] = vbuf[56] - vf[2]/2;

	vbuf[66] = vbuf[54] - vr[0]/2;
	vbuf[67] = vbuf[55] - vr[1]/2;
	vbuf[68] = vbuf[56] - vr[2]/2;

	ibuf[ 0] = vlen;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen;
	ibuf[ 3] = vlen+2;

	ibuf[ 4] = vlen+3;
	ibuf[ 5] = vlen+4;
	ibuf[ 6] = vlen+3;
	ibuf[ 7] = vlen+5;

	ibuf[ 8] = vlen+6;
	ibuf[ 9] = vlen+7;
	ibuf[10] = vlen+6;
	ibuf[11] = vlen+8;

	ibuf[12] = vlen+ 9;
	ibuf[13] = vlen+10;
	ibuf[14] = vlen+ 9;
	ibuf[15] = vlen+11;
}
void gl41line_rectselect(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 12, 8);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*12;j+=6){
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}
	carveline_rectselect(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void carveline_hexagon(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vu)
{
	//0
	vbuf[ 0] = vc[0] + vr[0];
	vbuf[ 1] = vc[1] + vr[1];
	vbuf[ 2] = vc[2] + vr[2];

	//180
	vbuf[18] = vc[0] - vr[0];
	vbuf[19] = vc[1] - vr[1];
	vbuf[20] = vc[2] - vr[2];

	float v[4];
	v[0] = vr[0];
	v[1] = vr[1];
	v[2] = vr[2];
	quaternion_operation(v, vu, PI/3);

	//60
	vbuf[ 6] = vc[0] + v[0];
	vbuf[ 7] = vc[1] + v[1];
	vbuf[ 8] = vc[2] + v[2];

	//240
	vbuf[24] = vc[0] - v[0];
	vbuf[25] = vc[1] - v[1];
	vbuf[26] = vc[2] - v[2];

	v[0] -= vr[0];
	v[1] -= vr[1];
	v[2] -= vr[2];

	//120
	vbuf[12] = vc[0] + v[0];
	vbuf[13] = vc[1] + v[1];
	vbuf[14] = vc[2] + v[2];

	//300
	vbuf[30] = vc[0] - v[0];
	vbuf[31] = vc[1] - v[1];
	vbuf[32] = vc[2] - v[2];

	ibuf[0] = vlen;
	ibuf[1] = vlen+1;
	ibuf[2] = vlen+1;
	ibuf[3] = vlen+2;
	ibuf[4] = vlen+2;
	ibuf[5] = vlen+3;
	ibuf[6] = vlen+3;
	ibuf[7] = vlen+4;
	ibuf[8] = vlen+4;
	ibuf[9] = vlen+5;
	ibuf[10] = vlen+5;
	ibuf[11] = vlen+0;
}
void gl41line_hexagon(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 6, 6);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*6;j+=6){
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}
	carveline_hexagon(vbuf,vlen, ibuf,0, vc,vr,vu);
}




#define lineacc (acc*2)
void gl41line_circle(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, lineacc, lineacc);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*lineacc;j+=6){
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}
	carvelineindex_circle(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void carveline_pyramid3()
{
}
void carveline_pyramid4()
{
}
void carveline_pyramid5()
{
}
void carveline_pyramid6()
{
}




void carveline_cone(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vu)
{
	int a,b,j,k;
	float s,t;
	float r[4];

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		a = j*6;

		vbuf[a+0] = vc[0] + r[0];
		vbuf[a+1] = vc[1] + r[1];
		vbuf[a+2] = vc[2] + r[2];

		//bottom
		ibuf[a+0] = vlen+acc;
		ibuf[a+1] = vlen+j;

		//upper
		ibuf[a+2] = vlen+acc+1;
		ibuf[a+3] = vlen+j;

		//curve
		ibuf[a+4] = vlen+j;
		ibuf[a+5] = vlen+(j+1)%acc;
	}

	a = acc*6;

	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = vc[2];

	vbuf[a+3] = vc[0]+vu[0];
	vbuf[a+4] = vc[1]+vu[1];
	vbuf[a+5] = vc[2]+vu[2];
}
void gl41line_cone(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, acc + 2, acc * 3);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(acc+2);j+=6){
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}
	carveline_cone(vbuf,vlen, ibuf,0, vc,vr,vu);
}




void carveline_prism3()
{
}




void carveline_prism4(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vbuf[ 0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2] - vu[2];

	vbuf[ 6] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[ 7] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[ 8] = vc[2] + vr[2] - vf[2] - vu[2];

	vbuf[12] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[13] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[14] = vc[2] - vr[2] + vf[2] - vu[2];

	vbuf[18] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[19] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[20] = vc[2] + vr[2] + vf[2] - vu[2];

	vbuf[24] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[25] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[26] = vc[2] - vr[2] - vf[2] + vu[2];

	vbuf[30] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[31] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[32] = vc[2] + vr[2] - vf[2] + vu[2];

	vbuf[36] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[37] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[38] = vc[2] - vr[2] + vf[2] + vu[2];

	vbuf[42] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[43] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[44] = vc[2] + vr[2] + vf[2] + vu[2];

	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+0;
	ibuf[ 3] = vlen+2;
	ibuf[ 4] = vlen+3;
	ibuf[ 5] = vlen+1;
	ibuf[ 6] = vlen+3;
	ibuf[ 7] = vlen+2;

	ibuf[ 8] = vlen+0;
	ibuf[ 9] = vlen+4;
	ibuf[10] = vlen+1;
	ibuf[11] = vlen+5;
	ibuf[12] = vlen+2;
	ibuf[13] = vlen+6;
	ibuf[14] = vlen+3;
	ibuf[15] = vlen+7;

	ibuf[16] = vlen+4;
	ibuf[17] = vlen+5;
	ibuf[18] = vlen+4;
	ibuf[19] = vlen+6;
	ibuf[20] = vlen+7;
	ibuf[21] = vlen+5;
	ibuf[22] = vlen+7;
	ibuf[23] = vlen+6;
}
void gl41line_prism4(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 8, 12);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*8;j+=6){
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}
	carveline_prism4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




void carveline_prism5()
{
}
void carveline_prism6()
{
}
void carveline_cask()
{
}




void carveline_cylinder(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vu)
{
	int a,b,j,k;
	float s,t;
	float r[4];

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		a = j*12;
		b = j*6;
		vbuf[a+ 0] = vc[0] - vu[0] + r[0];
		vbuf[a+ 1] = vc[1] - vu[1] + r[1];
		vbuf[a+ 2] = vc[2] - vu[2] + r[2];

		vbuf[a+ 6] = vc[0] + vu[0] + r[0];
		vbuf[a+ 7] = vc[1] + vu[1] + r[1];
		vbuf[a+ 8] = vc[2] + vu[2] + r[2];

		ibuf[a+0] = vlen + j*2;
		ibuf[a+1] = vlen + ((j+1)%acc)*2;

		ibuf[a+2] = vlen + 1 + j*2;
		ibuf[a+3] = vlen + 1 + ((j+1)%acc)*2;

		ibuf[a+4] = vlen + j*2;
		ibuf[a+5] = vlen + 1 + j*2;
	}
}
void gl41line_cylinder(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, acc * 2, acc * 3);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(acc*2);j+=6){
		vbuf[j+3] = rr;
		vbuf[j+4] = gg;
		vbuf[j+5] = bb;
	}
	carveline_cylinder(vbuf,vlen,ibuf,0, vc,vr,vu);
}




void carveline_gear(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, int teethcount)
{
	int j,k;
	float a,c,s;
	vec3 tc,tr,tf;
	for(k=0;k<teethcount;k++){
		a = (k*4-1)*tau/(teethcount*4);
		c = getcos(a);
		s = getsin(a);
		for(j=0;j<3;j++){
			vbuf[k*6*4 +0 +j] = vc[j] +vr[j]*c*0.9 + vf[j]*s*0.9;
			vbuf[k*6*4 +6 +j] = vc[j] +vr[j]*c + vf[j]*s;
		}

		a = (k*4+1)*tau/(teethcount*4);
		c = getcos(a);
		s = getsin(a);
		for(j=0;j<3;j++){
			vbuf[k*6*4 +12 +j] = vc[j] +vr[j]*c + vf[j]*s;
			vbuf[k*6*4 +18 +j] = vc[j] +vr[j]*c*0.9 + vf[j]*s*0.9;
		}
	}

	for(j=0;j<teethcount*4;j++){
		ibuf[j*2 + 0] = vlen + j+0;
		ibuf[j*2 + 1] = vlen + (j+1)%(teethcount*4);
	}
}
void gl41line_gear(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, int teethcount)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, teethcount*4, teethcount*4);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(teethcount*4);j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carveline_gear(vbuf,vlen, ibuf,0, vc,vr,vf, teethcount);
}
void gl41line_rotategear(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, int teethcount, float a)
{
	int j;
	vec3 tr,tf;
	float c = getcos(a);
	float s = getsin(a);
	for(j=0;j<3;j++){
		tr[j] = vr[j]*c + vf[j]*s;
		tf[j] = vf[j]*c - vr[j]*s;
	}
	gl41line_gear(win, rgb, vc, tr, tf, teethcount);
}





void carveline_tetrahedron()
{
}
void carveline_octahedron()
{
}




void carveline_dodecahedron(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float a = 1.618;
	float b = 1.0/1.618;

	//(+-1, +-1, +-1)
	vbuf[  0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[  1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[  2] = vc[2]-vr[2]-vf[2]-vu[2];

	vbuf[  6] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[  7] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[  8] = vc[2]+vr[2]-vf[2]-vu[2];

	vbuf[ 12] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[ 13] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[ 14] = vc[2]-vr[2]+vf[2]-vu[2];

	vbuf[ 18] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[ 19] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[ 20] = vc[2]+vr[2]+vf[2]-vu[2];

	vbuf[ 24] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[ 25] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[ 26] = vc[2]-vr[2]-vf[2]+vu[2];

	vbuf[ 30] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[ 31] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[ 32] = vc[2]+vr[2]-vf[2]+vu[2];

	vbuf[ 36] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[ 37] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[ 38] = vc[2]-vr[2]+vf[2]+vu[2];

	vbuf[ 42] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[ 43] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[ 44] = vc[2]+vr[2]+vf[2]+vu[2];

	//(0, +-a, +-b)
	vbuf[ 48] = vc[0] - a*vf[0] - b*vu[0];
	vbuf[ 49] = vc[1] - a*vf[1] - b*vu[1];
	vbuf[ 50] = vc[2] - a*vf[2] - b*vu[2];

	vbuf[ 54] = vc[0] + a*vf[0] - b*vu[0];
	vbuf[ 55] = vc[1] + a*vf[1] - b*vu[1];
	vbuf[ 56] = vc[2] + a*vf[2] - b*vu[2];

	vbuf[ 60] = vc[0] - a*vf[0] + b*vu[0];
	vbuf[ 61] = vc[1] - a*vf[1] + b*vu[1];
	vbuf[ 62] = vc[2] - a*vf[2] + b*vu[2];

	vbuf[ 66] = vc[0] + a*vf[0] + b*vu[0];
	vbuf[ 67] = vc[1] + a*vf[1] + b*vu[1];
	vbuf[ 68] = vc[2] + a*vf[2] + b*vu[2];

	//(+-b, 0, +-a)
	vbuf[ 72] = vc[0] - b*vr[0] - a*vu[0];
	vbuf[ 73] = vc[1] - b*vr[1] - a*vu[1];
	vbuf[ 74] = vc[2] - b*vr[2] - a*vu[2];

	vbuf[ 78] = vc[0] + b*vr[0] - a*vu[0];
	vbuf[ 79] = vc[1] + b*vr[1] - a*vu[1];
	vbuf[ 80] = vc[2] + b*vr[2] - a*vu[2];

	vbuf[ 84] = vc[0] - b*vr[0] + a*vu[0];
	vbuf[ 85] = vc[1] - b*vr[1] + a*vu[1];
	vbuf[ 86] = vc[2] - b*vr[2] + a*vu[2];

	vbuf[ 90] = vc[0] + b*vr[0] + a*vu[0];
	vbuf[ 91] = vc[1] + b*vr[1] + a*vu[1];
	vbuf[ 92] = vc[2] + b*vr[2] + a*vu[2];

	//(+-a, +-b, 0)
	vbuf[ 96] = vc[0] - a*vr[0] - b*vf[0];
	vbuf[ 97] = vc[1] - a*vr[1] - b*vf[1];
	vbuf[ 98] = vc[2] - a*vr[2] - b*vf[2];

	vbuf[102] = vc[0] + a*vr[0] - b*vf[0];
	vbuf[103] = vc[1] + a*vr[1] - b*vf[1];
	vbuf[104] = vc[2] + a*vr[2] - b*vf[2];

	vbuf[108] = vc[0] - a*vr[0] + b*vf[0];
	vbuf[109] = vc[1] - a*vr[1] + b*vf[1];
	vbuf[110] = vc[2] - a*vr[2] + b*vf[2];

	vbuf[114] = vc[0] + a*vr[0] + b*vf[0];
	vbuf[115] = vc[1] + a*vr[1] + b*vf[1];
	vbuf[116] = vc[2] + a*vr[2] + b*vf[2];

	//front
	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+8;
	ibuf[ 2] = vlen+1;
	ibuf[ 3] = vlen+8;
	ibuf[ 4] = vlen+8;
	ibuf[ 5] = vlen+10;
	ibuf[ 6] = vlen+4;
	ibuf[ 7] = vlen+10;
	ibuf[ 8] = vlen+5;
	ibuf[ 9] = vlen+10;

	//back
	ibuf[10] = vlen+2;
	ibuf[11] = vlen+9;
	ibuf[12] = vlen+3;
	ibuf[13] = vlen+9;
	ibuf[14] = vlen+9;
	ibuf[15] = vlen+11;
	ibuf[16] = vlen+6;
	ibuf[17] = vlen+11;
	ibuf[18] = vlen+7;
	ibuf[19] = vlen+11;

	//bottom
	ibuf[20] = vlen+0;
	ibuf[21] = vlen+12;
	ibuf[22] = vlen+2;
	ibuf[23] = vlen+12;
	ibuf[24] = vlen+12;
	ibuf[25] = vlen+13;
	ibuf[26] = vlen+1;
	ibuf[27] = vlen+13;
	ibuf[28] = vlen+3;
	ibuf[29] = vlen+13;

	//upper
	ibuf[30] = vlen+4;
	ibuf[31] = vlen+14;
	ibuf[32] = vlen+6;
	ibuf[33] = vlen+14;
	ibuf[34] = vlen+14;
	ibuf[35] = vlen+15;
	ibuf[36] = vlen+5;
	ibuf[37] = vlen+15;
	ibuf[38] = vlen+7;
	ibuf[39] = vlen+15;

	//left
	ibuf[40] = vlen+0;
	ibuf[41] = vlen+16;
	ibuf[42] = vlen+4;
	ibuf[43] = vlen+16;
	ibuf[44] = vlen+16;
	ibuf[45] = vlen+18;
	ibuf[46] = vlen+2;
	ibuf[47] = vlen+18;
	ibuf[48] = vlen+6;
	ibuf[49] = vlen+18;

	//right
	ibuf[50] = vlen+1;
	ibuf[51] = vlen+17;
	ibuf[52] = vlen+5;
	ibuf[53] = vlen+17;
	ibuf[54] = vlen+17;
	ibuf[55] = vlen+19;
	ibuf[56] = vlen+3;
	ibuf[57] = vlen+19;
	ibuf[58] = vlen+7;
	ibuf[59] = vlen+19;
}
void gl41line_dodecahedron(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 20, 30);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*20;j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carveline_dodecahedron(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




void carveline_icosahedron(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	//(+-m, 0, +-n)
	vbuf[ 0] = vc[0] - m*vr[0] - n*vu[0];
	vbuf[ 1] = vc[1] - m*vr[1] - n*vu[1];
	vbuf[ 2] = vc[2] - m*vr[2] - n*vu[2];

	vbuf[ 6] = vc[0] + m*vr[0] - n*vu[0];
	vbuf[ 7] = vc[1] + m*vr[1] - n*vu[1];
	vbuf[ 8] = vc[2] + m*vr[2] - n*vu[2];

	vbuf[12] = vc[0] - m*vr[0] + n*vu[0];
	vbuf[13] = vc[1] - m*vr[1] + n*vu[1];
	vbuf[14] = vc[2] - m*vr[2] + n*vu[2];

	vbuf[18] = vc[0] + m*vr[0] + n*vu[0];
	vbuf[19] = vc[1] + m*vr[1] + n*vu[1];
	vbuf[20] = vc[2] + m*vr[2] + n*vu[2];

	//(0, +-n, +-m)
	vbuf[24] = vc[0] - n*vf[0] - m*vu[0];
	vbuf[25] = vc[1] - n*vf[1] - m*vu[1];
	vbuf[26] = vc[2] - n*vf[2] - m*vu[2];

	vbuf[30] = vc[0] + n*vf[0] - m*vu[0];
	vbuf[31] = vc[1] + n*vf[1] - m*vu[1];
	vbuf[32] = vc[2] + n*vf[2] - m*vu[2];

	vbuf[36] = vc[0] - n*vf[0] + m*vu[0];
	vbuf[37] = vc[1] - n*vf[1] + m*vu[1];
	vbuf[38] = vc[2] - n*vf[2] + m*vu[2];

	vbuf[42] = vc[0] + n*vf[0] + m*vu[0];
	vbuf[43] = vc[1] + n*vf[1] + m*vu[1];
	vbuf[44] = vc[2] + n*vf[2] + m*vu[2];

	//(+-n, +-m, 0)
	vbuf[48] = vc[0] - n*vr[0] - m*vf[0];
	vbuf[49] = vc[1] - n*vr[1] - m*vf[1];
	vbuf[50] = vc[2] - n*vr[2] - m*vf[2];

	vbuf[54] = vc[0] + n*vr[0] - m*vf[0];
	vbuf[55] = vc[1] + n*vr[1] - m*vf[1];
	vbuf[56] = vc[2] + n*vr[2] - m*vf[2];

	vbuf[60] = vc[0] - n*vr[0] + m*vf[0];
	vbuf[61] = vc[1] - n*vr[1] + m*vf[1];
	vbuf[62] = vc[2] - n*vr[2] + m*vf[2];

	vbuf[66] = vc[0] + n*vr[0] + m*vf[0];
	vbuf[67] = vc[1] + n*vr[1] + m*vf[1];
	vbuf[68] = vc[2] + n*vr[2] + m*vf[2];

	ibuf[ 0] = vlen+2;
	ibuf[ 1] = vlen+3;
	ibuf[ 2] = vlen+3;
	ibuf[ 3] = vlen+7;
	ibuf[ 4] = vlen+2;
	ibuf[ 5] = vlen+7;
	ibuf[ 6] = vlen+7;
	ibuf[ 7] = vlen+11;
	ibuf[ 8] = vlen+3;
	ibuf[ 9] = vlen+11;

	ibuf[10] = vlen+3;
	ibuf[11] = vlen+6;
	ibuf[12] = vlen+3;
	ibuf[13] = vlen+9;
	ibuf[14] = vlen+6;
	ibuf[15] = vlen+9;
	ibuf[16] = vlen+6;
	ibuf[17] = vlen+2;
	ibuf[18] = vlen+9;
	ibuf[19] = vlen+11;

	ibuf[20] = vlen+0;
	ibuf[21] = vlen+1;
	ibuf[22] = vlen+0;
	ibuf[23] = vlen+4;
	ibuf[24] = vlen+0;
	ibuf[25] = vlen+5;

	ibuf[26] = vlen+0;
	ibuf[27] = vlen+8;
	ibuf[28] = vlen+0;
	ibuf[29] = vlen+10;
	ibuf[30] = vlen+1;
	ibuf[31] = vlen+4;

	ibuf[32] = vlen+4;
	ibuf[33] = vlen+8;
	ibuf[34] = vlen+1;
	ibuf[35] = vlen+5;
	ibuf[36] = vlen+5;
	ibuf[37] = vlen+10;
	ibuf[38] = vlen+8;
	ibuf[39] = vlen+10;

	ibuf[40] = vlen+1;
	ibuf[41] = vlen+11;
	ibuf[42] = vlen+5;
	ibuf[43] = vlen+7;
	ibuf[44] = vlen+10;
	ibuf[45] = vlen+2;
	ibuf[46] = vlen+8;
	ibuf[47] = vlen+6;
	ibuf[48] = vlen+4;
	ibuf[49] = vlen+9;
	ibuf[50] = vlen+1;
	ibuf[51] = vlen+9;
	ibuf[52] = vlen+5;
	ibuf[53] = vlen+11;
	ibuf[54] = vlen+10;
	ibuf[55] = vlen+7;
	ibuf[56] = vlen+8;
	ibuf[57] = vlen+2;
	ibuf[58] = vlen+4;
	ibuf[59] = vlen+6;
}
void gl41line_icosahedron(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, 12, 30);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*12;j++){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carveline_icosahedron(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




#define accx (acc)
#define accy (acc|0x1)
void carveline_sphere(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j,k,a,b;
	float c,s;
	vec3 tc, tr, tf;

	for(k=0;k<accy;k++)
	{
		s = (2*k-accy+1)*PI/(2*accy+2);
		c = getcos(s);
		s = getsin(s);

		tc[0] = vc[0] + vu[0]*s;
		tc[1] = vc[1] + vu[1]*s;
		tc[2] = vc[2] + vu[2]*s;
		tr[0] = vr[0]*c;
		tr[1] = vr[1]*c;
		tr[2] = vr[2]*c;
		tf[0] = vf[0]*c;
		tf[1] = vf[1]*c;
		tf[2] = vf[2]*c;

		for(j=0;j<accx;j++)
		{
			s = j*tau/accx;
			c = getcos(s);
			s = getsin(s);

			a = (k*accx + j)*6;
			b = (k*accx + j)*2;

			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;

			ibuf[b+0] = vlen+(k*accx)+j;
			ibuf[b+1] = vlen+(k*accx)+(j+1)%accx;
		}
	}

	a = accx*accy*6;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];

	vbuf[a+ 6] = vc[0]+vu[0];
	vbuf[a+ 7] = vc[1]+vu[1];
	vbuf[a+ 8] = vc[2]+vu[2];

	for(k=0;k<accx;k++)
	{
		a = accy*accx*2 + (accy-1)*k*2;
		for(j=0;j<accy-1;j++)
		{
			ibuf[a + 2*j + 0] = vlen+k+(j*accx);
			ibuf[a + 2*j + 1] = vlen+k+(j*accx)+accx;
		}
	}

	a = accy*accx*2 + (accy-1)*accx*2;
	for(j=0;j<accx;j++)
	{
		ibuf[a + 4*j + 0] = vlen+accx*accy;
		ibuf[a + 4*j + 1] = vlen+j;
		ibuf[a + 4*j + 2] = vlen+accx*accy+1;
		ibuf[a + 4*j + 3] = vlen+accx*(accy-1)+j;
	}
}
void gl41line_sphere(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = line3d_vars(win, 0, &vbuf, &ibuf, accx*accy+2, accx*accy+accx*(accy+1));
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(accx*accy+2);j++){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carveline_sphere(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
