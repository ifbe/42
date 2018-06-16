#include "libuser.h"
void sudoku_generate(void*);
void sudoku_solve(void*);




//
static int px,py;
static u8 data[9][9];




static void sudoku_read_pixel(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	int t1, t2, t3, t4;
	int cx, cy, ww, hh;
	if(sty)
	{
		cx = sty->vc[0];
		cy = sty->vc[1];
		ww = sty->vr[0];
		hh = sty->vf[1];
	}
	else
	{
		cx = win->width/2;
		cy = win->height/2;
		ww = win->width/2;
		hh = win->height/2;
	}

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

			if(data[y][x] != 0)
			{
				drawdecimal(win, 0,
					(cx-ww-4)+(2*x+1)*ww/9,
					(cy-hh-8)+(2*y+1)*hh/9,
					data[y][x]
				);
			}
		}
	}
}
static void sudoku_read_vbo(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u32 rgb;
	int x,y;
	vec3 tc, tr, tf, tu, f;
	float* vc = sty->vc;
	float* vr = sty->vr;
	float* vf = sty->vf;
	float* vu = sty->vu;
	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if((x>2)&&(x<6)&&(y>2)&&(y<6))rgb = 0xcccccc;
			else if((x<3)&&(y<3))rgb = 0x444444;
			else if((x<3)&&(y>5))rgb = 0x444444;
			else if((x>5)&&(y<3))rgb = 0x444444;
			else if((x>5)&&(y>5))rgb = 0x444444;
			else rgb = 0x888888;

			f[0] = (x+x+1)/9.0 - 1.0;
			f[1] = (y+y+1)/9.0 - 1.0;
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
			carvesolid_prism4(win, rgb, tc, tr, tf, tu);
			if(data[y][x] != 0)
			{
				tc[0] += tu[0] + vu[0]*0.01;
				tc[1] += tu[1] + vu[1]*0.01;
				tc[2] += tu[2] + vu[2]*0.01;
				tr[0] = vr[0] / 18;
				tr[1] = vr[1] / 18;
				tr[2] = vr[2] / 18;
				tf[0] = vf[0] / 18;
				tf[1] = vf[1] / 18;
				tf[2] = vf[2] / 18;
				carveascii(win, ~rgb, tc, tr, tf, 0x30+data[y][x]);
			}
		}
	}
}
static void sudoku_read_json(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
}
static void sudoku_read_html(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;

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
				0x30+data[y][x]
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}
static void sudoku_read_tui(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y,j,k,ret,color;
	int stride = win->stride;
	char* p = (char*)(win->buf);

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(data[y][x] == 0)continue;

			//position
			ret = (3*y+1)*stride + 6*x + 2;
			ret <<= 2;

			//color
			if( (px == x)&&(py == y) )color = 1;
			else if( ((x>2)&&(x<6)) && ((y<3)|(y>5)) )color = 2;
			else if( ((y>2)&&(y<6)) && ((x<3)|(x>5)) )color = 2;
			else color = 4;
			gentui_rect(win, color, x*6, y*3, x*6+5, y*3+2);

			//data
			p[ret] = data[y][x] + 0x30;
		}
	}
}
static void sudoku_read_cli(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	int x,y;
	say("sudoku(%x,%x,%x)\n",win,act,sty);

	for(y=0;y<9;y++)
	{
		for(x=0;x<9;x++)
		{
			if(data[y][x] == 0)continue;
			say("%d	",data[y][x]);
		}
		say("\n");
	}
}
static void sudoku_read(
	struct arena* win, struct style* sty,
	struct actor* act, struct pinid* pin)
{
	u64 fmt = win->fmt;

	if(fmt == _cli_)sudoku_read_cli(win, sty, act, pin);
	else if(fmt == _tui_)sudoku_read_tui(win, sty, act, pin);
	else if(fmt == _html_)sudoku_read_html(win, sty, act, pin);
	else if(fmt == _json_)sudoku_read_json(win, sty, act, pin);
	else if(fmt == _vbo_)sudoku_read_vbo(win, sty, act, pin);
	else sudoku_read_pixel(win, sty, act, pin);
}
static void sudoku_write(
	struct actor* act, struct pinid* pin,
	struct arena* win, struct style* sty,
	struct event* ev, int len)
{
	u64 what = ev->what;
	u64 key = ev->why;
	if(what == _kbd_)
	{
		if(key == 0x48)	//up
		{
			if(py<1)return;
			py--;
		}
		else if(key == 0x4b)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(key == 0x4d)	//right
		{
			if(px<0)return;
			if(px>=8)return;
			px++;
		}
		else if(key == 0x50)	//down
		{
			if(py<0)return;
			if(py>=8)return;
			py++;
		}
	}
}
static void sudoku_list()
{
}
static void sudoku_choose()
{
}
static void sudoku_stop(struct actor* act, struct pinid* pin)
{
}
static void sudoku_start(struct actor* act, struct pinid* pin)
{
	u8* p;
	int j;
	px = py = 0;

	p = (u8*)data;
	for(j=0;j<81;j++)
	{
		if(0 != p[j])break;
	}
	if(j >= 81)sudoku_generate(data);
	else sudoku_solve(data);
}
static void sudoku_delete(struct actor* act, u8* buf)
{
	if(0 == act)return;
	if(_COPY_ == act->type)memorydelete(act->buf);
}
static void sudoku_create(struct actor* act, u8* buf)
{
	u8* p;
	int j,k;
	if(0 == act)return;
	else if(_orig_ == act->type)act->buf = data;
	else if(_copy_ == act->type)act->buf = memorycreate(81);

	if(0 == buf)return;
	p = act->buf;
	k = 0;
	for(j=0;j<81;j++)p[j] = 0;
	for(j=0;j<0x100;j++)
	{
		if((buf[j]>=0x30)&&(buf[j]<=0x39))
		{
			//say("%c", buf[j]);
			p[k] = buf[j]-0x30;
			k++;
		}
		if(k >= 81)break;
	}
	say("\n");
}




void sudoku_register(struct actor* p)
{
	p->type = _orig_;
	p->name = hex64('s', 'u', 'd', 'o', 'k', 'u', 0, 0);

	p->oncreate = (void*)sudoku_create;
	p->ondelete = (void*)sudoku_delete;
	p->onstart  = (void*)sudoku_start;
	p->onstop   = (void*)sudoku_stop;
	p->onlist   = (void*)sudoku_list;
	p->onchoose = (void*)sudoku_choose;
	p->onread   = (void*)sudoku_read;
	p->onwrite  = (void*)sudoku_write;
}
