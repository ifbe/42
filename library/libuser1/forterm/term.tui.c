#include "libuser.h"




void gentui_rect(struct actor* win, u32 rgb, int x0, int y0, int x1, int y1)
{
	int x,y;
	int stride = win->stride;
	int height = win->height;
	u8* buf = (u8*)(win->buf);

	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			buf[4*(y*stride+x) + 3] = rgb;
		}
	}
}
void gentui_utf8(struct actor* win, u32 rgb, int x, int y, u8* utf, int len)
{
	int j;
	int stride = win->stride;
	int height = win->height;
	u8* buf = (u8*)(win->buf);
	if(utf == 0)return;
	if(len == 0)while(utf[len] != 0)len++;

	x = x&0xfffe;
	for(j=0;j<len;j++)buf[4*(y*stride+x) + j] = utf[j];
	for(;j<6;j++)buf[4*(y*stride+x) + j] = 0;
}
void gentui_str(struct actor* win, u32 rgb, int x, int y, u8* str, int len)
{
	int j;
	int stride = win->stride;
	int height = win->height;
	u8* buf = (u8*)(win->buf);
	if(str == 0)return;
	if(len == 0)while(str[len] != 0)len++;
	for(j=0;j<len;j++)buf[4*(y*stride+x+j)] = str[j];
}
void gentui_text(struct actor* win, u32 rgb, int x, int y, u8* buf, int len)
{
	int j, k;
	int cc, dy;
	if(buf == 0)return;
	if(len == 0)
	{
		while(buf[len] != 0)len++;
	}

	k = 0;
	dy = 0;
	for(j=0;j<len;j++)
	{
		if((buf[j] != 0)&&(buf[j] != '\n'))continue;

		if(j>k)
		{
			gentui_str(win, rgb, x, y+dy, buf+k, j-k);
		}
		k = j+1;
		dy++;
	}
}
void gentui_decstr(struct actor* win, u32 rgb, int x, int y, int data)
{
	u8 str[16];
	int len = data2decstr(data, str);
	gentui_str(win, rgb, x, y, str, len);
}




int tuinode_read(struct actor* win, struct event* ev)
{
	struct relation* rel;
	//gentui_rect(win, 4, 0, 0, 80, 25);
	//gentui_str(win, 1, 0, 0, (void*)"fuck", 4);

	rel = win->orel0;
	while(1){
		if(0 == rel)break;
		if(_act_ == rel->dsttype){
			actorread((void*)rel->dst, (void*)rel->src, 0, 0, 0, 0);
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int tuinode_write(struct actor* win, struct event* ev)
{
	return 0;
}