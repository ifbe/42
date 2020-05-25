#include "libuser.h"




void gentui_rect(struct entity* win, u32 rgb, int x0, int y0, int x1, int y1)
{
	int x,y;
	int width = win->width;
	int height = win->height;
	u8* buf = (u8*)(win->textbuf);

	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			buf[4*(y*width+x) + 3] = rgb;
		}
	}
}
void gentui_utf8(struct entity* win, u32 rgb, int x, int y, u8* utf, int len)
{
	int j;
	int width = win->width;
	int height = win->height;
	u8* buf = (u8*)(win->textbuf);
	if(utf == 0)return;
	if(len == 0)while(utf[len] != 0)len++;

	x = x&0xfffe;
	for(j=0;j<len;j++)buf[4*(y*width+x) + j] = utf[j];
	for(;j<6;j++)buf[4*(y*width+x) + j] = 0;
}
void gentui_str(struct entity* win, u32 rgb, int x, int y, u8* str, int len)
{
	int j;
	int width = win->width;
	int height = win->height;
	u8* buf = (u8*)(win->textbuf);
	if(str == 0)return;
	if(len == 0)while(str[len] != 0)len++;
	for(j=0;j<len;j++)buf[4*(y*width+x+j)] = str[j];
}
void gentui_text(struct entity* win, u32 rgb, int x, int y, u8* buf, int len)
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
void gentui_decstr(struct entity* win, u32 rgb, int x, int y, int data)
{
	u8 str[16];
	int len = data2decstr(data, str);
	gentui_str(win, rgb, x, y, str, len);
}




int tuinode_read(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//gentui_rect(wnd, 4, 0, 0, 80, 25);
	//gentui_str(wnd, 1, 0, 0, (void*)"fuck", 4);
	int x,y;
	int www = wnd->width;
	int hhh = wnd->height;
	u8* tmp = wnd->textbuf;
	for(y=0;y<hhh;y++){
		for(x=0;x<www;x++){
			tmp[(www*y + x)*4 + 0] = ' ';
			tmp[(www*y + x)*4 + 1] = 0;
			tmp[(www*y + x)*4 + 2] = 7;
			tmp[(www*y + x)*4 + 3] = 0;
		}
	}

	struct relation* rel = wnd->orel0;
	while(1){
		if(0 == rel)break;
		if(_ent_ == rel->dsttype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].flag = rel->dstflag;
			entityread(stack[sp+1].pchip, stack[sp+1].flag, stack,sp+2, arg,key, buf,len);
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int tuinode_write(_sup* wnd,int foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
