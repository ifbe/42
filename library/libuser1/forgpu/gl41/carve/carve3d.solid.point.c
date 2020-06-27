#include "libuser.h"
#define acc 18




static char solidpoint_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";

static char solidpoint_frag[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";




static int point3d_fill(struct glsrc* src)
{
	if(0 == src->vs){
		src->vs = solidpoint_vert;
		src->fs = solidpoint_frag;
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
int point3d_vars(struct entity* win, int unused, float** vbuf, int vcnt)
{
	if(0 == win)return -1;
	if(0 == win->gl_solid)return -2;

	struct glsrc* src = win->gl_solid[point3d];
	if(0 == src){
		src = win->gl_solid[point3d] = memorycreate(0x1000, 0);
		if(0 == src)return -3;
	}

	int vlen,ret;
	struct vertex* vtx = src->vtx;
	if(0 == vtx->vbuf){
		ret = point3d_fill(src);
		if(ret < 0)return -4;
	}

	vlen = vtx->vbuf_h;
	*vbuf = (vtx->vbuf) + (24*vlen);
	vtx->vbuf_h += vcnt;

	return vlen;
}




void gl41point(struct entity* win, u32 rgb, vec3 vc)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, 1);

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




void carvepoint_bezier(float* vbuf, int vlen,
	vec3 va, vec3 vb, vec3 vt)
{
	int j;
	float t;
	for(j=0;j<=acc;j++)
	{
		t = (float)j / acc;

		vbuf[6*j+0] = (1.0-t)*(1.0-t)*va[0] + 2*t*(1.0-t)*vt[0] + t*t*vb[0];
		vbuf[6*j+1] = (1.0-t)*(1.0-t)*va[1] + 2*t*(1.0-t)*vt[1] + t*t*vb[1];
		vbuf[6*j+2] = (1.0-t)*(1.0-t)*va[2] + 2*t*(1.0-t)*vt[2] + t*t*vb[2];
	}
}
void gl41point_bezier(struct entity* win, u32 rgb,
	vec3 va, vec3 vb, vec3 vt)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, acc);

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*acc;j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvepoint_bezier(vbuf,0, va,vb,vt);
}




void gl41point_triangle(struct entity* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
}
void gl41point_rect(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
}




void carvepoint_circle(float* vbuf, int vlen,
	vec3 vc, vec3 vr, vec3 vu)
{
	int j,k;
	float s,t;
	float r[4];

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		vbuf[6*j+0] = vc[0] + r[0];
		vbuf[6*j+1] = vc[1] + r[1];
		vbuf[6*j+2] = vc[2] + r[2];
	}
}
void gl41point_circle(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, acc);

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*acc;j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvepoint_circle(vbuf,0, vc,vr,vu);
}




void gl41point_pyramid3()
{
}
void gl41point_pyramid4()
{
}
void gl41point_pyramid5()
{
}
void gl41point_pyramid6()
{
}




void carvepoint_cone(float* vbuf, int vlen,
	vec3 vc, vec3 vr, vec3 vu)
{
	int j,k;
	float s,t;
	float r[4];

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);

		vbuf[6*j+0] = vc[0] + r[0];
		vbuf[6*j+1] = vc[1] + r[1];
		vbuf[6*j+2] = vc[2] + r[2];
	}

	j = acc*6;

	vbuf[j+ 0] = vc[0];
	vbuf[j+ 1] = vc[1];
	vbuf[j+ 2] = vc[2];

	vbuf[j+ 6] = vc[0]+vu[0];
	vbuf[j+ 7] = vc[1]+vu[1];
	vbuf[j+ 8] = vc[2]+vu[2];
}
void gl41point_cone(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, acc+2);

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(acc+2);j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvepoint_cone(vbuf,0, vc,vr,vu);
}




void gl41point_prism3()
{
}
void gl41point_prism4()
{
}
void gl41point_prism5()
{
}
void gl41point_prism6()
{
}
void gl41point_cask(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
}




void carvepoint_cylinder(float* vbuf, int vlen,
	vec3 vc, vec3 vr, vec3 vu)
{
	int j,k;
	float s,t;
	float q[4];
	float r[4];

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		q[0] = vu[0];
		q[1] = vu[1];
		q[2] = vu[2];
		quaternion_operation(r, q, j*tau/acc);

		k = j*12;

		vbuf[k+ 0] = vc[0] - vu[0] + r[0];
		vbuf[k+ 1] = vc[1] - vu[1] + r[1];
		vbuf[k+ 2] = vc[2] - vu[2] + r[2];

		vbuf[k+ 6] = vc[0] + vu[0] + r[0];
		vbuf[k+ 7] = vc[1] + vu[1] + r[1];
		vbuf[k+ 8] = vc[2] + vu[2] + r[2];
	}
}
void gl41point_cylinder(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, acc*2);

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(acc*2);j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvepoint_cylinder(vbuf,0, vc,vr,vu);
}




void gl41point_tetrahedron()
{
}
void gl41point_octahedron()
{
}




void carvepoint_dodecahedron(float* vbuf, int vlen,
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
}
void gl41point_dodecahedron(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, 20);

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*20;j+=6){
		vbuf[j + 3] = rr;
		vbuf[j + 4] = gg;
		vbuf[j + 5] = bb;
	}
	carvepoint_dodecahedron(vbuf,0, vc,vr,vf,vu);
}




void carvepoint_icosahedron(float* vbuf, int vlen,
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
}
void gl41point_icosahedron(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, 12);

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*12;j+=6){
		vbuf[j + 0] = rr;
		vbuf[j + 1] = gg;
		vbuf[j + 2] = bb;
	}
	carvepoint_icosahedron(vbuf,0, vc,vr,vf,vu);
}




#define accx (acc*2)
#define accy (acc*2+1)
void carvepoint_sphere(float* vbuf, int vlen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j,k,a;
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
			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;
		}
	}

	a = accx*accy*6;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];

	vbuf[a+ 6] = vc[0]+vu[0];
	vbuf[a+ 7] = vc[1]+vu[1];
	vbuf[a+ 8] = vc[2]+vu[2];
}
void gl41point_sphere(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	point3d_vars(win, 0, &vbuf, accx*accy+2);

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<6*(accx*accy+2);j+=6){
		vbuf[j + 0] = rr;
		vbuf[j + 1] = gg;
		vbuf[j + 2] = bb;
	}
	carvepoint_sphere(vbuf,0, vc,vr,vf,vu);
}
