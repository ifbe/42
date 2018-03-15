#include "actor.h"
#define PI 3.1415926535897932384626433832795028841971693993151
#define _json_ hex32('j','s','o','n')
#define _xml_ hex32('x','m','l',0)
//test
void clock_register(void*);
void codeimg_register(void*);
void doodle_register(void*);
void example_register(void*);
void fractal_register(void*);
void palette_register(void*);
//game
void the2048_register(void*);
void chess_register(void*);
void klotski_register(void*);
void maze_register(void*);
void ooxx_register(void*);
void pegged_register(void*);
void poker_register(void*);
void rubikscube_register(void*);
void snake_register(void*);
void sudoku_register(void*);
void tetris_register(void*);
void weiqi_register(void*);
void xiangqi_register(void*);
//tool
void browser_register(void*);
void calculator_register(void*);
void camera_register(void*);
void circuit_register(void*);
void font_register(void*);
void qrcode_register(void*);
void sketchpad_register(void*);
void spectrum_register(void*);
void stl_register(void*);
void terminal_register(void*);
//hack
void algorithm_register(void*);
void bintree_register(void*);
void bplus_register(void*);
void fs_register(void*);
void graph_register(void*);
void hex_register(void*);
void input_register(void*);
//
int actorcreate(void*, u8*);
int actordelete(void*, u8*);
void* allocstyle();
void* allocpinid();
//
void* samepinprevchip(void*);
void* samepinnextchip(void*);
void* relation_read(u64);
void relation_write(void*, void*, u64, void*, void*, u64);
//
void draw8bit_rect(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1);
void carveascii_area(
	struct arena* win, u32 rgb,
	float cx, float cy, float cz,
	float rx, float ry, float rz,
	float fx, float fy, float fz,
	float x0, float y0, float x1, float y1);




static struct arena* arena = 0;
static struct actor* actor = 0;
int arenaactor(struct arena* win, struct actor* act)
{
	int min;
	int w = win->w;
	int h = win->h;
	int d = (w+h) / 2;
	struct style* sty;
	struct pinid* pin;

	sty = allocstyle();
	if(0 == sty)return 0;

	pin = allocpinid();
	if(0 == pin)return 0;

	sty->cx = w/2;
	sty->cy = h/2;
	sty->cz = 0.0;

	if(w<h)min = w/2;
	else min = h/2;
	sty->rx = min;
	sty->fy = min;
	sty->uz = min;

	relation_write(
		win, sty, _win_,
		act, pin, _act_
	);

	//win->onstart(win, sty);
	act->onstart(act, pin);
	return 0;
}




void login_read_pixel(struct arena* win)
{
	struct relation* rel;
	u32 c;
	int x,y,j,k;
	int w = win->w;
	int h = win->h;

	//arena
	for(j=0;j<64;j++)
	{
		c = arena[j].type;
		if(0 == c)break;
		else if(win == &arena[j])c = 0xffff00ff;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(
			win, c,
			(x+0)*w/8+1, h-1 - (y+1)*h/32,
			(x+1)*w/8-1, h+1 - (y+0)*h/32,
			(u8*)&arena[j].type, 8
		);
	}

	//actor
	for(j=0;j<64;j++)
	{
		c = actor[j].type & 0xff;
		if(0 == c)break;
		else if(j == win->flag3)c = 0xffff00ff;
		else if((c >= 'a')&&(c <= 'z'))c = 0x40808080;
		else c = 0x80ffffff;

		x = j%8;
		y = j/8;
		drawicon_1(
			win, c,
			(x+0)*w/8+1, (y+0)*h/32+1,
			(x+1)*w/8-1, (y+1)*h/32-1,
			(u8*)&actor[j].name, 8
		);
	}

	for(j=0;j<8;j++)
	{
		if(0 == arena[j].type)break;
		rel = arena[j].irel;
		while(1)
		{
			if(0 == rel)break;
			k = (void*)(rel->selfchip) - (void*)actor;
			k = k / sizeof(struct actor);
			drawline(win, 0xffeeff,
				(2*(k%8)+1)*w/16,
				(2*(k/8)+1)*h/64,
				(2*(j%8)+1)*w/16,
				h-(2*(j/8)+1)*h/64
			);
			rel = samepinnextchip(rel);
		}
	}
}
void login_read_8bit(struct arena* win)
{
	int x,y;
	int j,c;
	for(j=0;j<64;j++)
	{
		if(j == win->flag3)c = 0x80;
		else c = 0x42;

		x = j%4;
		y = j/4;
		draw8bit_rect(
			win, c,
			 81+x*40, 61+y*10,
			118+x*40, 68+y*10
		);
	}
}
void login_read_vbo(struct arena* win)
{
	u32 c;
	int j,k;
	float x,y;

	carveline(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		0.0, 0.0, 100.0
	);
	carveascii_area(
		win, 0xffffff,
		0.0, 0.0, 0.0,
		100.0, 0.0, 0.0,
		0.0, 100.0, 0.0,
		0.0, 0.0, 1.0, 1.0
	);

	for(j=0;j<64;j++)
	{
		if(j == win->flag3)
		{
			k = 4.0;
			c = 0x00ff00;
		}
		else
		{
			k = 1.0;
			c = 0xffffff;
		}

		x = cosine(j/PI)/32.0;
		y = sine(j/PI)/32.0;
		carvestring(
			win, c,
			x*256, y*256, j+1,
			x*64, y*64, 0.0,
			-y*64, x*64, 0.0,
			(u8*)&actor[j].name, 8
		);
	}
}
void login_read_html(struct arena* win)
{
	u32 c;
	int j;
	int len = win->len;
	u8* buf = win->buf;

	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;

		if(j == win->flag3)c = 0xff0000;
		else c = 0xffffff;

		len += mysnprintf(
			buf+len, 0x1000,
			"<div style=\"float:left;width:25%%;color:#%06x;\">%.8s</div>",
			c, (u8*)&actor[j].name
		);
	}
	win->len = len;
}
void login_read_tui(struct arena* win)
{
	int j,k,x,y;
	int ww = ((win->w)/2)&0xfffc;
	int hh = (win->h)/2;

	gentui_rect(win, 4, ww/2, hh/2, ww*3/2, hh*3/2);

	for(j=0;j<64;j++)
	{
		if(0 == actor[j].name)break;

		if(j == win->flag3)k=1;
		else k=2;

		x = j%4;
		x = ww + (x-2)*ww/4;
		y = j/4;
		y = hh + (y-4);

		gentui_rect(win, k, x, y, x+7, y);
		gentui_str(win, 0, x, y, (u8*)&actor[j].name, 8);
	}
}
void login_read_cli(struct arena* win)
{
}
void login_read(struct arena* win)
{
	if(win->fmt == _cli_)login_read_cli(win);
	else if(win->fmt == _tui_)login_read_tui(win);
	else if(win->fmt == _vbo_)login_read_vbo(win);
	else if(win->fmt == _html_)login_read_html(win);
	else if(win->fmt == _8bit_)login_read_8bit(win);
	else login_read_pixel(win);
}
void login_write(struct arena* win, struct event* ev)
{
	struct arena* p;
	struct actor* q;
	u16* src;
	u16* dst;
	int j,k,x,y,flag3;
	j = (ev->what)&0xff;
	k = ((ev->what)>>8)&0xff;
	flag3 = win->flag3;

	if(j == 'p')
	{
		x = (ev->why)&0xffff;
		x = (x*8) / (win->w);
		y = ((ev->why)>>16)&0xffff;
		y = (y*32) / (win->h);
		if('@' == k)
		{
			src = (u16*)(&ev->why);
			dst = (u16*)(&win->touchmove[10]);
			dst[0] = src[0];
			dst[1] = src[1];
			if(y < 8)win->flag3 = x + (y*8);
		}
		else if('+' == k)
		{
			src = (u16*)(&ev->why);
			dst = (u16*)(&win->touchdown[10]);
			dst[0] = src[0];
			dst[1] = src[1];
			if(y < 8)win->flag3 = x + (y*8);
		}
		else if('-' == k)
		{
			j = win->touchdown[10].x;
			j = (j*8) / (win->w);
			k = win->touchdown[10].y;
			k = (k*32) / (win->h);

			if((j<0)|(j>=8))return;
			if((k<0)|(k>=32))return;
			if((x<0)|(x>=8))return;
			if((y<0)|(y>=32))return;

			if((j==x)&&(k==y))
			{
				flag3 = x + (y*8);
				if(y<16)
				{
					q = &actor[flag3];
					if((q->type)&0xff < 'a')return;
					actorcreate(q, 0);
				}
				else
				{
					y = 31-y;
					say("@arena:%d\n", (y*8)+x);
				}
				return;
			}
			else if((k<16)&&(y>=16))
			{
				y = 31-y;
				p = &arena[x + (y*8)];
				if(0 == p->type)return;

				q = &actor[j + (k*8)];
				if(0 == q->type)return;

				arenaactor(p, q);
			}
			else if((y<16)&&(k>=16))
			{
				k = 31-k;
				p = &arena[j + (k*8)];
				if(0 == p->type)return;

				q = &actor[x + (y*8)];
				if(0 == q->type)return;

				say("actor:%d to arena:%d\n", x+(y*8), j+(k*8));
			}

			win->flag0 = 0;
			win->flag3 = 0;
		}
	}
	else if(_char_ == ev->what)
	{
		if((ev->why == 0xd)|(ev->why == 0xa))
		{
			arenaactor(win, &actor[flag3]);
			win->flag0 = 0;
			win->flag3 = 0;
		}
		else if(ev->why == 0x435b1b)
		{
			win->flag3 = (flag3+1)%64;
		}
		else if(ev->why == 0x445b1b)
		{
			win->flag3 = (flag3+63)%64;
		}
	}
	else if(_kbd_ == ev->what)
	{
		if(ev->why == 0x4b)
		{
			win->flag3 = (flag3+63)%64;
		}
		else if(ev->why == 0x4d)
		{
			win->flag3 = (flag3+1)%64;
		}
	}
}
void content_create(void* addr)
{
	void* temp;
	arena = addr + 0x000000;
	actor = addr + 0x100000;
	temp = actor;

	//test.clock
	clock_register(temp);
	temp += sizeof(struct actor);

	//test.codeimg
	codeimg_register(temp);
	temp += sizeof(struct actor);

	//test.doodle
	doodle_register(temp);
	temp += sizeof(struct actor);

	//test.example
	example_register(temp);
	temp += sizeof(struct actor);

	//test.fractal
	fractal_register(temp);
	temp += sizeof(struct actor);

	//test.palette
	palette_register(temp);
	temp += sizeof(struct actor);

	//game.2048
	the2048_register(temp);
	temp += sizeof(struct actor);

	//game.chess
	chess_register(temp);
	temp += sizeof(struct actor);

	//game.klotski
	klotski_register(temp);
	temp += sizeof(struct actor);

	//game.maze
	maze_register(temp);
	temp += sizeof(struct actor);

	//game.ooxx
	ooxx_register(temp);
	temp += sizeof(struct actor);

	//game.pegged
	pegged_register(temp);
	temp += sizeof(struct actor);

	//game.poker
	poker_register(temp);
	temp += sizeof(struct actor);

	//game.rubik
	rubikscube_register(temp);
	temp += sizeof(struct actor);

	//game.snake
	snake_register(temp);
	temp += sizeof(struct actor);

	//game.sudoku
	sudoku_register(temp);
	temp += sizeof(struct actor);

	//game.tetris
	tetris_register(temp);
	temp += sizeof(struct actor);

	//game.weiqi
	weiqi_register(temp);
	temp += sizeof(struct actor);

	//game.xiangqi
	xiangqi_register(temp);
	temp += sizeof(struct actor);

	//tool.browser
	browser_register(temp);
	temp += sizeof(struct actor);

	//tool.calculator
	calculator_register(temp);
	temp += sizeof(struct actor);

	//tool.camera
	camera_register(temp);
	temp += sizeof(struct actor);

	//tool.circuit
	circuit_register(temp);
	temp += sizeof(struct actor);

	//tool.font
	font_register(temp);
	temp += sizeof(struct actor);

	//tool.qrcode
	qrcode_register(temp);
	temp += sizeof(struct actor);

	//tool.sketchpad
	sketchpad_register(temp);
	temp += sizeof(struct actor);

	//tool.stl
	stl_register(temp);
	temp += sizeof(struct actor);

	//tool.spectrum
	spectrum_register(temp);
	temp += sizeof(struct actor);

	//tool.terminal
	terminal_register(temp);
	temp += sizeof(struct actor);

	//hack.algorithm
	algorithm_register(temp);
	temp += sizeof(struct actor);

	//hack.bintree
	bintree_register(temp);
	temp += sizeof(struct actor);

	//hack.bplus
	bplus_register(temp);
	temp += sizeof(struct actor);

	//hack.fs
	fs_register(temp);
	temp += sizeof(struct actor);

	//hack.graph
	graph_register(temp);
	temp += sizeof(struct actor);

	//hack.hex
	hex_register(temp);
	temp += sizeof(struct actor);

	//hack.input
	input_register(temp);
	temp += sizeof(struct actor);
}
void content_delete()
{
}