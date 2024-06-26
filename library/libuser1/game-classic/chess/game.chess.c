#include "libuser.h"




static u8 buffer[8][8];




static void chess_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u32 color;
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

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(((x+y+32)%2) != 0)color = 0x111111;
			else color = 0xffffff;

			drawsolid_rect(win, color,
				(cx-ww)+(2*x+0)*ww/8, (cy-hh)+(2*y+0)*hh/8,
				(cx-ww)+(2*x+2)*ww/8, (cy-hh)+(2*y+2)*hh/8
			);
			if(buffer[y][x] == 0)continue;

			drawascii(win, 0xff00ff,
				(cx-ww-4)+(2*x+1)*ww/8, (cy-hh-8)+(2*y+1)*hh/8,
				buffer[y][x]
			);
		}
	}
}
static void chess_draw_gl41(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	u32 rgb;
	int x,y;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			f[0] = (x+x-7)/8.0;
			f[1] = (7-y-y)/8.0;
			f[2] = 0.01;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];

			tr[0] = vr[0]/8.1;
			tr[1] = vr[1]/8.1;
			tr[2] = vr[2]/8.1;
			tf[0] = vf[0]/8.1;
			tf[1] = vf[1]/8.1;
			tf[2] = vf[2]/8.1;
			tu[0] = vu[0]*f[2];
			tu[1] = vu[1]*f[2];
			tu[2] = vu[2]*f[2];

			if(((x+y+32)%2) != 0)rgb = 0x111111;
			else rgb = 0xffffff;
			gl41solid_prism4(ctx, rgb, tc, tr, tf, tu);
		}
	}
}
static void chess_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void chess_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y,color;

	//<head>
	htmlprintf(win, 1,
		".chbg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".chfg{width:12.5%%;height:12.5%%;float:left;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"chbg\">\n");
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			if(0 != ((x+y)%2))color = 0x111111;
			else color = 0xffffff;

			htmlprintf(win, 2,
				"<div class=\"chfg\" style=\"background-color:#%06x\">%d</div>\n",
				color, buffer[y][x]
			);
		}
	}
	htmlprintf(win, 2, "</div>\n");
}
static void chess_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}
static void chess_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u8 ch;
	int x,y;
	logtoall("chess(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			ch = buffer[y][x];
			if(ch <= 0x20)logtoall("_	");
			else logtoall("%c	", ch);
		}
		logtoall("\n");
	}
}




static void chess_take_bywnd(_obj* ent,void* slot, _obj* caller,void* area, _syn* stack,int sp)
{
	switch(caller->vfmt){
	case _rgba_:
		break;
	case _gl41list_:
		break;
	}
}
static void chess_taking_byworld_bycam_bywnd(_obj* ent,void* slot, _syn* stack,int sp)
{
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;
	
	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	chess_draw_gl41(ent,slot, wor,geom, wnd,area);
}




static void chess_taking(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
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
		chess_take_bywnd(ent,foot, caller,area, stack,sp);
		break;
	default:
		chess_taking_byworld_bycam_bywnd(ent,foot, stack,sp);
		break;
	}
}
static void chess_giving(_obj* ent,void* foot, _syn* stack,int sp, p64 arg,int key, void* buf,int len)
{
	//logtoall("@chess:%x,%x\n", ev->why, ev->what);
}
static void chess_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void chess_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void chess_search(_obj* act)
{
}
static void chess_modify(_obj* act)
{
}
static void chess_delete(_obj* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memoryfree(act->listptr.buf0);
}
static void chess_create(_obj* act)
{
	int j,k;
	if(0 == act)return;
	if(_orig_ == act->type)act->listptr.buf0 = buffer;
	if(_copy_ == act->type)act->listptr.buf0 = memoryalloc(64, 0);

	for(k=0;k<8;k++)
	{
		for(j=0;j<8;j++)
		{
			buffer[k][j] = 0;
		}
	}

	//
	for(j=0;j<8;j++)
	{
		buffer[1][j] = 'p';
		buffer[6][j] = 'P';
	}

	//
	buffer[0][0] = 'r';
	buffer[0][1] = 'n';
	buffer[0][2] = 'b';
	buffer[0][3] = 'k';
	buffer[0][4] = 'q';
	buffer[0][5] = 'b';
	buffer[0][6] = 'n';
	buffer[0][7] = 'r';

	//
	buffer[7][0] = 'R';
	buffer[7][1] = 'N';
	buffer[7][2] = 'B';
	buffer[7][3] = 'Q';
	buffer[7][4] = 'K';
	buffer[7][5] = 'B';
	buffer[7][6] = 'N';
	buffer[7][7] = 'R';
}




void chess_register(_obj* p)
{
	p->kind = _game_;
	p->type = hex64('c', 'h', 'e', 's', 's', 0, 0, 0);
	p->vfmt = _orig_;

	p->oncreate = (void*)chess_create;
	p->ondelete = (void*)chess_delete;
	p->onreader = (void*)chess_search;
	p->onwriter = (void*)chess_modify;

	p->onattach = (void*)chess_attach;
	p->ondetach = (void*)chess_detach;
	p->ontaking = (void*)chess_taking;
	p->ongiving = (void*)chess_giving;
}
