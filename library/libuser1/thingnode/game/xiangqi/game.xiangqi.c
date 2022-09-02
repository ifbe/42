#include "libuser.h"
#define _mind_ hex32('s','o','u','l')
#define _sync_ hex32('s','y','n','c')
#define PICKx whdf.ix0
#define PICKy whdf.iy0
#define TURN whdf.iw0
void xiangqi_generate(char (*data)[9]);
void xiangqi_move(char (*data)[9], int* turn, int px, int py, int x, int y);




struct perxiangqi{
	char data[10][9];
	int px;
	int py;
	int qx;
	int qy;
};
int xiangqi_import(u8* buf, u8* str)
{
	int x,y,j;
	u8 tmp[0x100];
	j = openreadclose(str, 0, tmp, 0x100);
	printmemory(tmp, 0x100);
	if(j<=0)return 0;

	x = y = 0;
	for(j=0;j<0x100;j++)
	{
		if(0xd == tmp[j])continue;
		if(0xa == tmp[j]){
			x = 0;y += 1;
			if(y >= 10)break;
			continue;
		}
		if(x<9)
		{
			if(	((tmp[j] >= 'A')&&(tmp[j] <= 'Z')) |
				((tmp[j] >= 'a')&&(tmp[j] <= 'z')) )
			{
				buf[y*9+x] = tmp[j];
			}
			else buf[y*9+x] = 0;
			x++;
		}
	}
	return 1;
}
void xiangqi_copydata(u8* dst, u8* src)
{
	int j;
	for(j=0;j<90;j++)dst[j] = src[j];
}
int xiangqi_pickup(struct perxiangqi* xq, int x, int y, int turn)
{
	if( (x==xq->px) && (y==xq->py) )
	{
		xq->px = xq->py = -1;
		return 1;
	}

	//chess choosing
	if( (xq->px<0) | (xq->py<0) | (xq->data[xq->py][xq->px] == 0) )
	{
		if( (xq->data[y][x]>='a') && (xq->data[y][x]<='z') && ((turn&1) == 0) )
		{
			xq->px = x;
			xq->py = y;
		}
		if( (xq->data[y][x]>='A') && (xq->data[y][x]<='Z') && ((turn&1) == 1) )
		{
			xq->px = x;
			xq->py = y;
		}
		return 2;
	}

	return 0;
}
void xiangqi_event(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty,
	struct event* ev, int len)
{
	int x, y, ret;
	u64 key = ev->why;
	u64 what = ev->what;

	struct perxiangqi* xq = act->listptr.buf0;
	if(what == _kbd_)
	{
		if(key == 0x48)	//up
		{
			if(xq->qy<1)return;
			xq->qy--;
		}
		else if(key == 0x4b)	//left
		{
			if(xq->qx<1)return;
			xq->qx--;
		}
		else if(key == 0x4d)	//right
		{
			if(xq->qx<0)return;
			if(xq->qx>=8)return;
			xq->qx++;
		}
		else if(key == 0x50)	//down
		{
			if(xq->qy<0)return;
			if(xq->qy>=9)return;
			xq->qy++;
		}
	}

	else if(what == _char_)
	{
		if(key == 0x20)
		{
			ret = xiangqi_pickup(xq, xq->qx, xq->qy, act->TURN);
			if(ret > 0)return;

			//move?
			xiangqi_move(xq->data, &act->TURN, xq->px, xq->py, xq->qx, xq->qy);
		}
		else if(key == 0x415b1b)
		{
			if(xq->qy<1)return;	//up
			xq->qy--;
		}
		else if(key == 0x425b1b)	//down
		{
			if(xq->qy<0)return;
			if(xq->qy>=9)return;
			xq->qy++;
		}
		else if(key == 0x435b1b)	//right
		{
			if(xq->qx<0)return;
			if(xq->qx>=8)return;
			xq->qx++;
		}
		else if(key == 0x445b1b)	//left
		{
			if(xq->qx<1)return;
			xq->qx--;
		}
	}

	else if(what == 0x2b70)
	{
		//x = key & 0xffff;
		//y = (key >> 16) & 0xffff;
		//say("%d,%d => ",x,y);

		//x = (x*9)>>16;
		//y = (y*10)>>16;
		//say("%d,%d\n",x,y);

		x = (key & 0xffff) / 6;
		y = ((key >> 16) & 0xffff) / 3;

		if(x < 0)return;
		if(x > 8)return;
		if(y < 0)return;
		if(y > 9)return;

		//pick?
		ret = xiangqi_pickup(xq, x, y, act->TURN);
		if(ret > 0)return;

		//move?
		xiangqi_move(xq->data, &act->TURN, xq->px, xq->py, x, y);
		xq->px = xq->py = -1;
	}
}




void* char2hanzi(int val)
{
	switch(val)
	{
		//black
		case 'A':return "車";
		case 'B':return "馬";
		case 'C':return "象";
		case 'D':return "士";
		case 'E':return "将";
		case 'S':return "卒";
		case 'Z':return "炮";

		//red
		case 'a':return "車";
		case 'b':return "馬";
		case 'c':return "相";
		case 'd':return "仕";
		case 'e':return "帥";
		case 's':return "兵";
		case 'z':return "炮";
	}
	return 0;
}
void xiangqi_draw_pixel(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	u32 black, brown, red;
	u32 chesscolor, fontcolor, temp;
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

	black = 0;
	if(0x626772 == (win->hfmt&0xffffff))
	{
		temp = 0x256f8d;
		red = 0xff;
		brown = 0x36878d;
	}
	else
	{
		temp = 0x8d6f25;
		red = 0xff0000;
		brown = 0x8d8736;
	}
	drawsolid_rect(win, temp, cx-ww, cy-hh, cx+ww, cy+hh);

	//heng
	for(y=-5;y<5;y++)
	{
		drawline(
			win, 0,
			cx - ww*8/9,
			cy + hh*(2*y+1)/10,
			cx + ww*8/9,
			cy + hh*(2*y+1)/10
		);
	}

	//shu
	for(x=-4;x<5;x++)
	{
		drawline(win, 0,
			cx + x*ww*2/9,	cy - hh*9/10,
			cx + x*ww*2/9,	cy - hh/10);
		drawline(win, 0,
			cx + x*ww*2/9,	cy + hh*9/10,
			cx + x*ww*2/9,	cy + hh/10);
	}

	//pie,na
	drawline(win, 0,
		cx - ww*2/9,
		cy - hh*9/10,
		cx + ww*2/9,
		cy - hh*5/10
	);
	drawline(win, 0,
		cx + ww*2/9,
		cy - hh*9/10,
		cx - ww*2/9,
		cy - hh*5/10
	);
	drawline(win, 0,
		cx - ww*2/9,
		cy + hh*9/10,
		cx + ww*2/9,
		cy + hh*5/10
	);
	drawline(win, 0,
		cx + ww*2/9,
		cy + hh*9/10,
		cx - ww*2/9,
		cy + hh*5/10
	);

	struct perxiangqi* xq = act->listptr.buf0;

	//chess
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			if(*(u8*)pin == 'r')temp = xq->data[y][x];
			else temp = xq->data[9-y][8-x];

			if(     (temp >= 'A')&&(temp <= 'Z'))fontcolor = black;
			else if((temp >= 'a')&&(temp <= 'z'))fontcolor = red;
			else continue;

			if( (xq->px == x)&&(xq->py == y) )chesscolor = 0xabcdef;
			else chesscolor = brown;

			drawsolid_circle(
				win, chesscolor,
				cx + (2*x-8)*ww/9,
				cy + (2*y-9)*hh/10,
				hh/10
			);

			drawutf8_fit(win, fontcolor,
				cx + (2*x-9)*ww/9,
				cy + (2*y-10)*hh/10,
				cx + (2*x-7)*ww/9,
				cy + (2*y-8)*hh/10,
				char2hanzi(temp), 0
			);
		}//forx
	}//fory
}/*
static void xiangqi_dx11draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,y;
	u32 chesscolor, fontcolor, temp;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	dx11solid_rect(ctx, 0x8d6f25, vc, vr, vf);

	for(y=-5;y<5;y++)
	{
		f[0] = 8.0/9;
		f[1] = (2*y+1)/10.0;
		f[2] = 0.0;
		tc[0] = vc[0] - f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] - f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] - f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		dx11line(ctx, 0x222222, tc, tr);
	}
	for(x=-4;x<5;x++)
	{
		f[0] = x*2/9.0;
		f[1] = -1.0/10.0;
		f[2] = 0.0;
		tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		f[1] = -9.0/10.0;
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		dx11line(ctx, 0x222222, tc, tr);

		f[1] = 1.0/10.0;
		tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		f[1] = 9.0/10.0;
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		dx11line(ctx, 0x222222, tc, tr);
	}

	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			//empty
			if(data[y][x] < 'A')continue;

			//>0x41
			else if(data[y][x] <= 'Z')fontcolor = 0;

			//>0x61
			else if(data[y][x] <= 'z')fontcolor = 0xff0000;

			f[0] = (x+x-8)/9.0;
			f[1] = (y+y-9)/10.0;
			f[2] = 1.0/20;
			tc[0] = vc[0] + f[0]*vr[0] - f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] - f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] - f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / 9.1;
			tr[1] = vr[1] / 9.1;
			tr[2] = vr[2] / 9.1;
			tf[0] = vf[0] / 9.1;
			tf[1] = vf[1] / 9.1;
			tf[2] = vf[2] / 9.1;
			tu[0] = vu[0] / 20.0;
			tu[1] = vu[1] / 20.0;
			tu[2] = vu[2] / 20.0;
			dx11solid_cylinder(ctx, 0xf9d65b, tc, tr, tf, tu);
			//if((x==act->PICKx)&&(y==act->PICKy))dx11opaque_prism4(ctx, 0x8000ffff, tc,tr,tf,tu);

			tc[0] += tu[0] + vu[0]*0.01;
			tc[1] += tu[1] + vu[1]*0.01;
			tc[2] += tu[2] + vu[2]*0.01;
			tr[0] = vr[0] / 8;
			tr[1] = vr[1] / 8;
			tr[2] = vr[2] / 8;
			tf[0] = vf[0] / 8;
			tf[1] = vf[1] / 8;
			tf[2] = vf[2] / 8;
			dx11utf8_center(ctx, fontcolor, tc, tr, tf, (u8*)char2hanzi(data[y][x]), 0);
		}
	}
}*/
static void xiangqi_draw_gl41_nocam(
	_obj* act, struct style* part,
	_obj* wnd, struct style* area)
{
	struct fstyle fs;
	fs.vc[0] = 0.0;fs.vc[1] = 0.0;fs.vc[2] = 0.0;
	fs.vr[0] = 1.0;fs.vr[1] = 0.0;fs.vr[2] = 0.0;
	fs.vf[0] = 0.0;fs.vf[1] = 1.0;fs.vf[2] = 0.0;
	fs.vt[0] = 0.0;fs.vt[1] = 0.0;fs.vt[2] = 1.0;

	gl41data_before(wnd);
	gl41data_01cam(wnd);
	gl41solid_rect(wnd, 0x00ff00, fs.vc, fs.vr, fs.vf);
	gl41data_after(wnd);
}
static void xiangqi_gl41draw(
	_obj* act, struct style* part,
	_obj* win, struct style* geom,
	_obj* ctx, struct style* area)
{
	int x,y;
	u32 chesscolor, fontcolor, temp;
	vec3 tc, tr, tf, tu, f;
	float* vc = geom->fs.vc;
	float* vr = geom->fs.vr;
	float* vf = geom->fs.vf;
	float* vu = geom->fs.vt;
	gl41solid_rect(ctx, 0x8d6f25, vc, vr, vf);

	struct perxiangqi* xq = act->listptr.buf0;
	for(y=-5;y<5;y++)
	{
		f[0] = 8.0/9;
		f[1] = (2*y+1)/10.0;
		f[2] = 0.0;
		tc[0] = vc[0] - f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] - f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] - f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		gl41line(ctx, 0x222222, tc, tr);
	}
	for(x=-4;x<5;x++)
	{
		f[0] = x*2/9.0;
		f[1] = -1.0/10.0;
		f[2] = 0.0;
		tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		f[1] = -9.0/10.0;
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		gl41line(ctx, 0x222222, tc, tr);

		f[1] = 1.0/10.0;
		tc[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tc[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tc[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		f[1] = 9.0/10.0;
		tr[0] = vc[0] + f[0]*vr[0] + f[1]*vf[0] + f[2]*vu[0];
		tr[1] = vc[1] + f[0]*vr[1] + f[1]*vf[1] + f[2]*vu[1];
		tr[2] = vc[2] + f[0]*vr[2] + f[1]*vf[2] + f[2]*vu[2];
		gl41line(ctx, 0x222222, tc, tr);
	}

	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			if(     (xq->data[y][x] >= 'A')&&(xq->data[y][x] <= 'Z'))fontcolor = 0;
			else if((xq->data[y][x] >= 'a')&&(xq->data[y][x] <= 'z'))fontcolor = 0xff0000;
			else continue;

			f[0] = (x+x-8)/9.0;
			f[1] = (y+y-9)/10.0;
			f[2] = 1.0/20;
			tc[0] = vc[0] + f[0]*vr[0] - f[1]*vf[0] + f[2]*vu[0];
			tc[1] = vc[1] + f[0]*vr[1] - f[1]*vf[1] + f[2]*vu[1];
			tc[2] = vc[2] + f[0]*vr[2] - f[1]*vf[2] + f[2]*vu[2];
			tr[0] = vr[0] / 9.1;
			tr[1] = vr[1] / 9.1;
			tr[2] = vr[2] / 9.1;
			tf[0] = vf[0] / 9.1;
			tf[1] = vf[1] / 9.1;
			tf[2] = vf[2] / 9.1;
			tu[0] = vu[0] / 20.0;
			tu[1] = vu[1] / 20.0;
			tu[2] = vu[2] / 20.0;
			gl41solid_cylinder(ctx, 0xf9d65b, tc, tr, tf, tu);
			if((x==act->PICKx)&&(y==act->PICKy))gl41opaque_prism4(ctx, 0x8000ffff, tc,tr,tf,tu);

			tc[0] += tu[0] + vu[0]*0.01;
			tc[1] += tu[1] + vu[1]*0.01;
			tc[2] += tu[2] + vu[2]*0.01;
			tr[0] = vr[0] / 8;
			tr[1] = vr[1] / 8;
			tr[2] = vr[2] / 8;
			tf[0] = vf[0] / 8;
			tf[1] = vf[1] / 8;
			tf[2] = vf[2] / 8;
			gl41utf8_center(ctx, fontcolor, tc, tr, tf, (u8*)char2hanzi(xq->data[y][x]), 0);
		}
	}
}




static void xiangqi_draw_json(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
}




static void xiangqi_draw_html(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y;
	u8* tmp;
	struct perxiangqi* xq = act->listptr.buf0;

	//<head>
	htmlprintf(win, 1,
		".xqbg{width:50%%;height:50%%;float:left;background-color:#8d6f25;text-align:center;}\n"
		".xqfg{width:11%%;height:10%%;float:left;background-color:#efcdab;border-radius:50%%;}\n"
	);

	//<body>
	htmlprintf(win, 2, "<div class=\"xqbg\">\n");
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			tmp = char2hanzi(xq->data[y][x]);
			if(0 == tmp)continue;

			htmlprintf(win, 2,
				"<div class=\"xqfg\">%s</div>\n",
				tmp
			);
		}//forx
	}//fory
	htmlprintf(win, 2, "</div>\n");
}




static void xiangqi_draw_tui(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	int x,y,color;
	int width = win->whdf.width;
	int height = win->whdf.height;
	u8* buf = win->tuitext.buf;
	u8* q;

	struct perxiangqi* xq = act->listptr.buf0;
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			q = char2hanzi(xq->data[y][x]);
			if(0 == q){
				if((x==xq->qx)&&(y==xq->qy))color = 5;
				else color = 7;
				gentui_rect(win, color, x*6, y*3, x*6+5, y*3+2);
			}
			else{
				//color
				if( (xq->px==x)&& (xq->py==y) )color = 5;
				else if( (xq->qx==x)&& (xq->qy==y) )color = 2;
				else if(xq->data[y][x] >= 'a')color = 1;
				else color = 4;
				gentui_rect(win, color, x*6, y*3, x*6+5, y*3+2);

				//character
				gentui_utf8(win, 0, x*6+2, y*3+1, q, 0);
			}
		}
	}
}
static void xiangqi_draw_cli(
	_obj* act, struct style* pin,
	_obj* win, struct style* sty)
{
	say("xiangqi(%x,%x,%x,%x)\n", win, act, sty, pin);
	int x,y,tmp;
	struct perxiangqi* xq = act->listptr.buf0;
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			tmp = xq->data[y][x];
			if(0 == char2hanzi(tmp))tmp = '.';
			say("%c%c", tmp, (x==8) ? '\n' : ' ');
		}
	}
}




static void xiangqi_taking_bycam(_obj* ent,void* foot, _syn* stack,int sp)
{
	if(0 == stack)return;
	_obj* wor;struct style* geom;
	_obj* wnd;struct style* area;

	wor = stack[sp-2].pchip;geom = stack[sp-2].pfoot;
	wnd = stack[sp-6].pchip;area = stack[sp-6].pfoot;
	switch(wnd->hfmt){
	case _dx11list_:
	case _mt20list_:
	case _gl41list_:
	case _vk12list_:
		xiangqi_gl41draw(ent,foot, wor,geom, wnd,area);
		break;
	}
}
static void xiangqi_taking(_obj* ent,void* slot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(0 == stack)return;
	_obj* caller;struct style* area;
	caller = stack[sp-2].pchip;area = stack[sp-2].pfoot;

	switch(caller->hfmt){
	case _cli_:
		xiangqi_draw_cli(ent,slot, caller,area);
		break;
	case _tui_:
		xiangqi_draw_tui(ent,slot, caller,area);
		break;
	case _gl41list_:
		xiangqi_draw_gl41_nocam(ent,slot, caller,area);
		break;
	default:
		xiangqi_taking_bycam(ent,slot, stack,sp);
		break;
	}
}
static void xiangqi_giving(_obj* ent,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	say("@xiangqi_giving: %p\n", foot);
	struct perxiangqi* xq = ent->listptr.buf0;

	switch(stack[sp-1].foottype){
	case _mind_:
		xiangqi_copydata((u8*)xq->data, buf);
		return;
	}

	give_data_into_peer(ent,_mind_, stack,sp, 0,0, buf, 16);	//on event
	give_data_into_peer(ent,_sync_, stack,sp, 0,0, xq->data, 9*10);	//on change
}
static void xiangqi_detach(struct halfrel* self, struct halfrel* peer)
{
}
static void xiangqi_attach(struct halfrel* self, struct halfrel* peer)
{
}




static void xiangqi_search(_obj* act, u8* buf)
{
}
static void xiangqi_modify(_obj* act, u8* buf)
{
}
static void xiangqi_delete(_obj* act, u8* buf)
{
	if(0 == act)return;
	if(act->listptr.buf0){
		memorydelete(act->listptr.buf0);
		act->listptr.buf0 = 0;
	}
}
static void xiangqi_create(_obj* act, void* str)
{
	int ret=0;
	void* buf;
	if(0 == act)return;
say("@xiangqi_create:%llx\n",str);

	buf = memorycreate(sizeof(struct perxiangqi), 0);
	act->listptr.buf0 = buf;

	if(str)ret = xiangqi_import(buf, str);
	if((0==str)|(ret<=0))xiangqi_generate(buf);

	for(ret=0;ret<90;ret+=9)printmemory(buf+ret, 9);

	act->PICKx = act->PICKy = 0;
	act->TURN = 0;
}




void xiangqi_register(_obj* p)
{
	p->type = _orig_;
	p->hfmt = hex64('x', 'i', 'a', 'n', 'g', 'q', 'i', 0);

	p->oncreate = (void*)xiangqi_create;
	p->ondelete = (void*)xiangqi_delete;
	p->onreader = (void*)xiangqi_search;
	p->onwriter = (void*)xiangqi_modify;

	p->onattach = (void*)xiangqi_attach;
	p->ondetach = (void*)xiangqi_detach;
	p->ontaking = (void*)xiangqi_taking;
	p->ongiving = (void*)xiangqi_giving;
}
