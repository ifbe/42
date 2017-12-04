#include "actor.h"
void gentui_rect(struct arena* win, u32 rgb, int x0, int y0, int x1, int y1)
{
	int x,y;
	int w = win->w;
	int h = win->h;
	u8* buf = (u8*)(win->buf);

	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			buf[4*(y*w+x) + 3] = rgb;
		}
	}
}
void gentui_utf8(struct arena* win, u32 rgb, int x, int y, u8* utf, int len)
{
	int j;
	int w = win->w;
	int h = win->h;
	u8* buf = (u8*)(win->buf);
	if(utf == 0)return;
	if(len == 0)while(utf[len] != 0)len++;

	x = x&0xfffe;
	for(j=0;j<len;j++)buf[4*(y*w+x) + j] = utf[j];
	for(;j<6;j++)buf[4*(y*w+x) + j] = 0;
}
void gentui_str(struct arena* win, u32 rgb, int x, int y, u8* str, int len)
{
	int j;
	int w = win->w;
	int h = win->h;
	u8* buf = (u8*)(win->buf);
	if(str == 0)return;
	if(len == 0)while(str[len] != 0)len++;
	for(j=0;j<len;j++)buf[4*(y*w+x+j)] = str[j];
}
void gentui_decstr(struct arena* win, u32 rgb, int x, int y, int data)
{
	u8 str[16];
	int len = data2decstr(data, str);
	gentui_str(win, rgb, x, y, str, len);
}
