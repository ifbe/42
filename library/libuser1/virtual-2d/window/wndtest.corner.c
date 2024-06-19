#include "libuser.h"
void gl41data_whcam(_obj* wnd, struct style* area);




struct privdata{
	struct xyzwpair touch[7];
	struct xyzwpair mouse[1];
	u8 leftbot;
	u8 rightbot;
	u8 lefttop;
	u8 righttop;
};
int corner_bestfit(int x, int y)
{
	if(x<y)return x/16;
	else return y/16;
}


/*
void corner_gl41_drag_lefttop(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	float x, float y)
{
	vec3 tc,tr,tf;

	tc[0] = x;
	tc[1] = y;
	tc[2] = -0.99;
	tr[0] = 1.0+tc[0];
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = tc[1]-1.0;
	tf[2] = 0.0;
	gl41line_rect(win, 0xffffff, tc, tr, tf);
	gl41solid_circle(win, 0xffffff, tc, tr, tf);
}
void corner_gl41_drag_righttop(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	float x, float y)
{
	vec3 tc,tr,tf;

	tc[0] = x;
	tc[1] = y;
	tc[2] = -0.99;

	tr[0] = -1.0;
	tr[1] = -1.0;
	tr[2] = -0.99;
	tf[0] = 1.0;
	tf[1] = -1.0;
	tf[2] = -0.99;
	gl41solid_triangle(win, 0x0000ff, tc, tr, tf);

	tf[0] = -1.0;
	tf[1] = 1.0;
	tf[2] = -0.99;
	gl41solid_triangle(win, 0x00ffff, tc, tr, tf);

	tr[0] = 1.0;
	tr[1] = 1.0;
	tr[2] = -0.99;
	gl41solid_triangle(win, 0xff0000, tc, tr, tf);

	tf[0] = 1.0;
	tf[1] = -1.0;
	tf[2] = -0.99;
	gl41solid_triangle(win, 0xffff00, tc, tr, tf);
}
void corner_gl41_drag_leftbot(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	float x, float y)
{
	vec3 tc, tr, tf;

	tc[0] = x;
	tc[1] = 0.0;
	tc[2] = -0.9;
	tr[0] = 1.0+tc[0];
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0;
	tf[2] = 0.0;
	gl41solid_rect(win, 0x404040, tc, tr, tf);

	tc[0] = 0.0;
	tc[1] = y;
	tr[0] = 1.0;
	tr[1] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0+tc[1];
	gl41solid_rect(win, 0x404040, tc, tr, tf);
}
void corner_gl41_drag_rightbot(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	float x0, float y0, float xn, float yn)
{
	vec3 tc, tr, tf;

	tr[0] = x0;
	tr[1] = -1.0;
	tr[2] = -0.9;
	tf[0] = 1.0;
	tf[1] = y0;
	tf[2] = -0.9;
	tc[0] = xn;
	tc[1] = yn;
	tc[2] = -0.9;
	gl41solid_triangle(win, 0x808080, tc, tr, tf);
	tc[0] = 1.0;
	tc[1] = -1.0;
	gl41solid_triangle(win, 0x000000, tc, tr, tf);
}
void corner_gl41_drag(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int j;
	float w,h,c;
	float x0,y0,xn,yn;

	w = win->whdf.width;
	h = win->whdf.height;
	if(w<h)c = w / 32;
	else c = h / 32;

	struct privdata* priv = (void*)act->priv_256b;
	if(priv->mouse[0].z0){
		x0 = priv->mouse[0].x0;
		y0 = priv->mouse[0].y0;
		xn = priv->mouse[0].xn;
		yn = priv->mouse[0].yn;
	}
	else if(priv->touch[0].z0){
		x0 = priv->touch[0].x0;
		y0 = priv->touch[0].y0;
		xn = priv->touch[0].xn;
		yn = priv->touch[0].yn;
	}
	else return;


	if(y0 < c){
		if(x0 < c){
			corner_gl41_drag_lefttop(
				act, pin, win, sty,
				xn/w - 1.0, 1.0 - yn/h
			);
		}
		else if(x0+c > w){
			corner_gl41_drag_righttop(
				act, pin, win, sty,
				2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
	else if(y0+c > h){
		if(x0 < c){
			corner_gl41_drag_leftbot(
				act, pin, win, sty,
				xn/w - 1.0, -yn/h
			);
		}
		else if(x0+c > w){
			x0 = ((h-yn)*(h-yn)/(xn-w) + (w+xn)) / 2.0;
			x0 = 2*x0/w - 1.0;
			y0 = ((xn-w)*(xn-w)/(yn-h) + (yn+h)) / 2.0;
			y0 = 1.0 - 2*y0/h;
			corner_gl41_drag_rightbot(
				act, pin, win, sty,
				x0, y0, 2.0*xn/w - 1.0, 1.0 - 2.0*yn/h
			);
		}
	}
}




void corner_gl41_hover(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	float w,h,c;
	float x0,y0,xn,yn;
	vec3 vc, vr, vf;
	vec3 ta, tb, tc;

	w = win->whdf.width;
	h = win->whdf.height;
	if(w<h)c = w / 32;
	else c = h / 32;

	struct privdata* priv = (void*)act->priv_256b;
	x0 = priv->mouse[0].x0;
	y0 = priv->mouse[0].y0;
	xn = priv->mouse[0].xn;
	yn = priv->mouse[0].yn;

	vr[0] = 2*(float)c / (float)w;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = 2*(float)c / (float)h;
	vf[2] = 0.0;

	//right, bot
	if((xn+c > w)&&(yn+c > h)){
		ta[0] = 0.0;
		ta[1] = -1.0;
		ta[2] = 0.0;
		tb[0] = 1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = 1.0;
		tc[1] = -1.0;
		tc[2] = -0.9;
		gl41line_bezier(win, 0xff0000ff, ta, tb, tc);
	}
	else{
		vc[0] = 1.0;
		vc[1] = -1.0;
		vc[2] = -0.99;
		gl41solid_circle(win, 0x0000ff, vc, vr, vf);
	}

	//left, bot
	if((xn < c)&&(yn+c > h)){
		ta[0] = 0.0;
		ta[1] = -1.0;
		ta[2] = 0.0;
		tb[0] = -1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = -1.0;
		tc[1] = -1.0;
		tc[2] = -0.9;
		gl41line_bezier(win, 0xffff0000, ta, tb, tc);
	}
	else{
		vc[0] = -1.0;
		vc[1] = -1.0;
		vc[2] = -0.99;
		gl41solid_circle(win, 0xff0000, vc, vr, vf);
	}

	//left, top
	if((xn < c)&&(yn < c)){
		ta[0] = 0.0;
		ta[1] = 1.0;
		ta[2] = 0.0;
		tb[0] = -1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = -1.0;
		tc[1] = 1.0;
		tc[2] = -0.9;
		gl41line_bezier(win, 0xff00ffff, ta, tb, tc);
	}
	else{
		vc[0] = -1.0;
		vc[1] = 1.0;
		vc[2] = -0.99;
		gl41solid_circle(win, 0x00ffff, vc, vr, vf);
	}

	//right, top
	if((xn+c > w)&&(yn < c)){
		ta[0] = 0.0;
		ta[1] = 1.0;
		ta[2] = 0.0;
		tb[0] = 1.0;
		tb[1] = 0.0;
		tb[2] = 0.0;
		tc[0] = 1.0;
		tc[1] = 1.0;
		tc[2] = -0.9;
		gl41line_bezier(win, 0xffffff00, ta, tb, tc);
	}
	else{
		vc[0] = 1.0;
		vc[1] = 1.0;
		vc[2] = -0.99;
		gl41solid_circle(win, 0xffff00, vc, vr, vf);
	}
}




void corner_gl41_popup(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	struct relation* rel = act->orel0;
	if(0 == rel)return;

	struct style* st = (void*)(rel->srcfoot);
	if(0 == st)return;
	if('#' == st->is.uc[3])return;

	int j,rgb;
	vec3 tc,tr,tf;

	tr[0] = 0.0;
	tr[1] = 0.0;
	tr[2] = 0.0;
	tf[0] = 0.0;
	tf[1] = 1.0/33;
	tf[2] = 0.0;
	tc[0] = 0.0;
	tc[1] = 0.0;
	tc[2] = -0.9;

	for(j=0;j<8;j++){

		tc[0] = -7.0/8;
		tc[1] = (-31+j*2)/32.0;
		tr[0] = 1.0/8;
		gl41opaque_rect(win, 0x3f000080, tc, tr, tf);

		tr[0] = 1.0/32;
		gl41string_center(win, 0xffffff, tc, tr, tf, (void*)"test", 8);
	}
}*/




void corner_gl41_inner(
	_obj* ent, struct style* slot,
	_obj* wnd, struct style* area,
	_syn* stack,int sp)
{
	struct relation* rel = ent->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_takeby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, 0,0, 0,0);
		}

		rel = samesrcnextdst(rel);
	}

	//corver_gl41_hover(ent, slot, wnd, area);
	//corner_gl41_drag(ent, slot, wnd, area);
	//corner_gl41_popup(act, pin, wnd, sty);

}
void corner_gl41_lefttop(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	_syn* stack,int sp)
{
	int w = win->whdf.fbwidth;
	int h = win->whdf.fbheight;
	int c = corner_bestfit(w,h);
	//logtoall("w=%d,h=%d,c=%d\n",w,h,c);

	vec3 vc, vr, vf;
	vc[0] = 0.0;
	vc[1] = h;
	vc[2] = 0;
	vr[0] = c;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = c;
	vf[2] = 0.0;

	struct privdata* priv = (void*)act->priv_256b;
	if(priv->lefttop){
		corner_gl41_inner(act,pin, win,sty, stack,sp);
	}
	else{
		gl41solid_circle(win, 0xff0000, vc, vr, vf);
	}
}
void corner_gl41_righttop(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	_syn* stack,int sp)
{
	int w = win->whdf.fbwidth;
	int h = win->whdf.fbheight;
	int c = corner_bestfit(w,h);
	//logtoall("w=%d,h=%d,c=%d\n",w,h,c);

	vec3 vc, vr, vf;
	vc[0] = w;
	vc[1] = h;
	vc[2] = 0;
	vr[0] = c;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = c;
	vf[2] = 0.0;

	struct privdata* priv = (void*)act->priv_256b;
	if(priv->righttop){
		//corner_gl41_inner(act,pin, win,sty, stack,sp);
	}
	else{
		gl41solid_circle(win, 0x0000ff, vc, vr, vf);
	}
}
void corner_gl41_leftbot(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	_syn* stack,int sp)
{
	int w = win->whdf.fbwidth;
	int h = win->whdf.fbheight;
	int c = corner_bestfit(w,h);
	//logtoall("w=%d,h=%d,c=%d\n",w,h,c);

	vec3 vc, vr, vf;
	vc[0] = 0.0;
	vc[1] = 0.0;
	vc[2] = 0.0;
	vr[0] = c;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = c;
	vf[2] = 0.0;

	struct privdata* priv = (void*)act->priv_256b;
	if(priv->leftbot){
		//corner_gl41_inner(act,pin, win,sty, stack,sp);
	}
	else{
		gl41solid_circle(win, 0xffff00, vc, vr, vf);
	}
}
void corner_gl41_rightbot(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	_syn* stack,int sp)
{
	int w = win->whdf.fbwidth;
	int h = win->whdf.fbheight;
	int c = corner_bestfit(w,h);
	//logtoall("w=%d,h=%d,c=%d\n",w,h,c);

	vec3 vc, vr, vf;
	vc[0] = w;
	vc[1] = 0.0;
	vc[2] = 0.0;
	vr[0] = c;
	vr[1] = 0.0;
	vr[2] = 0.0;
	vf[0] = 0.0;
	vf[1] = c;
	vf[2] = 0.0;

	struct privdata* priv = (void*)act->priv_256b;
	if(priv->rightbot){
		//corner_gl41_inner(act,pin, win,sty, stack,sp);
	}
	else{
		gl41solid_circle(win, 0x00ffff, vc, vr, vf);
	}
}
void corner_gl41_tearpaper(
	_obj* ent, struct style* pin,
	_obj* win, struct style* sty,
	_syn* stack,int sp)
{
	struct privdata* priv = (void*)ent->priv_256b;
	if(0 == priv->mouse[0].z0)return;

	float evw = win->whdf.width;
	float evh = win->whdf.height;
	float fbw = win->whdf.fbwidth;
	float fbh = win->whdf.fbheight;

	//left,bot = 0,0
	float x0 = 0;
	float y0 = 0;
	if( (priv->mouse[0].x0<32) && (priv->mouse[0].y0<32) ){
		//left top
		x0 = 0;
		y0 = evh;
	}
	else if( (priv->mouse[0].x0 > evw-32) && (priv->mouse[0].y0<32) ){
		//right top
		x0 = evw;
		y0 = evh;
	}
	else if( (priv->mouse[0].x0<32) && (priv->mouse[0].y0 > evh-32) ){
		//left bot
		x0 = 0;
		y0 = 0;
	}
	else if( (priv->mouse[0].x0 > evw-32) && (priv->mouse[0].y0 > evh-32) ){
		//right bot
		x0 = evw;
		y0 = 0;
	}
	else return;
	//drag point = xn,yn
	float xn = priv->mouse[0].xn;
	float yn = evh - priv->mouse[0].yn;
	//center of them
	float xmid=(x0+xn)/2;
	float ymid=(y0+yn)/2;
	logtoall("0=%f,%f,1=%f,%f,c=%f,%f\n",x0,y0,xn,yn,xmid,ymid);

	//linear equation: (y-ymid)/(x-xmid) = -(xn-x0)/(yn-y0);
	float py = y0;
	float px = xmid-(y0-ymid)*(yn-y0)/(xn-x0);
	float qx = x0;
	float qy = ymid-(x0-xmid)*(xn-x0)/(yn-y0);
	logtoall("%f,%f->%f,%f\n",px,py,qx,qy);

	//eventspace[0,1024] -> renderspace[-2048/2,2048/2]
	vec3 v0,v1;
	v0[0] = fbw*(x0/evw);
	v0[1] = fbh*(y0/evh);
	v0[2] = 0.0;
	v1[0] = fbw*(xn/evw);
	v1[1] = fbh*(yn/evh);
	v1[2] = 0.0;
	vec3 va,vb;
	va[0] = fbw*(px/evw);
	va[1] = fbh*(py/evh);
	va[2] = 0.0;
	vb[0] = fbw*(qx/evw);
	vb[1] = fbh*(qy/evh);
	vb[2] = 0.0;
	gl41line(win, 0x00ffff, v0, v1);
	gl41line(win, 0x00ffff, va, vb);
	gl41solid_triangle(win, 0x000000, v0,va,vb);
	gl41solid_triangle(win, 0xff0000, v1,va,vb);
}




void corner_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x0,y0,xn,yn;
	int w,h,c,t;

	w = win->whdf.width;
	h = win->whdf.height;
	if(w<h)c = w>>6;
	else c = h>>6;
/*

//---------hover
	x0 = priv->mouse[0].x0;
	y0 = priv->mouse[0].y0;
	xn = priv->mouse[0].xn;
	yn = priv->mouse[0].yn;

	if((xn < c)&&(yn+c > h))t = 4;
	else t = 1;
	drawsolid_circle(win, 0x0000ff, 0, h, c*t);

	if((xn+c > w)&&(yn+c > h))t = 4;
	else t = 1;
	drawsolid_circle(win, 0xff0000, w, h, c*t);

	if((xn < c)&&(yn < c))t = 4;
	else t = 1;
	drawsolid_circle(win, 0x00ffff, 0, 0, c*t);

	if((xn+c > w)&&(yn < c))t = 4;
	else t = 1;
	drawsolid_circle(win, 0xffff00, w, 0, c*t);


//--------------drag
	if(priv->mouse[0].z0){
		x0 = priv->mouse[0].x0;
		y0 = priv->mouse[0].y0;
		xn = priv->mouse[0].xn;
		yn = priv->mouse[0].yn;
	}
	else if(priv->touch[0].z0){
		x0 = priv->touch[0].x0;
		y0 = priv->touch[0].y0;
		xn = priv->touch[0].xn;
		yn = priv->touch[0].yn;
	}

	if(y0 < c)
	{
		if(x0 < c)
		{
			drawsolid_rect(win, 0x404040, 0, 0, xn, yn);
		}
		else if(x0+c > w)
		{
			drawsolid_triangle(win, 0x0000ff, xn, yn, 0, h-1, w-1, h-1);
			drawsolid_triangle(win, 0x00ffff, xn, yn, 0, h-1,   0,   0);
			drawsolid_triangle(win, 0xff0000, xn, yn, 0,   0, w-1,   0);
			drawsolid_triangle(win, 0xffff00, xn, yn, w-1, 0, w-1, h-1);
		}
	}
	else if(y0+c > h)
	{
		if(x0 < c)
		{
			drawsolid_rect(win, 0x404040, 0, 0, xn, h);
			drawsolid_rect(win, 0x404040, 0, yn, w, h);
		}
		else if(x0+c > w)
		{
			x0 = ((h-yn)*(h-yn)/(xn-w) + (w+xn)) / 2;
			y0 = ((xn-w)*(xn-w)/(yn-h) + (yn+h)) / 2;
			drawsolid_triangle(win, 0x808080, x0, h, w, y0, xn, yn);
			drawsolid_triangle(win, 0x000000, x0, h, w, y0, w, h);
		}
	}
*/
}



/*
static int corner_event_twig(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	struct relation* rel;
	_obj* ar;
	struct style* st;
	int x,y,j;

	rel = act->orel0;
	if(0 == rel)return 0;

	st = (void*)(rel->srcfoot);
	if(0 == st)return 0;
	if('#' == st->is.uc[3])return 0;

	int w = win->whdf.width;
	int h = win->whdf.height;

	short* t = (void*)ev;
	if(t[0] > w/8)return 0;
	if(t[1] < h/2)return 0;

	rel = win->orel0;
	while(1){
		if(0 == rel)break;
		if(_sup_ == rel->dstnodetype){
			ar = (void*)(rel->dstchip);
			//if(_ui3d_ == ar->type)goto found;
			//if(_ui2d_ == ar->type)goto found;
		}
		rel = samesrcnextdst(rel);
	}
	return 0;

found:
	x = 0;
	y = (h-t[1])*32/h;
	rel = ar->orel0;
	while(1){
		if(0 == rel)break;

		if(_ent_ == rel->dstnodetype){
			if(x == y){
				st = (void*)(rel->srcfoot);

				j = st->is.uc[3];
				if('#' == j)j = 0;
				else j = '#';
				st->is.uc[3] = j;

				break;
			}

			x += 1;
			if(x >= 16)break;
		}

		rel = samesrcnextdst(rel);
	}
	return 1;
}




static int corner_onoff(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	struct relation* rel;
	struct style* st;
	int j;

	rel = act->orel0;
	if(0 == rel)return 0;

	st = (void*)(rel->srcfoot);
	if(0 == st)return 0;

	j = st->is.uc[3];
	if('#' == j)j = 0;
	else j = '#';
	st->is.uc[3] = j;

	return 1;
}
static int corner_event_root(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	short* t;
	int w,h,ret;
	int x,y;

	t = (void*)ev;
	x = t[0];
	y = t[1];

	w = win->whdf.width;
	h = win->whdf.height;
	if(w<h)ret = w>>4;
	else ret = h>>4;

	if(y+ret > h)
	{
		if(x < ret)
		{
			corner_onoff(act, pin, win, sty);
			return 1;
		}
		if(x+ret > w)
		{
			return 1;
		}
	}
	if(y < ret)
	{
		if(x < ret)
		{
			return 1;
		}
		if(x+ret > w)
		{
			return 1;
		}
	}
	return 0;
}*/




void corner_read_bywnd(
	_obj* ent, struct style* slot,
	_obj* wnd, struct style* area,
	_syn* stack,int sp)
{
	switch(wnd->vfmt){
	case _rgba_:
		corner_draw_pixel(ent,slot, wnd,area);
		break;
	case _gl41list_:
		gl41data_before(wnd);
		gl41data_whcam(wnd, area);

		corner_gl41_leftbot(ent,slot, wnd,area, stack,sp);
		corner_gl41_rightbot(ent,slot, wnd,area, stack,sp);
		corner_gl41_lefttop(ent,slot, wnd,area, stack,sp);
		corner_gl41_righttop(ent,slot, wnd,area, stack,sp);

		corner_gl41_tearpaper(ent,slot, wnd,area, stack,sp);

		gl41data_after(wnd);
		break;
	}
}
static int corner_event(
	_obj* ent, struct style* slot,
	_obj* wnd, struct style* area,
	struct event* ev)
{
	int w = wnd->whdf.fbwidth;
	int h = wnd->whdf.fbheight;
	int c = corner_bestfit(w,h);

	struct privdata* priv = (void*)ent->priv_256b;
	u16* d = (u16*)&ev->why;
	switch(ev->what){
	case 0x2b70:
		logtoall("+(%d,%d), (%d,%d,%d)\n", d[0],d[1], w,h,c);
		priv->mouse[0].x0 = d[0];
		priv->mouse[0].y0 = d[1];
		priv->mouse[0].z0 = 1;
		break;
	case 0x4070:
		//logtoall("@%d,%d\n", d[0], d[1]);
		priv->mouse[0].xn = d[0];
		priv->mouse[0].yn = d[1];
		break;
	case 0x2d70:
		logtoall("-%d,%d\n", d[0], d[1]);
		priv->mouse[0].z0 = 0;
		goto handlepointer;
	case 0x2b74:
		logtoall("+(%d,%d), (%d,%d,%d)\n", d[0],d[1], w,h,c);
		priv->mouse[0].x0 = d[0];
		priv->mouse[0].y0 = d[1];
		priv->mouse[0].z0 = 1;
		break;
	case 0x4074:
		//logtoall("@%d,%d\n", d[0], d[1]);
		priv->mouse[0].xn = d[0];
		priv->mouse[0].yn = d[1];
		break;
	case 0x2d74:
		logtoall("-%d,%d\n", d[0], d[1]);
		priv->mouse[0].z0 = 0;
		goto handlepointer;
	}
	return 0;

handlepointer:
	if( (d[0] < c) && (d[1] < c) ){
		logtoall("lefttop");
		priv->lefttop = !priv->lefttop;
	}
	if( (d[0] > w-c) && (d[1] < c) ){
		logtoall("righttop");
		priv->righttop = !priv->righttop;
	}
	if( (d[0] < c) && (d[1] > h-c) ){
		logtoall("leftbot");
		priv->leftbot = !priv->leftbot;
	}
	if( (d[0] > w-c) && (d[1] > h-c) ){
		logtoall("rightbot");
		priv->rightbot = !priv->rightbot;
	}
	return 0;
}




static int corner_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@corner_read\n");
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		corner_read_bywnd(ent,slot, caller,area, stack,sp);
		break;
	}
	return 0;
}
static int corner_giving(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//printmemory(buf,16);
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	struct event* ev = buf;
	int type = ev->what&0xff;
	if( ('p' == type) | ('t' == type)){
		corner_event(ent,slot, caller,area, ev);
	}
	else{
		return _thru_;
	}
	return 0;
}
static int corner_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
static int corner_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




static int corner_search(_obj* win)
{
	return 0;
}
static int corner_modify(_obj* win)
{
	return 0;
}
static int corner_delete(_obj* win)
{
	return 0;
}
static int corner_create(_obj* act, u8* str)
{
	return 0;
}




void corner_register(_obj* p)
{
	p->vfmt = _orig_;
	p->type = hex64('c', 'o', 'r', 'n', 'e', 'r', 0, 0);

	p->oncreate = (void*)corner_create;
	p->ondelete = (void*)corner_delete;
	p->onreader = (void*)corner_search;
	p->onwriter = (void*)corner_modify;

	p->onattach = (void*)corner_attach;
	p->ondetach = (void*)corner_detach;
	p->ontaking = (void*)corner_taking;
	p->ongiving = (void*)corner_giving;
}
