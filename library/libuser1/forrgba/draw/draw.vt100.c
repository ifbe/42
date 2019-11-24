#include "libuser.h"
struct txtcfg
{
	u32 bg;
	u32 fg;
	int x;
	int y;
};
struct uartterm
{
	u8* buf;
	u32 len;
	u32 fg;
	u32 bg;

	int width;
	int height;
	int vimw;
	int vimh;

	int left;
	int right;
	int top;
	int bottom;

	int curx;
	int cury;
};
static u32 getcolor[256] = {
0x010101,0xff4030,0x40c040,0xffc000,0x0040c0,0x802080,0x30c0ff,0xcccccc,
0x888888,0xff0000,0x00ff00,0xffff00,0x0000ff,0xff00ff,0x00ffff,0xffffff,
};
void drawvt100_create()
{
	int j;
	int r,g,b;
	for(r=0;r<6;r++){
	for(g=0;g<6;g++){
	for(b=0;b<6;b++){
		j = 16+(36*r)+(6*g)+b;
		getcolor[j] = (0x280000*r)|(0x2800*g)|(0x28*b);
		//say("%x:%x\n",j,getcolor[j]);
	}
	}
	}
	for(r=232;r<256;r++)getcolor[r] = 0x010101 + 0x0a0a0a*r;
}




int drawvt100_color(u8* p, struct txtcfg* cfg)
{
	//reset
	if(p == 0)
	{
		cfg->fg = 0xcccccc;
		return 0;
	}

	//reset
	if(p[0] == '0')
	{
		cfg->fg = 0xcccccc;
		return 0;
	}

	//heavy
	else if(p[0] == '1')
	{
		cfg->fg = 0xffffff;
		return 1;
	}

	//foreground
	if(p[0] == '3')
	{
		if(p[1] == '0')cfg->fg = 0x000000;
		else if(p[1] == '1')cfg->fg = 0xff0000;
		else if(p[1] == '2')cfg->fg = 0x00ff00;
		else if(p[1] == '3')cfg->fg = 0xffff00;
		else if(p[1] == '4')cfg->fg = 0x0000ff;
		else if(p[1] == '5')cfg->fg = 0xff00ff;
		else if(p[1] == '6')cfg->fg = 0x00ff00;
		else if(p[1] == '7')cfg->fg = 0xcccccc;
		return 3;
	}

	//background
	else if(p[0] == '4')
	{
		if(p[1] == '0')cfg->bg = 0x000000;
		else if(p[1] == '1')cfg->bg = 0xff0000;
		else if(p[1] == '2')cfg->bg = 0x00ff00;
		else if(p[1] == '3')cfg->bg = 0xffff00;
		else if(p[1] == '4')cfg->bg = 0x0000ff;
		else if(p[1] == '5')cfg->bg = 0xff00ff;
		else if(p[1] == '6')cfg->bg = 0x00ffff;
		else if(p[1] == '7')cfg->bg = 0xcccccc;
		return 4;
	}
	return -1;
}
static int drawvt100_1b(u8* p, struct txtcfg* cfg)
{
	int j;
	int x,y;
	if(p[0] != 0x1b)return 0;
	if(p[1] != 0x5b)return 1;
	//printmemory(p, 16);

	//1b 5b 41: cursor up
	if(p[2] == 0x41)
	{
		//printf("a");
		return 3;
	}

	//1b 5b 42: cursor down
	if(p[2] == 0x42)
	{
		//printf("b");
		return 3;
	}

	//1b 5b 43: cursor forward
	if(p[2] == 0x43)
	{
		//printf("c");
		return 3;
	}

	//1b 5b 44: cursor backward
	if(p[2] == 0x44)
	{
		//printf("d");
		return 3;
	}

	//1b 5b H/f
	if((p[2] == 'H')|(p[2] == 'f'))
	{
		return 3;
	}

	//1b 5b J
	if(p[j+2] == 'J')
	{
		return 3;
	}

	//1b 5b K
	if(p[j+2] == 'K')
	{
		return 3;
	}

	//1b 5b m
	if(p[2] == 'm')
	{
		drawvt100_color(0, cfg);
		return 3;
	}

	//1b 5b ? m
	if(p[3] == 'm')
	{
		drawvt100_color(p+2, cfg);
		return 4;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 4;
	}

	//1b 5b ? ? h
	if(p[4] == 'h')
	{
		return 5;
	}

	//1b 5b ? ? l
	if(p[4] == 'l')
	{
		return 5;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		drawvt100_color(p+2, cfg);
		return 5;
	}

	if(p[3] == ';')
	{
		//1b 5b ? ; ? m
		if(p[5] == 'm')
		{
			drawvt100_color(p+2, cfg);
			drawvt100_color(p+4, cfg);
			return 6;
		}

		//1b 5b ? ; ? ? m
		else if(p[6] == 'm')
		{
			drawvt100_color(p+2, cfg);
			drawvt100_color(p+4, cfg);
			return 7;
		}
	}

	//1b 5b ? ? ; ? ? m
	if( (p[4] == ';') && (p[7] == 'm') )
	{
		drawvt100_color(p+2, cfg);
		drawvt100_color(p+5, cfg);
		return 8;
	}

	for(j=2;j<10;j++)
	{
		if(p[j] == 'r')return j+1;
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			//drawvt100_position(p+2, cfg);
			return j+1;
		}
	}

	printmemory(p, 16);
	return 0;
}




void drawvt100(
	struct entity* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int len)
{
	u32 bg, fg;
	int j, k;
	int x, y, z;
	int flag, last=0;
	struct txtcfg cfg = {0x000000, 0xcccccc, 0, 0};

	for(j=0;j<len;j++)
	{
		if((cfg.y)*16+16 > y1-y0)break;

		x = cfg.x;
		y = cfg.y;
		bg = cfg.bg;
		fg = cfg.fg;
		flag = 0;

		if(buf[j] == 0)flag = 1;
		else if(buf[j] == '\n')flag = 2;
		else if(buf[j] == 0x1b)
		{
			z = drawvt100_1b(buf+j, &cfg);
			if(z >= 2)flag = 3;
		}
		else if(buf[j] >= 0x80)
		{
			if(buf[j] < 0xe0)z = 2;
			else if(buf[j] < 0xf0)z = 3;
			else if(buf[j] < 0xf8)z = 4;
			else if(buf[j] < 0xfc)z = 5;
			else if(buf[j] < 0xfe)z = 6;
			else z = 0;

			if(z >= 2)flag = 4;
		}
		if(flag == 0)continue;

		k = (x1-x0)/8 - x;
		if(k > j-last)k = j-last;

		if(k > 0)
		{
			if(bg != 0)drawsolid_rect(
				win, bg,
				x0+(x*8), y0 + (y*16),
				x0+(x*8) + k*8, y0 + (y*16) + 16
			);
			drawstring(
				win, fg,
				x0 + (x*8), y0 + (y*16),
				buf+last, k
			);
		}

		if(flag == 1)break;
		else if(flag == 2)
		{
			cfg.x = 0;
			cfg.y += 1;
			last = j+1;
		}
		else if(flag == 3)
		{
			cfg.x += k;
			last = j+z;
			j += z-1;
		}
		else if(flag == 4)
		{
			cfg.x += k;
			if(8*(cfg.x) < x1-x0-16)
			{
				drawutf8(
					win, cfg.fg,
					x0 + (cfg.x)*8, y0 + (cfg.y)*16,
					buf+j, z
				);
			}
			cfg.x += 2;

			last = j+z;
			j += z-1;
		}
	}
}




void drawterm(struct entity* win, struct uartterm* term, int x0, int y0, int x1, int y1)
{
	u32 bg, fg;
	int x,y;
	int xmax, ymax;
	int cursorx, cursory;
	int w = term->width;
	int h = term->height;
	u8* buf = term->buf;
	u8* aaa;

	xmax = (x1-x0)/8;
	if(xmax > w)xmax = w;
	ymax = (y1-y0)/16;
	if(ymax > 25)ymax = 25;

	cursorx = term->curx;
	cursory = (term->cury)-(term->top);
	buf += (term->top)*w*4;
	for(y=0;y<ymax;y++)
	{
		for(x=0;x<xmax;x++)
		{
			aaa = buf + (w*y*4) + (x*4);
			if(aaa[0] < 0x80)
			{
				bg = aaa[3];
				fg = aaa[2];
				bg = getcolor[bg%256];
				fg = getcolor[fg%256];
				if(bg != 0)
				{
					drawsolid_rect(
						win, bg,
						x0+(x*8), y0 + (y*16),
						x0+(x*8) + 8, y0 + (y*16) + 16
					);
				}
				drawascii(
					win, fg,
					x0 + (x*8), y0 + (y*16),
					aaa[0]
				);
				if((x == cursorx)&&(y == cursory))
				{
					drawsolid_rect(
						win, 0xffffff,
						x0+(x*8), y0 + (y*16),
						x0+(x*8) + 8, y0 + (y*16) + 16
					);
				}
			}
			else
			{
				bg = aaa[7];
				fg = aaa[6];
				bg = getcolor[bg%256];
				fg = getcolor[fg%256];
				if(bg != 0)
				{
					drawsolid_rect(
						win, bg,
						x0+(x*8), y0 + (y*16),
						x0+(x*8) + 8, y0 + (y*16) + 16
					);
				}
				drawutf8(
					win, fg,
					x0 + (x*8), y0 + (y*16),
					aaa, 0
				);
				if((x == cursorx)&&(y == cursory))
				{
					drawsolid_rect(
						win, 0xffffff,
						x0+(x*8), y0 + (y*16),
						x0+(x*8) + 16, y0 + (y*16) + 16
					);
				}
				x++;
			}

		}
	}
}
