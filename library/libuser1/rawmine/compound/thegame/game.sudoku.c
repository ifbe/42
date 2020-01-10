#include "libuser.h"
void sudoku_generate(void*);
void sudoku_solve(void*);




int sudoku_import(char* file, u8* buf)
{
	int x,y,j;
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
			if(y >= 9)break;
			continue;
		}
		if(x<9)
		{
			if((tmp[j] >= 0x30)&&(tmp[j] <= 0x39))buf[y*9+x] = tmp[j] - 0x30;
			else buf[y*9+x] = 0;
			x++;
		}
	}

	//printmemory(buf, 81);
	return 1;
}




static void sudoku_draw_pixel(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	int t1, t2, t3, t4;
	int cx, cy, ww, hh;
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

	u8* data = act->buf;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			t1 = (cx-ww)+(2*x+0)*ww/9;
			t2 = (cy-hh)+(2*y+0)*hh/9;
			t3 = (cx-ww)+(2*x+2)*ww/9;
			t4 = (cy-hh)+(2*y+2)*hh/9;
			drawsolid_rect(win, 0xcccccc, t1, t2, t3, t4);
			drawline_rect(win, 0x222222, t1, t2, t3, t4);

			if(data[y*9+x] != 0)
			{
				drawdecimal(win, 0,
					(cx-ww-4)+(2*x+1)*ww/9,
					(cy-hh-8)+(2*y+1)*hh/9,
					data[y*9+x]
				);
			}
		}
	}
}
static void sudoku_draw_gl41(
	struct entity* act, struct style* part,
	struct entity* win, struct style* geom,
	struct entity* ctx, struct style* area)
{
	u32 rgb;
	int x,y;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->f.vc;
	float* vr = geom->f.vr;
	float* vf = geom->f.vf;
	float* vu = geom->f.vt;

	u8* data = act->buf;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if((act->ix0 == x)&&(act->iy0 == y))rgb = 0xff0000;
			else if((x>2)&&(x<6)&&(y>2)&&(y<6))rgb = 0xcccccc;
			else if((x<3)&&(y<3))rgb = 0x444444;
			else if((x<3)&&(y>5))rgb = 0x444444;
			else if((x>5)&&(y<3))rgb = 0x444444;
			else if((x>5)&&(y>5))rgb = 0x444444;
			else rgb = 0x888888;

			f[0] = (x+x+1)/9.0 - 1.0;
			f[1] = 1.0 - (y+y+1)/9.0;
			f[2] = 1.0/36;
			tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / 10;
			tr[1] = vr[1] / 10;
			tr[2] = vr[2] / 10;
			tf[0] = vf[0] / 10;
			tf[1] = vf[1] / 10;
			tf[2] = vf[2] / 10;
			tu[0] = vu[0] * f[2];
			tu[1] = vu[1] * f[2];
			tu[2] = vu[2] * f[2];
			gl41solid_prism4(ctx, rgb, tc, tr, tf, tu);
			if(data[y*9+x] != 0)
			{
				tc[0] += tu[0] + vu[0]*0.01;
				tc[1] += tu[1] + vu[1]*0.01;
				tc[2] += tu[2] + vu[2]*0.01;
				tr[0] = vr[0] / 9;
				tr[1] = vr[1] / 9;
				tr[2] = vr[2] / 9;
				tf[0] = vf[0] / 9;
				tf[1] = vf[1] / 9;
				tf[2] = vf[2] / 9;
				carveascii_center(ctx, ~rgb, tc, tr, tf, 0x30+data[y*9+x]);
			}
		}
	}
}
static void sudoku_draw_json(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
}
static void sudoku_draw_html(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	u8* data = act->buf;

	//<head>
	htmlprintf(win, 1,
		".sudokubg{width:50%%;height:50%%;float:left;background-color:#000;text-align:center;}\n"
		".sudokufg{width:10.9%%;height:10.9%%;float:left;background-color:#ccc;margin:0.1%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"sudokubg\">\n");
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			htmlprintf(win, 2,
				"<div class=\"sudokufg\">%c</div>\n",
				0x30+data[y*9+x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void sudoku_draw_tui(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y,j,k,ret,color;
	int stride = win->stride;
	char* p = (char*)(win->buf);

	u8* data = act->buf;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(data[y*9+x] == 0)continue;

			//position
			ret = (3*y+1)*stride + 6*x + 2;
			ret <<= 2;

			//color
			if( (act->ix0 == x)&&(act->ix0 == y) )color = 1;
			else if( ((x>2)&&(x<6)) && ((y<3)|(y>5)) )color = 2;
			else if( ((y>2)&&(y<6)) && ((x<3)|(x>5)) )color = 2;
			else color = 4;
			gentui_rect(win, color, x*6, y*3, x*6+5, y*3+2);

			//data
			p[ret] = data[y*9+x] + 0x30;
		}
	}
}
static void sudoku_draw_cli(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty)
{
	int x,y;
	u8* data = act->buf;

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(data[y*9+x] == 0)continue;
			say("%d	",data[y*9+x]);
		}
		say("\n");
	}
}




static void sudoku_event(
	struct entity* act, struct style* pin,
	struct entity* win, struct style* sty,
	struct event* ev, int len)
{
	u64 what = ev->what;
	u64 key = ev->why;
	if(what == _kbd_)
	{
		if(key == 0x48)	//up
		{
			if(act->iy0 < 1)return;
			act->iy0--;
		}
		else if(key == 0x4b)	//left
		{
			if(act->ix0 < 1)return;
			act->ix0--;
		}
		else if(key == 0x4d)	//right
		{
			if(act->ix0 < 0)return;
			if(act->ix0 >= 8)return;
			act->ix0++;
		}
		else if(key == 0x50)	//down
		{
			if(act->iy0 < 0)return;
			if(act->iy0 >= 8)return;
			act->iy0++;
		}
	}
}




static void sudoku_read(struct halfrel* self, struct halfrel* peer, struct halfrel** stack, int rsp, void* buf, int len)
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
		if('v' == len)sudoku_draw_gl41(act,part, win,geom, wnd,area);
	}
}
static void sudoku_write(struct halfrel* self, struct halfrel* peer, void* arg, int idx, void* buf, int len)
{
	//if 'ev i' == self.foot
	struct entity* act = (void*)(self->chip);
	struct style* pin = (void*)(self->foot);
	struct entity* win = (void*)(peer->chip);
	struct style* sty = (void*)(peer->foot);
	struct event* ev = (void*)buf;
	//sudoku_event(act, pin, win, sty, ev, 0);
}
static void sudoku_discon(struct halfrel* self, struct halfrel* peer)
{
}
static void sudoku_linkup(struct halfrel* self, struct halfrel* peer)
{
}




static void sudoku_search(struct entity* act, u8* buf)
{
}
static void sudoku_modify(struct entity* act, u8* buf)
{
}
static void sudoku_delete(struct entity* act, u8* buf)
{
	if(0 == act)return;
	memorydelete(act->buf);
}
static void sudoku_create(struct entity* act, void* str)
{
	int ret;
	void* buf;
	if(0 == act)return;

	//malloc
	buf = memorycreate(81, 0);
	if(0 == buf)return;

	//read
	ret = 0;
	if(str)ret = sudoku_import(str, buf);
	if((0==str)|(ret<=0))sudoku_generate(buf);

	//solve
	sudoku_solve(buf);
	for(ret=0;ret<81;ret+=9)printmemory(buf+ret, 9);

	act->buf = buf;
	act->ix0 = 0;
	act->iy0 = 0;
}




void sudoku_register(struct entity* p)
{
	p->type = _orig_;
	p->fmt = hex64('s', 'u', 'd', 'o', 'k', 'u', 0, 0);

	p->oncreate = (void*)sudoku_create;
	p->ondelete = (void*)sudoku_delete;
	p->onsearch = (void*)sudoku_search;
	p->onmodify = (void*)sudoku_modify;

	p->onlinkup = (void*)sudoku_linkup;
	p->ondiscon = (void*)sudoku_discon;
	p->onread  = (void*)sudoku_read;
	p->onwrite = (void*)sudoku_write;
}
