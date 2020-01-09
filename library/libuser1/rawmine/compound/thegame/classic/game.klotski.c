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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid_rect(win, 0xffffff, vc, vr, vf);
}
static void klotski_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void klotski_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
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
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void klotski_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}




static void klotski_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void klotski_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
}
static void klotski_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void klotski_start(struct halfrel* self, struct halfrel* peer)
{
}




static void klotski_search(struct entity* act)
{
}
static void klotski_modify(struct entity* act)
{
}
static void klotski_delete(struct entity* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void klotski_create(struct entity* act)
{
	if(0 == act)return;
	if(_orig_ == act->type)act->buf = data;
	if(_copy_ == act->type)act->buf = memorycreate(20, 0);

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




void klotski_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('k', 'l', 'o', 't', 's', 'k', 'i', 0);

	p->oncreate = (void*)klotski_create;
	p->ondelete = (void*)klotski_delete;
	p->onsearch = (void*)klotski_search;
	p->onmodify = (void*)klotski_modify;

	p->onstart = (void*)klotski_start;
	p->onstop  = (void*)klotski_stop;
	p->onread  = (void*)klotski_read;
	p->onwrite = (void*)klotski_write;
}
