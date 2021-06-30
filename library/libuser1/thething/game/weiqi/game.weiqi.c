#include "libuser.h"
int ray_plane(vec3 ray[], vec3 rect[], vec3 out);




struct perweiqi{
	u8 data[19][19];
	int px;
	int py;
	int turn;
};
int weiqi_import(char* file, u8* buf)
{
	int x,y,j;
	u8 tmp[0x200];
	j = openreadclose(file, 0, tmp, 0x200);
	//printmemory(tmp, 0x100);
	if(j<=0)return 0;

	x = y = 0;
	for(j=0;j<0x200;j++)
	{
		if(0xd == tmp[j])continue;
		if(0xa == tmp[j]){
			x = 0;y += 1;
			if(y >= 19)break;
			continue;
		}
		if(x<19)
		{
			buf[y*19+x] = tmp[j];
			x++;
		}
	}

	//printmemory(buf, 81);
	return 1;
}




static void weiqi_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 c;
	int x, y, cx, cy, ww, hh;
	struct perweiqi* per = act->buf0;

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

	//rgb? bgr?
	if( ((win->fmt)&0xffffff) == 0x626772)c = 0x256f8d;
	else c = 0x8d6f25;

	drawsolid_rect(win, c, cx-ww, cy-hh, cx+ww, cy+hh);
	for(y=-9;y<10;y++)		//heng
	{
		drawline(win, 0,
			cx - ww*18/19, cy + y*hh*2/19,
			cx + ww*18/19, cy + y*hh*2/19
		);
	}
	for(x=-9;x<10;x++) 		//shu
	{
		drawline(win, 0,
			cx + x*ww*2/19, cy - hh*18/19,
			cx + x*ww*2/19, cy + hh*18/19
		);
	}
	for(y = cy - hh*12/19; y <= cy + hh*12/19; y += hh*12/19)
	{
		for(x = cx - ww*12/19; x <= cx + ww*12/19; x += ww*12/19)
		{
			drawsolid_circle(win, 0, x, y, ww/3/19);
		}
	}

	//zi
	for(y=-9;y<=9;y++)
	{
		for(x=-9;x<=9;x++)
		{
			if(per->data[y+9][x+9] == 'b')c = 0x444444;
			else if(per->data[y+9][x+9] == 'w')c = 0xffffff;
			else continue;

			drawsolid_circle(
				win, c,
				cx + x*ww*2/19,
				cy + y*hh*2/19,
				ww/19
			);
		}
	}
}




static void weiqi_draw_gl41_nocam(
	struct entity* act, struct style* part,
	struct entity* wnd, struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	gl41data_01cam(wnd);
	gl41solid_rect(wnd, 0x00ff00, fs.vc, fs.vr, fs.vf);
	gl41data_after(wnd);
}
static void weiqi_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y;
	int j,k,rgb;
	vec3 tc, tr, tf, tu;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vt = geom->fs.vt;
	for(j=0;j<3;j++){
		tu[j] = vt[j]/2;
		tc[j] = vc[j] - tu[j];	//bottom half is chessboard
	}
	gl41solid_prism4(ctx, 0xf9d65b, tc, vr, vf, tu);

	for(y=-9;y<10;y++)
	{
		for(j=0;j<3;j++){
			tc[j] = vc[j] - vr[j]*(18.0/19.0) + vf[j]*(y*2.0/19.0) + vt[j]/10.0;
			tr[j] = tc[j] + vr[j]*36.0/19.0;
		}
		gl41line(ctx, 0x222222, tc, tr);
	}
	for(x=-9;x<10;x++)
	{
		for(j=0;j<3;j++){
			tc[j] = vc[j] + vr[j]*(x*2.0/19.0) - vf[j]*(18.0/19.0) + vt[j]/10.0;
			tr[j] = tc[j] + vf[j]*36.0/19.0;
		}
		gl41line(ctx, 0x222222, tc, tr);
	}

	for(j=0;j<3;j++){
		tr[j] = vr[j]/100.0;
		tf[j] = vf[j]/100.0;
	}
	for(y=-6;y<=6;y+=6){
		for(x=-6;x<=6;x+=6){
			for(j=0;j<3;j++)tc[j] = vc[j] + vr[j]*x*2/19 + vf[j]*y*2/19+vt[j]/10.0;
			gl41solid_circle(ctx,0, tc,tr,tf);
		}
	}

	u8* data = act->buf0;
	k = 0;
	for(j=0;j<3;j++){
		tc[j] = vc[j] + vt[j]/2;
		tr[j] = vr[j]/19;
		tf[j] = vf[j]/19;
		tu[j] = vt[j]/2;
	}
	for(y=-9;y<10;y++)
	{
		for(x=-9;x<10;x++)
		{
			j = data[19*(y+9) + x+9];
			if('w' == j)rgb = 0xe0e0e0;
			else if('b' == j)rgb = 0x202020;
			else continue;

			tc[0] = vc[0] + tr[0]*x*2 - tf[0]*y*2 + tu[0];
			tc[1] = vc[1] + tr[1]*x*2 - tf[1]*y*2 + tu[1];
			tc[2] = vc[2] + tr[2]*x*2 - tf[2]*y*2 + tu[2];
			gl41solid_sphere(ctx, rgb, tc, tr, tf, tu);

			k++;
			//if(k>50)return;
		}
	}
}
static void weiqi_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void weiqi_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	struct perweiqi* per = win->buf0;

	//<head>
	htmlprintf(win, 1,
		".wqbg{width:50%%;height:50%%;float:left;background-color:#f9d65b;text-align:center;}\n"
		".wqfg{width:5.26%%;height:5.26%%;float:left;background-color:#ffffff;border-radius:50%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"wqbg\">\n");
	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"wqfg\">%d</div>\n",
				per->data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void weiqi_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y,j,k,ret,color;
	int width = win->width;
	int height = win->height;
	struct perweiqi* per = win->buf0;
	u8* p = win->buf0;

	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			//position
			ret = y*width + x*2;
			ret <<= 2;

			//color
			if( (per->px == x) && (per->py == y) )color = 7;
			else if(per->data[y][x] == 'b')color = 4;
			else if(per->data[y][x] == 'w')color = 1;
			else continue;

			//
			p[ret] = 0x20;
			p[ret + 3] = color;
			p[ret + 4] = 0x20;
			p[ret + 7] = color;
		}
	}
}
static void weiqi_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}



/*
int weiqi_haveqi(int x,int y, u8 c)
{
	if(x <0)return 0;
	if(x>18)return 0;
	if(y <0)return 0;
	if(y>18)return 0;
	if(0 == data[y][x])return 1;
	if(c == data[y][x])return 1;
	return 0;
}
void weiqi_check(int x,int y)
{
	if(x<0)return;
	if(x>18)return;
	if(y<0)return;
	if(y>18)return;

	u8 c = data[y][x];
	if(0 == c)return;
	if(weiqi_haveqi(x-1,y,c))return;
	if(weiqi_haveqi(x+1,y,c))return;
	if(weiqi_haveqi(x,y-1,c))return;
	if(weiqi_haveqi(x,y+1,c))return;
	data[y][x] = 0;
}*/
int weiqi_forbid(int x, int y, u8 c)
{
/*
	foreach k in 上下左右{
		if(是边)continue;

		k = 这个;
		if(0 == k)return 0;
		if(c == k){
			if(这个能供气)return 0;
		}
		if(c != k){
			if(这个能吃掉)return 0;
		}
	}
	return 1;
*/
	return 0;
}
void weiqi_putone(struct perweiqi* per, vec3 v)
{
	int x = 19*v[0];
	int y = 19*(1.0-v[1]);
	say("%d,%d\n",x,y);

	if(x<0)return;
	if(x>18)return;
	if(y<0)return;
	if(y>18)return;

	if(0 != per->data[y][x])return;
	if(per->turn&1){
		if(weiqi_forbid(x,y,'w'))return;
		per->data[y][x] = 'w';
		per->turn++;
	}
	else{
		if(weiqi_forbid(x,y,'b'))return;
		per->data[y][x] = 'b';
		per->turn++;
	}
}
void weiqi_intersect(float* out, vec3 ray[], struct fstyle* sty)
{
	vec3 plane[2]={
		{sty->vc[0],sty->vc[1],sty->vc[2]},
		{sty->vt[0],sty->vt[1],sty->vt[2]}
	};

	vec3 tmp;
	int ret = ray_plane(ray, plane, tmp);
	if(ret <= 0)return;

	//vec_vc_to_p
	tmp[0] -= sty->vc[0];
	tmp[1] -= sty->vc[1];
	tmp[2] -= sty->vc[2];

	//[-1,1]
	float lx = vec3_getlen(sty->vr);
	float ly = vec3_getlen(sty->vf);
	out[0] = vec3_dot(sty->vr, tmp) / lx / lx;
	out[1] = vec3_dot(sty->vf, tmp) / ly / ly;

	//[0,1]
	out[0] = (out[0]+1.0)/2;
	out[1] = (out[1]+1.0)/2;
	say("%f,%f\n", out[0],out[1]);
}




static void weiqi_taking_bycam(_ent* ent,void* slot, _syn* stack,int sp)
{
	if(0 == stack)return;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->fmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		weiqi_draw_gl41(ent,slot, wor,geom, wnd,area);
		break;
	}
}
static void weiqi_taking(_ent* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;
	struct entity* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->fmt){
	case _gl41list_:
		weiqi_draw_gl41_nocam(ent,slot, caller,area);
		break;
	default:
		weiqi_taking_bycam(ent,slot, stack,sp);
		break;
	}
}
static void weiqi_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct entity* caller;struct style* geom;
	caller = stack[sp-2].pchip;geom = stack[sp-2].pfoot;

	switch(caller->type){
	case _scene3d_:
	case _reality_:{
		vec3* ray = buf;
/*		say("%f,%f,%f->%f,%f,%f\n",
			ray[0][0],ray[0][1],ray[0][2],
			ray[1][0],ray[1][1],ray[1][2]);
*/
		float out[3];
		weiqi_intersect(out, ray, &geom->fs);
		weiqi_putone(ent->buf0, out);
		break;
	}
	}
}
static void weiqi_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void weiqi_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void weiqi_search(struct entity* act)
{
}
static void weiqi_modify(struct entity* act)
{
}
static void weiqi_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf0);
}
static void weiqi_create(struct entity* act, void* str)
{
	if(0 == act)return;

	u8* buf = memorycreate(sizeof(struct perweiqi), 0);
	act->buf0 = buf;
	if(0 == buf)return;

	struct perweiqi* per = act->buf0;
	per->px = per->py = 0;
	per->turn = 0;

	//read
	int ret = 0;
	if(str)ret = weiqi_import(str, buf);
	if((0==str)|(ret<=0)){for(ret=0;ret<19*19;ret++)buf[ret] = 0;}

	//print
	for(ret=0;ret<19*19;ret+=19)printmemory(buf+ret, 19);
}




void weiqi_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('w', 'e', 'i', 'q','i', 0, 0, 0);

	p->oncreate = (void*)weiqi_create;
	p->ondelete = (void*)weiqi_delete;
	p->onsearch = (void*)weiqi_search;
	p->onmodify = (void*)weiqi_modify;

	p->onlinkup = (void*)weiqi_linkup;
	p->ondiscon = (void*)weiqi_discon;
	p->ontaking = (void*)weiqi_taking;
	p->ongiving = (void*)weiqi_giving;
}
