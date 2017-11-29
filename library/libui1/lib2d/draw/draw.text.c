#include<actor.h>
struct txtcfg
{
	u32 bg;
	u32 fg;
	int x;
	int y;
};




int drawtext_color(u8* p, struct txtcfg* cfg)
{
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
}
static int drawtext_position(u8* p, struct txtcfg* cfg)
{
	int t;
	int x=0,y=0;
	for(t=0;t<4;t++)
	{
		if(p[t] == ';')
		{
			t++;
			break;
		}
		y = (y*10) + p[t] - 0x30;
	}
	for(;t<8;t++)
	{
		if( (p[t] == 'H') | (p[t] == 'f') )
		{
			t++;
			break;
		}
		x = (x*10) + p[t] - 0x30;
	}

	cfg->x = x-1;
	cfg->y = y-1;
	return 0;
}
static int drawtext_1b(u8* p, struct txtcfg* cfg)
{
	int j;
	int x,y;
	if(p[0] != 0x1b)return 0;
	if(p[1] != 0x5b)return 1;

	if(p[2] == '?')
	{
		for(j=3;j<8;j++)
		{
			if( (p[j] == 'h') | (p[j] == 'l') )
			{
				//clear screen&&goto (0,0)
				return j+1;
			}
		}
	}

	//1b 5b 4a bf: openwrt special
	if(p[2] == 0x4a)
	{
		if(p[3] == 0xbf) return 4;
	}

	//1b 5b 4b: erase from here to the end
	if(p[2] == 0x4b)
	{
		//printf("        \b\b\b\b\b\b\b\b");
		return 3;
	}

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

	//1b 5b ? m
	if(p[3] == 'm')
	{
		drawtext_color(p+2, cfg);
		return 4;
	}

	//1b 5b ? n
	if(p[3] == 'n')
	{
		return 4;
	}

	//1b 5b ? ? m
	if(p[4] == 'm')
	{
		drawtext_color(p+2, cfg);
		return 5;
	}

	if(p[3] == ';')
	{
		//1b 5b ? ; ? m
		if(p[5] == 'm')
		{
			drawtext_color(p+2, cfg);
			drawtext_color(p+4, cfg);
			return 6;
		}

		//1b 5b ? ; ? ? m
		else if(p[6] == 'm')
		{
			drawtext_color(p+2, cfg);
			drawtext_color(p+4, cfg);
			return 7;
		}
	}

	//1b 5b ? ? ; ? ? m
	if( (p[4] == ';') && (p[7] == 'm') )
	{
		drawtext_color(p+2, cfg);
		drawtext_color(p+5, cfg);
		return 8;
	}

	for(j=2;j<10;j++)
	{
		if( (p[j] == 'H') | (p[j] == 'f') )
		{
			drawtext_position(p+2, cfg);
			return j+1;
		}
	}

	return 0;
}




void drawtext(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int len)
{
	u32 c;
	int j, k;
	int x, y, z;
	int flag, last=0;
	struct txtcfg cfg = {0x000000, 0xcccccc, 0, 0};

	for(j=0;j<len;j++)
	{
		if((cfg.y)*16+16 > y1-y0)break;

		c = cfg.fg;
		flag = 0;

		if(buf[j] == 0)flag = 1;
		else if(buf[j] == '\n')flag = 2;
		else if(buf[j] == 0x1b)
		{
			z = drawtext_1b(buf+j, &cfg);
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

		k = (x1-x0)/8;
		if(k > j-last)k = j-last;

		if(k > 0)
		{
			drawstring(
				win, c,
				x0 + (cfg.x)*8, y0 + (cfg.y)*16,
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
			drawsolid_rect(
				win, cfg.fg,
				x0 + (cfg.x)*8, y0 + (cfg.y)*16,
				x0 + (cfg.x)*8+16, y0 + (cfg.y)*16+16
			);
			cfg.x += 2;

			last = j+z;
			j += z-1;
		}
	}
}