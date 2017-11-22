#include "actor.h"
void drawline(void*,
	int x1,int y1,int x2,int y2, u32 color);
void drawsolid_rect(void*,
	int x1, int y1, int x2, int y2, u32 color);
void drawsolid_circle(void*,
	int x, int y, int r, u32 color);
void drawascii(
	void*, u8 data, int size,
	int x, int y, u32 fg, u32 bg);




static char data[10][9];
static int turn;
static int px,py,qx,qy;




void* char2hanzi(int val)
{
	switch(val)
	{
		case 'a':return "车";
		case 'b':return "马";
		case 'c':return "象";
		case 'd':return "士";
		case 'e':return "将";
		case 's':return "卒";
		case 'z':return "炮";

		case 'A':return "车";
		case 'B':return "马";
		case 'C':return "相";
		case 'D':return "仕";
		case 'E':return "帅";
		case 'S':return "兵";
		case 'Z':return "炮";

		default:return 0;
	}
}
static int htmlcircle(char* p, int x, int y)
{
	u32 textcolor;
	char* hanzi;
	char ch;

	ch = data[y][x];
	if( (ch>='a') && (ch<='z') )
	{
		textcolor=0;
	}
	else if( (ch >='A') && (ch <= 'Z') )
	{
		textcolor=0xff0000;
	}
	else return 0;

	hanzi = char2hanzi(data[y][x]);
	return mysnprintf(
		p, 0x1000,
		"<div class=\"circle\" style=\""
		"left:%d%;"
		"top:%d%;"
		"background:#ffff00;"
		"color:#%06x;"
		"\"><br>%s</div>",
		x*11, y*10,
		textcolor, hanzi
	);
}
static void xiangqi_read_html(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y;
	char* p = (char*)(win->buf);
	return;

	p += mysnprintf(
		p, 0x1000,
		"<style type=\"text/css\">"
		".circle{"
		"position:absolute;"
		"border-radius:50%;"
		"width:10%;"
		"height:10%;"
		"text-align:center;"
		"}"
		"</style>"
	);
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			p += htmlcircle(p, x, y);
		}//forx
	}//fory
}




static void xiangqi_read_text(struct arena* win, struct actor* act, struct style* sty)
{
	int x,y,j,k,ret,color;
	int width = win->w;
	int height = win->h;
	u8* p = (u8*)(win->buf);
	u8* q;

	//
	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			q = char2hanzi(data[y][x]);
			if(q == 0)
			{
				if(qx != x)continue;
				if(qy != y)continue;
			}

			//position
			ret = (3*y+1)*width + x*8 + 2;
			ret <<= 2;

			//color
			if( (px==x)&& (py==y) )color=5;
			else if( (qx==x)&& (qy==y) )color=7;
			else if(data[y][x] >= 'a')color=1;
			else color=4;
			for(j=-1;j<=1;j++)
			{
				for(k=-2;k<=3;k++)
				{
					p[ret +(j*width*4) +(k*4) +3] = color;
				}
			}

			//character
			if(q != 0)mysnprintf(p+ret, 4, "%s", q);

		}
	}
}




void xiangqi_read_pixel(struct arena* win, struct actor* act, struct style* sty)
{
	u32 black, brown, red;
	u32 chesscolor, fontcolor, temp;
	int x,y,half;
	int cx = (win->w) * (sty->cx) / 0x10000;
	int cy = (win->h) * (sty->cy) / 0x10000;
	int w = (win->w) * (sty->wantw) / 0x10000;
	int h = (win->h) * (sty->wanth) / 0x10000;

	if(w/9 != h/10)
	{
		h = (w*10 + h*9)/18;
		w = h*9/10;
		sty->wantw = w * 0x10000 / (win->w);
		sty->wanth = h * 0x10000 / (win->h);
	}
	half = h/20;

	//
	black=0;
	if( ((win->fmt)&0xffffffff) == 0x61626772)
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
	drawsolid_rect(win, cx-w/2, cy-h/2, cx+w/2, cy+h/2, temp);

	//heng
	for(y=-5;y<5;y++)
	{
		drawline(win,
			cx - half*8,	cy + half*(2*y+1),
			cx + half*8,	cy + half*(2*y+1),	0);
	}

	//shu
	for(x=-4;x<5;x++)
	{
		drawline(win,
			cx + x*half*2,	cy - half*9,
			cx + x*half*2,	cy - half*1,	0);
		drawline(win,
			cx + x*half*2,	cy + half*9,
			cx + x*half*2,	cy + half*1,	0);
	}

	//pie,na
	drawline(win,
		cx - half*2,	cy - half*9,
		cx + half*2,	cy - half*5,	0);
	drawline(win,
		cx + half*2,	cy - half*9,
		cx - half*2,	cy - half*5,	0);
	drawline(win,
		cx - half*2,	cy + half*9,
		cx + half*2,	cy + half*5,	0);
	drawline(win,
		cx + half*2,	cy + half*9,
		cx - half*2,	cy + half*5,	0);

	//chess
	for(y=0;y<10;y++)
	{
		for(x=0;x<9;x++)
		{
			//empty
			if(data[y][x] < 'A')continue;

			//>0x41
			else if(data[y][x] <= 'Z')
			{
				fontcolor = black;
			}

			//>0x61
			else if(data[y][x] <= 'z')
			{
				fontcolor = red;
			}

			if( (px == x)&&(py == y) )chesscolor = 0xabcdef;
			else chesscolor = brown;

			drawsolid_circle(win,
				cx + (2*x-8)*half,
				cy + (2*y-9)*half,
				half,
				chesscolor
			);

			drawascii(
				win, data[y][x], half/8,
				cx + (2*x-8)*half - (half/8*8/2),
				cy + (2*y-9)*half - (half/8*16/2),
				fontcolor,
				0
			);
		}//forx
	}//fory
}




static void xiangqi_read(struct arena* win, struct actor* act, struct style* sty)
{
	u32 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		xiangqi_read_text(win, act, sty);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		xiangqi_read_html(win, act, sty);
	}

	//pixel
	else
	{
		xiangqi_read_pixel(win, act, sty);
	}
}
void xiangqi_move(int px, int py, int x, int y)
{
	int min, max, t, u;
	say("(%d,%d) -> (%d,%d)\n", px, py, x, y);

	//chess going
	if(data[py][px] == 'S')		//兵
	{
		if(
			( (x == px) && (y == py+1) ) |
			( (y>4) && (x == px-1) && (y == py) ) |
			( (y>4) && (x == px+1) && (y == py) )
		)
		{
			if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				turn++;
			}
		}
	}
	else if(data[py][px] == 's')	//兵
	{
		if(
			( (x == px) && (y == py-1) ) |
			( (y<5) && (x == px-1) && (y == py) ) |
			( (y<5) && (x == px+1) && (y == py) )
		)
		{
			if( (data[y][x] < 'a') | (data[y][x] > 'z') )
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				turn++;
			}
		}
	}
	else if(data[py][px] == 'Z')	//炮
	{
		if(data[y][x] == 0)
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}

		//打隔子
		else if(data[y][x] >= 'a')
		{
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[y][t] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}

			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'z')	//炮
	{
		if(data[y][x] == 0)
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}

		//打隔子
		else if( (data[y][x] >= 'A') && (data[y][x] <= 'Z') )
		{
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[y][t] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}

			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 1)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'A')	//车
	{
		if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'a')	//车
	{
		if( (data[y][x] < 'a') | (data[y][x] > 'z') )
		{
			//横移
			if(y == py)
			{
				if(x < px){min = x; max=px;}
				else {min=px; max=x;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if( data[y][t] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}

			//竖移
			if(x == px)
			{
				if(y < py){min = y; max=py;}
				else {min=py; max=y;}

				u = 0;
				for(t=min+1;t<max;t++)
				{
					if(data[t][x] != 0)u++;
				}
				if(u == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'B')	//马
	{
		if(data[y][x] < 'A' | data[y][x] > 'Z')
		{
			if( (x == px-2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px-1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
			else if( (x == px+2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px+1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
			else if( (y == py-2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py-1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
			else if( (y == py+2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py+1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'b')	//马
	{
		if(data[y][x] < 'a' | data[y][x] > 'z')
		{
			if( (x == px-2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px-1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
			else if( (x == px+2) && (y == py-1) | (y == py+1) )
			{
				if(data[py][px+1] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
			else if( (y == py-2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py-1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
			else if( (y == py+2) && ( (x == px-1) | (x == px+1) ) )
			{
				if(data[py+1][px] == 0)
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'C')	//象
	{
		if( (y<5) && ( (data[y][x] < 'A') | (data[y][x] > 'Z') ) )
		{
			if(
			( (x == px-2) && (y == py-2) && (data[py-1][px-1]==0) )|
			( (x == px-2) && (y == py+2) && (data[py+1][px-1]==0) )|
			( (x == px+2) && (y == py-2) && (data[py-1][px+1]==0) )|
			( (x == px+2) && (y == py+2) && (data[py+1][px+1]==0) )
			)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				turn++;
			}
		}
	}
	else if(data[py][px] == 'c')	//象
	{
		if( (y>4) && ( (data[y][x] < 'a') | (data[y][x] > 'z') ) )
		{
			if(
			( (x == px-2) && (y == py-2) && (data[py-1][px-1]==0) )|
			( (x == px-2) && (y == py+2) && (data[py+1][px-1]==0) )|
			( (x == px+2) && (y == py-2) && (data[py-1][px+1]==0) )|
			( (x == px+2) && (y == py+2) && (data[py+1][px+1]==0) )
			)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				turn++;
			}
		}
	}
	else if(data[py][px] == 'D')	//士
	{
		if( (x>=3)&&(x<=5)&&(y>=0)&&(y<=2) )
		{
			if(
				( (x == px-1) && (y == py-1) ) |
				( (x == px-1) && (y == py+1) ) |
				( (x == px+1) && (y == py-1) ) |
				( (x == px+1) && (y == py+1) )
			)
			{
				if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'd')	//士
	{
		if( (x>=3)&&(x<=5)&&(y>=7)&&(y<=9) )
		{
			if(
				( (x == px+1) && (y == py-1) ) |
				( (x == px+1) && (y == py+1) ) |
				( (x == px-1) && (y == py-1) ) |
				( (x == px-1) && (y == py+1) )
			)
			{
				if( (data[y][x] < 'a') | (data[y][x] > 'z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}
	}
	else if(data[py][px] == 'E')	//将
	{
		if( (x>=3)&&(x<=5)&&(y>=0)&&(y<=2) )
		{
			if(
				( (x == px) && (y == py-1) ) |
				( (x == px) && (y == py+1) ) |
				( (x == px-1) && (y == py) ) |
				( (x == px+1) && (y == py) )
			)
			{
				if( (data[y][x] < 'A') | (data[y][x] > 'Z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}

		else if( (x == px) && (data[y][x] == 'e') )
		{
			u=0;
			for(t=py+1;t<y;t++)
			{
				if(data[t][px] != 0)u++;
			}
			if(u == 0)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				turn++;
			}
		}
	}
	else if(data[py][px] == 'e')	//将
	{
		if( (x>=3)&&(x<=5)&&(y>=7)&&(y<=9) )
		{
			if(
				( (x == px) && (y == py-1) ) |
				( (x == px) && (y == py+1) ) |
				( (x == px-1) && (y == py) ) |
				( (x == px+1) && (y == py) )
			)
			{
				if( (data[y][x] < 'a') | (data[y][x] > 'z') )
				{
					data[y][x] = data[py][px];
					data[py][px] = 0;
					turn++;
				}
			}
		}

		else if( (x == px) && (data[y][x] == 'E') )
		{
			u=0;
			for(t=y+1;t<py;t++)
			{
				if(data[t][px] != 0)u++;
			}
			if(u == 0)
			{
				data[y][x] = data[py][px];
				data[py][px] = 0;
				turn++;
			}
		}
	}
}
int xiangqi_pickup(int x, int y)
{
	if( (x==px) && (y==py) )
	{
		px = py = -1;
		return 1;
	}

	//chess choosing
	if( (px<0) | (py<0) | (data[py][px] == 0) )
	{
		if( (data[y][x]>='a') && (data[y][x]<='z') && ((turn&1) == 0) )
		{
			px = x;
			py = y;
		}
		if( (data[y][x]>='A') && (data[y][x]<='Z') && ((turn&1) == 1) )
		{
			px = x;
			py = y;
		}
		return 2;
	}

	return 0;
}
void xiangqi_write(struct event* ev)
{
	int x, y, ret;
	u64 what = ev->what;
	u64 key = ev->why;

	if(what == 0x64626b)
	{
		if(key == 0x25)	//left
		{
			if(qx<1)return;
			qx--;
		}
		else if(key == 0x26)	//up
		{
			if(qy<1)return;
			qy--;
		}
		else if(key == 0x27)	//right
		{
			if(qx<0)return;
			if(qx>=8)return;
			qx++;
		}
		else if(key == 0x28)	//down
		{
			if(qy<0)return;
			if(qy>=9)return;
			qy++;
		}
	}

	else if(what == 0x72616863)
	{
		if(key == 0x20)
		{
			ret = xiangqi_pickup(qx, qy);
			if(ret > 0)return;

			//是移动操作吗
			xiangqi_move(px, py, qx, qy);
		}
	}

	else if(what == 0x2d70)
	{
		x = key & 0xffff;
		y = (key >> 16) & 0xffff;
		//say("%d,%d => ",x,y);

		x = (x*9)>>16;
		y = (y*10)>>16;
		//say("%d,%d\n",x,y);

		if(x < 0)return;
		if(x > 8)return;
		if(y < 0)return;
		if(y > 9)return;

		//是选中操作吗
		ret = xiangqi_pickup(x, y);
		if(ret > 0)return;

		//是移动操作吗
		xiangqi_move(px, py, x, y);
		px = py = -1;
	}
}




static void xiangqi_list()
{
}
static void xiangqi_choose()
{
}
static void xiangqi_start()
{
	int j;
	char* p=(char*)data;

	px = py = -1;
	qx = qy = 0;
	turn = 0;
	for(j=0;j<90;j++)p[j]=0;




	for(j=0;j<=4;j++)
	{
		//(black)JU,MA,XIANG,SHI,JIANG
		data[0][ j ]='A' + j;
		data[0][8-j]='A' + j;
		
		//(red)ju,ma,xiang,shi,jiang
		data[9][ j ]='a' + j;
		data[9][8-j]='a' + j;
	}
	for(j=0;j<5;j++)
	{
		//(red)SOLDIER
		data[3][j*2]='S';

		//(black)soldier
		data[6][j*2]='s';
	}

	//(red)PAO
	data[2][1]='Z';
	data[2][7]='Z';

	//(black)pao
	data[7][1]='z';
	data[7][7]='z';
}
static void xiangqi_stop()
{
}
void xiangqi_create(void* base, struct actor* act)
{
	act->type = hex32('g', 'a', 'm', 'e');
	act->name = hex64('x', 'i', 'a', 'n', 'g', 'q', 'i', 0);
	act->irel = 0;
	act->orel = 0;

	act->start = (void*)xiangqi_start;
	act->stop = (void*)xiangqi_stop;
	act->list = (void*)xiangqi_list;
	act->choose = (void*)xiangqi_choose;
	act->read = (void*)xiangqi_read;
	act->write = (void*)xiangqi_write;
}
void xiangqi_delete()
{
}
