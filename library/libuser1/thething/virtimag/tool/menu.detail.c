#include "libuser.h"




void drawentity(struct entity* win, int val, int x0, int y0, int xn, int yn)
{
	void* buf;
	int x,y;
	int xa,ya,xb,yb;
	drawline_rect(win, 0xc0c0c0, x0, y0, xn, yn);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			xa = ((4-x)*x0 + (x+0)*xn)/4;
			ya = ((4-y)*y0 + (y+0)*yn)/4;
			xb = ((3-x)*x0 + (x+1)*xn)/4;
			yb = ((3-y)*y0 + (y+1)*yn)/4;
			drawsolid_rect(win, 0x404040, xa+2, ya+2, xb-2, yb-2);

			if(y)continue;
			switch(x)
			{
				case 0:buf = &win->tier;break;
				case 1:buf = &win->type;break;
				case 2:buf = &win->fmt; break;
				//case 3:buf = &win->vfmt;break;
			}
			drawstring_fit(win, 0xffffff, xa, ya, xb, yb, buf, 8);
		}
	}
}
void carveentity(struct entity* win, int val, vec3 vc, vec3 vr, vec3 vf)
{
	int x,y;
	void* buf;
	vec3 tc,tr,tf,tu;
	gl41line_rect(win, 0xffffff, vc, vr, vf);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			tc[0] = vc[0] + (x-1.5)*vr[0]/2 + (1.5-y)*vf[0]/2;
			tc[1] = vc[1] + (x-1.5)*vr[1]/2 + (1.5-y)*vf[1]/2;
			tc[2] = -0.7;
			tr[0] = vr[0]/4.1;
			tr[1] = 0.0;
			tr[2] = 0.0;
			tf[0] = 0.0;
			tf[1] = vf[1]/4.1;
			tf[2] = 0.0;
			gl41solid_rect(win, 0xffffff, tc, tr, tf);

			if(y)continue;
			switch(x)
			{
				case 0:buf = &win->tier;break;
				case 1:buf = &win->type;break;
				case 2:buf = &win->fmt; break;
				//case 3:buf = &win->vfmt;break;
			}

			tc[2] = -0.8;
			tr[0] = vr[0]/4;
			tr[1] = 0.0;
			tr[2] = 0.0;
			tf[0] = 0.0;
			tf[1] = vf[1]/4;
			tf[2] = 0.0;
			carvestring_center(win, 0x000000, tc, tr, tf, buf, 8);
		}
	}
}




void detail_draw_gl41_node(struct entity* ctx, struct entity* one, vec3 vc, vec3 vr, vec3 vf)
{
	int j;
	vec3 tr,tf;
	for(j=0;j<3;j++){
		tr[j] = vr[j]*2;
		tf[j] = vf[j]*2;
	}
	gl41line_circle(ctx, 0x404040, vc,tr,tf);
	carvestring_center(ctx, 0xff0000, vc,vr,vf, (void*)&one->fmt, 8);
}
void detail_draw_gl41_foot(struct entity* ctx, void* aaa, void* bbb, vec3 src, vec3 dst, vec3 vr, vec3 vf, vec3 vt)
{
	int j;
	vec3 tc,t0,tr,tf;
	vec3 d1,d2,t1,t2;
	gl41line(ctx, 0xff0000, src, dst);

	for(j=0;j<3;j++){
		tc[j] = (src[j]+dst[j])/2;
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
		d2[j] = d1[j] = (src[j]-dst[j])/3;
	}
	quaternion_operation(d1, vt, PI/12);
	quaternion_operation(d2, vt,-PI/12);

	for(j=0;j<3;j++){
		t1[j] = tc[j] + d1[j];
		t2[j] = tc[j] + d2[j];
	}
	gl41line_triangle(ctx, 0xff0000, tc, t1, t2);
	for(j=0;j<3;j++)t0[j] = (src[j]*2 + dst[j]*1)/3;
	carvestring_center(ctx, 0xff0000, t0, tr, tf, aaa, 4);

	for(j=0;j<3;j++){
		t1[j] = tc[j] - d1[j];
		t2[j] = tc[j] - d2[j];
	}
	gl41line_triangle(ctx, 0xff0000, tc, t1, t2);
	for(j=0;j<3;j++)t0[j] = (src[j]*1 + dst[j]*2)/3;
	carvestring_center(ctx, 0xff0000, t0, tr, tf, bbb, 4);
}
int detail_draw_gl41(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int j,k,cnt;
	float a,c,s;
	struct relation* rel;
	struct halfrel* self[8];
	struct halfrel* peer[8];

	vec3 tc, tr, tf, tt;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vt = geom->f.vt;
	gl41line_rect(ctx, 0xffffff, vc,vr,vf);

	for(j=0;j<3;j++){
		tr[j] = vr[j]/16;
		tf[j] = vf[j]/16;
	}
	detail_draw_gl41_node(ctx, act, vc,tr,tf);

	cnt = 0;
	rel = act->orel0;
	while(1){
		if(0 == rel)break;
		if(cnt >= 8)break;
		self[cnt] = (void*)(rel->src);
		peer[cnt] = (void*)(rel->dst);
		cnt++;
		rel = samesrcnextdst(rel);
	}

	rel = act->irel0;
	while(1){
		if(0 == rel)break;
		if(cnt >= 8)break;
		self[cnt] = (void*)(rel->dst);
		peer[cnt] = (void*)(rel->src);
		cnt++;
		rel = samedstnextsrc(rel);
	}

	for(k=0;k<cnt;k++){
		a = 2*PI*k/cnt;
		c = cosine(a)/2;
		s = sine(a)/2;
		for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*c + vf[j]*s;

		detail_draw_gl41_node(ctx, peer[k]->pchip, tc, tr, tf);
		detail_draw_gl41_foot(ctx, &self[k]->flag, &peer[k]->flag, vc, tc, vr, vf, vt);
	}
	return 0;
}
int detail_draw_pixel(
	struct entity* act, struct style* slot,
	struct entity* win, struct style* geom)
{
	return 0;
}




static void detail_read(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		detail_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static int detail_write(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 1;
}
static void detail_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void detail_linkup(struct halfrel* self, struct halfrel* peer)
{
    say("@detail_linkup\n");
}




void detail_search(struct entity* act)
{
}
void detail_modify(struct entity* act)
{
}
void detail_delete(struct entity* act)
{
}
void detail_create(struct entity* act, void* str)
{
    say("@detail_create\n");
}




void detail_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('d', 'e', 't', 'a', 'i', 'l', 0, 0);

	p->oncreate = (void*)detail_create;
	p->ondelete = (void*)detail_delete;
	p->onsearch = (void*)detail_search;
	p->onmodify = (void*)detail_modify;

	p->onlinkup = (void*)detail_linkup;
	p->ondiscon = (void*)detail_discon;
	p->onread  = (void*)detail_read;
	p->onwrite = (void*)detail_write;
}
