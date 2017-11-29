#include<actor.h>




void drawtext(
	struct arena* win, u32 rgb,
	int x0, int y0, int x1, int y1,
	u8* buf, int len)
{
	int j,k,m,n;
	m = n = 0;
	for(j=0;j<len;j++)
	{
		if(n*16+16 > y1-y0)break;
		if((buf[j] != 0)&&(buf[j] != '\n'))continue;

		k = (x1-x0)/8;
		if(k > j-m)k = j-m;

		drawstring(
			win, 0xffffff,
			x0, y0+(n*16),
			buf+m, k
		);

		m = j+1;
		n++;
		if(buf[j] == 0)break;
	}
}