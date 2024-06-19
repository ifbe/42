#include "libuser.h"
#define s30 0.5
#define s45 0.7071067811865476
#define s60 0.8660254037844386
void world2clip_projznzp(mat4 mat, struct fstyle* frus);
void clip2world_projznzp(mat4 mat, struct fstyle* frus);




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
void test_prepgl(struct gl41data* data)
{
	//shader
	data->src.vs = vshader;
	data->src.fs = fshader;

	//vertex
	struct vertex* vtx = &data->src.vtx[0];
	vtx->vbuf = vbuf;
	vtx->vbuf_len = 4*6*8;
	vtx->ibuf = ibuf;
	vtx->ibuf_len = 2*3*2*6;

	//argument
	world2clip_projznzp(cammvp, &sty);
	mat4_transpose(cammvp);

	data->dst.arg[0].data = cammvp;
	data->dst.arg[0].name = "cammvp";
	data->dst.arg[0].fmt = 'm';

	data->src.shader_enq = 1;
	data->src.vbuf_enq = 1;
	data->src.ibuf_enq = 1;
}
void test_tickgl(_obj* ent,void* slot, _obj* wnd,void* area)
{
	float a = 2*PI*(timeread_us()%5000000)/5000000.0;
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

	world2clip_projznzp(cammvp, &sty);
	mat4_transpose(cammvp);

	wnd->gl41easy.solid = ent->listptr.buf0;
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
void test_tickpcm(_obj* ent,void* slot, _obj* sup,void* geom)
{
	sup->pcmeasy.data = ent->listptr.buf1;
}




static void test_read_bywnd(_obj* ent,void* slot, _obj* wnd,void* area)
{
	switch(wnd->vfmt){
	case _rgba8888_:
		break;
	case _gl41easy_:
		test_tickgl(ent,slot, wnd,area);
	case _gl41list_:
		break;
	}
}
static void test_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
}




void test_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _pcm_:
		test_tickpcm(ent,slot, caller,area);
		break;
	case _wnd_:
	case _render_:
		test_read_bywnd(ent,slot, caller,area);
		break;
	default:
		test_read_byworld_bycam_bywnd(ent,slot, stack,sp);
		break;
	}
}
int test_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	return 0;
}
int test_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int test_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int test_search(_obj* act)
{
	return 0;
}
int test_modify(_obj* act)
{
	return 0;
}
int test_delete(_obj* act)
{
	return 0;
}
int test_create(_obj* act)
{
	act->listptr.buf0 = memoryalloc(0x1000, 0);
	test_prepgl(act->listptr.buf0);

	act->listptr.buf1 = memoryalloc(0x20000, 0);
	test_preppcm(act->listptr.buf1);

	return 0;
}




void test_exit()
{
}
void test_init(void* addr)
{
}
