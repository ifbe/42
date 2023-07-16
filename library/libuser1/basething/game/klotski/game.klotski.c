#include "libuser.h"
#define caocao 0
#define machao 1
#define zhaoyun 2
#define huangzhong 3
#define zhangfei 4
#define guanyu 5
#define bing0 6
#define bing1 7
#define bing2 8
#define bing3 9
static u8 data[5][4];
static u32 color[10] =
{
	0xffffff, 0x0000ff, 0x00ff00, 0x00ffff,
	0xff0000, 0xff00ff,
	0x882266, 0x367638, 0x069713, 0x123490
};




static void klotski_draw_pixel(
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

	for(y=0;y<5;y++)
	{
		for(x=0;x<4;x++)
		{
			if(data[y][x] > 9)continue;
			drawsolid_rect(
				win, color[data[y][x]],
				cx+(x-2)*ww/2,
				cy+(2*y-5)*hh/5,
				cx+(x-1)*ww/2,
				cy+(2*y-3)*hh/5
			);
		}
	}
}
static void klotski_draw_gl41(
	_obj* act, struct style* part,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_rect(wnd, 0xffffff, vc, vr, vf);
}
static void klotski_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void klotski_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;

	//<head>
	htmlprintf(win, 1,
		".klbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".klfg{width:25%%;height:20%%;float:left;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"klbg\">\n");
	for(y=0;y<5;y++)
	{
		for(x=0;x<4;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"klfg\" style=\"background-color:#%06x\">%d</div>\n",
				color[data[y][x]], data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void klotski_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void klotski_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void klotski_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	klotski_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void klotski_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	if(0 == stack)return;

	//foot defined behavior
	switch(stack[sp-1].foottype){
	}

	//caller defined behavior
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	default:
		klotski_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void klotski_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void klotski_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void klotski_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void klotski_search(_obj* act)
{
}
static void klotski_modify(_obj* act)
{
}
static void klotski_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void klotski_create(_obj* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = data;
	if(_copy_ == act->type)act->listptr.buf0 = memorycreate(20, 0);

	data[0][1] = data[0][2] = data[1][1] = data[1][2] = caocao;
	data[0][0] = data[1][0] = machao;
	data[0][3] = data[1][3] = zhaoyun;
	data[2][0] = data[3][0] = huangzhong;
	data[2][3] = data[3][3] = zhangfei;
	data[2][1] = data[2][2] = guanyu;
	data[3][1] = bing0;
	data[3][2] = bing1;
	data[4][0] = bing2;
	data[4][3] = bing3; 
	data[4][1] = data[4][2] = 0xff;
}




void klotski_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('k', 'l', 'o', 't', 's', 'k', 'i', 0);

	p->oncreate = (void*)klotski_create;
	p->ondelete = (void*)klotski_delete;
	p->onreader = (void*)klotski_search;
	p->onwriter = (void*)klotski_modify;

	p->onattach = (void*)klotski_attach;
	p->ondetach = (void*)klotski_detach;
	p->ontaking = (void*)klotski_taking;
	p->ongiving = (void*)klotski_giving;
}
