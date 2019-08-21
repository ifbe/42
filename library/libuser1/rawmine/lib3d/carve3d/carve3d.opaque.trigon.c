#include "libuser.h"
#define acc 32




static char opaquetrigon_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 n;\n"
"layout(location = 2)in mediump vec4 c;\n"
"out mediump vec4 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

static char opaquetrigon_frag[] =
GLSL_VERSION
"in mediump vec4 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = colour;\n"
"}\n";




static int opaque3d_fill(struct glsrc* src)
{
	src->method = 'i';
	src->geometry = 3;

	if(0 == src->vs){
		src->vs = opaquetrigon_vert;
		src->fs = opaquetrigon_frag;
		src->shader_enq = 1;
	}

	if(0 == src->ibuf){
		src->ibuf_len = 0x100000;
		src->ibuf = memorycreate(src->ibuf_len);
		if(0 == src->ibuf)return -2;

		src->ibuf_w = 2*3;
		src->ibuf_h = 0;	//(src->ibuf_len) / (src->ibuf_w);
		src->ibuf_fmt = 0x222;
		src->ibuf_enq = 1;
	}

	if(0 == src->vbuf){
		src->vbuf_len = 0x1000000;
		src->vbuf = memorycreate(src->vbuf_len);
		if(0 == src->vbuf)return -1;

		src->vbuf_w = 4*4*3;
		src->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		src->vbuf_fmt = vbuffmt_444;
		src->vbuf_enq = 1;
	}

	return 0;
}
int opaque3d_vars(struct actor* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct datapair* mod = win->gl_opaque;
	struct glsrc* src = &mod[opaquetrigon3d].src;
	int vlen = src->vbuf_h;
	int ilen = src->ibuf_h;

	*vbuf = (src->vbuf) + (48*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carveopaque_triangle(struct actor* win, u32 rgba,
	vec3 v0, vec3 v1, vec3 v2)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 3, 1);

	vec3 n;
	n[0] = (v1[1]-v0[1])*(v2[2]-v0[2]) - (v1[2]-v0[2])*(v2[1]-v0[1]);
	n[1] = (v1[2]-v0[2])*(v2[0]-v0[0]) - (v1[0]-v0[0])*(v2[2]-v0[2]);
	n[2] = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0]);

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = 1.0;
	vbuf[ 4] = n[0];
	vbuf[ 5] = n[1];
	vbuf[ 6] = n[2];
	vbuf[ 7] = 1.0;
	vbuf[ 8] = rr;
	vbuf[ 9] = gg;
	vbuf[10] = bb;
	vbuf[11] = aa;

	vbuf[12] = v1[0];
	vbuf[13] = v1[1];
	vbuf[14] = v1[2];
	vbuf[15] = 1.0;
	vbuf[16] = n[0];
	vbuf[17] = n[1];
	vbuf[18] = n[2];
	vbuf[19] = 1.0;
	vbuf[20] = rr;
	vbuf[21] = gg;
	vbuf[22] = bb;
	vbuf[23] = aa;

	vbuf[24] = v2[0];
	vbuf[25] = v2[1];
	vbuf[26] = v2[2];
	vbuf[27] = 1.0;
	vbuf[28] = n[0];
	vbuf[29] = n[1];
	vbuf[30] = n[2];
	vbuf[31] = 1.0;
	vbuf[32] = rr;
	vbuf[33] = gg;
	vbuf[34] = bb;
	vbuf[35] = aa;

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void carveopaque_rect(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 4, 2);

	vec3 n;
	n[0] = vr[1]*vf[2] - vr[2]*vf[1];
	n[1] = vr[2]*vf[0] - vr[0]*vf[2];
	n[2] = vr[0]*vf[1] - vr[1]*vf[0];

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];
	vbuf[ 3] = 1.0;
	vbuf[ 4] = n[0];
	vbuf[ 5] = n[1];
	vbuf[ 6] = n[2];
	vbuf[ 7] = 1.0;
	vbuf[ 8] = rr;
	vbuf[ 9] = gg;
	vbuf[10] = bb;
	vbuf[11] = aa;

	vbuf[12] = vc[0] + vr[0] - vf[0];
	vbuf[13] = vc[1] + vr[1] - vf[1];
	vbuf[14] = vc[2] + vr[2] - vf[2];
	vbuf[15] = 1.0;
	vbuf[16] = n[0];
	vbuf[17] = n[1];
	vbuf[18] = n[2];
	vbuf[19] = 1.0;
	vbuf[20] = rr;
	vbuf[21] = gg;
	vbuf[22] = bb;
	vbuf[23] = aa;

	vbuf[24] = vc[0] - vr[0] + vf[0];
	vbuf[25] = vc[1] - vr[1] + vf[1];
	vbuf[26] = vc[2] - vr[2] + vf[2];
	vbuf[27] = 1.0;
	vbuf[28] = n[0];
	vbuf[29] = n[1];
	vbuf[30] = n[2];
	vbuf[31] = 1.0;
	vbuf[32] = rr;
	vbuf[33] = gg;
	vbuf[34] = bb;
	vbuf[35] = aa;

	vbuf[36] = vc[0] + vr[0] + vf[0];
	vbuf[37] = vc[1] + vr[1] + vf[1];
	vbuf[38] = vc[2] + vr[2] + vf[2];
	vbuf[39] = 1.0;
	vbuf[40] = n[0];
	vbuf[41] = n[1];
	vbuf[42] = n[2];
	vbuf[43] = 1.0;
	vbuf[44] = rr;
	vbuf[45] = gg;
	vbuf[46] = bb;
	vbuf[47] = aa;

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void carveopaque_circle(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

#define circieacc (acc*2)
	int a,b,j;
	float c,s;
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, circieacc+1, circieacc);

	vec3 n;
	n[0] = vr[1]*vf[2] - vr[2]*vf[1];
	n[1] = vr[2]*vf[0] - vr[0]*vf[2];
	n[2] = vr[0]*vf[1] - vr[1]*vf[0];
	for(j=0;j<circieacc;j++)
	{
		a = j*12;
		b = j*3;

		c = cosine(j*tau/circieacc);
		s = sine(j*tau/circieacc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2] + vr[2]*c + vf[2]*s;
		vbuf[a+3] = 1.0;

		vbuf[a+4] = n[0];
		vbuf[a+5] = n[1];
		vbuf[a+6] = n[2];
		vbuf[a+7] = 1.0;

		vbuf[a+ 8] = rr;
		vbuf[a+ 9] = gg;
		vbuf[a+10] = bb;
		vbuf[a+11] = aa;

		ibuf[b+0] = vlen + circieacc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%circieacc;
	}

	a = circieacc*12;
	vbuf[a+ 0] = vc[0];
	vbuf[a+ 1] = vc[1];
	vbuf[a+ 2] = vc[2];
	vbuf[a+ 3] = 1.0;
	vbuf[a+ 4] = n[0];
	vbuf[a+ 5] = n[1];
	vbuf[a+ 6] = n[2];
	vbuf[a+ 7] = 1.0;
	vbuf[a+ 8] = rr;
	vbuf[a+ 9] = gg;
	vbuf[a+10] = bb;
	vbuf[a+11] = aa;
}




void carveopaque_pyramid3()
{
}
void carveopaque_pyramid4()
{
}
void carveopaque_pyramid5()
{
}
void carveopaque_pyramid6()
{
}
void carveopaque_cone(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int a,b,j;
	float f;
	float r[4];

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, acc + 1, acc);

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);
		f = (r[0]*r[0] + r[1]*r[1] + r[2]*r[2]) / (vu[0]*vu[0] + vu[1]*vu[1] + vu[2]*vu[2]);

		a = j*12;
		vbuf[a+ 0] = vc[0] + r[0];
		vbuf[a+ 1] = vc[1] + r[1];
		vbuf[a+ 2] = vc[2] + r[2];
		vbuf[a+ 3] = 1.0;
		vbuf[a+ 4] = r[0] + vu[0]*f;
		vbuf[a+ 5] = r[1] + vu[1]*f;
		vbuf[a+ 6] = r[2] + vu[2]*f;
		vbuf[a+ 7] = 1.0;
		vbuf[a+ 8] = rr;
		vbuf[a+ 9] = gg;
		vbuf[a+10] = bb;
		vbuf[a+11] = aa;

		b = j*3;
		ibuf[b+0] = vlen+acc;
		ibuf[b+1] = vlen+j;
		ibuf[b+2] = vlen+(j+1)%acc;
	}

	a = acc*12;
	vbuf[a+ 0] = vc[0]+vu[0];
	vbuf[a+ 1] = vc[1]+vu[1];
	vbuf[a+ 2] = vc[2]+vu[2];
	vbuf[a+ 3] = 1.0;
	vbuf[a+ 4] = vu[0];
	vbuf[a+ 5] = vu[1];
	vbuf[a+ 6] = vu[2];
	vbuf[a+ 7] = 1.0;
	vbuf[a+ 8] = rr;
	vbuf[a+ 9] = gg;
	vbuf[a+10] = bb;
	vbuf[a+11] = aa;
}




void carveopaque_prism3()
{
}
void carveopaque_prism4(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int j;
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 24, 12);

	for(j=0;j<24*12;j+=12)
	{
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}

	vbuf[2*12+0] = vbuf[1*12+0] = vbuf[0*12+0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[2*12+1] = vbuf[1*12+1] = vbuf[0*12+1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[2*12+2] = vbuf[1*12+2] = vbuf[0*12+2] = vc[2] - vr[2] - vf[2] - vu[2];
	vbuf[0*12+4] = -vu[0];
	vbuf[0*12+5] = -vu[1];
	vbuf[0*12+6] = -vu[2];
	vbuf[1*12+4] = -vf[0];
	vbuf[1*12+5] = -vf[1];
	vbuf[1*12+6] = -vf[2];
	vbuf[2*12+4] = -vr[0];
	vbuf[2*12+5] = -vr[1];
	vbuf[2*12+6] = -vr[2];

	vbuf[5*12+0] = vbuf[4*12+0] = vbuf[3*12+0] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[5*12+1] = vbuf[4*12+1] = vbuf[3*12+1] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[5*12+2] = vbuf[4*12+2] = vbuf[3*12+2] = vc[2] + vr[2] - vf[2] - vu[2];
	vbuf[3*12+4] = -vu[0];
	vbuf[3*12+5] = -vu[1];
	vbuf[3*12+6] = -vu[2];
	vbuf[4*12+4] = -vf[0];
	vbuf[4*12+5] = -vf[1];
	vbuf[4*12+6] = -vf[2];
	vbuf[5*12+4] = vr[0];
	vbuf[5*12+5] = vr[1];
	vbuf[5*12+6] = vr[2];

	vbuf[8*12+0] = vbuf[7*12+0] = vbuf[6*12+0] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[8*12+1] = vbuf[7*12+1] = vbuf[6*12+1] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[8*12+2] = vbuf[7*12+2] = vbuf[6*12+2] = vc[2] - vr[2] + vf[2] - vu[2];
	vbuf[6*12+4] = -vu[0];
	vbuf[6*12+5] = -vu[1];
	vbuf[6*12+6] = -vu[2];
	vbuf[7*12+4] = vf[0];
	vbuf[7*12+5] = vf[1];
	vbuf[7*12+6] = vf[2];
	vbuf[8*12+4] = -vr[0];
	vbuf[8*12+5] = -vr[1];
	vbuf[8*12+6] = -vr[2];

	vbuf[11*12+0] = vbuf[10*12+0] = vbuf[9*12+0] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[11*12+1] = vbuf[10*12+1] = vbuf[9*12+1] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[11*12+2] = vbuf[10*12+2] = vbuf[9*12+2] = vc[2] + vr[2] + vf[2] - vu[2];
	vbuf[ 9*12+4] = -vu[0];
	vbuf[ 9*12+5] = -vu[1];
	vbuf[ 9*12+6] = -vu[2];
	vbuf[10*12+4] = vf[0];
	vbuf[10*12+5] = vf[1];
	vbuf[10*12+6] = vf[2];
	vbuf[11*12+4] = vr[0];
	vbuf[11*12+5] = vr[1];
	vbuf[11*12+6] = vr[2];

	vbuf[14*12+0] = vbuf[13*12+0] = vbuf[12*12+0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[14*12+1] = vbuf[13*12+1] = vbuf[12*12+1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[14*12+2] = vbuf[13*12+2] = vbuf[12*12+2] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[12*12+4] = vu[0];
	vbuf[12*12+5] = vu[1];
	vbuf[12*12+6] = vu[2];
	vbuf[13*12+4] = -vf[0];
	vbuf[13*12+5] = -vf[1];
	vbuf[13*12+6] = -vf[2];
	vbuf[14*12+4] = -vr[0];
	vbuf[14*12+5] = -vr[1];
	vbuf[14*12+6] = -vr[2];

	vbuf[17*12+0] = vbuf[16*12+0] = vbuf[15*12+0] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[17*12+1] = vbuf[16*12+1] = vbuf[15*12+1] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[17*12+2] = vbuf[16*12+2] = vbuf[15*12+2] = vc[2] + vr[2] - vf[2] + vu[2];
	vbuf[15*12+4] = vu[0];
	vbuf[15*12+5] = vu[1];
	vbuf[15*12+6] = vu[2];
	vbuf[16*12+4] = -vf[0];
	vbuf[16*12+5] = -vf[1];
	vbuf[16*12+6] = -vf[2];
	vbuf[17*12+4] = vr[0];
	vbuf[17*12+5] = vr[1];
	vbuf[17*12+6] = vr[2];

	vbuf[20*12+0] = vbuf[19*12+0] = vbuf[18*12+0] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[20*12+1] = vbuf[19*12+1] = vbuf[18*12+1] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[20*12+2] = vbuf[19*12+2] = vbuf[18*12+2] = vc[2] - vr[2] + vf[2] + vu[2];
	vbuf[18*12+4] = vu[0];
	vbuf[18*12+5] = vu[1];
	vbuf[18*12+6] = vu[2];
	vbuf[19*12+4] = vf[0];
	vbuf[19*12+5] = vf[1];
	vbuf[19*12+6] = vf[2];
	vbuf[20*12+4] = -vr[0];
	vbuf[20*12+5] = -vr[1];
	vbuf[20*12+6] = -vr[2];

	vbuf[23*12+0] = vbuf[22*12+0] = vbuf[21*12+0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[23*12+1] = vbuf[22*12+1] = vbuf[21*12+1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[23*12+2] = vbuf[22*12+2] = vbuf[21*12+2] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[21*12+4] = vu[0];
	vbuf[21*12+5] = vu[1];
	vbuf[21*12+6] = vu[2];
	vbuf[22*12+4] = vf[0];
	vbuf[22*12+5] = vf[1];
	vbuf[22*12+6] = vf[2];
	vbuf[23*12+4] = vr[0];
	vbuf[23*12+5] = vr[1];
	vbuf[23*12+6] = vr[2];

	//bottom
	ibuf[ 0] = vlen + 0 + 0;
	ibuf[ 1] = vlen + 0 + 3;
	ibuf[ 2] = vlen + 0 + 6;
	ibuf[ 3] = vlen + 0 + 3;
	ibuf[ 4] = vlen + 0 + 6;
	ibuf[ 5] = vlen + 0 + 9;

	//near
	ibuf[ 6] = vlen + 1 + 0;
	ibuf[ 7] = vlen + 1 + 3;
	ibuf[ 8] = vlen + 1 + 12;
	ibuf[ 9] = vlen + 1 + 3;
	ibuf[10] = vlen + 1 + 12;
	ibuf[11] = vlen + 1 + 15;

	//right
	ibuf[12] = vlen + 2 + 3;
	ibuf[13] = vlen + 2 + 9;
	ibuf[14] = vlen + 2 + 15;
	ibuf[15] = vlen + 2 + 9;
	ibuf[16] = vlen + 2 + 15;
	ibuf[17] = vlen + 2 + 21;

	//far
	ibuf[18] = vlen + 1 + 6;
	ibuf[19] = vlen + 1 + 9;
	ibuf[20] = vlen + 1 + 18;
	ibuf[21] = vlen + 1 + 9;
	ibuf[22] = vlen + 1 + 18;
	ibuf[23] = vlen + 1 + 21;

	//left
	ibuf[24] = vlen + 2 + 0;
	ibuf[25] = vlen + 2 + 6;
	ibuf[26] = vlen + 2 + 12;
	ibuf[27] = vlen + 2 + 6;
	ibuf[28] = vlen + 2 + 12;
	ibuf[29] = vlen + 2 + 18;

	//top
	ibuf[30] = vlen + 0 + 12;
	ibuf[31] = vlen + 0 + 15;
	ibuf[32] = vlen + 0 + 18;
	ibuf[33] = vlen + 0 + 15;
	ibuf[34] = vlen + 0 + 18;
	ibuf[35] = vlen + 0 + 21;
}
void carveopaque_prism5()
{
}
void carveopaque_prism6()
{
}
void carveopaque_cask(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int a,b,j;
	float c,s;
	vec3 vv;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, acc * 2, acc * 2);

	for(j=0;j<acc;j++)
	{
		a = j*12*2;
		b = j*6;
		c = cosine(j*tau/acc);
		s = sine(j*tau/acc);

		vv[0] = c*vr[0] - s*vf[0];
		vv[1] = c*vr[1] - s*vf[1];
		vv[2] = c*vr[2] - s*vf[2];

		vbuf[a+ 0] = vc[0] - vu[0] + vv[0];
		vbuf[a+ 1] = vc[1] - vu[1] + vv[1];
		vbuf[a+ 2] = vc[2] - vu[2] + vv[2];
		vbuf[a+ 3] = 1.0;
		vbuf[a+ 4] = vbuf[a+0] - vc[0];
		vbuf[a+ 5] = vbuf[a+1] - vc[1];
		vbuf[a+ 6] = vbuf[a+2] - vc[2];
		vbuf[a+ 7] = 1.0;
		vbuf[a+ 8] = rr;
		vbuf[a+ 9] = gg;
		vbuf[a+10] = bb;
		vbuf[a+11] = aa;

		vbuf[a+12] = vc[0] + vu[0] + vv[0];
		vbuf[a+13] = vc[1] + vu[1] + vv[1];
		vbuf[a+14] = vc[2] + vu[2] + vv[2];
		vbuf[a+15] = 1.0;
		vbuf[a+16] = vbuf[a+12] - vc[0];
		vbuf[a+17] = vbuf[a+13] - vc[1];
		vbuf[a+18] = vbuf[a+14] - vc[2];
		vbuf[a+19] = 1.0;
		vbuf[a+20] = rr;
		vbuf[a+21] = gg;
		vbuf[a+22] = bb;
		vbuf[a+23] = aa;

		ibuf[b+0] = vlen + j*2;
		ibuf[b+1] = vlen + ((j+1)%acc)*2;
		ibuf[b+2] = vlen + 1 + j*2;

		ibuf[b+3] = vlen + 1 + ((j+1)%acc)*2;
		ibuf[b+4] = vlen + ((j+1)%acc)*2;
		ibuf[b+5] = vlen + 1 + j*2;
	}
}
void carveopaque_cylinder(struct actor* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vec3 tc;
	vec3 tf;
	carveopaque_cask(win, rgb, vc, vr, vf, vu);

	tc[0] = vc[0]+vu[0];
	tc[1] = vc[1]+vu[1];
	tc[2] = vc[2]+vu[2];
	carveopaque_circle(win, rgb, tc, vr, vf);

	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	tf[0] = -vf[0];
	tf[1] = -vf[1];
	tf[2] = -vf[2];
	carveopaque_circle(win, rgb, tc, vr, tf);
}




void carveopaque_tetrahedron()
{
}
void carveopaque_octahedron()
{
}
void carveopaque_dodecahedron(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int j;
	float a = 1.618;
	float b = 1.0/1.618;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 20, 36);

	//(+-1, +-1, +-1)
	vbuf[0*12 + 0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[0*12 + 1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[0*12 + 2] = vc[2]-vr[2]-vf[2]-vu[2];

	vbuf[1*12 + 0] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[1*12 + 1] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[1*12 + 2] = vc[2]+vr[2]-vf[2]-vu[2];

	vbuf[2*12 + 0] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[2*12 + 1] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[2*12 + 2] = vc[2]-vr[2]+vf[2]-vu[2];

	vbuf[3*12 + 0] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[3*12 + 1] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[3*12 + 2] = vc[2]+vr[2]+vf[2]-vu[2];

	vbuf[4*12 + 0] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[4*12 + 1] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[4*12 + 2] = vc[2]-vr[2]-vf[2]+vu[2];

	vbuf[5*12 + 0] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[5*12 + 1] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[5*12 + 2] = vc[2]+vr[2]-vf[2]+vu[2];

	vbuf[6*12 + 0] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[6*12 + 1] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[6*12 + 2] = vc[2]-vr[2]+vf[2]+vu[2];

	vbuf[7*12 + 0] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[7*12 + 1] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[7*12 + 2] = vc[2]+vr[2]+vf[2]+vu[2];

	//(0, +-a, +-b)
	vbuf[8*12 + 0] = vc[0] - a*vf[0] - b*vu[0];
	vbuf[8*12 + 1] = vc[1] - a*vf[1] - b*vu[1];
	vbuf[8*12 + 2] = vc[2] - a*vf[2] - b*vu[2];

	vbuf[9*12 + 0] = vc[0] + a*vf[0] - b*vu[0];
	vbuf[9*12 + 1] = vc[1] + a*vf[1] - b*vu[1];
	vbuf[9*12 + 2] = vc[2] + a*vf[2] - b*vu[2];

	vbuf[10*12 + 0] = vc[0] - a*vf[0] + b*vu[0];
	vbuf[10*12 + 1] = vc[1] - a*vf[1] + b*vu[1];
	vbuf[10*12 + 2] = vc[2] - a*vf[2] + b*vu[2];

	vbuf[11*12 + 0] = vc[0] + a*vf[0] + b*vu[0];
	vbuf[11*12 + 1] = vc[1] + a*vf[1] + b*vu[1];
	vbuf[11*12 + 2] = vc[2] + a*vf[2] + b*vu[2];

	//(+-b, 0, +-a)
	vbuf[12*12 + 0] = vc[0] - b*vr[0] - a*vu[0];
	vbuf[12*12 + 1] = vc[1] - b*vr[1] - a*vu[1];
	vbuf[12*12 + 2] = vc[2] - b*vr[2] - a*vu[2];

	vbuf[13*12 + 0] = vc[0] + b*vr[0] - a*vu[0];
	vbuf[13*12 + 1] = vc[1] + b*vr[1] - a*vu[1];
	vbuf[13*12 + 2] = vc[2] + b*vr[2] - a*vu[2];

	vbuf[14*12 + 0] = vc[0] - b*vr[0] + a*vu[0];
	vbuf[14*12 + 1] = vc[1] - b*vr[1] + a*vu[1];
	vbuf[14*12 + 2] = vc[2] - b*vr[2] + a*vu[2];

	vbuf[15*12 + 0] = vc[0] + b*vr[0] + a*vu[0];
	vbuf[15*12 + 1] = vc[1] + b*vr[1] + a*vu[1];
	vbuf[15*12 + 2] = vc[2] + b*vr[2] + a*vu[2];

	//(+-a, +-b, 0)
	vbuf[16*12 + 0] = vc[0] - a*vr[0] - b*vf[0];
	vbuf[16*12 + 1] = vc[1] - a*vr[1] - b*vf[1];
	vbuf[16*12 + 2] = vc[2] - a*vr[2] - b*vf[2];

	vbuf[17*12 + 0] = vc[0] + a*vr[0] - b*vf[0];
	vbuf[17*12 + 1] = vc[1] + a*vr[1] - b*vf[1];
	vbuf[17*12 + 2] = vc[2] + a*vr[2] - b*vf[2];

	vbuf[18*12 + 0] = vc[0] - a*vr[0] + b*vf[0];
	vbuf[18*12 + 1] = vc[1] - a*vr[1] + b*vf[1];
	vbuf[18*12 + 2] = vc[2] - a*vr[2] + b*vf[2];

	vbuf[19*12 + 0] = vc[0] + a*vr[0] + b*vf[0];
	vbuf[19*12 + 1] = vc[1] + a*vr[1] + b*vf[1];
	vbuf[19*12 + 2] = vc[2] + a*vr[2] + b*vf[2];

	for(j=0;j<20*12;j+=12)
	{
		vbuf[j+ 4] = vbuf[j + 0] - vc[0];
		vbuf[j+ 5] = vbuf[j + 1] - vc[1];
		vbuf[j+ 6] = vbuf[j + 2] - vc[2];
		vbuf[j+ 7] = 1.0;
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}

	//front
	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+8;
	ibuf[ 3] = vlen+0;
	ibuf[ 4] = vlen+1;
	ibuf[ 5] = vlen+12;
	ibuf[ 6] = vlen+12;
	ibuf[ 7] = vlen+13;
	ibuf[ 8] = vlen+1;

	ibuf[ 9] = vlen+4;
	ibuf[10] = vlen+5;
	ibuf[11] = vlen+10;
	ibuf[12] = vlen+4;
	ibuf[13] = vlen+5;
	ibuf[14] = vlen+14;
	ibuf[15] = vlen+14;
	ibuf[16] = vlen+15;
	ibuf[17] = vlen+5;

	//back
	ibuf[18] = vlen+2;
	ibuf[19] = vlen+3;
	ibuf[20] = vlen+9;
	ibuf[21] = vlen+2;
	ibuf[22] = vlen+3;
	ibuf[23] = vlen+12;
	ibuf[24] = vlen+12;
	ibuf[25] = vlen+13;
	ibuf[26] = vlen+3;

	ibuf[27] = vlen+6;
	ibuf[28] = vlen+7;
	ibuf[29] = vlen+11;
	ibuf[30] = vlen+6;
	ibuf[31] = vlen+7;
	ibuf[32] = vlen+14;
	ibuf[33] = vlen+14;
	ibuf[34] = vlen+15;
	ibuf[35] = vlen+7;

	//bottom
	ibuf[36] = vlen+0;
	ibuf[37] = vlen+2;
	ibuf[38] = vlen+12;
	ibuf[39] = vlen+0;
	ibuf[40] = vlen+2;
	ibuf[41] = vlen+16;
	ibuf[42] = vlen+16;
	ibuf[43] = vlen+18;
	ibuf[44] = vlen+2;

	ibuf[45] = vlen+1;
	ibuf[46] = vlen+3;
	ibuf[47] = vlen+13;
	ibuf[48] = vlen+1;
	ibuf[49] = vlen+3;
	ibuf[50] = vlen+17;
	ibuf[51] = vlen+17;
	ibuf[52] = vlen+19;
	ibuf[53] = vlen+3;

	//upper
	ibuf[54] = vlen+4;
	ibuf[55] = vlen+6;
	ibuf[56] = vlen+14;
	ibuf[57] = vlen+4;
	ibuf[58] = vlen+6;
	ibuf[59] = vlen+16;
	ibuf[60] = vlen+16;
	ibuf[61] = vlen+18;
	ibuf[62] = vlen+6;

	ibuf[63] = vlen+5;
	ibuf[64] = vlen+7;
	ibuf[65] = vlen+15;
	ibuf[66] = vlen+5;
	ibuf[67] = vlen+7;
	ibuf[68] = vlen+17;
	ibuf[69] = vlen+17;
	ibuf[70] = vlen+19;
	ibuf[71] = vlen+7;

	//left
	ibuf[72] = vlen+0;
	ibuf[73] = vlen+4;
	ibuf[74] = vlen+16;
	ibuf[75] = vlen+0;
	ibuf[76] = vlen+4;
	ibuf[77] = vlen+8;
	ibuf[78] = vlen+8;
	ibuf[79] = vlen+10;
	ibuf[80] = vlen+4;

	ibuf[81] = vlen+2;
	ibuf[82] = vlen+6;
	ibuf[83] = vlen+18;
	ibuf[84] = vlen+2;
	ibuf[85] = vlen+6;
	ibuf[86] = vlen+9;
	ibuf[87] = vlen+9;
	ibuf[88] = vlen+11;
	ibuf[89] = vlen+6;

	//right
	ibuf[90] = vlen+1;
	ibuf[91] = vlen+5;
	ibuf[92] = vlen+17;
	ibuf[93] = vlen+1;
	ibuf[94] = vlen+5;
	ibuf[95] = vlen+8;
	ibuf[96] = vlen+8;
	ibuf[97] = vlen+10;
	ibuf[98] = vlen+5;

	ibuf[99] = vlen+3;
	ibuf[100] = vlen+7;
	ibuf[101] = vlen+19;
	ibuf[102] = vlen+3;
	ibuf[103] = vlen+7;
	ibuf[104] = vlen+9;
	ibuf[105] = vlen+9;
	ibuf[106] = vlen+11;
	ibuf[107] = vlen+7;
}
void carveopaque_icosahedron(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 12, 20);

	//(+-m, 0, +-n)
	vbuf[0*12 + 0] = vc[0] - m*vr[0] - n*vu[0];
	vbuf[0*12 + 1] = vc[1] - m*vr[1] - n*vu[1];
	vbuf[0*12 + 2] = vc[2] - m*vr[2] - n*vu[2];

	vbuf[1*12 + 0] = vc[0] + m*vr[0] - n*vu[0];
	vbuf[1*12 + 1] = vc[1] + m*vr[1] - n*vu[1];
	vbuf[1*12 + 2] = vc[2] + m*vr[2] - n*vu[2];

	vbuf[2*12 + 0] = vc[0] - m*vr[0] + n*vu[0];
	vbuf[2*12 + 1] = vc[1] - m*vr[1] + n*vu[1];
	vbuf[2*12 + 2] = vc[2] - m*vr[2] + n*vu[2];

	vbuf[3*12 + 0] = vc[0] + m*vr[0] + n*vu[0];
	vbuf[3*12 + 1] = vc[1] + m*vr[1] + n*vu[1];
	vbuf[3*12 + 2] = vc[2] + m*vr[2] + n*vu[2];

	//(0, +-n, +-m)
	vbuf[4*12 + 0] = vc[0] - n*vf[0] - m*vu[0];
	vbuf[4*12 + 1] = vc[1] - n*vf[1] - m*vu[1];
	vbuf[4*12 + 2] = vc[2] - n*vf[2] - m*vu[2];

	vbuf[5*12 + 0] = vc[0] + n*vf[0] - m*vu[0];
	vbuf[5*12 + 1] = vc[1] + n*vf[1] - m*vu[1];
	vbuf[5*12 + 2] = vc[2] + n*vf[2] - m*vu[2];

	vbuf[6*12 + 0] = vc[0] - n*vf[0] + m*vu[0];
	vbuf[6*12 + 1] = vc[1] - n*vf[1] + m*vu[1];
	vbuf[6*12 + 2] = vc[2] - n*vf[2] + m*vu[2];

	vbuf[7*12 + 0] = vc[0] + n*vf[0] + m*vu[0];
	vbuf[7*12 + 1] = vc[1] + n*vf[1] + m*vu[1];
	vbuf[7*12 + 2] = vc[2] + n*vf[2] + m*vu[2];

	//(+-n, +-m, 0)
	vbuf[8*12 + 0] = vc[0] - n*vr[0] - m*vf[0];
	vbuf[8*12 + 1] = vc[1] - n*vr[1] - m*vf[1];
	vbuf[8*12 + 2] = vc[2] - n*vr[2] - m*vf[2];

	vbuf[9*12 + 0] = vc[0] + n*vr[0] - m*vf[0];
	vbuf[9*12 + 1] = vc[1] + n*vr[1] - m*vf[1];
	vbuf[9*12 + 2] = vc[2] + n*vr[2] - m*vf[2];

	vbuf[10*12 + 0] = vc[0] - n*vr[0] + m*vf[0];
	vbuf[10*12 + 1] = vc[1] - n*vr[1] + m*vf[1];
	vbuf[10*12 + 2] = vc[2] - n*vr[2] + m*vf[2];

	vbuf[11*12 + 0] = vc[0] + n*vr[0] + m*vf[0];
	vbuf[11*12 + 1] = vc[1] + n*vr[1] + m*vf[1];
	vbuf[11*12 + 2] = vc[2] + n*vr[2] + m*vf[2];

	for(j=0;j<12*12;j+=12)
	{
		vbuf[j+ 4] = vbuf[j + 0] - vc[0];
		vbuf[j+ 5] = vbuf[j + 1] - vc[1];
		vbuf[j+ 6] = vbuf[j + 2] - vc[2];
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}

	ibuf[ 0] = vlen+0;
	ibuf[ 1] = vlen+1;
	ibuf[ 2] = vlen+5;

	ibuf[ 3] = vlen+0;
	ibuf[ 4] = vlen+5;
	ibuf[ 5] = vlen+10;

	ibuf[ 6] = vlen+0;
	ibuf[ 7] = vlen+10;
	ibuf[ 8] = vlen+8;

	ibuf[ 9] = vlen+0;
	ibuf[10] = vlen+8;
	ibuf[11] = vlen+4;

	ibuf[12] = vlen+0;
	ibuf[13] = vlen+1;
	ibuf[14] = vlen+4;

	ibuf[15] = vlen+3;
	ibuf[16] = vlen+2;
	ibuf[17] = vlen+6;

	ibuf[18] = vlen+3;
	ibuf[19] = vlen+6;
	ibuf[20] = vlen+9;

	ibuf[21] = vlen+3;
	ibuf[22] = vlen+9;
	ibuf[23] = vlen+11;

	ibuf[24] = vlen+3;
	ibuf[25] = vlen+11;
	ibuf[26] = vlen+7;

	ibuf[27] = vlen+3;
	ibuf[28] = vlen+2;
	ibuf[29] = vlen+7;

	ibuf[30] = vlen+1;
	ibuf[31] = vlen+11;
	ibuf[32] = vlen+9;

	ibuf[33] = vlen+5;
	ibuf[34] = vlen+7;
	ibuf[35] = vlen+11;

	ibuf[36] = vlen+10;
	ibuf[37] = vlen+2;
	ibuf[38] = vlen+7;

	ibuf[39] = vlen+8;
	ibuf[40] = vlen+6;
	ibuf[41] = vlen+2;

	ibuf[42] = vlen+4;
	ibuf[43] = vlen+9;
	ibuf[44] = vlen+6;

	ibuf[45] = vlen+1;
	ibuf[46] = vlen+11;
	ibuf[47] = vlen+5;

	ibuf[48] = vlen+5;
	ibuf[49] = vlen+7;
	ibuf[50] = vlen+10;

	ibuf[51] = vlen+10;
	ibuf[52] = vlen+2;
	ibuf[53] = vlen+8;

	ibuf[54] = vlen+8;
	ibuf[55] = vlen+6;
	ibuf[56] = vlen+4;

	ibuf[57] = vlen+4;
	ibuf[58] = vlen+9;
	ibuf[59] = vlen+1;
}
void carveopaque_sphere(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

#define accx (acc)
#define accy (acc|0x1)
	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, accx*accy+2, accx*accy*2);

	for(k=0;k<accy;k++)
	{
		s = (2*k-accy+1)*PI/(2*accy+2);
		c = cosine(s);
		s = sine(s);

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
			c = cosine(s);
			s = sine(s);

			a = (k*accx + j)*12;
			vbuf[a+ 0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+ 1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+ 2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a+ 4] = vbuf[a+0] - vc[0];
			vbuf[a+ 5] = vbuf[a+1] - vc[1];
			vbuf[a+ 6] = vbuf[a+2] - vc[2];
			vbuf[a+ 8] = rr;
			vbuf[a+ 9] = gg;
			vbuf[a+10] = bb;
			vbuf[a+11] = aa;

			if(k >= accy-1)continue;
			b = k*accx*6;
			ibuf[b + 6*j + 0] = vlen+(k*accx)+j;
			ibuf[b + 6*j + 1] = vlen+(k*accx)+(j+1)%accx;
			ibuf[b + 6*j + 2] = vlen+(k*accx)+accx+j;
			ibuf[b + 6*j + 3] = vlen+(k*accx)+(j+1)%accx;
			ibuf[b + 6*j + 4] = vlen+(k*accx)+accx+j;
			ibuf[b + 6*j + 5] = vlen+(k*accx)+accx+(j+1)%accx;
		}
	}

	a = accx*accy*12;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = 1.0;
	vbuf[a+ 4] = -vu[0];
	vbuf[a+ 5] = -vu[1];
	vbuf[a+ 6] = -vu[2];
	vbuf[a+ 7] = 1.0;
	vbuf[a+ 8] = rr;
	vbuf[a+ 9] = gg;
	vbuf[a+10] = bb;
	vbuf[a+11] = aa;

	vbuf[a+12] = vc[0]+vu[0];
	vbuf[a+13] = vc[1]+vu[1];
	vbuf[a+14] = vc[2]+vu[2];
	vbuf[a+15] = 1.0;
	vbuf[a+16] = vu[0];
	vbuf[a+17] = vu[1];
	vbuf[a+18] = vu[2];
	vbuf[a+19] = 1.0;
	vbuf[a+20] = rr;
	vbuf[a+21] = gg;
	vbuf[a+22] = bb;
	vbuf[a+23] = aa;

	b = (accy-1)*accx*6;
	for(j=0;j<accx;j++)
	{
		ibuf[b + (6*j) + 0] = vlen+accx*accy;
		ibuf[b + (6*j) + 1] = vlen+j;
		ibuf[b + (6*j) + 2] = vlen+(j+1)%accx;
		ibuf[b + (6*j) + 3] = vlen+accx*accy+1;
		ibuf[b + (6*j) + 4] = vlen+accx*(accy-1)+j;
		ibuf[b + (6*j) + 5] = vlen+accx*(accy-1)+(j+1)%accx;
	}
}
void carveopaque_tokamak(struct actor* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vu)
{
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;

	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, acc*acc*2, acc*acc);
}
