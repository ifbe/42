#include "libuser.h"




char* table = ".'`^\",:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$";
int ascii4y(int y)
{
	return table[((y&0xff)*0x44)>>8];
}
void yuvx_to_ascii(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	int x,y,dx,dy;
	int avgy,avgu,avgv;
	int avgr,avgg,avgb;
	for(y=0;y<dsth;y++){
		for(x=0;x<dstw;x++){
			if(x*8+8 > srcw)continue;
			if(y*16+16 > srch)continue;
			avgy = 0;
			avgu = 0;
			avgv = 0;
			for(dy=0;dy<16;dy++){
				for(dx=0;dx<8;dx++){
					avgy += srcbuf[4*(srcw*(y*16+dy)+(x*8+dx))+0];
					avgu += srcbuf[4*(srcw*(y*16+dy)+(x*8+dx))+1];
					avgv += srcbuf[4*(srcw*(y*16+dy)+(x*8+dx))+2];
				}
			}
			avgy = avgy >> 7;
			avgu = avgu >> 7;
			avgv = avgv >> 7;
			avgb =  1.164 * (avgy - 16) +  2.018 * (avgu - 128);
			avgg =  1.164 * (avgy - 16) -  0.391 * (avgu - 128) - 0.813 * (avgv - 128);
			avgr =  1.164 * (avgy - 16)                         + 1.596 * (avgv - 128);
			dstbuf[4*(y*dstw+x) + 0] = ascii4y(avgy);
			dstbuf[4*(y*dstw+x) + 2] &= (avgr<0x40) ? ~0x01 : ~0;
			dstbuf[4*(y*dstw+x) + 2] &= (avgg<0x40) ? ~0x02 : ~0;
			dstbuf[4*(y*dstw+x) + 2] &= (avgb<0x40) ? ~0x04 : ~0;
		}
	}
}
void rgbx_to_ascii(
	u8* srcbuf, int srclen, int srcw, int srch,
	u8* dstbuf, int dstlen, int dstw, int dsth)
{
	int x,y,dx,dy;
	int avgy,avgu,avgv;
	int avgr,avgg,avgb;
	for(y=0;y<dsth;y++){
		for(x=0;x<dstw;x++){
			if(x*8+8 > srcw)continue;
			if(y*16+16 > srch)continue;
			avgr = 0;
			avgg = 0;
			avgb = 0;
			for(dy=0;dy<16;dy++){
				for(dx=0;dx<8;dx++){
					avgr += srcbuf[4*(srcw*(y*16+dy)+(x*8+dx))+0];
					avgg += srcbuf[4*(srcw*(y*16+dy)+(x*8+dx))+1];
					avgb += srcbuf[4*(srcw*(y*16+dy)+(x*8+dx))+2];
				}
			}
			avgr = avgr >> 7;
			avgg = avgg >> 7;
			avgb = avgb >> 7;
			avgy = ( 77*avgr +150*avgg + 29*avgb)>>8;
//			avgu =((-44*avgr - 87*avgg +131*avgb)>>8) + 128;
//			avgv =((131*avgr -110*avgg - 21*avgb)>>8) + 128 ;
			dstbuf[4*(y*dstw+x) + 0] = ascii4y(avgy);
			dstbuf[4*(y*dstw+x) + 2] &= (avgr<0x40) ? ~0x01 : ~0;
			dstbuf[4*(y*dstw+x) + 2] &= (avgg<0x40) ? ~0x02 : ~0;
			dstbuf[4*(y*dstw+x) + 2] &= (avgb<0x40) ? ~0x04 : ~0;
		}
	}
}




void gentui_rect(_obj* win, u32 rgb, int x0, int y0, int x1, int y1)
{
	int x,y;
	int width = win->whdf.width;
	int height = win->whdf.height;
	u8* buf = (u8*)(win->tuitext.buf);

	for(y=y0;y<=y1;y++)
	{
		for(x=x0;x<=x1;x++)
		{
			buf[4*(y*width+x) + 3] = rgb;
		}
	}
}
void gentui_utf8(_obj* win, u32 rgb, int x, int y, u8* utf, int len)
{
	int j;
	int width = win->whdf.width;
	int height = win->whdf.height;
	u8* buf = (u8*)(win->tuitext.buf);
	if(utf == 0)return;
	if(len == 0)while(utf[len] != 0)len++;

	x = x&0xfffe;
	for(j=0;j<len;j++)buf[4*(y*width+x) + j] = utf[j];
	for(;j<6;j++)buf[4*(y*width+x) + j] = 0;
}
void gentui_str(_obj* win, u32 rgb, int x, int y, u8* str, int len)
{
	int j;
	int width = win->whdf.width;
	int height = win->whdf.height;
	u8* buf = (u8*)(win->tuitext.buf);
	if(str == 0)return;
	if(len == 0)while(str[len] != 0)len++;
	for(j=0;j<len;j++)buf[4*(y*width+x+j)] = str[j];
}
void gentui_text(_obj* win, u32 rgb, int x, int y, u8* buf, int len)
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
void gentui_decstr(_obj* win, u32 rgb, int x, int y, int data)
{
	u8 str[16];
	int len = data2decstr(data, str);
	gentui_str(win, rgb, x, y, str, len);
}




int tuinode_take(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	//gentui_rect(wnd, 4, 0, 0, 80, 25);
	//gentui_str(wnd, 1, 0, 0, (void*)"fuck", 4);
	int x,y;
	int www = wnd->whdf.width;
	int hhh = wnd->whdf.height;
	u8* tmp = wnd->tuitext.buf;
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
		if(_ent_ == rel->dstnodetype){
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_takeby(stack[sp+1].pchip, stack[sp+1].pfoot, stack,sp+2, arg,key, buf,len);
		}
		rel = samesrcnextdst(rel);
	}
	return 0;
}
int tuinode_give(_obj* wnd,void* foot, _syn* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
