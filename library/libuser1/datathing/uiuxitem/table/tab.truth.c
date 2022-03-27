#include "libuser.h"
#define _karnaugh_ hex32('k','a','r','n')
void gl41data_convert(_obj* wnd, struct style* area, struct event* ev, vec3 v);
void gl41data_before(_obj* wnd);
void gl41data_after(_obj* wnd);
void gl41data_01cam(_obj* wnd);




static int truth_search(_obj* act, u32 foot, struct halfrel* self[], struct halfrel* peer[])
{
	struct relation* rel;
	_obj* world;

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		world = (void*)(rel->srcchip);
		if(_virtual_ == world->type){
			self[0] = (void*)&rel->dstchip;
			peer[0] = (void*)&rel->srcchip;
			return 1;
		}
		rel = samedstnextsrc(rel);
	}
	return 0;
}
static int truth_modify(_obj* act)
{
	return 0;
}
static int truth_delete(_obj* act)
{
	return 0;
}
static int truth_create(_obj* act, u8* str)
{
	int j;
	u8* out = (void*)act->priv_256b;
	for(j=0;j<16;j++)out[j] = getrandom()&1;
	return 0;
}




void truth_draw_gl41(
	_obj* act, struct style* part,
	_obj* scn, struct style* geom,
	_obj* wnd, struct style* area)
{
	u8* out;
	u8 ch[8];
	int x,y,j,rgb;
	vec3 tc,tr,tf;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	for(j=0;j<3;j++)tc[j] = vc[j] -vt[j]/100.0;
	gl41opaque_rect(wnd, 0x80800000, tc, vr, vf);

	out = (void*)act->priv_256b;
	for(j=0;j<3;j++){
		tr[j] = vr[j]/8.08;
		tf[j] = vf[j]/17.17;
	}
	for(y=0;y<16+1;y++){
		if(0 == y){
			ch[0] = 'A';
			ch[1] = 'B';
			ch[2] = 'C';
			ch[3] = 'D';
			ch[5] = 'Y';
		}
		else{
			ch[0] = '0' + (((y-1)>>3)&1);
			ch[1] = '0' + (((y-1)>>2)&1);
			ch[2] = '0' + (((y-1)>>1)&1);
			ch[3] = '0' + (((y-1)>>0)&1);
			ch[5] = '0' + out[y-1];
		}
		for(x=0;x<8;x++){
			for(j=0;j<3;j++)tc[j] = vc[j] +vr[j]*(x*2-7)/8 + vf[j]*(16-2*y)/17;
			if((x == act->whdf.ix0)&&(y == act->whdf.iy0))rgb = 0xff00ff;
			else rgb = 0x808080;
			gl41line_rect(wnd, rgb, tc, tr, tf);
			if(4 == x)continue;
			if(6 <= x)continue;
			gl41ascii_center(wnd, rgb, tc, tr, tf, ch[x]);
		}
	}
}
void truth_draw_pixel(_obj* win, struct style* sty)
{
}
void truth_draw_html(_obj* win, struct style* sty)
{
}
void truth_draw_tui(_obj* win, struct style* sty)
{
}
void truth_draw_cli(_obj* win, struct style* sty)
{
}




static void truth_read_bywnd(_obj* ent,struct style* slot, _obj* wnd, struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;
	gl41data_before(wnd);
	truth_draw_gl41(ent, 0, 0,(void*)&fs, wnd,area);
	gl41data_01cam(wnd);
	gl41data_after(wnd);
}
static void truth_write_bywnd(_obj* ent,struct style* slot, _obj* wnd, struct style* area,
	_syn* stack,int sp, struct event* ev,int len)
{
	if('p' == (ev->what&0xff)){
		vec3 xyz;
		gl41data_convert(wnd, area, ev, xyz);
		ent->whdf.ix0 = (int)(8*xyz[0]);
		ent->whdf.iy0 = (int)(17*(1.0-xyz[1]));

		if(0x2b70 == ev->what){
			u8* out = (void*)ent->priv_256b;
			int x = ent->whdf.ix0;
			int y = ent->whdf.iy0;
			if((5 == x)&&(y > 0)&&(y <= 16))out[y-1] ^= 1;
			give_data_into_peer(ent,_karnaugh_, stack,sp, 0,0, out,16);
		}
	}
}
static int truth_write_bykarnaugh(_obj* ent, u8* i)
{
	u8* o = (void*)ent->priv_256b;
	printmemory(i, 16);

	o[0] = i[0];
	o[1] = i[1];
	o[3] = i[2];
	o[2] = i[3];
	o[4] = i[4];
	o[5] = i[5];
	o[7] = i[6];
	o[6] = i[7];

	o[12] = i[8];
	o[13] = i[9];
	o[15] = i[10];
	o[14] = i[11];
	o[ 8] = i[12];
	o[ 9] = i[13];
	o[11] = i[14];
	o[10] = i[15];
	return 0;
}




static int truth_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	switch(wnd->hfmt){
	case _gl41list_:
		truth_read_bywnd(ent,slot, wnd,area);break;
	}
	return 0;
}
static int truth_giving(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(_karnaugh_ == stack[sp-1].foottype){
		return truth_write_bykarnaugh(ent, buf);
	}

	_obj* wnd = stack[sp-2].pchip;
	struct style* area = stack[sp-2].pfoot;
	switch(wnd->hfmt){
	case _gl41list_:truth_write_bywnd(ent,slot, wnd,area, stack,sp, buf,len);break;
	}
	return 0;
}
static int truth_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int truth_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




void truth_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('t','r','u','t','h', 0, 0, 0);

	p->oncreate = (void*)truth_create;
	p->ondelete = (void*)truth_delete;
	p->onreader = (void*)truth_search;
	p->onwriter = (void*)truth_modify;

	p->onattach = (void*)truth_attach;
	p->ondetach = (void*)truth_detach;
	p->ontaking = (void*)truth_taking;
	p->ongiving = (void*)truth_giving;
}
