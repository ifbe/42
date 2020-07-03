#include "libuser.h"
#define s30 0.5
#define s45 0.7071067811865476
#define s60 0.8660254037844386
void matproj(mat4 m, struct fstyle* sty);




static char vshader[] =
GLSL_VERSION
"layout(location = 0)in mediump vec3 v;\n"
"layout(location = 1)in mediump vec3 c;\n"
"out mediump vec3 colour;\n"
"uniform mat4 cammvp;\n"
"void main(){\n"
	"colour = c;\n"
	"gl_Position = cammvp * vec4(v, 1.0);\n"
"}\n";
static char fshader[] =
GLSL_VERSION
"in mediump vec3 colour;\n"
"out mediump vec4 FragColor;\n"
"void main(){\n"
	"FragColor = vec4(colour, 1.0);\n"
"}\n";
static float vbuf[6*8] = {
	-1.0,-1.0,-1.0,		0.0, 0.0, 0.0,
	 1.0,-1.0,-1.0,		1.0, 0.0, 0.0,
	-1.0, 1.0,-1.0,		0.0, 1.0, 0.0,
	 1.0, 1.0,-1.0,		1.0, 1.0, 0.0,

	-1.0,-1.0, 1.0,		0.0, 0.0, 1.0,
	 1.0,-1.0, 1.0,		1.0, 0.0, 1.0,
	-1.0, 1.0, 1.0,		0.0, 1.0, 1.0,
	 1.0, 1.0, 1.0,		1.0, 1.0, 1.0
};
static u16 ibuf[3*2*6] = {
	2, 0, 6,	0, 4, 6,
	3, 1, 7,	1, 5, 7,

	0, 1, 5,	0, 5, 4,
	6, 3, 2,	6, 7, 3,

	0, 2, 1,	1, 2, 3,
	4, 5, 7,	7, 6, 4
};
static struct fstyle sty = {
	.vl = {-1.0, 0.0, 0.0,-1.0},
	.vr = { 1.0, 0.0, 0.0, 1.0},
	.vn = { 0.0,-s60, s30, 1.0},
	.vf = { 0.0, s60,-s30, 10000000.0},
	.vb = { 0.0,-s30,-s60,-1.0},
	.vt = { 0.0, s30, s60, 1.0},
	.vq = { 0.0, 0.0, 0.0, 0.0},
	.vc = { 0.0, -s60*4, s30*4, 0.0},
};
static mat4 cammvp;
void test_prepgl(struct glsrc* src)
{
	//shader
	src->vs = vshader;
	src->fs = fshader;
	src->shader_enq = 1;

	//argument
	matproj(cammvp, &sty);
	mat4_transpose(cammvp);

	src->arg[0].data = cammvp;
	src->arg[0].name = "cammvp";
	src->arg[0].fmt = 'm';

	//vertex
	struct vertex* vtx = &src->vtx[0];
	vtx->vbuf = vbuf;
	vtx->vbuf_len = 4*6*8;
	vtx->ibuf = ibuf;
	vtx->ibuf_len = 2*3*2*6;

	src->vbuf_enq = 1;
	src->ibuf_enq = 1;
}
void test_tickgl(struct entity* ent, struct supply* sup)
{
	float a = 2*PI*(timeread()%5000000)/5000000.0;
	float c = getcos(a);
	float s = getsin(a);

	sty.vc[0] = 4*c;
	sty.vc[1] = 4*s;
	sty.vc[2] = 2.0;

	sty.vn[0] = -sty.vc[0];
	sty.vn[1] = -sty.vc[1];
	sty.vn[2] = -sty.vc[2];
	vec3_normalize(sty.vn);
	sty.vf[0] = sty.vn[0];
	sty.vf[1] = sty.vn[1];
	sty.vf[2] = sty.vn[2];

	sty.vr[0] = -s;
	sty.vr[1] = c;
	sty.vr[2] = 0;
	sty.vl[0] = -sty.vr[0];
	sty.vl[1] = -sty.vr[1];
	sty.vl[2] = -sty.vr[2];

	vec3_cross(sty.vt, sty.vr, sty.vf);
	vec3_normalize(sty.vt);
	sty.vb[0] = -sty.vt[0];
	sty.vb[1] = -sty.vt[1];
	sty.vb[2] = -sty.vt[2];

	matproj(cammvp, &sty);
	mat4_transpose(cammvp);

	sup->gleasy_solid = ent->buf0;
}




void test_preppcm(struct pcmdata* pcm)
{
	int j;
	short* buf;
	pcm->fmt = hex32('s','1','6',0);
	pcm->chan = 1;
	pcm->rate = 44100;
	pcm->count = 65536;

	buf = pcm->buf;
	for(j=0;j<44100;j++)buf[j] = (short)(4096.0*getsin(j*tau/100));
}
void test_tickpcm(struct entity* ent, struct supply* sup)
{
	sup->pcmeasy_data = ent->buf1;
}





int test_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct supply* wnd = stack[sp-2].pchip;
	switch(wnd->fmt){
	case _gl41easy_:test_tickgl(ent,wnd);break;
	case _pcm_:test_tickpcm(ent,wnd);break;
	}
	return 0;
}
int test_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int test_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int test_linkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int test_search(struct entity* act)
{
	return 0;
}
int test_modify(struct entity* act)
{
	return 0;
}
int test_delete(struct entity* act)
{
	return 0;
}
int test_create(struct entity* act)
{
	act->buf0 = memorycreate(0x1000, 0);
	test_prepgl(act->buf0);

	act->buf1 = memorycreate(0x20000, 0);
	test_preppcm(act->buf1);

	return 0;
}




void test_free()
{
}
void test_init(void* addr)
{
}
