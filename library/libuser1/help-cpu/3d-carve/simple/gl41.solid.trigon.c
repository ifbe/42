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




static char dx11solidtrigon_vert[] =
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
static char dx11solidtrigon_frag[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




static char gl41solidtrigon_vert[] =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 v;\n"
"layout(location = 1)in vec3 n;\n"
"layout(location = 2)in vec3 c;\n"
"out vec3 objxyz;\n"
"out vec3 normal;\n"
"out vec3 albedo;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"objxyz = v;\n"
	"normal = n;\n"
	"albedo = pow(c, vec3(2.2));\n"
	"gl_Position = cammvp * vec4(objxyz,1.0);\n"
"}\n";
static char gl41solidtrigon_frag[] =
GLSL_VERSION
GLSL_PRECISION
"in vec3 objxyz;\n"
"in vec3 normal;\n"
"in vec3 albedo;\n"
//"in vec3 mtrfao;\n"
"out vec4 FragColor;\n"
"uniform vec3 camxyz;\n"
"const float PI = 3.1415926535897932384626433832795028841971693993151;\n"

"float getD(float v, float r){\n"
    "float a2 = r*r*r*r;\n"
    "float de = (v*v * (a2 - 1.0) + 1.0);\n"
    "return a2 / (PI * de * de);\n"
"}\n"
"float getG(float v, float r){\n"
    "float k = (r+1.0) * (r+1.0) / 8.0;\n"
    "return v / (v * (1.0 - k) + k);\n"
"}\n"
"vec3 pbr(){\n"
	"vec3 mtrfao = vec3(0.0, 0.5, 1.0);\n"
	"float metal = mtrfao.x;\n"
	"float rough = mtrfao.y;\n"
	"float amocc = mtrfao.z;\n"

	"vec3 N = normalize(normal);\n"
	"vec3 E = normalize(camxyz - objxyz);\n"
	"vec3 F0 = mix(vec3(0.04), albedo, metal);\n"

	"vec3 litrgb = vec3(1.0, 1.0, 1.0);\n"
	"vec3 litdir[4];\n"
	"litdir[0] = vec3(-1.0, 0.0, 1.0);\n"
	"litdir[1] = vec3( 1.0, 0.0, 1.0);\n"
	"litdir[2] = vec3( 0.0,-1.0, 1.0);\n"
	"litdir[3] = vec3( 0.0, 1.0, 1.0);\n"

	"vec3 ocolor = vec3(0.03) * albedo * amocc;\n"
	"for(int j=0;j<4;j++){\n"
		"vec3 L = litdir[j];\n"
		//"float distance = length(L);\n"
		//"float attenuation = 1.0 / (distance * distance);\n"
		//"vec3 radiance = litrgb * attenuation;\n"
		"vec3 radiance = litrgb;\n"

		"L = normalize(L);\n"
		"vec3 H = normalize(E + L);\n"
		"float NdotL = max(dot(N, L), 0.0);\n"
		"float NdotE = max(dot(N, E), 0.0);\n"
		"float NdotH = max(dot(N, H), 0.0);\n"
		"float HdotE = max(dot(H, E), 0.0);\n"

		"float G = getG(NdotL, rough)*getG(NdotE, rough);\n"
		"float D = getD(NdotH, rough);\n"
		"vec3 F = F0 + (1.0 - F0) * pow(1.0 - HdotE, 5.0);\n"

		"vec3 kS = F;\n"
		"vec3 kD = (vec3(1.0) - kS) * (1.0 - metal);\n"
		"vec3 specular = (D * G * F) / max(4.0 * NdotE * NdotL, 0.001);\n"
		"ocolor += (kD * albedo / PI + specular) * radiance * NdotL;\n"
	"}\n"

	"ocolor = ocolor / (ocolor + vec3(1.0));\n"
	"ocolor = pow(ocolor, vec3(1.0/2.2));\n"
	"return ocolor;\n"
"}\n"

"void main(){\n"
	"vec3 pbrout = pbr();\n"
	"FragColor = vec4(pbrout, 1.0);\n"
"}\n";


/*
//"layout(location = 0)out vec4 outxyz\n;"
//"layout(location = 1)out vec4 outuvw\n;"

"vec3 dirsun0 = vec3(1.0, 1.0, 1.0);\n"
"vec3 dirsun1 = vec3(-1.0, 0.0, 0.0);\n"

//"vec3 LA = vec3(1.0, 1.0, 1.0);\n"
"vec3 L0 = vec3(0.5, 0.5, 0.5);\n"
"vec3 L1 = vec3(0.5, 0.5, 0.5);\n"
//"vec3 KA = vec3(0.231250, 0.231250, 0.231250);\n"
"vec3 KD = vec3(0.277500, 0.277500, 0.277500);\n"
"vec3 KS = vec3(0.773911, 0.773911, 0.773911);\n"

"vec3 sun0(){\n"
	"vec3 N = normalize(normal);\n"
	"vec3 L = normalize(dirsun0);\n"
	"float NL = dot(N, L);\n"
	//"float NL = clamp(dot(N, L), -1.0, 1.0);\n"
	"vec3 ret = L0*KD*max(NL, 0.0);\n"
	"if(NL < 0.0)return ret;\n"

	"vec3 E = normalize(camxyz - vertex);\n"
	"vec3 H = normalize(E + L);\n"
	"float NH = clamp(dot(N, H), 0.0, 1.0);\n"
	"return ret + L0*KS*pow(NH, 89.6);\n"
"}\n"
"vec3 sun1(){\n"
	"vec3 N = normalize(normal);\n"
	"vec3 L = normalize(dirsun1);\n"
	"float NL = dot(N, L);\n"
	//"float NL = clamp(dot(N, L), -1.0, 1.0);\n"
	"vec3 ret = L1*KD*max(NL, 0.0);\n"
	"if(NL < 0.0)return ret;\n"

	"vec3 E = normalize(camxyz - vertex);\n"
	"vec3 H = normalize(E + L);\n"
	"float NH = clamp(dot(N, H), 0.0, 1.0);\n"
	"return ret + L1*KS*pow(NH, 89.6);\n"
"}\n"
//"float shadow(){\n"
	//"if(uvw.z - texture(tex0, uvw.xy).r > 0.000001)return 0.1;\n"
	//"return 1.0;\n"
//"}\n"
"void main(){\n"
	"vec3 c = colour;\n"
	"c += sun0() / 2.0;\n"
	"c += sun1() / 2.0;\n"
	"c = vec3(clamp(c.x, 0.0, 1.0), clamp(c.y, 0.0, 1.0), clamp(c.z, 0.0, 1.0));\n"
	"FragColor = vec4(c, 1.0);\n"
	//"outxyz = vec4(vertex, 1.0);\n"
	//"outuvw = vec4(normal, 1.0);\n"
"}\n";*/




static int trigon3d_fill(_obj* win, struct mysrc* src)
{
	if(0 == src->vs){
		switch(win->hfmt){
		case _gl41list_:
			src->vs = gl41solidtrigon_vert;
			src->fs = gl41solidtrigon_frag;
			break;
		case _dx11list_:
			src->vs = dx11solidtrigon_vert;
			src->fs = dx11solidtrigon_frag;
			break;
		case _mt20list_:
			src->vs = shadersource;
			break;
		default:return -3;
		}
		src->shader_enq = 1;
	}

	struct vertex* vtx = src->vtx;
	vtx->opaque = 0;
	vtx->geometry = 3;
	//vtx->fill = 2;

	if(0 == vtx->ibuf){
		vtx->ibuf_len = 0x100000;
		vtx->ibuf = memoryalloc(vtx->ibuf_len, 0);
		if(0 == vtx->ibuf)return -2;

		vtx->ibuf_w = 2*3;
		vtx->ibuf_h = 0;	//(src->ibuf_len) / (src->ibuf_w);
		vtx->ibuf_fmt = 0x222;
		src->ibuf_enq = 1;
	}

	if(0 == vtx->vbuf){
		vtx->vbuf_len = 65536*4*9;		//65535*4*9
		vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
		if(0 == vtx->vbuf)return -1;

		vtx->vbuf_w = 4*3*3;
		vtx->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		vtx->vbuf_fmt = vbuffmt_333;
		src->vbuf_enq = 1;
	}

	return 0;
}
int trigon3d_vars(_obj* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	if(0 == win)return -1;
	if(0 == win->gl41list.world[0].solid)return -2;

	struct gl41data* p = win->gl41list.world[0].solid[trigon3d];
	if(0 == p){
		p = win->gl41list.world[0].solid[trigon3d] = memoryalloc(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ilen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = trigon3d_fill(win, &p->src);
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




void gl41solid_triangle(_obj* win, u32 rgb,
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
	carvetrigonindex_triangle_v3n3x3(vbuf,vlen, ibuf,0, v0,v1,v2);
}




void gl41solid_rect(_obj* win, u32 rgb,
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
	carvetrigonindex_rect_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void gl41solid_circle(_obj* win, u32 rgb,
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
	carvetrigonindex_circle_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf);
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
void gl41solid_cone(_obj* win, u32 rgb,
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
void gl41solid_prism4(_obj* win, u32 rgb,
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
	carvetrigonindex_prism4_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41solid_prism5()
{
}
void gl41solid_prism6()
{
}




void gl41solid_cask(_obj* win, u32 rgb,
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
	carvetrigonindex_cask_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41solid_cylinder(_obj* win, u32 rgb,
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
		c = getcos(a);
		s = getsin(a);
		for(j=0;j<3;j++){
			vbuf[k*9*4 +0 +j] = vc[j] +vr[j]*c*0.9 + vf[j]*s*0.9;
			vbuf[k*9*4 +9 +j] = vc[j] +vr[j]*c + vf[j]*s;
		}

		a = (k*4+1)*tau/(teethcount*4);
		c = getcos(a);
		s = getsin(a);
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
void gl41solid_gear(_obj* win, u32 rgb,
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
void gl41solid_rotategear(_obj* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vt, int teethcount, float a)
{
	int j;
	vec3 tr,tf;
	float c = getcos(a);
	float s = getsin(a);
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
void gl41solid_dodecahedron(_obj* win, u32 rgb,
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
void gl41solid_icosahedron(_obj* win, u32 rgb,
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




void gl41solid_sphere(_obj* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = trigon3d_vars(win, 0, &vbuf, &ibuf, sphereaccx*sphereaccy+2, sphereaccx*sphereaccy*2);
	if(vlen < 0)return;

	int j;
	float bb = (float)(rgb&0xff) / 255.0;
	float gg = (float)((rgb>>8)&0xff) / 255.0;
	float rr = (float)((rgb>>16)&0xff) / 255.0;
	for(j=0;j<9*(sphereaccx*sphereaccy+2);j+=9){
		vbuf[j + 6] = rr;
		vbuf[j + 7] = gg;
		vbuf[j + 8] = bb;
	}
	carvetrigonindex_sphere_v3n3x3(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41solid_spheretest(_obj* win, u32 rgb, vec3 vc)
{
	vec3 vr = {16.0, 0.0, 0.0};
	vec3 vf = {0.0, 16.0, 0.0};
	vec3 vt = {0.0, 0.0, 16.0};
	gl41solid_sphere(win,rgb, vc, vr, vf, vt);
}




void carvesolid_tokamak(float* vbuf, int vlen, u16* ibuf, int ilen,
	vec3 vc, vec3 vr, vec3 vu)
{
}
void gl41solid_tokamak(_obj* win, u32 rgb,
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
