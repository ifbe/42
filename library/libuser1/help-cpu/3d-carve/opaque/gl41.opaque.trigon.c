#include "libuser.h"
#define acc 32
#define circleacc 32
#define sphereaccx 16
#define sphereaccy (sphereaccx|0x1)
void carvetrigonindex_triangle_v4n4x4(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 v0,vec3 v1,vec3 v2);
void carvetrigonindex_rect_v4n4x4(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_circle_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf);
void carvetrigonindex_cone_v4n4x4(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vt);
void carvetrigonindex_prism4_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_cask_v4n4x4(    float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_sphere_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_tokamak_v4n4x4( float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_tetrahedron_v4n4x4( float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_octahedron_v4n4x4(  float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_dodecahedron_v4n4x4(float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);
void carvetrigonindex_icosahedron_v4n4x4( float* vbuf,int vlen, u16* ibuf,int ilen, vec3 vc,vec3 vr,vec3 vf,vec3 vt);




static char dx11opaquetrigon_vert[] =
"cbuffer VSConstantBuffer : register(b0){\n"
	"matrix matmvp;\n"
"};\n"
"struct VSin{\n"
	"float3 v : PA;\n"
	"float3 n : PB;\n"
	"float4 c : PC;\n"
"};\n"
"struct VSout{\n"
	"float4 where : SV_POSITION;\n"
	"float4 color : COLOR;\n"
"};\n"
"VSout main(VSin input){\n"
	"VSout output;\n"
	"output.where = mul(float4(input.v, 1.0), matmvp);\n"
	"output.color = input.c;\n"
	"return output;\n"
"}\n";
static char dx11opaquetrigon_frag[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




static char gl41pplltrigon_vert[] =
GLSL_VERSION
GLSL_PRECISION
"layout(location = 0)in vec3 v;\n"
"layout(location = 1)in vec3 n;\n"
"layout(location = 2)in vec4 c;\n"
"out vec3 objxyz;\n"
"out vec3 normal;\n"
"out vec4 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"objxyz = v;\n"
	"normal = n;\n"
	"vec3 tmp = pow(vec3(c), vec3(2.2));\n"
	"colour = vec4(tmp, c.w);\n"
	"gl_Position = cammvp * vec4(objxyz,1.0);\n"
"}\n";

//early_fragment_tests: set this, depthtest before pixelshader
static char gl41pplltrigon_frag[] =
GLSL_VERSION
GLSL_PRECISION
"layout(early_fragment_tests) in;\n"
"in vec3 objxyz;\n"
"in vec3 normal;\n"
"in vec4 colour;\n"
//"in vec3 mtrfao;\n"
"out vec4 FragColor;\n"
"uniform vec3 camxyz;\n"
"const float PI = 3.1415926535897932384626433832795028841971693993151;\n"

"layout(binding = 0, offset = 0) uniform atomic_uint cur;\n"
"layout(binding = 0, offset = 4) uniform atomic_uint cnt;\n"
"struct ppllheaddef{\n"
    "uint prev;\n"
    "uint count;\n"
"};\n"
"layout (std430, binding = 0) buffer ppllheadbuf{\n"
    "ppllheaddef ppllhead[];\n"
"};\n"
"struct pplldatadef{\n"
    "uint prev;\n"
    "uint temp;\n"
    "uint color;\n"
    "float depth;\n"
"};\n"
"layout (std430, binding = 1) buffer pplldatabuf{\n"
    "pplldatadef pplldata[];\n"
"};\n"
"void insert(vec4 c){\n"
	"uint color = packUnorm4x8(c);\n"
	"float depth = (gl_FragCoord.z * 2.0 - 1.0) / gl_FragCoord.w;\n"
	"uint x = uint(gl_FragCoord.x);\n"
	"uint y = uint(gl_FragCoord.y);\n"
	"uint index = y*1024 + x;\n"
/*
	//clear buffer
    "uint datacount = ppllhead[index].count;\n"
    "uint dataindex = ppllhead[index].prev;\n"
    "for(uint j;j<datacount;j++){\n"
        "if(abs(depth - pplldata[dataindex].depth) < 0.1)return;\n"
        "dataindex = pplldata[dataindex].prev;\n"
    "}\n"
*/
	//if(cur+1 >= cnt)return
	"uint datacur = atomicCounterIncrement(cur);\n"

	//if(ppllhead[index].count >= 4)return;
	"uint thiscnt = atomicAdd(ppllhead[index].count, 1);\n"

	"uint prevone = atomicExchange(ppllhead[index].prev, datacur);\n"

	"pplldata[datacur].prev = prevone;\n"
	"pplldata[datacur].temp = x;\n"
	"pplldata[datacur].color = color;\n"
	"pplldata[datacur].depth = depth;\n"
"}\n"

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
	"vec3 albedo = vec3(colour);\n"

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
	"float alpha = colour.w;\n"
	"vec3 pbrout = pbr();\n"
	"insert(vec4(pbrout, alpha));\n"
	//"FragColor = vec4(pbrout, alpha);\n"
"}\n";

static char gl41opaquetrigon_vert[] =
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

static char gl41opaquetrigon_frag[] =
GLSL_VERSION
"in mediump vec4 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = colour;\n"
"}\n";




static int opaque3d_fill(_obj* win, struct mysrc* src)
{
	if(0 == src->vs){
		switch(win->vfmt){
		case _gl41list_:
			if(_ppll_ == win->gl41list.rendermode){
			src->vs = gl41pplltrigon_vert;
			src->fs = gl41pplltrigon_frag;
			}
			else{
			src->vs = gl41opaquetrigon_vert;
			src->fs = gl41opaquetrigon_frag;
			}
			break;
		case _dx11list_:
			src->vs = dx11opaquetrigon_vert;
			src->fs = dx11opaquetrigon_frag;
			break;
		default:return -3;
		}
		src->shader_enq = 1;
	}

	struct vertex* vtx = src->vtx;
	vtx->opaque = 1;
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
		vtx->vbuf_len = 65536*4*12;
		vtx->vbuf = memoryalloc(vtx->vbuf_len, 0);
		if(0 == vtx->vbuf)return -1;

		vtx->vbuf_w = 4*4*3;
		vtx->vbuf_h = 0;	//(src->vbuf_len) / (src->vbuf_w);
		vtx->vbuf_fmt = vbuffmt_444;
		src->vbuf_enq = 1;
	}

	return 0;
}
int opaque3d_vars(_obj* win, int unused, float** vbuf, u16** ibuf, int vcnt, int icnt)
{
	if(0 == win)return -1;
	if(0 == win->gl41list.world[0].opaque)return -2;

	struct gl41data* p = win->gl41list.world[0].opaque[opaquetrigon3d];
	if(0 == p){
		p = win->gl41list.world[0].opaque[opaquetrigon3d] = memoryalloc(0x1000, 0);
		if(0 == p)return -3;
	}

	int vlen,ilen,ret;
	struct vertex* vtx = p->src.vtx;
	if(0 == vtx->vbuf){
		ret = opaque3d_fill(win, &p->src);
		if(ret < 0)return -3;
	}

	vlen = vtx->vbuf_h;
	ilen = vtx->ibuf_h;
	*vbuf = (vtx->vbuf) + (48*vlen);
	*ibuf = (vtx->ibuf) + (6*ilen);
	vtx->vbuf_h += vcnt;
	vtx->ibuf_h += icnt;

	return vlen;
}




void gl41opaque_triangle(_obj* win, u32 rgba,
	vec3 v0, vec3 v1, vec3 v2)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 3, 1);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*3;j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_triangle_v4n4x4(vbuf,vlen, ibuf,0, v0,v1,v2);
}




void gl41opaque_rect(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 4, 2);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*4;j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_rect_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void gl41opaque_circle(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, circleacc+1, circleacc);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(circleacc+1);j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_circle_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf);
}




void gl41opaque_pyramid3()
{
}
void gl41opaque_pyramid4()
{
}
void gl41opaque_pyramid5()
{
}
void gl41opaque_pyramid6()
{
}




void gl41opaque_prism3()
{
}
void gl41opaque_prism4(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 24, 12);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*24;j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_prism4_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41opaque_prism5()
{
}
void gl41opaque_prism6()
{
}




void gl41opaque_cone(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vt)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, acc + 1, acc);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(acc+1);j+=12){
		vbuf[j + 8] = rr;
		vbuf[j + 9] = gg;
		vbuf[j +10] = bb;
		vbuf[j +11] = aa;
	}
	carvetrigonindex_cone_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vt);
}
void gl41opaque_cask(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, acc * 2, acc * 2);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(acc*2);j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_cask_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41opaque_cylinder(_obj* win, u32 rgb,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	vec3 tc;
	vec3 tf;
	gl41opaque_cask(win, rgb, vc, vr, vf, vu);

	tc[0] = vc[0]+vu[0];
	tc[1] = vc[1]+vu[1];
	tc[2] = vc[2]+vu[2];
	gl41opaque_circle(win, rgb, tc, vr, vf);

	tc[0] = vc[0]-vu[0];
	tc[1] = vc[1]-vu[1];
	tc[2] = vc[2]-vu[2];
	tf[0] = -vf[0];
	tf[1] = -vf[1];
	tf[2] = -vf[2];
	gl41opaque_circle(win, rgb, tc, vr, tf);
}
void gl41opaque_sphere(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, sphereaccx*sphereaccy+2, sphereaccx*sphereaccy*2);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(sphereaccx*sphereaccy+2);j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_sphere_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41opaque_tokamak(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, acc*acc*2, acc*acc);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*(sphereaccx*sphereaccy*2);j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_tokamak_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}




void gl41opaque_tetrahedron()
{
}
void gl41opaque_octahedron()
{
}
void gl41opaque_dodecahedron(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 20, 36);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*20;j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_dodecahedron_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
void gl41opaque_icosahedron(_obj* win, u32 rgba,
	vec3 vc, vec3 vr, vec3 vf, vec3 vu)
{
	float* vbuf;
	u16* ibuf;
	int vlen = opaque3d_vars(win, 0, &vbuf, &ibuf, 12, 20);
	if(vlen < 0)return;

	int j;
	u8* t = (void*)&rgba;
	float bb = (float)t[0] / 255.0;
	float gg = (float)t[1] / 255.0;
	float rr = (float)t[2] / 255.0;
	float aa = (float)t[3] / 255.0;
	for(j=0;j<12*12;j+=12){
		vbuf[j+ 8] = rr;
		vbuf[j+ 9] = gg;
		vbuf[j+10] = bb;
		vbuf[j+11] = aa;
	}
	carvetrigonindex_icosahedron_v4n4x4(vbuf,vlen, ibuf,0, vc,vr,vf,vu);
}
