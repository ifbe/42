#include "libuser.h"
#define GERBUF buf0
#define DSTBUF buf1




int rs274x_parse(float* vbuf, int vlen, u8* str, int len)
{
	int j;
	//say("%.*s\n", len, str);
	if('X' == str[0]){
		j = decstr2float(str+1, &vbuf[vlen*3]);
		vbuf[vlen*3+0] /= 1000.0;

		if('Y' == str[1+j]){
			j = decstr2float(str+2+j, &vbuf[vlen*3+1]);
			vbuf[vlen*3+1] /= 1000.0;
		}
		else{
			if(vlen > 0)vbuf[vlen*3+1] = vbuf[vlen*3-3+1];
			else vbuf[vlen*3+1] = 0.0;
		}
	}
	else if('Y' == str[0]){
		if(vlen > 0)vbuf[vlen*3] = vbuf[vlen*3-3+0];
		else vbuf[vlen*3] = 0.0;

		j = decstr2float(str+1, &vbuf[vlen*3+1]);
		vbuf[vlen*3+1] /= 1000.0;
	}
	else return 0;

	vbuf[vlen*3+2] = 0.0;
	say("%f,%f,%f\n", vbuf[vlen*3+0], vbuf[vlen*3+1], vbuf[vlen*3+2]);
	return 1;
}




static void gerber_search(struct entity* act)
{
}
static void gerber_modify(struct entity* act)
{
}
static void gerber_delete(struct entity* act)
{
	if(0 == act)return;
}
static void gerber_create(struct entity* act, void* arg)
{
	int j,k,cnt;
	u8* buf;
	float* dst;
	if(0 == act)return;

	if(arg){
		buf = act->GERBUF = memorycreate(0x100000, 0);
		openreadclose(arg, 0, buf, 0x100000);
		//printmemory(buf, 0x100);

		dst = act->DSTBUF = memorycreate(0x100000, 0);

		k = cnt = 0;
		for(j=0;j<0x10000;j++){
			if('\n' > buf[j])break;
			if('\r' == buf[j]){
				cnt += rs274x_parse(dst, cnt, buf+k, j-k);
				j += 1;
				k = j+1;
				continue;
			}
			if('\n' == buf[j]){
				cnt += rs274x_parse(dst, cnt, buf+k, j-k);
				k = j+1;
			}
		}
		act->len = cnt;
	}
}




static void gerber_draw_vbo3d(
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




static void gerber_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
		if('v' == len)gerber_draw_vbo3d(act,slot, scn,geom, wnd,area);
	}
}
static void gerber_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void gerber_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void gerber_start(struct halfrel* self, struct halfrel* peer)
{
}




void gerber_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('g', 'e', 'r', 'b', 'e','r', 0, 0);

	p->oncreate = (void*)gerber_create;
	p->ondelete = (void*)gerber_delete;
	p->onsearch = (void*)gerber_search;
	p->onmodify = (void*)gerber_modify;

	p->onstart = (void*)gerber_start;
	p->onstop  = (void*)gerber_stop;
	p->onread  = (void*)gerber_read;
	p->onwrite = (void*)gerber_write;
}
