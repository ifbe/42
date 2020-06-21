#include "libuser.h"




static int turn;
static u8 data[3][3];




static void ooxx_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float* vc = sty->fs.vc;
	float* vr = sty->fs.vr;
	float* vf = sty->fs.vf;
	float* vu = sty->fs.vt;
	gl41solid_rect(win, 0x444444, vc, vr, vf);
}
static void ooxx_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ooxx_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void ooxx_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u8 ch;
	int x,y;
	say("ooxx(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<3;y++)
	{
		for(x=0;x<3;x++)
		{
			ch = data[y][x];
			if((ch!='o') && (ch!='x'))say("_	");
			else say("%c	",ch);
		}
		say("\n");
	}
}




void ooxx_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	char val;
	int x,y;
/*
	int width = haha->width;
	int height = haha->height;
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
say("%d,%d\n",x,y);
		if(0 != data[y][x])return;

		if((turn&0x1) == 0x1)val='o';
		else val='x';

		data[y][x] = val;
		turn++;
	}
}




static void ooxx_taking(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void ooxx_giving(_ent* ent,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
}
static void ooxx_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void ooxx_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void ooxx_search(struct entity* act)
{
}
static void ooxx_modify(struct entity* act)
{
}
static void ooxx_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void ooxx_create(struct entity* act)
{
	int x,y;
	if(0 == act)return;
	act->buf0 = memorycreate(16, 0);

	turn = 0;
	for(y=0;y<3;y++){
		for(x=0;x<3;x++){
			data[y][x] = 0;
		}
	}
}




void ooxx_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex32('o', 'o', 'x', 'x');

	p->oncreate = (void*)ooxx_create;
	p->ondelete = (void*)ooxx_delete;
	p->onsearch = (void*)ooxx_search;
	p->onmodify = (void*)ooxx_modify;

	p->onlinkup = (void*)ooxx_linkup;
	p->ondiscon = (void*)ooxx_discon;
	p->ontaking = (void*)ooxx_taking;
	p->ongiving = (void*)ooxx_giving;
}
