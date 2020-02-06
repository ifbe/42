#include "libuser.h"
#define level 4
void rubikscube_generate(void*, int);
void rubikscube_solve(void*, int);




//green, blue, red, orange, yellow, white
u32 rubikcolor[6] = {0x00ff00,0x0000ff,0xff0000,0xfa8010,0xffff00,0xffffff};




//left, right, near, far, bottom, upper
int rubikscube_import(char* file, u8 (*buf)[4][4])
{
	int x,y;
	int j,t;
	u8 tmp[0x100];
	j = openreadclose(file, 0, tmp, 0x100);
	//printmemory(tmp, 0x100);
	if(j<=0)return 0;

	x = y = 0;
	for(j=0;j<0x100;j++)
	{
		if(0xd == tmp[j])continue;
		if(0xa == tmp[j]){
			x = 0;y += 1;
			if(y >= 6)break;
			continue;
		}
		if(x<16)
		{
			t = tmp[j];
			switch(t)
			{
				case 'g':t = 0x30;break;
				case 'b':t = 0x31;break;
				case 'r':t = 0x32;break;
				case 'o':t = 0x33;break;
				case 'y':t = 0x34;break;
				case 'w':t = 0x35;break;
			}
			if((t >= 0x30)&&(t <= 0x35))
			{
				buf[y][x/4][x%4] = t - 0x30;
				x++;
			}
		}
	}

	//printmemory(buf, 81);
	return 1;
}




static void rubikscube_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	u32 bg;
	int x, y, cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->f.vc[0];
		cy = sty->f.vc[1];
		ww = sty->f.vr[0];
		hh = sty->f.vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}
	drawline_rect(win, 0x00ff00, cx-ww, cy-hh, cx+ww, cy+hh);

	u8 (*buf)[4][4] = act->buf0;
	if(0 == buf)return;

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			bg = rubikcolor[(buf[0][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//right
			bg = rubikcolor[(buf[1][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+4*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+4*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			bg = rubikcolor[(buf[2][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);

			//far
			bg = rubikcolor[(buf[3][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+6*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+2*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+6*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+2*level)*hh/4/level
			);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			bg = rubikcolor[(buf[4][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0+4*level)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2+4*level)*hh/4/level
			);

			//upper
			bg = rubikcolor[(buf[5][y][x])%6];
			drawsolid_rect(win, bg,
				(cx-ww+1) + (2*x+0+2*level)*ww/4/level,
				(cy-hh+1) + (2*y+0)*hh/4/level,
				(cx-ww-1) + (2*x+2+2*level)*ww/4/level,
				(cy-hh-1) + (2*y+2)*hh/4/level
			);
		}
	}
}
static void rubikscube_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	int x,y,rgb;
	vec3 f;
	vec3 tc, tr, tf, tu;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	u8 (*buf)[4][4] = act->buf0;
	if(0 == buf)return;

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//left
			f[0] = -1.0;
			f[1] = 1.0 - (2.0*x+1.0)/level;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = -vf[0] / (level+0.5);
			tr[1] = -vf[1] / (level+0.5);
			tr[2] = -vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[0][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);

			//right
			f[0] = 1.0;
			f[1] = (2.0*x+1.0)/level - 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vf[0] / (level+0.5);
			tr[1] = vf[1] / (level+0.5);
			tr[2] = vf[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[1][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//near
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = -1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[2][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);

			//far
			f[0] = 1.0 - (2.0*x+1.0)/level;
			f[1] = 1.0;
			f[2] = (2.0*y+1.0)/level - 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = -vr[0] / (level+0.5);
			tr[1] = -vr[1] / (level+0.5);
			tr[2] = -vr[2] / (level+0.5);
			tf[0] = vu[0] / (level+0.5);
			tf[1] = vu[1] / (level+0.5);
			tf[2] = vu[2] / (level+0.5);
			rgb = rubikcolor[(buf[3][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);
		}
	}

	for(y=0;y<level;y++)
	{
		for(x=0;x<level;x++)
		{
			//bottom
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = (2.0*y+1.0)/level - 1.0;
			f[2] = -1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = -vf[0] / (level+0.5);
			tf[1] = -vf[1] / (level+0.5);
			tf[2] = -vf[2] / (level+0.5);
			rgb = rubikcolor[(buf[4][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);

			//upper
			f[0] = (2.0*x+1.0)/level - 1.0;
			f[1] = (2.0*y+1.0)/level - 1.0;
			f[2] = 1.0;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / (level+0.5);
			tr[1] = vr[1] / (level+0.5);
			tr[2] = vr[2] / (level+0.5);
			tf[0] = vf[0] / (level+0.5);
			tf[1] = vf[1] / (level+0.5);
			tf[2] = vf[2] / (level+0.5);
			rgb = rubikcolor[(buf[5][y][x])%6];
			gl41solid_rect(ctx, rgb, tc, tr, tf);
		}
	}
}
static void rubikscube_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rubikscube_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int len = win->rawlen;
	u8* buf = win->rawbuf;

	len += mysnprintf(
		buf+len, 0x100000-len,
		"<div id=\"rubik\" style=\"width:50%%;height:100px;float:left;background-color:#404040;\">"
	);
	len += mysnprintf(buf+len, 0x100000-len, "</div>\n");
	win->rawlen = len;
}
static void rubikscube_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void rubikscube_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	say("rubik(%x,%x,%x)\n",win,act,sty);
}




static void rubikscube_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	if(ev->what == _kbd_)
	{
	}
	else if(ev->what == _char_)
	{
	}
}




static void rubikscube_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
{
	//wnd -> cam
	struct entity* wnd;struct style* area;

	//cam -> world
	struct entity* wrd;struct style* camg;

	//world -> this
	struct entity* win;struct style* geom;
	struct entity* act;struct style* part;

	if(stack){
		wnd = stack[rsp-4]->pchip;area = stack[rsp-4]->pfoot;
		wrd = stack[rsp-1]->pchip;camg = stack[rsp-1]->pfoot;

		win = peer->pchip;geom = peer->pfoot;
		act = self->pchip;part = self->pfoot;
		if('v' == len)rubikscube_draw_gl41(act,part, win,geom, wnd,area);
	}
	//rubikscube_draw(act, pin, win, sty);
}
static void rubikscube_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	say("@rubik_write\n");
}
static void rubikscube_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void rubikscube_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void rubikscube_search(struct entity* act)
{
}
static void rubikscube_modify(struct entity* act)
{
}
static void rubikscube_delete(struct entity* act)
{
	if(0 == act)return;
	if(act->buf0){
		memorydelete(act->buf0);
		act->buf0 = 0;
	}
}
static void rubikscube_create(struct entity* act, void* str)
{
	int ret;
	void* buf;
	if(0 == act)return;
//printmemory(str,4);

	//malloc
	buf = act->buf0 = memorycreate(6 * level * level, 0);
	if(0 == buf)return;

	//read
	ret = 0;
	if(str)ret = rubikscube_import(str, buf);
	if((0==str)|(ret<=0))rubikscube_generate(buf, level);

	//
	for(ret=0;ret<6;ret++)printmemory(buf + level*level*ret, level*level);
}




void rubikscube_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('r', 'u', 'b', 'i', 'k', 0, 0, 0);

	p->oncreate = (void*)rubikscube_create;
	p->ondelete = (void*)rubikscube_delete;
	p->onsearch = (void*)rubikscube_search;
	p->onmodify = (void*)rubikscube_modify;

	p->onlinkup = (void*)rubikscube_linkup;
	p->ondiscon = (void*)rubikscube_discon;
	p->onread  = (void*)rubikscube_read;
	p->onwrite = (void*)rubikscube_write;
}
