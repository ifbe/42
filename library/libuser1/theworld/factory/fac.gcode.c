#include "libuser.h"
#define GERBUF buf0
#define DSTBUF buf1
#define CNTBUF data2




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

		act->CNTBUF = cnt;
		say("len=%x\n", 12*cnt);
	}
}




static void gcode_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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




static void gcode_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack&&('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		gcode_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}
static void gcode_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void gcode_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void gcode_linkup(struct halfrel* self, struct halfrel* peer)
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

	p->onlinkup = (void*)gcode_linkup;
	p->ondiscon = (void*)gcode_discon;
	p->ontaking = (void*)gcode_taking;
	p->ongiving = (void*)gcode_giving;
}
