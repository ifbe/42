#include "libuser.h"
#define GERBUF listptr.buf0
#define DSTBUF listptr.buf1
#define CNTBUF listu64.data4




int gcode_line(float* vbuf, int vlen, u8* str, int len)
{
	int j;
	int x,y,z;
	//logtoall("%.*s\n", len, str);

	if('G' != str[0])return 0;
	if(('0' != str[1]) && ('1' != str[1]))return 0;

	x = y = z = 0;
	for(j=3;j<len;j++){
		//if(('F' == str[j]) | ('E' == str[j]))
		if('X' == str[j])x = j+1;
		if('Y' == str[j])y = j+1;
		if('Z' == str[j])z = j+1;
	}
	//logtoall("x=%d,y=%d,z=%d\n",x,y,z);

	if(0 == x){
		if(vlen <= 0)vbuf[3*vlen+0] = 0;
		else vbuf[3*vlen+0] = vbuf[3*vlen-3];
	}
	else decstr2float(str+x, &vbuf[3*vlen+0]);

	if(0 == y){
		if(vlen <= 0)vbuf[3*vlen+1] = 0;
		else vbuf[3*vlen+1] = vbuf[3*vlen-3+1];
	}
	else decstr2float(str+y, &vbuf[3*vlen+1]);

	if(0 == z){
		if(vlen <= 0)vbuf[3*vlen+2] = 0;
		else vbuf[3*vlen+2] = vbuf[3*vlen-3+2];
	}
	else decstr2float(str+z, &vbuf[3*vlen+2]);

	logtoall("%f,%f,%f\n", vbuf[3*vlen+0], vbuf[3*vlen+1], vbuf[3*vlen+2]);
	return 1;
}
int gcode_parse(float* dst, u8* buf)
{
	int j,k,cnt;
	k = cnt = 0;
	for(j=0;j<0x100000;j++){
		if('\n' > buf[j])break;
		if('\r' == buf[j]){
			cnt += gcode_line(dst, cnt, buf+k, j-k);
			j += 1;
			k = j+1;
			continue;
		}
		if('\n' == buf[j]){
			cnt += gcode_line(dst, cnt, buf+k, j-k);
			k = j+1;
		}
	}
	return cnt;
}




static void gcode_search(_obj* act)
{
}
static void gcode_modify(_obj* act)
{
}
static void gcode_delete(_obj* act)
{
	if(0 == act)return;
}
static void gcode_create(_obj* act, void* arg)
{
	int cnt;
	u8* buf;
	float* dst;
	if(0 == act)return;

	if(arg){
		buf = act->GERBUF = memoryalloc(0x100000, 0);
		openreadclose(arg, 0, buf, 0x100000);
		//printmemory(buf, 0x100);

		dst = act->DSTBUF = memoryalloc(0x100000, 0);
		cnt = gcode_parse(dst, buf);

		act->CNTBUF = cnt;
		logtoall("len=%x\n", 12*cnt);
	}
}




static void gcode_draw_gl41(
	_obj* act, struct style* slot,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41line_rect(ctx, 0x404040, vc, vr, vf);

	int j;
	int cnt = act->CNTBUF;
	float* buf = act->DSTBUF;
	for(j=0;j<cnt-1;j++){
		gl41line(ctx, 0xff00ff, &buf[j*3], &buf[j*3+3]);
	}
}




static void gcode_read_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* scn;struct style* geom;
	_obj* wnd;struct style* area;

	scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	gcode_draw_gl41(ent,slot, scn,geom, wnd,area);
}





static void gcode_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		gcode_read_byworld_bycam_bywnd(ent,slot, stack,sp);
	}
}
static void gcode_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void gcode_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void gcode_attach(struct halfrel* self, struct halfrel* peer)
{
}




void gcode_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('g', 'c', 'o', 'd', 'e', 0, 0, 0);

	p->oncreate = (void*)gcode_create;
	p->ondelete = (void*)gcode_delete;
	p->onreader = (void*)gcode_search;
	p->onwriter = (void*)gcode_modify;

	p->onattach = (void*)gcode_attach;
	p->ondetach = (void*)gcode_detach;
	p->ontaking = (void*)gcode_taking;
	p->ongiving = (void*)gcode_giving;
}
