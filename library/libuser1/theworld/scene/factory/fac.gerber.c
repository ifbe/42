#include "libuser.h"
#define GERBUF buf0
#define DSTBUF buf1
#define CNTBUF data2




#define stride 4
int rs274x_edge(float* vbuf, int vlen)
{
	int j;
	float lx = 10000.0;
	float ly = 10000.0;
	float rx =-10000.0;
	float ry =-10000.0;

	for(j=0;j<vlen;j++){
		if(lx > vbuf[j*stride+0])lx = vbuf[j*stride+0];
		if(rx < vbuf[j*stride+0])rx = vbuf[j*stride+0];

		if(ly > vbuf[j*stride+1])ly = vbuf[j*stride+1];
		if(ry < vbuf[j*stride+1])ry = vbuf[j*stride+1];
	}

	vbuf[(vlen+0)*stride+0] = lx;
	vbuf[(vlen+0)*stride+1] = ly;
	vbuf[(vlen+0)*stride+2] = 0.0;
	vbuf[(vlen+1)*stride+0] = rx;
	vbuf[(vlen+1)*stride+1] = ry;
	vbuf[(vlen+1)*stride+2] = 0.0;
	say("%f,%f,    %f,%f\n", lx, ly, rx, ry);
	return 0;
}
int rs274x_line(float* vbuf, int vlen, u8* str, int len)
{
	int j;
	int x,y,d;
	if(('X' != str[0]) && ('Y' != str[0]) && ('D' != str[0]))return 0;

	x = y = d = 0;
	for(j=0;j<len;j++){
		if('X' == str[j])x = j+1;
		if('Y' == str[j])y = j+1;
		if('D' == str[j])d = j+1;
	}

	if(0 == x){
		if(vlen <= 0)vbuf[vlen*stride+0] = 0.0;
		else vbuf[vlen*stride+0] = vbuf[(vlen-1)*stride];
	}
	else{
		decstr2float(str+x, &vbuf[vlen*stride+0]);
		vbuf[vlen*stride+0] /= 1000.0;
	}

	if(0 == y){
		if(vlen <= 0)vbuf[vlen*stride+0] = 0.0;
		else vbuf[vlen*stride+1] = vbuf[(vlen-1)*stride+1];
	}
	else{
		decstr2float(str+y, &vbuf[vlen*stride+1]);
		vbuf[vlen*stride+1] /= 1000.0;
	}

	if(0 == d){
		if(vlen <= 0)vbuf[vlen*stride+0] = 0.0;
		else vbuf[vlen*stride+3] = vbuf[(vlen-1)*stride+3];
	}
	else decstr2float(str+d, &vbuf[vlen*stride+3]);

	vbuf[vlen*stride+2] = 0.0;
	say("%f,%f,%f,%f\n", vbuf[vlen*stride+0], vbuf[vlen*stride+1], vbuf[vlen*stride+2], vbuf[vlen*stride+3]);
	return 1;
}
int rs274x_parse(float* dst, u8* buf)
{
	int j,k,cnt;
	k = cnt = 0;
	for(j=0;j<0x10000;j++){
		if('\n' > buf[j])break;
		if('\r' == buf[j]){
			cnt += rs274x_line(dst, cnt, buf+k, j-k);
			j += 1;
			k = j+1;
			continue;
		}
		if('\n' == buf[j]){
			cnt += rs274x_line(dst, cnt, buf+k, j-k);
			k = j+1;
		}
	}
	return cnt;
}
/*
int rs274x_parse(float* vbuf, int vlen, u8* str, int len)
{
	int j;
	//say("%.*s\n", len, str);
	if('X' == str[0]){
		j = 1+decstr2float(str+1, &vbuf[vlen*stride]);
		vbuf[vlen*stride+0] /= 1000.0;

		if('Y' == str[j]){
			j += 1+decstr2float(str+1+j, &vbuf[vlen*stride+1]);
			vbuf[vlen*stride+1] /= 1000.0;

			if('D' == str[j]){
				decstr2float(str+j+1, &vbuf[vlen*stride+3]);
			}
		}
		else if('D' == str[j]){
			if(vlen > 0)vbuf[vlen*stride+1] = vbuf[vlen*stride-stride+1];
			else vbuf[vlen*stride+1] = 0.0;

			decstr2float(str+j+1, &vbuf[vlen*stride+3]);
		}
		else{
			if(vlen > 0)vbuf[vlen*stride+1] = vbuf[vlen*stride-stride+1];
			else vbuf[vlen*stride+1] = 0.0;
		}
	}
	else if('Y' == str[0]){
		if(vlen > 0)vbuf[vlen*stride] = vbuf[vlen*stride-stride+0];
		else vbuf[vlen*stride] = 0.0;

		j = decstr2float(str+1, &vbuf[vlen*stride+1]);
		vbuf[vlen*stride+1] /= 1000.0;

		if('D' == str[1+j]){
			j = decstr2float(str+2+j, &vbuf[vlen*stride+3]);
		}
	}
	else if('D' == str[0]){
		if(vlen > 0){
			vbuf[vlen*stride+0] = vbuf[vlen*stride-stride+0];
			vbuf[vlen*stride+1] = vbuf[vlen*stride-stride+1];
		}
		else{
			vbuf[vlen*stride+0] = 0.0;
			vbuf[vlen*stride+1] = 0.0;
		}
		j = decstr2float(str+1, &vbuf[vlen*stride+3]);
	}
	else return 0;

	vbuf[vlen*stride+2] = 0.0;
	say("%f,%f,%f,%f\n", vbuf[vlen*stride+0], vbuf[vlen*stride+1], vbuf[vlen*stride+2], vbuf[vlen*stride+3]);
	return 1;
}*/




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
	int cnt;
	u8* buf;
	float* dst;
	if(0 == act)return;

	if(arg){
		buf = act->GERBUF = memorycreate(0x100000, 0);
		openreadclose(arg, 0, buf, 0x100000);
		//printmemory(buf, 0x100);

		dst = act->DSTBUF = memorycreate(0x100000, 0);
		cnt = rs274x_parse(dst, buf);

		act->CNTBUF = cnt;
		say("len=%x\n", 4*4*cnt);

		rs274x_edge(dst, cnt);
	}
}




static void gerber_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_rect(ctx, 0xffffff, vc, vr, vf);

	int cnt = act->CNTBUF;
	float* buf = act->DSTBUF;
	for(j=0;j<3;j++){
		tc[j] = (buf[cnt*stride+j]+buf[(cnt+1)*stride+j])/2 - vt[j]/10000.0;
		tr[j] = 0.0;
		tf[j] = 0.0;
	}
	tr[0] = (buf[(cnt+1)*stride+0] - buf[(cnt+0)*stride+0])/2;
	tf[1] = (buf[(cnt+1)*stride+1] - buf[(cnt+0)*stride+1])/2;
	//say("%f,%f,%f,  %f,%f,%f,  %f,%f,%f\n", tc[0],tc[1],tc[2], tr[0],tr[1],tr[2], tf[0],tf[1],tf[2]);
	gl41solid_rect(ctx, 0x004000, tc, tr, tf);

	for(j=0;j<cnt-1;j++){
		if(buf[j*stride+3] < 0.5)rgb = 0;
		else if(buf[j*stride+3] < 1.5)rgb = 0xff0000;
		else if(buf[j*stride+3] < 2.5)rgb = 0x00ff00;
		else if(buf[j*stride+3] < 3.5)rgb = 0x0000ff;
		else rgb = 0xffffff;
		gl41line(ctx, rgb, &buf[(j+0)*stride], &buf[(j+1)*stride]);
	}
}




static void gerber_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* scn;struct style* geom;
	struct entity* wnd;struct style* area;

	if(stack&&('v'==key)){
		slot = stack[sp-1].pfoot;
		scn = stack[sp-2].pchip;geom = stack[sp-2].pfoot;;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		gerber_draw_gl41(ent,slot, scn,geom, wnd,area);
	}
}
static void gerber_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void gerber_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void gerber_linkup(struct halfrel* self, struct halfrel* peer)
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

	p->onlinkup = (void*)gerber_linkup;
	p->ondiscon = (void*)gerber_discon;
	p->ontaking = (void*)gerber_taking;
	p->ongiving = (void*)gerber_giving;
}
