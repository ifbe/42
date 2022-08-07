#include"libuser.h"
void* supply_alloc();
void* supply_recycle(void*);




void window_take()
{
}
void window_give()
{
}
void window_attach()
{
}
void window_detach()
{
}




void window_write(_obj* src)
{
	int x,y;
	u8 bg=0,fg=0,c=0;
	u8* p;
	u8* q;
	u8* buf = (u8*)(src->buf);

	for(y=0;y<25;y++)
	{
		for(x=0;x<80;x++)
		{
			p = buf + (y*320) + (x*4);
			q = (u8*)(u64)(0xb8000 + (y*160) + (x*2));
			if(p[0] > 0x80)
			{
				bg = p[7];
				fg = p[6];
				c = ((bg&0xa)<<4)+((bg&1)<<6)+((bg&4)<<2)+7;

				q[0] = '?';
				q[1] = c;
				q[2] = '?';
				q[3] = c;
				x++;
			}
			else
			{
				bg = p[3];
				fg = p[2];
				c = ((bg&0xa)<<4)+((bg&1)<<6)+((bg&4)<<2)+7;

				q[0] = p[0];
				q[1] = c;
			}
		}
	}
}
void window_read(char* addr)
{
}
void window_delete(_obj* w)
{
}
void window_create(_obj* w)
{
	w->fmt = _tui_;
	w->vfmt = 0;

	w->width = 80;
	w->height = 25;

	w->fbwidth = 80;
	//w->fbheight = 0;
}




void* window_alloc()
{
	return supply_alloc();
}




void initwindow()
{
}
void freewindow()
{
}
