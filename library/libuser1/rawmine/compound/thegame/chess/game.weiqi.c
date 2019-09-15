#include "libuser.h"




static int px, py, turn;




static u8 data[19][19];
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u32 c;
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
			if(data[y+9][x+9] == 'b')c = 0x444444;
			else if(data[y+9][x+9] == 'w')c = 0xffffff;
			else continue;

			drawsolid_circle(
				win, c,
				cx + x*ww*2/19,
				cy + y*hh*2/19,
				ww/19
			);
		}
	}
}/*
static void weiqi_draw_vbo2d(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;
	int j,rgb;
	vec3 tc, tr, tf, tu, f;
	if(0 == sty)sty = defaultstyle_vbo2d();

	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid2d_rect(win, 0xf9d65b, vc, vr, vf);

	for(y=-9;y<10;y++)
	{
		f[0] = 18.0/19;
		f[1] = y*2.0/19;
		f[2] = 1.0/19/4;
		tc[0] = vc[0] - f[0]*vr[0] + f[1]*vf[0];
		tc[1] = vc[1] - f[0]*vr[1] + f[1]*vf[1];
		tc[2] = vc[2] - f[0]*vr[2] + f[1]*vf[2];
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
		carveline2d(win, 0x222222, tc, tr);
	}
	for(x=-9;x<10;x++)
	{
		f[0] = x*2.0/19;
		f[1] = 18.0/19;
		f[2] = 1.0/19/4;
		tc[0] = vc[0] + f[0]*vr[0] - f[1]*vf[0];
		tc[1] = vc[1] + f[0]*vr[1] - f[1]*vf[1];
		tc[2] = vc[2] + f[0]*vr[2] - f[1]*vf[2];
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2];
		carveline2d(win, 0x222222, tc, tr);
	}

	u8* data = act->buf;
	tr[0] = vr[0]/19;
	tr[1] = vr[1]/19;
	tr[2] = vr[2]/19;
	tf[0] = vf[0]/19;
	tf[1] = vf[1]/19;
	tf[2] = vf[2]/19;
	for(y=-9;y<10;y++)
	{
		for(x=-9;x<10;x++)
		{
			j = data[19*(y+9) + x+9];
			if('w' == j)rgb = 0xc0c0c0;
			else if('b' == j)rgb = 0x404040;
			else continue;

			tc[0] = vc[0] + tr[0]*x*2 - tf[0]*y*2;
			tc[1] = vc[1] + tr[1]*x*2 - tf[1]*y*2;
			tc[2] = vc[2] + tr[2]*x*2 - tf[2]*y*2 - 0.1;
			carvesolid2d_circle(win, rgb, tc, tr, tf);
		}
	}
}*/
static void weiqi_draw_vbo(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;
	int j,k,rgb;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->f.vc;
	float* vr = sty->f.vr;
	float* vf = sty->f.vf;
	float* vu = sty->f.vt;
	carvesolid_rect(win, 0xf9d65b, vc, vr, vf);

	for(y=-9;y<10;y++)
	{
		f[0] = 18.0/19;
		f[1] = y*2.0/19;
		f[2] = 1.0/19/4;
		tc[0] = vc[0] - f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] - f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] - f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		carveline(win, 0x222222, tc, tr);
	}
	for(x=-9;x<10;x++)
	{
		f[0] = x*2.0/19;
		f[1] = 18.0/19;
		f[2] = 1.0/19/4;
		tc[0] = vc[0] + f[0]*vr[0] - f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] + f[0]*vr[1] - f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] + f[0]*vr[2] - f[1]*vf[2] + f[2]*vu[2];
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		carveline(win, 0x222222, tc, tr);
	}

	u8* data = act->buf;
	k = 0;
	tr[0] = vr[0]/19;
	tr[1] = vr[1]/19;
	tr[2] = vr[2]/19;
	tf[0] = vf[0]/19;
	tf[1] = vf[1]/19;
	tf[2] = vf[2]/19;
	tu[0] = vu[0]/19/2;
	tu[1] = vu[1]/19/2;
	tu[2] = vu[2]/19/2;
	for(y=-9;y<10;y++)
	{
		for(x=-9;x<10;x++)
		{
			j = data[19*(y+9) + x+9];
			if('w' == j)rgb = 0xc0c0c0;
			else if('b' == j)rgb = 0x404040;
			else continue;

			tc[0] = vc[0] + tr[0]*x*2 - tf[0]*y*2 + tu[0];
			tc[1] = vc[1] + tr[1]*x*2 - tf[1]*y*2 + tu[1];
			tc[2] = vc[2] + tr[2]*x*2 - tf[2]*y*2 + tu[2];
			carvesolid_sphere(win, rgb, tc, tr, tf, tu);

			k++;
			//if(k>50)return;
		}
	}
}
static void weiqi_draw_json(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void weiqi_draw_html(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y;

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
				data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void weiqi_draw_tui(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	int x,y,j,k,ret,color;
	int width = win->stride;
	int height = win->height;
	u8* p = (u8*)(win->buf);

	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			//position
			ret = y*width + x*2;
			ret <<= 2;

			//color
			if( (px == x) && (py == y) )color = 7;
			else if(data[y][x] == 'b')color = 4;
			else if(data[y][x] == 'w')color = 1;
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
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
}
static void weiqi_draw(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)weiqi_draw_cli(act, pin, win, sty);
	else if(fmt == _tui_)weiqi_draw_tui(act, pin, win, sty);
	else if(fmt == _html_)weiqi_draw_html(act, pin, win, sty);
	else if(fmt == _json_)weiqi_draw_json(act, pin, win, sty);
	else if(fmt == _vbo_)
	{
		//if(_2d_ == win->vfmt)weiqi_draw_vbo2d(act, pin, win, sty);
		//else weiqi_draw_vbo(act, pin, win, sty);
	}
	else weiqi_draw_pixel(act, pin, win, sty);
}
static void weiqi_event(
	struct actor* act, struct style* pin,
	struct actor* win, struct style* sty,
	struct event* ev, int len)
{
	char val;
	int x,y;
	u64 what = ev->what;
	u64 key = ev->why;

	if(what == _kbd_)
	{
		if(key == 0x48)   //up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x4b)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x4d)   //right
		{
			if(px>=18)return;
			px++;
		}
		else if(key == 0x50)   //down
		{
			if(py>=18)return;
			py++;
		}
	}
	else if(what == _char_)
	{
		if(key == 0x20)
		{
			if((turn&1)==0)data[py][px] = 'b';
			else data[py][px] = 'w';
			turn++;
		}
	}
	else if(what == 0x2d70)
	{
		x=key & 0xffff;
		y=(key >> 16) & 0xffff;
		//say("%d,%d\n",x,y);

		x = (x*19)>>16;
		y = (y*19)>>16;
		//say("%d,%d\n",x,y);

		if(x<0)return;
		if(x>18)return;
		if(y<0)return;
		if(y>18)return;

		if((turn&0x1) == 0)data[y][x] = 'b';
		else data[y][x] = 'w';
		turn++;
	}
}




static void weiqi_read(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'draw' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct actor* ctx = buf;
	say("@weiqi_read:%llx,%llx,%llx\n",act,win,buf);

	if(ctx){
		if(_gl41data_ == ctx->type)weiqi_draw_vbo(act,pin,ctx,sty);
	}
	//weiqi_draw(act, pin, win, sty);
}
static void weiqi_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct actor* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct actor* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//weiqi_event(act, pin, win, sty, ev, 0);
}
static void weiqi_stop(struct halfrel* self, struct halfrel* peer)
{
}
static void weiqi_start(struct halfrel* self, struct halfrel* peer)
{
	turn = 0;
	px = py = 0;
}




static void weiqi_search(struct actor* act)
{
}
static void weiqi_modify(struct actor* act)
{
}
static void weiqi_delete(struct actor* act)
{
	if(0 == act)return;
	if(_copy_ == act->type)memorydelete(act->buf);
}
static void weiqi_create(struct actor* act, void* str)
{
	int ret;
	u8* buf;
	if(0 == act)return;

	if(_orig_ == act->type)buf = (void*)data;
	if(_copy_ == act->type)buf = memorycreate(19*19, 0);

	//read
	ret = 0;
	if(str)ret = weiqi_import(str, buf);
	if((0==str)|(ret<=0)){for(ret=0;ret<19*19;ret++)buf[ret] = 0;}

	//print
	for(ret=0;ret<19*19;ret+=19)printmemory(buf+ret, 19);

	//success
	act->buf = buf;
}




void weiqi_register(struct actor* p)
{
	p->type = _orig_;
	p->fmt = hex64('w', 'e', 'i', 'q','i', 0, 0, 0);

	p->oncreate = (void*)weiqi_create;
	p->ondelete = (void*)weiqi_delete;
	p->onsearch = (void*)weiqi_search;
	p->onmodify = (void*)weiqi_modify;

	p->onstart = (void*)weiqi_start;
	p->onstop  = (void*)weiqi_stop;
	p->onread  = (void*)weiqi_read;
	p->onwrite = (void*)weiqi_write;
}
