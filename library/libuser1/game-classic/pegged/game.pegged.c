#include "libuser.h"
static u8 data[7][7];




static void pegged_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->fs.vc[0];
		cy = sty->fs.vc[1];
		ww = sty->fs.vr[0];
		hh = sty->fs.vf[1];
	}
	else
	{
		cx = win->whdf.width/2;
		cy = win->whdf.height/2;
		ww = win->whdf.width/2;
		hh = win->whdf.height/2;
	}

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if(data[y][x] == 0)continue;

			drawline_rect(
				win, 0x808080,
				cx+(2*x-7)*ww/7,
				cy+(2*y-7)*hh/7,
				cx+(2*x-5)*ww/7,
				cy+(2*y-5)*hh/7
			);
			drawsolid_circle(
				win, 0xffffff,
				cx+(x-3)*ww*2/7,
				cy+(y-3)*hh*2/7,
				ww/14
			);
		}
	}
}
static void pegged_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	int x,y;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if(data[y][x] == 0)continue;

			f[0] = (2*x-6)/7.0;
			f[1] = (2*y-6)/7.0;
			f[2] = 0.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];

			tr[0] = vr[0] / 7;
			tr[1] = vr[1] / 7;
			tr[2] = vr[2] / 7;
			tf[0] = vf[0] / 7;
			tf[1] = vf[1] / 7;
			tf[2] = vf[2] / 7;
			gl41line_rect(wnd, 0x808080, tc, tr, tf);

			tr[0] = vr[0] / 14;
			tr[1] = vr[1] / 14;
			tr[2] = vr[2] / 14;
			tf[0] = vf[0] / 14;
			tf[1] = vf[1] / 14;
			tf[2] = vf[2] / 14;
			tu[0] = vu[0] / 14;
			tu[1] = vu[1] / 14;
			tu[2] = vu[2] / 14;
			gl41solid_sphere(wnd, 0xffffff, tc, tr, tf, tu);
		}
	}
}
static void pegged_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pegged_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;

	//<head>
	htmlprintf(win, 1,
		".pegbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".pegfg{width:14%%;height:14%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"pegbg\">\n");
	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"pegfg\">%d</div>\n",
				data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void pegged_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void pegged_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void pegged_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	pegged_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void pegged_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->type){
	case _wnd_:
	case _render_:
		break;
	default:
		pegged_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void pegged_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void pegged_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void pegged_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void pegged_search(_obj* act)
{
}
static void pegged_modify(_obj* act)
{
}
static void pegged_delete(_obj* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memoryfree(act->listptr.buf0);
}
static void pegged_create(_obj* act)
{
	int x,y;
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = data;
	if(_copy_ == act->type)act->listptr.buf0 = memoryalloc(49, 0);

	for(y=0;y<7;y++)
	{
		for(x=0;x<7;x++)
		{
			if((x>=2)&&(x<=4))data[y][x] = 1;
			else if((y>=2)&&(y<=4))data[y][x] = 1;
			else data[y][x] = 0;
		}
	}
}




void pegged_register(_obj* p)
{
	p->kind = _game_;
	p->type = hex64('p', 'e', 'g', 'g', 'e', 'd', 0, 0);
	p->vfmt = _orig_;

	p->oncreate = (void*)pegged_create;
	p->ondelete = (void*)pegged_delete;
	p->onreader = (void*)pegged_search;
	p->onwriter = (void*)pegged_modify;

	p->onattach = (void*)pegged_attach;
	p->ondetach = (void*)pegged_detach;
	p->ontaking = (void*)pegged_taking;
	p->ongiving = (void*)pegged_giving;
}
