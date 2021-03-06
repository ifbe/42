#include "libuser.h"
#define nbuf buf0
#define wbuf buf1
#define vbuf buf2
#define ibuf buf3




static void dancemat_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

	drawline(win, 0x6a4b23, cx-ww, cy+hh, cx+ww, cy+hh);
	drawsolid_rect(win, 0x404040, cx-ww/4, cy, cx+ww/4, cy+hh);

	drawsolid_rect(win, 0x008000, cx-ww, cy-hh*1/4, cx+ww, cy);
	drawsolid_rect(win, 0x008000, cx-ww*3/4, cy-hh/2, cx+ww*3/4, cy-hh/4);
	drawsolid_rect(win, 0x008000, cx-ww/2, cy-hh*3/4, cx+ww/2, cy-hh/2);
	drawsolid_rect(win, 0x008000, cx-ww/4, cy-hh, cx+ww/4, cy-hh*3/4);
}
static void dancemat_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* none)
{
	int x,y;
	int j,cnt;
	float* vbuf;
	short* nbuf;
	short* ibuf;

	vec3 cc;
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41opaque_rect(ctx, 0x80202020, vc, vr, vf);

#define sixplus3sqrt2 10.242640687119286
	cnt = 0;
	vbuf = act->vbuf;
	for(y=0;y<6;y++){
		for(x=0;x<6;x++){
			cc[0] = vc[0] + (x*4-10)*vr[0]/12 + (y*4-10)*vf[0]/12;
			cc[1] = vc[1] + (x*4-10)*vr[1]/12 + (y*4-10)*vf[1]/12;
			cc[2] = vc[2] + (x*4-10)*vr[2]/12 + (y*4-10)*vf[2]/12;

			vbuf[3*cnt+0] = cc[0] - vr[0]/sixplus3sqrt2;
			vbuf[3*cnt+1] = cc[1] - vr[1]/sixplus3sqrt2;
			vbuf[3*cnt+2] = cc[2] - vr[2]/sixplus3sqrt2;
			cnt += 1;

			vbuf[3*cnt+0] = cc[0] + vr[0]/sixplus3sqrt2;
			vbuf[3*cnt+1] = cc[1] + vr[1]/sixplus3sqrt2;
			vbuf[3*cnt+2] = cc[2] + vr[2]/sixplus3sqrt2;
			cnt += 1;

			vbuf[3*cnt+0] = cc[0] - vf[0]/sixplus3sqrt2;
			vbuf[3*cnt+1] = cc[1] - vf[1]/sixplus3sqrt2;
			vbuf[3*cnt+2] = cc[2] - vf[2]/sixplus3sqrt2;
			cnt += 1;

			vbuf[3*cnt+0] = cc[0] + vf[0]/sixplus3sqrt2;
			vbuf[3*cnt+1] = cc[1] + vf[1]/sixplus3sqrt2;
			vbuf[3*cnt+2] = cc[2] + vf[2]/sixplus3sqrt2;
			cnt += 1;
		}
	}

	tr[0] = vr[0]/96;
	tr[1] = vr[1]/96;
	tr[2] = vr[2]/96;
	tf[0] = vf[0]/96;
	tf[1] = vf[1]/96;
	tf[2] = vf[2]/96;
	tu[0] = vu[0]/96;
	tu[1] = vu[1]/96;
	tu[2] = vu[2]/96;
	nbuf = act->nbuf;
	for(x=0;x<cnt;x++){
		vbuf[x*3+0] += nbuf[x] * vu[0]/32768.0;
		vbuf[x*3+1] += nbuf[x] * vu[1]/32768.0;
		vbuf[x*3+2] += nbuf[x] * vu[2]/32768.0;
		gl41solid_sphere(ctx, act->ix0, &vbuf[x*3], tr, tf, tu);
	}

	ibuf = act->ibuf;
	for(j=0;j<200+48;j++){
		x = ibuf[j*2+0];
		y = ibuf[j*2+1];
		gl41line(ctx, act->iy0, &vbuf[x*3], &vbuf[y*3]);
	}
}
static void dancemat_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dancemat_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dancemat_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void dancemat_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void dancemat_write_data(struct entity* ent, struct entity* src, u8* buf, int len)
{
	int j,k;
	int cnt;
	u64 data;
	short* nbuf;
	if(0 == buf)return;
	if(0 != ncmp(buf, "FreeRunners:", 12))return;

	buf += 12;
	len -= 12;
	//say("(len=%d)", len);

	nbuf = ent->nbuf;
	if(0 == nbuf)return;

	j = k = cnt = 0;
	while(1){
		if(k >= len)break;
		if(cnt >= 144)break;

		data = 0;
		j = decstr2data(buf+k, &data);
		if(j <= 0)break;
		if(data < 0)break;
		if(data >= 32767)break;

		data *= 128;
		switch(cnt&0x3){
			case 0:
			case 3:nbuf[cnt+0] = data;break;
			case 1:nbuf[cnt+1] = data;break;
			case 2:nbuf[cnt-1] = data;break;
		}
		cnt += 1;
		k += j+1;

		//say("%d,", data);
	}
	//say("<cnt=%d>\n", cnt);
}




static void dancemat_wrl_cam_wnd(_ent* ent,void* slot, _syn* stack,int sp)
{
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	dancemat_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void dancemat_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].flag){
	}

	//caller defined behavior
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		dancemat_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void dancemat_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* src = stack[sp-2].pchip;
	switch(src->tier){
		case _sys_:
		case _art_:dancemat_write_data(ent, src, buf, len);break;
	}
}
static void dancemat_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void dancemat_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void dancemat_search(struct entity* act)
{
}
static void dancemat_modify(struct entity* act)
{
}
static void dancemat_delete(struct entity* act)
{
	if(0 == act)return;
	//if(_copy_ == act->type)memorydelete(act->buf);
}
static void dancemat_create(struct entity* act, u8* arg, int argc, u8** argv)
{
	u64 tmp;
	int x,y,j,k;
	short* nbuf;
	short* ibuf;
	if(0 == act)return;

	act->ix0 = 0x808080;
	act->iy0 = 0xff0000;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "ballcolor:", 10)){
			k = 10;
			while(0x20 == argv[j][k])k++;
			hexstr2data(argv[j]+k, &tmp);
			act->ix0 = tmp;
		}
		if(0 == ncmp(argv[j], "linecolor:", 10)){
			k = 10;
			while(0x20 == argv[j][k])k++;
			hexstr2data(argv[j]+k, &tmp);
			act->iy0 = tmp;
		}
	}

	act->vbuf = memorycreate(0x1000, 0);

	nbuf = act->nbuf = memorycreate(0x1000, 0);
	for(j=0;j<288;j++){
		nbuf[j] = (getrandom() & 0xfff);
		//say("%d\n", nbuf[j]);
	}

	ibuf = act->ibuf = memorycreate(0x1000, 0);
	j = 0;
	for(y=0;y<120;y+=24){
		for(x=1;x<18;x+=4){
			k = y+x;
			ibuf[j+ 0] = k+0;
			ibuf[j+ 1] = k+3;
			ibuf[j+ 2] = k+3;
			ibuf[j+ 3] = k+6;
			ibuf[j+ 4] = k+6;
			ibuf[j+ 5] = k+29;
			ibuf[j+ 6] = k+29;
			ibuf[j+ 7] = k+27;
			ibuf[j+ 8] = k+27;
			ibuf[j+ 9] = k+24;
			ibuf[j+10] = k+24;
			ibuf[j+11] = k+25;
			ibuf[j+12] = k+25;
			ibuf[j+13] = k+2;
			ibuf[j+14] = k+2;
			ibuf[j+15] = k+0;
			j+=16;
		}
	}
	for(x=2;x<23;x+=4){
		ibuf[j+0] = x;
		ibuf[j+1] = x-2;
		ibuf[j+2] = x;
		ibuf[j+3] = x-1;
		//say("%d:%d,%d,%d,%d\n", j, x, x-2, x, x-1);
		j+=4;
	}
	for(x=123;x<144;x+=4){
		ibuf[j+0] = x;
		ibuf[j+1] = x-3;
		ibuf[j+2] = x;
		ibuf[j+3] = x-2;
		//say("%d:%d,%d,%d,%d\n", j, x, x-2, x, x-1);
		j+=4;
	}
	for(y=0;y<121;y+=24){
		ibuf[j+0] = y;
		ibuf[j+1] = y+2;
		ibuf[j+2] = y;
		ibuf[j+3] = y+3;
		//say("%d:%d,%d,%d,%d\n", j, y, y+2, y, y+3);
		j+=4;
	}
	for(y=21;y<144;y+=24){
		ibuf[j+0] = y;
		ibuf[j+1] = y+1;
		ibuf[j+2] = y;
		ibuf[j+3] = y+2;
		//say("%d:%d,%d,%d,%d\n", j, y, y+2, y, y+3);
		j+=4;
	}
}




void dancemat_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('d','a','n','c','e','m','a','t');

	p->oncreate = (void*)dancemat_create;
	p->ondelete = (void*)dancemat_delete;
	p->onsearch = (void*)dancemat_search;
	p->onmodify = (void*)dancemat_modify;

	p->onlinkup = (void*)dancemat_linkup;
	p->ondiscon = (void*)dancemat_discon;
	p->ontaking = (void*)dancemat_taking;
	p->ongiving = (void*)dancemat_giving;
}
