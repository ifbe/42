#include "libuser.h"




static int turn;
static u8 data[3][3];




static void ooxx_draw_pixel(
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
	drawsolid_rect(win, 0x222222, cx-ww, cy-hh, cx+ww, cy+hh);

	//heng
	drawline(win, 0xffffff, cx-ww, cy-hh/3, cx+ww, cy-hh/3);
	drawline(win, 0xffffff, cx-ww, cy+hh/3, cx+ww, cy+hh/3);

	//shu
	drawline(win, 0xffffff, cx-ww/3, cy-hh, cx-ww/3, cy+hh);
	drawline(win, 0xffffff, cx+ww/3, cy-hh, cx+ww/3, cy+hh);

	//ox
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			if('o' == data[y][x])
			{
				drawline_circle(win, 0xff,
					cx+(x-1)*ww*2/3, cy+(y-1)*hh*2/3, ww/6
				);
			}
			else if('x' == data[y][x])
			{
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-5)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-3)*hh/6
				);
				drawline(win, 0xff0000,
					cx+(4*x-5)*ww/6, cy+(4*y-3)*hh/6,
					cx+(4*x-3)*ww/6, cy+(4*y-5)*hh/6
				);
			}
		}//forx
	}//fory
}
static void ooxx_draw_gl41(
	_obj* act, struct style* slot,
	_obj* wrl, struct style* geom,
	_obj* wnd, struct style* area)
{
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_rect(wnd, 0x444444, vc, vr, vf);
}
static void ooxx_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void ooxx_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;
	char p[2];
	p[0] = p[1] = 0;

	//<head>
	htmlprintf(win, 1,
		".oxbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".oxfg{width:33%%;height:33%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"oxbg\">\n");
	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			p[0] = data[y][x];
			if(('o' != p[0])&&('x' != p[0]))p[0] = 0;

			htmlprintf(win, 2, "<div class=\"oxfg\">%s</div>\n", p);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void ooxx_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void ooxx_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u8 ch;
	int x,y;
	logtoall("ooxx(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ch = data[y][x];
			if((ch!='o') && (ch!='x'))logtoall("_	");
			else logtoall("%c	",ch);
		}
		logtoall("\n");
	}
}




void ooxx_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	char val;
	int x,y;
/*
	int width = haha->whdf.width;
	int height = haha->whdf.height;
	int min = (width<height) ? width:height;
*/
	int min = 512;
	if(ev->what == 0x2d70)
	{
		x=(ev->why) & 0xffff;
		if(x>min)return;

		y=( (ev->why) >> 16 ) & 0xffff;
		if(y>min)return;

		x = x*3/min;
		y = y*3/min;
logtoall("%d,%d\n",x,y);
		if(0 != data[y][x])return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y][x] = val;
		turn++;
	}
}




static void ooxx_wrl_cam_wnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	ooxx_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void ooxx_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		ooxx_wrl_cam_wnd(ent,foot, stack,sp);
		break;
	}
}
static void ooxx_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
}
static void ooxx_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void ooxx_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void ooxx_search(_obj* act)
{
}
static void ooxx_modify(_obj* act)
{
}
static void ooxx_delete(_obj* act)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memoryfree(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void ooxx_create(_obj* act)
{
	int x,y;
	if(0 == act)return;
	act->listptr.buf0 = memoryalloc(16, 0);

	turn = 0;
	for(y=0;y<3;y++){
		for(x=0;x<3;x++){
			data[y][x] = 0;
		}
	}
}




void ooxx_register(_obj* p)
{
	p->kind = _game_;
	p->type = hex32('o', 'o', 'x', 'x');
	p->vfmt = _orig_;

	p->oncreate = (void*)ooxx_create;
	p->ondelete = (void*)ooxx_delete;
	p->onreader = (void*)ooxx_search;
	p->onwriter = (void*)ooxx_modify;

	p->onattach = (void*)ooxx_attach;
	p->ondetach = (void*)ooxx_detach;
	p->ontaking = (void*)ooxx_taking;
	p->ongiving = (void*)ooxx_giving;
}
