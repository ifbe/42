#include "libuser.h"
#define GERBUF buf0
#define DSTBUF buf1




int gcode_line(float* vbuf, int vlen, u8* str, int len)
{
	int j;
	int x,y,z;
	//say("%.*s\n", len, str);

	if('G' != str[0])return 0;
	if(('0' != str[1]) && ('1' != str[1]))return 0;

	x = y = z = 0;
	for(j=3;j<len;j++){
		//if(('F' == str[j]) | ('E' == str[j]))
		if('X' == str[j])x = j+1;
		if('Y' == str[j])y = j+1;
		if('Z' == str[j])z = j+1;
	}
	//say("x=%d,y=%d,z=%d\n",x,y,z);

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

	say("%f,%f,%f\n", vbuf[3*vlen+0], vbuf[3*vlen+1], vbuf[3*vlen+2]);
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




static void gcode_search(struct entity* act)
{
}
static void gcode_modify(struct entity* act)
{
}
static void gcode_delete(struct entity* act)
{
	if(0 == act)return;
}
static void gcode_create(struct entity* act, void* arg)
{
	int cnt;
	u8* buf;
	float* dst;
	if(0 == act)return;

	if(arg){
		buf = act->GERBUF = memorycreate(0x100000, 0);
		openreadclose(arg, 0, buf, 0x100000);
		//printmemory(buf, 0x100);

		dst = act->DSTBUF = memorycreate(0x100000, 0);
		cnt = gcode_parse(dst, buf);

		act->len = cnt;
		say("len=%x\n", 12*cnt);
	}
}




static void gcode_draw_vbo3d(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;
	carveline_rect(ctx, 0x404040, vc, vr, vf);

	int j;
	int cnt = act->len;
	float* buf = act->DSTBUF;
	for(j=0;j<cnt-1;j++){
		carveline(ctx, 0xff00ff, &buf[j*3], &buf[j*3+3]);
	}
}




static void gcode_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam, cam -> world
	struct entity* wnd;struct style* area;
	struct entity* wrd;struct style* camg;

	//scene -> terminal
	struct entity* scn;struct style* geom;
	struct entity* act;struct style* slot;

	if(stack){
		act = self->pchip;slot = self->pfoot;
		scn = peer->pchip;geom = peer->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		if('v' == len)gcode_draw_vbo3d(act,slot, scn,geom, wnd,area);
	}
}
static void gcode_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void gcode_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void gcode_start(struct halfrel* self, struct halfrel* peer)
{
}




void gcode_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'c', 'o', 'd', 'e', 0, 0, 0);

	p->oncreate = (void*)gcode_create;
	p->ondelete = (void*)gcode_delete;
	p->onsearch = (void*)gcode_search;
	p->onmodify = (void*)gcode_modify;

	p->onstart = (void*)gcode_start;
	p->onstop  = (void*)gcode_stop;
	p->onread  = (void*)gcode_read;
	p->onwrite = (void*)gcode_write;
}
