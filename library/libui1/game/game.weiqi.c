#define u64 unsigned long long 
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void line(
	int x1,int y1,int x2,int y2, u32 color);
void circlebody(
	int x, int y, int r, u32 color);
void circleframe(
	int x, int y, int r, u32 color);
void backgroundcolor(
	u64, u64, u64, u64,
	u32);
u32 getrandom();
//
void say(char*,...);




static struct temp{
	u64 type;
	u64 id;
	u64 start;
	u64 end;

	u64 buffer;
	u64 format;
	u64 width;
	u64 height;
}*haha;
//
static int turn;
static int px,py;
//
static char* data;




static void weiqi_read_html()
{
}
static void weiqi_read_text()
{
	int x,y,j,k,ret,color;
	int width=haha->width;
	int height=haha->height;
	u8* p = (u8*)(haha->buffer);

	//haha
	for(x=0;x<width*height*4;x++)p[x] = 0;
	for(y=0;y<19;y++)
	{
		for(x=0;x<19;x++)
		{
			//position
			ret = y*width + x*2;
			ret <<= 2;

			//color
			if( (px == x) && (py == y) )color = 7;
			else if(data[(y*19) + x] == 'b')color = 4;
			else if(data[(y*19) + x] == 'w')color = 1;
			else continue;

			//
			p[ret] = 0x20;
			p[ret + 3] = color;
			p[ret + 4] = 0x20;
			p[ret + 7] = color;
		}
	}
}
static void weiqi_read_pixel()
{
	int x,y;
	int cx,cy,half;
	u32 color;

	cx = (haha->width)/2;
	cy = (haha->height)/2;

	if(cy > cx)half = cx/20;
	else half = cy/20;

	if( ((haha->format)&0xffffffff) == 0x61626772)
	{
		color = 0x256f8d;
	}
	else
	{
		color = 0x8d6f25;
	}
	backgroundcolor(
		haha->buffer, 0, haha->width, haha->height,
		color
	);

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
static void weiqi_read()
{
	u32 temp = (haha->format)&0xffffffff;

	//text
	if(temp == 0x74786574)
	{
		weiqi_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		weiqi_read_html();
	}

	//pixel
	else
	{
		weiqi_read_pixel();
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

	if(*what == 0x64626b)
	{
		if(*key == 0x25)	//left
		{
			if(px<1)return;
			px--;
		}
		else if(*key == 0x26)   //up
		{
			if(py<1)return;
			py--;
		}
		else if(*key == 0x27)   //right
		{
			if(px>=18)return;
			px++;
		}
		else if(*key == 0x28)   //down
		{
			if(py>=18)return;
			py++;
		}
	}

	else if(*what == 0x72616863)
	{
		if(*key == 0x20)
		{
			if((turn&1)==0)data[(py*19)+px] = 'b';
			else data[(py*19)+px] = 'w';
			turn++;
		}
	}

	else if(*what == 0x2d6d)
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

		if((turn&0x1) == 0)data[(y*19) + x] = 'b';
		else data[(y*19) + x] = 'w';
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
	px=py=0;

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
