#include "libuser.h"




static u8 buffer[8][8];




static void chess_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
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
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chess_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void chess_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8 ch;
	int x,y;
	say("chess(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			ch = buffer[y][x];
			if(ch <= 0x20)say("_	");
			else say("%c	", ch);
		}
		say("\n");
	}
}




static void chess_taking(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	struct style* slot;
	struct entity* wor;struct style* geom;
	struct entity* wnd;struct style* area;
	if(stack && ('v'==key)){
		slot = stack[sp-1].pfoot;
		wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
		wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
		chess_draw_gl41(ent,slot, wor,geom, wnd,area);
	}
}
static void chess_giving(_ent* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//say("@chess:%x,%x\n", ev->why, ev->what);
}
static void chess_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void chess_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void chess_search(struct entity* act)
{
}
static void chess_modify(struct entity* act)
{
}
static void chess_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf0);
}
static void chess_create(struct entity* act)
{
	int j,k;
	if(0 == act)return;
	if(_orig_ == act->type)act->buf0 = buffer;
	if(_copy_ == act->type)act->buf0 = memorycreate(64, 0);

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




void chess_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('c', 'h', 'e', 's', 's', 0, 0, 0);

	p->oncreate = (void*)chess_create;
	p->ondelete = (void*)chess_delete;
	p->onsearch = (void*)chess_search;
	p->onmodify = (void*)chess_modify;

	p->onlinkup = (void*)chess_linkup;
	p->ondiscon = (void*)chess_discon;
	p->ontaking = (void*)chess_taking;
	p->ongiving = (void*)chess_giving;
}
