#include "actor.h"
void drawascii(
	void*, u8 ch, int size,
	int x, int y, u32 fg, u32 bg);
void rect(void*,
	int x0, int y0,
	int x1, int y1,
	u32 bc, u32 fc);




//max=32768
static int arealeft = 16384;
static int areatop = 32768;
static int arearight = 49152;
static int areabottom = 49152;
static char table[8][8] = {
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z',' ',' ',' ',' ',0x8,0xd,
	'0','1','2','3','4','5','6','7',
	'8','9',' ',' ',' ',' ',' ',' ',
	'+','-','*','/',' ',' ',' ',' ',
	'=',' ',' ',' ',' ',' ',' ',' '
};




void vkbd_read(struct arena* win)
{
	int x,y;
	int left,top,right,bottom;
	int width = win->w;
	int height = win->h;

	//[a,z]
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			left = (arealeft + x*(arearight-arealeft)/8)*width/65536;
			top = (areatop + y*(areabottom-areatop)/8)*height/65536;
			right = (arealeft + (x+1)*(arearight-arealeft)/8)*width/65536;
			bottom = (areatop + (y+1)*(areabottom-areatop)/8)*height/65536;
			//say("====%d,%d,%d,%d\n",left,top,right,bottom);

			rect(win,
				left, top,
				right, bottom,
				0xff00ff, 0
			);

			drawascii(
				win, table[y][x], 1,
				left, top, 0xffffffff, 0
			);
		}
	}
}
int vkbd_write(struct event* ev)
{
	int x,y;
	//say("%x,%x\n",x,y);

	if(ev->what == 0x2d70)
	{
		x = (ev->why)&0xffff;
		if(x < arealeft)return 1;
		if(x > arearight)return 1;

		y = ((ev->why)>>16)&0xffff;
		if(y < areatop)return 1;
		if(y > areabottom)return 1;

		x = (x-arealeft)*8/32768;
		y = (y-areatop)*8/16384;

		ev->what = 0x72616863;
		ev->why = table[y][x];
	}

	return 1;
}




void vkbd_create()
{
}
void vkbd_delete()
{
}
