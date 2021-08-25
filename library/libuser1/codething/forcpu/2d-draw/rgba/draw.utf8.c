#include "libuser.h"
int utf2unicode(u8* src,u32* dst);




static void* utf8table = 0;
void initutf8(void* addr)
{
	utf8table = addr;
}




void drawunicode_alpha(u8* buf, int w, int h, int xx, int yy, u32 unicode)
{
	u16 temp;
	u16* points;
	int x,y,offset;

	if(0 == utf8table)
	{
		return;
	}

	points = utf8table + 32*(unicode&0xffff);
	for(y=0;y<16;y++)
	{
		temp = points[y];
		for(x=0;x<16;x++)
		{
			offset = w*(yy+y) + xx+x;
			if(offset < 0)continue;
			if(0 == (temp&0x1))buf[offset] = 0;
			else buf[offset] = 0xff;

			temp = temp>>1;
		}//x
	}//y
}




void drawunicode_error(_obj* win, u32 rgb, int x, int y, u32 unicode)
{
	u8 ch;
	drawsolid_rect(win, 0x888888, x, y, x+16, y+16);

	ch = 0x30 + ((unicode >> 12) & 0xf);
	if(ch > 0x39)ch += 7;
	drawascii(win, 0xff00ff, x, y, ch);

	ch = 0x30 + ((unicode >> 8) & 0xf);
	if(ch > 0x39)ch += 7;
	drawascii(win, 0x0000ff, x+3, y, ch);

	ch = 0x30 + ((unicode >> 4) & 0xf);
	if(ch > 0x39)ch += 7;
	drawascii(win, 0x00ff00, x+6, y, ch);

	ch = 0x30 + (unicode  & 0xf);
	if(ch > 0x39)ch += 7;
	drawascii(win, 0xff0000, x+9, y, ch);
}
void drawunicode(_obj* win, u32 rgb, int xx, int yy, u32 unicode)
{
	u16 temp;
	u16* points;
	int x,y,offset;
	int width = win->whdf.width;
	int height = win->whdf.height;
	int stride = win->whdf.fbwidth>>2;
	u32* buf = (u32*)(win->rgbanode.buf);

	if(0 == utf8table)
	{
		drawunicode_error(win, rgb, xx, yy, unicode);
		return;
	}

	points = utf8table + 32*(unicode&0xffff);
	rgb |= 0xff000000;
	for(y=0;y<16;y++)
	{
		temp = points[y];
		for(x=0;x<16;x++)
		{
			offset = stride*(yy+y) + xx+x;
			if((offset >= 0)&&(offset < stride*height))
			{
				if( (temp&0x1) != 0 )buf[offset] = rgb;
			}

			temp = temp>>1;
		}//x
	}//y
}
void drawunicode_fit(_obj* win, u32 rgb, int x0, int y0, int x1, int y1, u32 unicode)
{
	u16 temp;
	u16* points;
	int x,y,j,k,m,n,scale,offset;
	int width = win->whdf.width;
	int height = win->whdf.height;
	int stride = win->whdf.fbwidth>>2;
	u32* buf = (u32*)(win->rgbanode.buf);

	if(0 == utf8table)return;
	points = utf8table + 32*(unicode&0xffff);

	rgb |= 0xff000000;
	if(y1-y0<x1-x0)scale = (y1-y0)/16;
	else scale = (x1-x0)/16;
	x0 = (x0+x1)/2 - 8*scale;
	y0 = (y0+y1)/2 - 8*scale;
	for(y=0;y<16;y++)
	{
		temp = points[y];
		for(x=0;x<16;x++)
		{
			for(k=0;k<scale;k++)
			{
				n = y0+y*scale+k;
				if(n < 0)continue;
				if(n >= height)continue;
				for(j=0;j<scale;j++)
				{
					m = x0+j+x*scale;
					if(m < 0)continue;
					if(m >= width)continue;

					offset = stride*n + m;
					if((offset >= 0)&&(offset < stride*height))
					{
						if( (temp&0x1) != 0 )buf[offset] = rgb;
					}
				}
			}
			temp = temp>>1;
		}//x
	}//y
}




void drawutf8(_obj* win, u32 rgb, int xx, int yy, u8* ch, int len)
{
	u32 unicode;
	utf2unicode(ch, &unicode);
	drawunicode(win, rgb, xx, yy, unicode);
}
void drawutf8_fit(_obj* win, u32 rgb, int x0, int y0, int x1, int y1, u8* ch, int len)
{
	u32 unicode;
	utf2unicode(ch, &unicode);
	drawunicode_fit(win, rgb, x0, y0, x1, y1, unicode);
}
