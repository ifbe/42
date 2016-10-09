#define u64 unsigned long long 
#define u32 unsigned int
//
void line(       int x1,int y1,int x2,int y2, u32 color);
void circlebody( int x, int y, int r, u32 color);
void circleframe(int x, int y, int r, u32 color);
void backgroundcolor(u32);
u32 getrandom();
//
void say(char*,...);




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 pixelbuffer;
        u64 pixelformat;
        u64 width;
        u64 height;
}*haha;

//
static int turn;
static char* data;




void weiqi_read()
{
	int x,y;
	int cx,cy,half;
	u32 color;

	cx = (haha->width)/2;
	cy = (haha->height)/2;

	if(cy > cx)half = cx/20;
	else half = cy/20;

	if( ((haha->pixelformat)&0xffffffff) == 0x61626772)
        {
		backgroundcolor(0x256f8d);
        }
	else
	{
		backgroundcolor(0x8d6f25);
	}

	//heng
	for(y=-9;y<10;y++)
	{
		line(	cx - half*2*9,	cy + half*2*y,
			cx + half*2*9,	cy + half*2*y,	0);
	}

	//shu
	for(x=-9;x<10;x++)
	{
		line(	cx + half*2*x,	cy - half*2*9,
			cx + half*2*x,	cy + half*2*9,	0);
	}

	//dian
	for(y = cy - half*2*6; y <= cy + half*2*6; y += half*2*6)
	{
		for(x = cx - half*2*6; x <= cx + half*2*6; x += half*2*6)
		{
			circlebody(x, y, half/4, 0);
		}
	}

	//zi
	for(y=-9;y<=9;y++)
	{
		for(x=-9;x<=9;x++)
		{
			if(data[(y+9)*19 + x+9] == 'b')color = 0;
			else if(data[(y+9)*19 + x+9] == 'w')color = 0xffffffff;
			else continue;

			circlebody(
				cx + half*2*x,
				cy + half*2*y,
				half,
				color
			);
		}
	}
}




void weiqi_write(u64* who, u64* what, u64* key)
{
	char val;
	int x,y;
	int cx,cy,half;

	cx = (haha->width)/2;
	cy = (haha->height)/2;
	if(cy > cx)half = cx/20;
	else half = cy/20;

	if(*what == 0x7466656C207A7978)
	{
		x=(*key) & 0xffff;
		y=( (*key) >> 16 ) & 0xffff;
		//say("%d,%d\n",x,y);

		x = (((x-cx)<<8)/half + 4736)>>9;
		y = (((y-cy)<<8)/half + 4736)>>9;
		//say("%d,%d\n",x,y);

		if(x<0)return;
		if(x>18)return;
		if(y<0)return;
		if(y>18)return;

		if((turn&0x1) == 0)data[y*19 + x] = 'b';
		else data[y*19 + x] = 'w';
		turn++;
	}
}




static void weiqi_list()
{
}
static void weiqi_choose()
{
}




static void weiqi_start()
{
	int x,y;

	turn=0;
	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			data[y*19 + x] = 0;
		}
	}
}
static void weiqi_stop()
{
}




void weiqi_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x656d6167;
	this[1] = 0x6971696577;

	this[10]=(u64)weiqi_start;
	this[11]=(u64)weiqi_stop;
	this[12]=(u64)weiqi_list;
	this[13]=(u64)weiqi_choose;
	this[14]=(u64)weiqi_read;
	this[15]=(u64)weiqi_write;

	data=base+0x300000;
}
void weiqi_delete()
{
}
