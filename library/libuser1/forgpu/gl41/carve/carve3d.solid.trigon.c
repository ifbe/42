#include "libuser.h"
#define acc 16




static char solidtrigon_vert[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 n;\n"
"layout(location = 2)in mediump vec3 c;\n"
"out mediump vec3 objxyz;\n"
"out mediump vec3 normal;\n"
"out mediump vec3 albedo;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"objxyz = v;\n"
	"normal = n;\n"
	"albedo = pow(c, vec3(2.2));\n"
	"gl_Position = cammvp * vec4(objxyz,1.0);\n"
"}\n";

static char solidtrigon_frag[] =
GLSL_VERSION
"in mediump vec3 objxyz;\n"
"in mediump vec3 normal;\n"
"in mediump vec3 albedo;\n"
//"in mediump vec3 mtrfao;\n"
"out mediump vec4 FragColor;\n"
"uniform mediump vec3 camxyz;\n"
"mediump vec3 mtrfao = vec3(0.0, 0.5, 1.0);\n"
"mediump float metal = mtrfao.x;\n"
"mediump float rough = mtrfao.y;\n"
"mediump float amocc = mtrfao.z;\n"
"mediump float PI = 3.1415926535897932384626433832795028841971693993151;\n"
"mediump float getD(mediump float v, mediump float r){\n"
    "float a2 = r*r*r*r;\n"
    "float de = (v*v * (a2 - 1.0) + 1.0);\n"
    "return a2 / (PI * de * de);\n"
"}\n"
"mediump float getG(mediump float v, mediump float r){\n"
    "float k = (r+1.0) * (r+1.0) / 8.0;\n"
    "return v / (v * (1.0 - k) + k);\n"
"}\n"
"void main(){\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 E = normalize(camxyz - objxyz);\n"
	"mediump vec3 F0 = mix(vec3(0.04), albedo, metal);\n"

	"mediump vec3 litrgb = vec3(1.0, 1.0, 1.0);\n"
	"mediump vec3 litdir[4];\n"
	"litdir[0] = vec3(-1.0, 0.0, 1.0);\n"
	"litdir[1] = vec3( 1.0, 0.0, 1.0);\n"
	"litdir[2] = vec3( 0.0,-1.0, 1.0);\n"
	"litdir[3] = vec3( 0.0, 1.0, 1.0);\n"

	"mediump vec3 ocolor = vec3(0.0);\n"
	"for(int j=0;j<4;j++){\n"
		"mediump vec3 L = litdir[j];\n"
		//"mediump float distance = length(L);\n"
		//"mediump float attenuation = 1.0 / (distance * distance);\n"
		//"mediump vec3 radiance = litrgb * attenuation;\n"
		"mediump vec3 radiance = litrgb;\n"

		"L = normalize(L);\n"
		"mediump vec3 H = normalize(E + L);\n"
		"mediump float NdotL = max(dot(N, L), 0.0);\n"
		"mediump float NdotE = max(dot(N, E), 0.0);\n"
		"mediump float NdotH = max(dot(N, H), 0.0);\n"
		"mediump float HdotE = max(dot(H, E), 0.0);\n"

		"mediump float G = getG(NdotL, rough)*getG(NdotE, rough);\n"
		"mediump float D = getD(NdotH, rough);\n"
		"mediump vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);\n"

		"mediump vec3 kS = F;\n"
		"mediump vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);\n"
		"mediump vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);\n"
		"ocolor += (kD * albedo / PI + specular) * radiance * NdotL;\n"
	"}\n"

	"ocolor += vec3(0.03) * albedo * amocc;\n"
	"ocolor = ocolor / (ocolor + vec3(1.0));\n"
	"ocolor = pow(ocolor, vec3(1.0/2.2));\n"
	"FragColor = vec4(ocolor, 1.0);\n"
"}\n";


/*
//"layout(location = 0)out mediump vec4 outxyz\n;"
//"layout(location = 1)out mediump vec4 outuvw\n;"

"mediump vec3 dirsun0 = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 dirsun1 = vec3(-1.0, 0.0, 0.0);\n"

//"mediump vec3 LA = vec3(1.0, 1.0, 1.0);\n"
"mediump vec3 L0 = vec3(0.5, 0.5, 0.5);\n"
"mediump vec3 L1 = vec3(0.5, 0.5, 0.5);\n"
//"mediump vec3 KA = vec3(0.231250, 0.231250, 0.231250);\n"
"mediump vec3 KD = vec3(0.277500, 0.277500, 0.277500);\n"
"mediump vec3 KS = vec3(0.773911, 0.773911, 0.773911);\n"

"vec3 sun0(){\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun0);\n"
	"mediump float NL = dot(N, L);\n"
	//"mediump float NL = clamp(dot(N, L), -1.0, 1.0);\n"
	"mediump vec3 ret = L0*KD*max(NL, 0.0);\n"
	"if(NL < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = clamp(dot(N, H), 0.0, 1.0);\n"
	"return ret + L0*KS*pow(NH, 89.6);\n"
"}\n"
"vec3 sun1(){\n"
	"mediump vec3 N = normalize(normal);\n"
	"mediump vec3 L = normalize(dirsun1);\n"
	"mediump float NL = dot(N, L);\n"
	//"mediump float NL = clamp(dot(N, L), -1.0, 1.0);\n"
	"mediump vec3 ret = L1*KD*max(NL, 0.0);\n"
	"if(NL < 0.0)return ret;\n"

	"mediump vec3 E = normalize(camxyz - vertex);\n"
	"mediump vec3 H = normalize(E + L);\n"
	"mediump float NH = clamp(dot(N, H), 0.0, 1.0);\n"
	"return ret + L1*KS*pow(NH, 89.6);\n"
"}\n"
//"float shadow(){\n"
	//"if(uvw.z - texture(tex0, uvw.xy).r > 0.000001)return 0.1;\n"
	//"return 1.0;\n"
//"}\n"
"void main(){\n"
	"mediump vec3 c = colour;\n"
	"c += sun0() / 2.0;\n"
	"c += sun1() / 2.0;\n"
	"c = vec3(clamp(c.x, 0.0, 1.0), clamp(c.y, 0.0, 1.0), clamp(c.z, 0.0, 1.0));\n"
	"FragColor = vec4(c, 1.0);\n"
	//"outxyz = vec4(vertex, 1.0);\n"
	//"outuvw = vec4(normal, 1.0);\n"
"}\n";*/




static int trigon3d_fill(struct glsrc* src)
{
	src->method = 'i';
	src->geometry = 3;

	if(0 == src->vs){
		src->vs = solidtrigon_vert;
		src->fs = solidtrigon_frag;
		src->shader_enq = 1;
	}

	if(0 == src->ibuf){
		src->ibuf_len = 0x100000;
		src->ibuf = memorycreate(src->ibuf_len, 0);
		if(0 == src->ibuf)return -2;

		src->ibuf_w = 2*3;
		src->ibuf_h = 0;	//(src->ibuf_len) / (src->ibuf_w);
		src->ibuf_fmt = 0x222;
		src->ibuf_enq = 1;
	}

	if(0 == src->vbuf){
		src->vbuf_len = 65536*4*9;		//65535*4*9
		src->vbuf = memorycreate(src->vbuf_len, 0);
		if(0 == src->vbuf)return -1;

		src->vbuf_w = 4*3*3;
		src->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		src->vbuf_fmt = vbuffmt_333;
		src->vbuf_enq = 1;
	}

	return 0;
}
int trigon3d_vars(struct entity* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	struct glsrc* src;
	int vlen,ilen,ret;
	if(0 == win)return -1;
	if(0 == win->gl_solid)return -2;

	src = win->gl_solid[trigon3d];
	if(0 == src){
		src = win->gl_solid[trigon3d] = memorycreate(0x200, 0);
		if(0 == src)return -3;
	}
	if(0 == src->vbuf){
		ret = trigon3d_fill(src);
		if(ret < 0)return -4;
	}
/*
	struct datapair* mod;
	struct glsrc* src;
	int vlen,ilen,ret;
	if(0 == win)return -1;

	mod = win->gl_solid;
	if(0 == mod)return -2;

	src = &mod[trigon3d].src;
	if(0 == src->vbuf){
		ret = trigon3d_fill(src);
		if(ret < 0)return -3;
	}
*/
	vlen = src->vbuf_h;
	ilen = src->ibuf_h;
	*vbuf = (src->vbuf) + (36*vlen);
	*ibuf = (src->ibuf) + (6*ilen);
	src->vbuf_h += vcnt;
	src->ibuf_h += icnt;

	return vlen;
}




void carvesolid_triangle(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 v0, vec3 v1, vec3 v2)
{
	vec3 n;
	n[0] = (v1[1]-v0[1])*(v2[2]-v0[2]) - (v1[2]-v0[2])*(v2[1]-v0[1]);
	n[1] = (v1[2]-v0[2])*(v2[0]-v0[0]) - (v1[0]-v0[0])*(v2[2]-v0[2]);
	n[2] = (v1[0]-v0[0])*(v2[1]-v0[1]) - (v1[1]-v0[1])*(v2[0]-v0[0]);
	vec3_setlen(n, 1.0);

	vbuf[ 0] = v0[0];
	vbuf[ 1] = v0[1];
	vbuf[ 2] = v0[2];
	vbuf[ 3] = n[0];
	vbuf[ 4] = n[1];
	vbuf[ 5] = n[2];

	vbuf[ 9] = v1[0];
	vbuf[10] = v1[1];
	vbuf[11] = v1[2];
	vbuf[12] = n[0];
	vbuf[13] = n[1];
	vbuf[14] = n[2];

	vbuf[18] = v2[0];
	vbuf[19] = v2[1];
	vbuf[20] = v2[2];
	vbuf[21] = n[0];
	vbuf[22] = n[1];
	vbuf[23] = n[2];

	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;
}
void gl41solid_triangle(struct entity* win, u32 rgb,
	vec3 v0, vec3 v1, vec3 v2)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, 3, 1);
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
	carvesolid_triangle(vbuf,vlen, ibuf,0, v0,v1,v2);
}




void carvesolid_rect(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf)
{
	vec3 n;
	n[0] = vr[1]*vf[2] - vr[2]*vf[1];
	n[1] = vr[2]*vf[0] - vr[0]*vf[2];
	n[2] = vr[0]*vf[1] - vr[1]*vf[0];
	vec3_setlen(n, 1.0);

	vbuf[ 0] = vc[0] - vr[0] - vf[0];
	vbuf[ 1] = vc[1] - vr[1] - vf[1];
	vbuf[ 2] = vc[2] - vr[2] - vf[2];
	vbuf[ 3] = n[0];
	vbuf[ 4] = n[1];
	vbuf[ 5] = n[2];

	vbuf[ 9] = vc[0] + vr[0] - vf[0];
	vbuf[10] = vc[1] + vr[1] - vf[1];
	vbuf[11] = vc[2] + vr[2] - vf[2];
	vbuf[12] = n[0];
	vbuf[13] = n[1];
	vbuf[14] = n[2];

	vbuf[18] = vc[0] - vr[0] + vf[0];
	vbuf[19] = vc[1] - vr[1] + vf[1];
	vbuf[20] = vc[2] - vr[2] + vf[2];
	vbuf[21] = n[0];
	vbuf[22] = n[1];
	vbuf[23] = n[2];

	vbuf[27] = vc[0] + vr[0] + vf[0];
	vbuf[28] = vc[1] + vr[1] + vf[1];
	vbuf[29] = vc[2] + vr[2] + vf[2];
	vbuf[30] = n[0];
	vbuf[31] = n[1];
	vbuf[32] = n[2];

	//index
	ibuf[0] = vlen + 0;
	ibuf[1] = vlen + 1;
	ibuf[2] = vlen + 2;

	ibuf[3] = vlen + 1;
	ibuf[4] = vlen + 2;
	ibuf[5] = vlen + 3;
}
void gl41solid_rect(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, 4, 2);
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
	carvesolid_rect(vbuf,vlen, ibuf,0, vc,vr,vf);
}




#define circleacc (acc*2)
void carvesolid_circle(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf)
{
	int a,b,j;
	float c,s;
	vec3 vu;
	vu[0] = vr[1]*vf[2] - vr[2]*vf[1];
	vu[1] = vr[2]*vf[0] - vr[0]*vf[2];
	vu[2] = vr[0]*vf[1] - vr[1]*vf[0];
	vec3_setlen(vu, 1.0);

	for(j=0;j<circleacc;j++)
	{
		a = j*9;
		b = j*3;

		c = cosine(j*tau/circleacc);
		s = sine(j*tau/circleacc);
		vbuf[a+0] = vc[0] + vr[0]*c + vf[0]*s;
		vbuf[a+1] = vc[1] + vr[1]*c + vf[1]*s;
		vbuf[a+2] = vc[2] + vr[2]*c + vf[2]*s;

		vbuf[a+3] = vu[0];
		vbuf[a+4] = vu[1];
		vbuf[a+5] = vu[2];

		ibuf[b+0] = vlen + circleacc;
		ibuf[b+1] = vlen + j;
		ibuf[b+2] = vlen + (j+1)%circleacc;
	}

	a = circleacc*9;
	vbuf[a+0] = vc[0];
	vbuf[a+1] = vc[1];
	vbuf[a+2] = vc[2];
	vbuf[a+3] = vu[0];
	vbuf[a+4] = vu[1];
	vbuf[a+5] = vu[2];
}
void gl41solid_circle(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, circleacc+1, circleacc);
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
	carvesolid_circle(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void gl41solid_pyramid3()
{
}
void gl41solid_pyramid4()
{
}
void gl41solid_pyramid5()
{
}
void gl41solid_pyramid6()
{
}




void carvesolid_cone(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vu)
{
	int a,b,j;
	float f;
	float r[4];

	for(j=0;j<acc;j++)
	{
		r[0] = vr[0];
		r[1] = vr[1];
		r[2] = vr[2];
		quaternion_operation(r, vu, j*tau/acc);
		f = (r[0]*r[0] + r[1]*r[1] + r[2]*r[2]) / (vu[0]*vu[0] + vu[1]*vu[1] + vu[2]*vu[2]);

		a = j*9;
		vbuf[a+0] = vc[0] + r[0];
		vbuf[a+1] = vc[1] + r[1];
		vbuf[a+2] = vc[2] + r[2];
		vbuf[a+3] = r[0] + vu[0]*f;
		vbuf[a+4] = r[1] + vu[1]*f;
		vbuf[a+5] = r[2] + vu[2]*f;

		b = j*3;
		ibuf[b+0] = vlen+acc;
		ibuf[b+1] = vlen+j;
		ibuf[b+2] = vlen+(j+1)%acc;
	}

	a = acc*9;
	vbuf[a+ 0] = vc[0]+vu[0];
	vbuf[a+ 1] = vc[1]+vu[1];
	vbuf[a+ 2] = vc[2]+vu[2];
	vbuf[a+ 3] = vu[0];
	vbuf[a+ 4] = vu[1];
	vbuf[a+ 5] = vu[2];
}
void gl41solid_cone(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, acc + 1, acc);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*(acc+1);j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvesolid_cone(vbuf,vlen, ibuf,0, vc,vr,vu);
}




void gl41solid_prism3()
{
}




void carvesolid_prism4(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vbuf[2*9+0] = vbuf[1*9+0] = vbuf[0*9+0] = vc[0] - vr[0] - vf[0] - vu[0];
	vbuf[2*9+1] = vbuf[1*9+1] = vbuf[0*9+1] = vc[1] - vr[1] - vf[1] - vu[1];
	vbuf[2*9+2] = vbuf[1*9+2] = vbuf[0*9+2] = vc[2] - vr[2] - vf[2] - vu[2];
	vbuf[0*9+3] = -vu[0];
	vbuf[0*9+4] = -vu[1];
	vbuf[0*9+5] = -vu[2];
	vbuf[1*9+3] = -vf[0];
	vbuf[1*9+4] = -vf[1];
	vbuf[1*9+5] = -vf[2];
	vbuf[2*9+3] = -vr[0];
	vbuf[2*9+4] = -vr[1];
	vbuf[2*9+5] = -vr[2];

	vbuf[5*9+0] = vbuf[4*9+0] = vbuf[3*9+0] = vc[0] + vr[0] - vf[0] - vu[0];
	vbuf[5*9+1] = vbuf[4*9+1] = vbuf[3*9+1] = vc[1] + vr[1] - vf[1] - vu[1];
	vbuf[5*9+2] = vbuf[4*9+2] = vbuf[3*9+2] = vc[2] + vr[2] - vf[2] - vu[2];
	vbuf[3*9+3] = -vu[0];
	vbuf[3*9+4] = -vu[1];
	vbuf[3*9+5] = -vu[2];
	vbuf[4*9+3] = -vf[0];
	vbuf[4*9+4] = -vf[1];
	vbuf[4*9+5] = -vf[2];
	vbuf[5*9+3] = vr[0];
	vbuf[5*9+4] = vr[1];
	vbuf[5*9+5] = vr[2];

	vbuf[8*9+0] = vbuf[7*9+0] = vbuf[6*9+0] = vc[0] - vr[0] + vf[0] - vu[0];
	vbuf[8*9+1] = vbuf[7*9+1] = vbuf[6*9+1] = vc[1] - vr[1] + vf[1] - vu[1];
	vbuf[8*9+2] = vbuf[7*9+2] = vbuf[6*9+2] = vc[2] - vr[2] + vf[2] - vu[2];
	vbuf[6*9+3] = -vu[0];
	vbuf[6*9+4] = -vu[1];
	vbuf[6*9+5] = -vu[2];
	vbuf[7*9+3] = vf[0];
	vbuf[7*9+4] = vf[1];
	vbuf[7*9+5] = vf[2];
	vbuf[8*9+3] = -vr[0];
	vbuf[8*9+4] = -vr[1];
	vbuf[8*9+5] = -vr[2];

	vbuf[11*9+0] = vbuf[10*9+0] = vbuf[9*9+0] = vc[0] + vr[0] + vf[0] - vu[0];
	vbuf[11*9+1] = vbuf[10*9+1] = vbuf[9*9+1] = vc[1] + vr[1] + vf[1] - vu[1];
	vbuf[11*9+2] = vbuf[10*9+2] = vbuf[9*9+2] = vc[2] + vr[2] + vf[2] - vu[2];
	vbuf[ 9*9+3] = -vu[0];
	vbuf[ 9*9+4] = -vu[1];
	vbuf[ 9*9+5] = -vu[2];
	vbuf[10*9+3] = vf[0];
	vbuf[10*9+4] = vf[1];
	vbuf[10*9+5] = vf[2];
	vbuf[11*9+3] = vr[0];
	vbuf[11*9+4] = vr[1];
	vbuf[11*9+5] = vr[2];

	vbuf[14*9+0] = vbuf[13*9+0] = vbuf[12*9+0] = vc[0] - vr[0] - vf[0] + vu[0];
	vbuf[14*9+1] = vbuf[13*9+1] = vbuf[12*9+1] = vc[1] - vr[1] - vf[1] + vu[1];
	vbuf[14*9+2] = vbuf[13*9+2] = vbuf[12*9+2] = vc[2] - vr[2] - vf[2] + vu[2];
	vbuf[12*9+3] = vu[0];
	vbuf[12*9+4] = vu[1];
	vbuf[12*9+5] = vu[2];
	vbuf[13*9+3] = -vf[0];
	vbuf[13*9+4] = -vf[1];
	vbuf[13*9+5] = -vf[2];
	vbuf[14*9+3] = -vr[0];
	vbuf[14*9+4] = -vr[1];
	vbuf[14*9+5] = -vr[2];

	vbuf[17*9+0] = vbuf[16*9+0] = vbuf[15*9+0] = vc[0] + vr[0] - vf[0] + vu[0];
	vbuf[17*9+1] = vbuf[16*9+1] = vbuf[15*9+1] = vc[1] + vr[1] - vf[1] + vu[1];
	vbuf[17*9+2] = vbuf[16*9+2] = vbuf[15*9+2] = vc[2] + vr[2] - vf[2] + vu[2];
	vbuf[15*9+3] = vu[0];
	vbuf[15*9+4] = vu[1];
	vbuf[15*9+5] = vu[2];
	vbuf[16*9+3] = -vf[0];
	vbuf[16*9+4] = -vf[1];
	vbuf[16*9+5] = -vf[2];
	vbuf[17*9+3] = vr[0];
	vbuf[17*9+4] = vr[1];
	vbuf[17*9+5] = vr[2];

	vbuf[20*9+0] = vbuf[19*9+0] = vbuf[18*9+0] = vc[0] - vr[0] + vf[0] + vu[0];
	vbuf[20*9+1] = vbuf[19*9+1] = vbuf[18*9+1] = vc[1] - vr[1] + vf[1] + vu[1];
	vbuf[20*9+2] = vbuf[19*9+2] = vbuf[18*9+2] = vc[2] - vr[2] + vf[2] + vu[2];
	vbuf[18*9+3] = vu[0];
	vbuf[18*9+4] = vu[1];
	vbuf[18*9+5] = vu[2];
	vbuf[19*9+3] = vf[0];
	vbuf[19*9+4] = vf[1];
	vbuf[19*9+5] = vf[2];
	vbuf[20*9+3] = -vr[0];
	vbuf[20*9+4] = -vr[1];
	vbuf[20*9+5] = -vr[2];

	vbuf[23*9+0] = vbuf[22*9+0] = vbuf[21*9+0] = vc[0] + vr[0] + vf[0] + vu[0];
	vbuf[23*9+1] = vbuf[22*9+1] = vbuf[21*9+1] = vc[1] + vr[1] + vf[1] + vu[1];
	vbuf[23*9+2] = vbuf[22*9+2] = vbuf[21*9+2] = vc[2] + vr[2] + vf[2] + vu[2];
	vbuf[21*9+3] = vu[0];
	vbuf[21*9+4] = vu[1];
	vbuf[21*9+5] = vu[2];
	vbuf[22*9+3] = vf[0];
	vbuf[22*9+4] = vf[1];
	vbuf[22*9+5] = vf[2];
	vbuf[23*9+3] = vr[0];
	vbuf[23*9+4] = vr[1];
	vbuf[23*9+5] = vr[2];

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
void gl41solid_prism4(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, 24, 12);
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
	carvesolid_prism4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




void gl41solid_prism5()
{
}
void gl41solid_prism6()
{
}




void carvesolid_cask(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int a,b,j;
	float c,s;
	vec3 vv;

	for(j=0;j<acc;j++)
	{
		a = j*18;
		b = j*6;
		c = cosine(j*tau/acc);
		s = sine(j*tau/acc);

		vv[0] = c*vr[0] - s*vf[0];
		vv[1] = c*vr[1] - s*vf[1];
		vv[2] = c*vr[2] - s*vf[2];

		vbuf[a+0] = vc[0] - vu[0] + vv[0];
		vbuf[a+1] = vc[1] - vu[1] + vv[1];
		vbuf[a+2] = vc[2] - vu[2] + vv[2];
		vbuf[a+3] = vbuf[a+0] - vc[0];
		vbuf[a+4] = vbuf[a+1] - vc[1];
		vbuf[a+5] = vbuf[a+2] - vc[2];

		vbuf[a+ 9] = vc[0] + vu[0] + vv[0];
		vbuf[a+10] = vc[1] + vu[1] + vv[1];
		vbuf[a+11] = vc[2] + vu[2] + vv[2];
		vbuf[a+12] = vbuf[a+ 9] - vc[0];
		vbuf[a+13] = vbuf[a+10] - vc[1];
		vbuf[a+14] = vbuf[a+11] - vc[2];

		ibuf[b+0] = vlen + j*2;
		ibuf[b+1] = vlen + ((j+1)%acc)*2;
		ibuf[b+2] = vlen + 1 + j*2;

		ibuf[b+3] = vlen + 1 + ((j+1)%acc)*2;
		ibuf[b+4] = vlen + ((j+1)%acc)*2;
		ibuf[b+5] = vlen + 1 + j*2;
	}
}
void gl41solid_cask(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, acc * 2, acc * 2);
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
	carvesolid_cask(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41solid_cylinder(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vec3 tc;
	vec3 tf;
	gl41solid_cask(win, rgb, vc, vr, vf, vu);

	tc[0] = vc[0]+vu[0];
	tc[1] = vc[1]+vu[1];
	tc[2] = vc[2]+vu[2];
	gl41solid_circle(win, rgb, tc, vr, vf);

	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	tf[0] = -vf[0];
	tf[1] = -vf[1];
	tf[2] = -vf[2];
	gl41solid_circle(win, rgb, tc, vr, tf);
}




void carvesolid_gear(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt, int teethcount)
{
	int j,k;
	for(j=0;j<teethcount*4+1;j++){
		vbuf[j*9 + 3] = vt[0];
		vbuf[j*9 + 4] = vt[1];
		vbuf[j*9 + 5] = vt[2];
	}

	float a,c,s;
	vec3 tc,tr,tf;
	for(k=0;k<teethcount;k++){
		a = (k*4-1)*tau/(teethcount*4);
		c = cosine(a);
		s = sine(a);
		for(j=0;j<3;j++){
			vbuf[k*9*4 +0 +j] = vc[j] +vr[j]*c*0.9 + vf[j]*s*0.9;
			vbuf[k*9*4 +9 +j] = vc[j] +vr[j]*c + vf[j]*s;
		}

		a = (k*4+1)*tau/(teethcount*4);
		c = cosine(a);
		s = sine(a);
		for(j=0;j<3;j++){
			vbuf[k*9*4 +18 +j] = vc[j] +vr[j]*c + vf[j]*s;
			vbuf[k*9*4 +27 +j] = vc[j] +vr[j]*c*0.9 + vf[j]*s*0.9;
		}
	}
	vbuf[teethcount*9*4 +0] = vc[0];
	vbuf[teethcount*9*4 +1] = vc[1];
	vbuf[teethcount*9*4 +2] = vc[2];

	for(j=0;j<teethcount;j++){
		ibuf[j*3*4 + 0] = vlen + j*4+0;
		ibuf[j*3*4 + 1] = vlen + j*4+1;
		ibuf[j*3*4 + 2] = vlen + j*4+2;

		ibuf[j*3*4 + 3] = vlen + j*4+0;
		ibuf[j*3*4 + 4] = vlen + j*4+2;
		ibuf[j*3*4 + 5] = vlen + j*4+3;

		ibuf[j*3*4 + 6] = vlen + j*4+0;
		ibuf[j*3*4 + 7] = vlen + j*4+3;
		ibuf[j*3*4 + 8] = vlen + teethcount*4;

		ibuf[j*3*4 + 9] = vlen + j*4+3;
		ibuf[j*3*4 +10] = vlen + (j*4+4)%(teethcount*4);
		ibuf[j*3*4 +11] = vlen + teethcount*4;
	}
}
void gl41solid_gear(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt, int teethcount)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, teethcount*4+1, teethcount*4);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*(teethcount*4+1);j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvesolid_gear(vbuf,vlen, ibuf,0, vc,vr,vf,vt, teethcount);
}
void gl41solid_rotategear(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt, int teethcount, float a)
{
	int j;
	vec3 tr,tf;
	float c = cosine(a);
	float s = sine(a);
	for(j=0;j<3;j++){
		tr[j] = vr[j]*c + vf[j]*s;
		tf[j] = vf[j]*c - vr[j]*s;
	}
	gl41solid_gear(win, rgb, vc, tr, tf, vt, teethcount);
}




void gl41solid_tetrahedron()
{
}
void gl41solid_octahedron()
{
}




void carvesolid_dodecahedron(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float a = 1.618;
	float b = 1.0/1.618;

	//(+-1, +-1, +-1)
	vbuf[ 0] = vc[0]-vr[0]-vf[0]-vu[0];
	vbuf[ 1] = vc[1]-vr[1]-vf[1]-vu[1];
	vbuf[ 2] = vc[2]-vr[2]-vf[2]-vu[2];

	vbuf[ 9] = vc[0]+vr[0]-vf[0]-vu[0];
	vbuf[10] = vc[1]+vr[1]-vf[1]-vu[1];
	vbuf[11] = vc[2]+vr[2]-vf[2]-vu[2];

	vbuf[18] = vc[0]-vr[0]+vf[0]-vu[0];
	vbuf[19] = vc[1]-vr[1]+vf[1]-vu[1];
	vbuf[20] = vc[2]-vr[2]+vf[2]-vu[2];

	vbuf[27] = vc[0]+vr[0]+vf[0]-vu[0];
	vbuf[28] = vc[1]+vr[1]+vf[1]-vu[1];
	vbuf[29] = vc[2]+vr[2]+vf[2]-vu[2];

	vbuf[36] = vc[0]-vr[0]-vf[0]+vu[0];
	vbuf[37] = vc[1]-vr[1]-vf[1]+vu[1];
	vbuf[38] = vc[2]-vr[2]-vf[2]+vu[2];

	vbuf[45] = vc[0]+vr[0]-vf[0]+vu[0];
	vbuf[46] = vc[1]+vr[1]-vf[1]+vu[1];
	vbuf[47] = vc[2]+vr[2]-vf[2]+vu[2];

	vbuf[54] = vc[0]-vr[0]+vf[0]+vu[0];
	vbuf[55] = vc[1]-vr[1]+vf[1]+vu[1];
	vbuf[56] = vc[2]-vr[2]+vf[2]+vu[2];

	vbuf[63] = vc[0]+vr[0]+vf[0]+vu[0];
	vbuf[64] = vc[1]+vr[1]+vf[1]+vu[1];
	vbuf[65] = vc[2]+vr[2]+vf[2]+vu[2];

	//(0, +-a, +-b)
	vbuf[72] = vc[0] - a*vf[0] - b*vu[0];
	vbuf[73] = vc[1] - a*vf[1] - b*vu[1];
	vbuf[74] = vc[2] - a*vf[2] - b*vu[2];

	vbuf[81] = vc[0] + a*vf[0] - b*vu[0];
	vbuf[82] = vc[1] + a*vf[1] - b*vu[1];
	vbuf[83] = vc[2] + a*vf[2] - b*vu[2];

	vbuf[90] = vc[0] - a*vf[0] + b*vu[0];
	vbuf[91] = vc[1] - a*vf[1] + b*vu[1];
	vbuf[92] = vc[2] - a*vf[2] + b*vu[2];

	vbuf[ 99] = vc[0] + a*vf[0] + b*vu[0];
	vbuf[100] = vc[1] + a*vf[1] + b*vu[1];
	vbuf[101] = vc[2] + a*vf[2] + b*vu[2];

	//(+-b, 0, +-a)
	vbuf[108] = vc[0] - b*vr[0] - a*vu[0];
	vbuf[109] = vc[1] - b*vr[1] - a*vu[1];
	vbuf[110] = vc[2] - b*vr[2] - a*vu[2];

	vbuf[117] = vc[0] + b*vr[0] - a*vu[0];
	vbuf[118] = vc[1] + b*vr[1] - a*vu[1];
	vbuf[119] = vc[2] + b*vr[2] - a*vu[2];

	vbuf[126] = vc[0] - b*vr[0] + a*vu[0];
	vbuf[127] = vc[1] - b*vr[1] + a*vu[1];
	vbuf[128] = vc[2] - b*vr[2] + a*vu[2];

	vbuf[135] = vc[0] + b*vr[0] + a*vu[0];
	vbuf[136] = vc[1] + b*vr[1] + a*vu[1];
	vbuf[137] = vc[2] + b*vr[2] + a*vu[2];

	//(+-a, +-b, 0)
	vbuf[144] = vc[0] - a*vr[0] - b*vf[0];
	vbuf[145] = vc[1] - a*vr[1] - b*vf[1];
	vbuf[146] = vc[2] - a*vr[2] - b*vf[2];

	vbuf[153] = vc[0] + a*vr[0] - b*vf[0];
	vbuf[154] = vc[1] + a*vr[1] - b*vf[1];
	vbuf[155] = vc[2] + a*vr[2] - b*vf[2];

	vbuf[162] = vc[0] - a*vr[0] + b*vf[0];
	vbuf[163] = vc[1] - a*vr[1] + b*vf[1];
	vbuf[164] = vc[2] - a*vr[2] + b*vf[2];

	vbuf[171] = vc[0] + a*vr[0] + b*vf[0];
	vbuf[172] = vc[1] + a*vr[1] + b*vf[1];
	vbuf[173] = vc[2] + a*vr[2] + b*vf[2];

	for(j=0;j<20*9;j+=9)
	{
		vbuf[j + 3] = vbuf[j + 0] - vc[0];
		vbuf[j + 4] = vbuf[j + 1] - vc[1];
		vbuf[j + 5] = vbuf[j + 2] - vc[2];
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
void gl41solid_dodecahedron(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, 20, 36);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*20;j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvesolid_dodecahedron(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




void carvesolid_icosahedron(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int j;
	float m = 0.52573111211913360602566908484788;
	float n = 0.85065080835203993218154049706301;

	//(+-m, 0, +-n)
	vbuf[ 0] = vc[0] - m*vr[0] - n*vu[0];
	vbuf[ 1] = vc[1] - m*vr[1] - n*vu[1];
	vbuf[ 2] = vc[2] - m*vr[2] - n*vu[2];

	vbuf[ 9] = vc[0] + m*vr[0] - n*vu[0];
	vbuf[10] = vc[1] + m*vr[1] - n*vu[1];
	vbuf[11] = vc[2] + m*vr[2] - n*vu[2];

	vbuf[18] = vc[0] - m*vr[0] + n*vu[0];
	vbuf[19] = vc[1] - m*vr[1] + n*vu[1];
	vbuf[20] = vc[2] - m*vr[2] + n*vu[2];

	vbuf[27] = vc[0] + m*vr[0] + n*vu[0];
	vbuf[28] = vc[1] + m*vr[1] + n*vu[1];
	vbuf[29] = vc[2] + m*vr[2] + n*vu[2];

	//(0, +-n, +-m)
	vbuf[36] = vc[0] - n*vf[0] - m*vu[0];
	vbuf[37] = vc[1] - n*vf[1] - m*vu[1];
	vbuf[38] = vc[2] - n*vf[2] - m*vu[2];

	vbuf[45] = vc[0] + n*vf[0] - m*vu[0];
	vbuf[46] = vc[1] + n*vf[1] - m*vu[1];
	vbuf[47] = vc[2] + n*vf[2] - m*vu[2];

	vbuf[54] = vc[0] - n*vf[0] + m*vu[0];
	vbuf[55] = vc[1] - n*vf[1] + m*vu[1];
	vbuf[56] = vc[2] - n*vf[2] + m*vu[2];

	vbuf[63] = vc[0] + n*vf[0] + m*vu[0];
	vbuf[64] = vc[1] + n*vf[1] + m*vu[1];
	vbuf[65] = vc[2] + n*vf[2] + m*vu[2];

	//(+-n, +-m, 0)
	vbuf[72] = vc[0] - n*vr[0] - m*vf[0];
	vbuf[73] = vc[1] - n*vr[1] - m*vf[1];
	vbuf[74] = vc[2] - n*vr[2] - m*vf[2];

	vbuf[81] = vc[0] + n*vr[0] - m*vf[0];
	vbuf[82] = vc[1] + n*vr[1] - m*vf[1];
	vbuf[83] = vc[2] + n*vr[2] - m*vf[2];

	vbuf[90] = vc[0] - n*vr[0] + m*vf[0];
	vbuf[91] = vc[1] - n*vr[1] + m*vf[1];
	vbuf[92] = vc[2] - n*vr[2] + m*vf[2];

	vbuf[ 99] = vc[0] + n*vr[0] + m*vf[0];
	vbuf[100] = vc[1] + n*vr[1] + m*vf[1];
	vbuf[101] = vc[2] + n*vr[2] + m*vf[2];

	for(j=0;j<12*9;j+=9)
	{
		vbuf[j + 3] = vbuf[j + 0] - vc[0];
		vbuf[j + 4] = vbuf[j + 1] - vc[1];
		vbuf[j + 5] = vbuf[j + 2] - vc[2];
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
void gl41solid_icosahedron(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, 12, 20);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*12;j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvesolid_icosahedron(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




#define accx (acc)
#define accy (acc|0x1)
void carvesolid_sphere(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	int a,b,j,k;
	float c,s;
	vec3 tc, tr, tf;

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

			a = (k*accx + j)*9;
			vbuf[a+0] = tc[0] + tr[0]*c + tf[0]*s;
			vbuf[a+1] = tc[1] + tr[1]*c + tf[1]*s;
			vbuf[a+2] = tc[2] + tr[2]*c + tf[2]*s;
			vbuf[a+3] = vbuf[a+0] - vc[0];
			vbuf[a+4] = vbuf[a+1] - vc[1];
			vbuf[a+5] = vbuf[a+2] - vc[2];

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

	a = accx*accy*9;

	vbuf[a+ 0] = vc[0]-vu[0];
	vbuf[a+ 1] = vc[1]-vu[1];
	vbuf[a+ 2] = vc[2]-vu[2];
	vbuf[a+ 3] = -vu[0];
	vbuf[a+ 4] = -vu[1];
	vbuf[a+ 5] = -vu[2];

	vbuf[a+ 9] = vc[0]+vu[0];
	vbuf[a+10] = vc[1]+vu[1];
	vbuf[a+11] = vc[2]+vu[2];
	vbuf[a+12] = vu[0];
	vbuf[a+13] = vu[1];
	vbuf[a+14] = vu[2];

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
void gl41solid_sphere(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, accx*accy+2, accx*accy*2);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*(accx*accy+2);j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvesolid_sphere(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




void carvesolid_tokamak(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vu)
{
}
void gl41solid_tokamak(struct entity* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, acc*acc*2, acc*acc);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*(acc*acc*2);j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvesolid_tokamak(vbuf,vlen, ibuf,0, vc,vr,vu);
}
