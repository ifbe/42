#include"libuser.h"




void windowwrite(struct arena* src)
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
void windowread(char* addr)
{
}
void windowlist()
{
}
void windowchange()
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct arena* w)
{
}
void windowcreate(struct arena* w)
{
	w->type = hex32('w','i','n',0);
	w->fmt = hex32('t','u','i',0);

	w->width = w->stride = 80;
	w->height = 25;
}




void initwindow()
{
}
void freewindow()
{
}