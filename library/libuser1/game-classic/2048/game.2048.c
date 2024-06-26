#include "libuser.h"
#define COUNT 8
#define _corner_ hex64('c', 'o', 'r', 'n', 'e', 'r', 0, 0)
void new2048(void*);
void left2048(void*);
void right2048(void*);
void up2048(void*);
void down2048(void*);
void gl41data_whcam(_obj* wnd, struct style* area);




static u8 len2048[17] = {
0, 1, 1, 1,
2, 2, 2, 3,
3, 3, 4, 4,
4, 4, 5, 5, 5
};
static u32 val2048[17] = {
   0,    2,    4,    8,
  16,   32,   64,  128,
 256,  512, 1024, 2048,
4096, 8192,16384,32768,65536
};
static u32 color2048[17] = {
0x444444, 0xfffff0, 0xffffc0, 0x995000,
0xc05000, 0xb03000, 0xff0000, 0xffffa0,
0xffff80, 0xffff00, 0xffffb0, 0x233333,
0x783d72, 0xd73762
};
struct per2048{
	u8 data[0x80];
	u8 curr;
};




static void the2048_search(_obj* act, u8* buf)
{
}
static void the2048_modify(_obj* act, u8* buf)
{
}
static void the2048_delete(_obj* act, u8* buf)
{
}
static void the2048_create(_obj* act, u8* buf)
{
	if(0 == act)return;

	struct per2048* per = (void*)act->priv_256b;
	per->curr = 0;

	new2048(per->data);
}




static void the2048_draw_pixel(
	_obj* act, struct style* pin,
	_obj* wnd, struct style* sty)
{
	u32 color;
	int x,y,x0,y0,x1,y1;
	int cx, cy, ww, hh;

	struct per2048* per = (void*)act->priv_256b;
	u8 (*tab)[4] = (void*)(per->data) + (per->curr)*16;

	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = wnd->whdf.width/2;
		cy = wnd->whdf.height/2;
		ww = wnd->whdf.width/2;
		hh = wnd->whdf.height/2;
	}

	//cubies
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			//color
			color = color2048[tab[y][x]];
			if(0x626772  == (wnd->vfmt&0xffffff) )	//bgra->rgba
			{
				color = 0xff000000
					+ ((color&0xff)<<16)
					+ (color&0xff00)
					+ ((color&0xff0000)>>16);
			}

			//cubie
			x0 = (cx+1) + (x-2)*ww/2;
			y0 = (cy+1) + (y-2)*hh/2;
			x1 = (cx-1) + (x-1)*ww/2;
			y1 = (cy-1) + (y-1)*hh/2;
			drawsolid_rect(wnd, color, x0, y0, x1, y1);

			if(len2048[tab[y][x]] == 0)continue;
			drawdec_fit(
				wnd, 0,
				x0, y0*3/4+y1/4,
				x1, y0/4+y1*3/4,
				val2048[tab[y][x]]
			);
		}
	}
}/*
static void the2048_draw_dx11(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	dx11solid_rect(ctx, 0x444444, vc, vr, vf);

	u32 rgb;
	int x,y;
	vec3 tc, tr, tf, tu, f;
	u8 (*tab)[4] = (void*)(per->data) + (per->curr)*16;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			rgb = color2048[tab[y][x]];
			//logtoall("%x\n", rgb);

			f[0] = (x+x-3) / 4.0;
			f[1] = (3-y-y) / 4.0;
			f[2] = val2048[tab[y][x]] / 2048.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];

			tr[0] = vr[0]/5;
			tr[1] = vr[1]/5;
			tr[2] = vr[2]/5;
			tf[0] = vf[0]/5;
			tf[1] = vf[1]/5;
			tf[2] = vf[2]/5;
			tu[0] = vu[0]*f[2];
			tu[1] = vu[1]*f[2];
			tu[2] = vu[2]*f[2];
			dx11solid_prism4(ctx, rgb, tc, tr, tf, tu);

			tc[0] += vu[0]*0.01;
			tc[1] += vu[1]*0.01;
			tc[2] += vu[2]*0.01;
			tr[0] = vr[0]/16;
			tr[1] = vr[1]/16;
			tr[2] = vr[2]/16;
			tf[0] = vf[0]/16;
			tf[1] = vf[1]/16;
			tf[2] = vf[2]/16;
			dx11decimal(ctx, ~rgb, tc, tr, tf, val2048[tab[y][x]]);
		}
	}
}*/
static void the2048_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	//logtoall("%s\n", __func__);
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_rect(ctx, 0x444444, vc, vr, vf);

	u32 rgb;
	int x,y;
	vec3 tc, tr, tf, tu, f;

	struct per2048* per = (void*)act->priv_256b;
	u8 (*tab)[4] = (void*)(per->data) + (per->curr)*16;

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			rgb = color2048[tab[y][x]];
			//logtoall("%x\n", rgb);

			f[0] = (x+x-3) / 4.0;
			f[1] = (3-y-y) / 4.0;
			f[2] = val2048[tab[y][x]] / 2048.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];

			tr[0] = vr[0]/5;
			tr[1] = vr[1]/5;
			tr[2] = vr[2]/5;
			tf[0] = vf[0]/5;
			tf[1] = vf[1]/5;
			tf[2] = vf[2]/5;
			tu[0] = vu[0]*f[2];
			tu[1] = vu[1]*f[2];
			tu[2] = vu[2]*f[2];
			gl41solid_prism4(ctx, rgb, tc, tr, tf, tu);

			tc[0] += tu[0] + vu[0]*0.01;
			tc[1] += tu[1] + vu[1]*0.01;
			tc[2] += tu[2] + vu[2]*0.01;
			tr[0] = vr[0]/16;
			tr[1] = vr[1]/16;
			tr[2] = vr[2]/16;
			tf[0] = vf[0]/16;
			tf[1] = vf[1]/16;
			tf[2] = vf[2]/16;
			gl41decimal(ctx, ~rgb, tc, tr, tf, val2048[tab[y][x]]);
		}
	}
}
static void the2048_draw_gl41_nocam(
	_obj* act, struct style* part,
	_obj* wnd, struct style* area)
{
	int j;
	struct fstyle fs;
	for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
	fs.vc[0] = (area->fs.vc[0]+area->fs.vq[0]) * wnd->whdf.fbwidth / 2.0;
	fs.vc[1] = (area->fs.vc[1]+area->fs.vq[1]) * wnd->whdf.fbheight / 2.0;
	fs.vr[0] = (area->fs.vq[0]-area->fs.vc[0]) * wnd->whdf.fbwidth / 2.0;
	fs.vf[1] = (area->fs.vq[1]-area->fs.vc[1]) * wnd->whdf.fbheight/ 2.0;
	fs.vt[2] = 1.0;

	gl41data_before(wnd);
	//gl41data_01cam(wnd);
	//gl41solid_rect(wnd, 0x00ff00, fs.vc, fs.vr, fs.vf);
	gl41data_whcam(wnd, area);
	the2048_draw_gl41(act,part, 0,(void*)&fs, wnd,area);
	gl41data_after(wnd);
}
static void the2048_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	struct per2048* per = (void*)act->priv_256b;
	u8 (*tab)[4] = (void*)(per->data) + (per->curr)*16;

	int x,y;
	int len = win->json.len;
	u8* buf = win->json.buf;

	len += mysnprintf(buf+len, 0x100000-len, "{\"2048\" : ");
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			len += mysnprintf(buf+len, 0x100000-len, "\"%d\",", tab[y][x]);
		}//forx
	}//fory
	len += mysnprintf(buf+len, 0x100000-len, "}\n");

	win->json.len = len;
}
static void the2048_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;
	struct per2048* per = (void*)act->priv_256b;
	u8 (*tab)[4] = (void*)(per->data) + (per->curr)*16;

	//<head>
	htmlprintf(win, 1,
		".b2048{width:100%%;height:100%%;float:left;background-color:#000;text-align:center;}\n"
		".g2048{width:24.8%%;height:24.8%%;margin:0.1%%;float:left;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"b2048\">\n");
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"g2048\" style=\"background-color:#%06x\">%d</div>\n",
				color2048[tab[y][x]], val2048[tab[y][x]]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void the2048_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;
	struct per2048* per = (void*)act->priv_256b;
	u8 (*tab)[4] = (void*)(per->data) + (per->curr)*16;

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			gentui_rect(win, 4, x*8, y*4, x*8+5, y*4+2);
			gentui_decstr(win, 7, x*8+2, y*4+1, val2048[tab[y][x]]);
		}
	}
}
static void the2048_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	struct per2048* per = (void*)act->priv_256b;
	u8 (*tab)[4] = (void*)(per->data) + (per->curr)*16;

	logtoall("2048(%x,%x,%x,%x)\n", win, act, sty, pin);
	logtoall("%d	%d	%d	%d\n",
		val2048[tab[0][0]],
		val2048[tab[0][1]],
		val2048[tab[0][2]],
		val2048[tab[0][3]]
	);
	logtoall("%d	%d	%d	%d\n",
		val2048[tab[1][0]],
		val2048[tab[1][1]],
		val2048[tab[1][2]],
		val2048[tab[1][3]]
	);
	logtoall("%d	%d	%d	%d\n",
		val2048[tab[2][0]],
		val2048[tab[2][1]],
		val2048[tab[2][2]],
		val2048[tab[2][3]]
	);
	logtoall("%d	%d	%d	%d\n",
		val2048[tab[3][0]],
		val2048[tab[3][1]],
		val2048[tab[3][2]],
		val2048[tab[3][3]]
	);
}




static void the2048_move(_obj* act, int op)
{
	int j;
	u8* p;
	u8* q;
	struct per2048* per = (void*)act->priv_256b;

	p = (void*)(per->data) + 16*(per->curr);
	(per->curr) = ((per->curr)+1)%COUNT;
	q = (void*)(per->data) + 16*(per->curr);
	for(j=0;j<16;j++)q[j] = p[j];

	switch(op){
	case 'a':left2048(q);break;
	case 'd':right2048(q);break;
	case 's':down2048(q);break;
	case 'w':up2048(q);break;
	}

	new2048(q);
}
static void the2048_event(_obj* act, struct event* ev)
{
	int k;
	short* s;
	//logtoall("%llx,%llx,%llx\n", act, pin, ev);
	logtoall("%16llx,%16llx,%16llx,%16llx\n",ev->when, ev->where, ev->what, ev->why);

	struct per2048* per = (void*)act->priv_256b;
	if(_char_ == ev->what)
	{
		switch(ev->why){
		case 0x8:per->curr = ((per->curr)+(COUNT-1))%COUNT;break;
		case 'a':
		case 'd':
		case 's':
		case 'w':the2048_move(act, ev->why);break;
		}
	}
	else if(_kbd_ == ev->what)
	{
		switch(ev->why){
		case 0x4b:the2048_move(act, 'a');break;
		case 0x4d:the2048_move(act, 'd');break;
		case 0x50:the2048_move(act, 's');break;
		case 0x48:the2048_move(act, 'w');break;
		}
	}
	else if(joy_left == (ev->what & joy_mask))
	{
		s = (void*)ev;
		if(s[3] & joyl_left) the2048_move(act, 'a');
		if(s[3] & joyl_right)the2048_move(act, 'd');
		if(s[3] & joyl_down) the2048_move(act, 's');
		if(s[3] & joyl_up)   the2048_move(act, 'w');
	}
}



//2048 - *** - wnd
static void the2048_read_bywnd(
_obj* ent,void* slot,
_obj* wnd,struct style* area,
_syn* stack,int sp)
{
	//logtoall("%s vfmt=%.4s\n", __func__, &wnd->vfmt);
	switch(wnd->vfmt){
	case _cli_:
		the2048_draw_cli(ent,slot, wnd,area);
		break;
	case _tui_:
	case _tui256_:
		the2048_draw_tui(ent,slot, wnd,area);
		break;
	case _rgba_:
		the2048_draw_pixel(ent,slot, wnd,area);
		break;
	case _gl41list_:
		the2048_draw_gl41_nocam(ent,slot, wnd,area);
		break;
	case _dx11list_:
	case _mt20list_:
	case _vk12list_:
		logtoall("wnd@%p\n", wnd);
		break;
	}
}

//2048 - mgr - wnd
static void the2048_read_bymgr(
_obj* ent,void* slot,
_obj* mgr,struct style* geom,
_syn* stack,int sp)
{
	_obj* wnd = stack[sp-4].pchip;
	struct style* area = stack[sp-4].pfoot;

	int j;
	struct fstyle fs;
	switch(wnd->vfmt){
	case _rgba_:
		the2048_draw_pixel(ent,slot, wnd,geom);
		break;
	case _gl41list_:
		for(j=0;j<3;j++)fs.vc[j] = fs.vr[j] = fs.vf[j] = fs.vt[j] = 0.0;
		fs.vr[0] = area->fs.vq[0] * wnd->whdf.fbwidth / 4.0;
		fs.vf[1] = area->fs.vq[1] * wnd->whdf.fbheight/ 4.0;
		fs.vt[2] = 1.0;
		the2048_draw_gl41(ent,slot, mgr,(void*)&fs, wnd,area);
	}
}

//2048 - world - cam - wnd
static void the2048_read_byworld_bycam_bywnd(
_obj* ent,void* slot,
_obj* world,struct style* geom,
_syn* stack,int sp)
{
	//logtoall("%s\n", __func__);
	if(0 == stack)return;

	_obj* wnd = stack[sp-6].pchip;
	struct style* area = stack[sp-6].pfoot;
	switch(wnd->vfmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		//logtoall("2222222\n");
		the2048_draw_gl41(ent,slot, world,geom, wnd,area);
		break;
	}
}

static void the2048_read_byworld(
_obj* ent,void* slot,
_obj* world,struct style* geom,
_syn* stack,int sp)
{
	_obj* caller2 = stack[sp-4].pchip;
	struct style* area = stack[sp-4].pfoot;
	switch(caller2->type){
	case _wnd_:
		the2048_draw_gl41_nocam(ent,slot, caller2,area);
		break;
	default:
		the2048_read_byworld_bycam_bywnd(ent,slot, world,geom, stack,sp);
	}
}




static void the2048_taking(_obj* ent,void* slot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@the2048_read begin\n");
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	//logtoall("callertype=%.4s\n", &caller->type);
	switch(caller->type){
	case _htmlroot_:
		the2048_draw_html(ent,slot, caller,area);
		break;
	case _wnd_:
	case _render_:
		the2048_read_bywnd(ent,slot, caller,area, stack,sp);
		break;
	case _corner_:
	case _wndmgr_:
	default:
		the2048_read_byworld(ent,slot, caller,area, stack,sp);
		break;
	}
	//logtoall("@the2048_read end\n");
}
static void the2048_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@the2048_write\n");
	switch(stack[sp-1].foottype){
	case _ioby_:the2048_move(ent, *(u8*)buf);break;
	default:the2048_event(ent, buf);
	}
}
static void the2048_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void the2048_attach(struct halfrel* self, struct halfrel* peer)
{
}




void the2048_register(_obj* p)
{
	p->kind = _game_;
	p->type = hex32('2','0','4','8');
	p->vfmt = _orig_;

	p->oncreate = (void*)the2048_create;
	p->ondelete = (void*)the2048_delete;
	p->onreader = (void*)the2048_search;
	p->onwriter = (void*)the2048_modify;

	p->onattach = (void*)the2048_attach;
	p->ondetach = (void*)the2048_detach;
	p->ontaking = (void*)the2048_taking;
	p->ongiving = (void*)the2048_giving;
}
