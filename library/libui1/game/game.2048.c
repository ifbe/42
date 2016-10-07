#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void printdecimal(int x, int y, int size, int data, u32 fg, u32 bg);
void rectbody(int x1, int y1, int x2, int y2, u32 color);
void rectframe(int x1, int y1, int x2, int y2, u32 color);
void rect(int x1, int y1, int x2, int y2, u32 bodycolor, u32 framecolr);
void line(int,int,int,int,u32 color);
void backgroundcolor(u32);
//
int data2decimalstring(u64 data,u8* string);
unsigned int getrandom();
//
int printmemory(char*,int);
int diary(char*, int, char*, ...);
int say(char*,...);




//
static struct temp{
	u64 type;
	u64 id;
	u64 start;
	u64 end;

	u64 pixelbuffer;
	u64 pixelformat;
	u64 width;
	u64 height;

	u64 f0;
	u64 f1;
	u64 f2;
	u64 f3;

	u64 f4;
	u64 f5;
	u64 f6;
	u64 f7;
}*haha;

//
static int num;
static void* buffer;




u32 the2048_color(int val)
{
	switch(val)
	{
		case    0:	return 0x555555;
		case    2:	return 0xfffff0;
		case    4:	return 0xffffc0;
		case    8:	return 0x995000;
		case   16:	return 0xc05000;
		case   32:	return 0xb03000;
		case   64:	return 0xff0000;
		case  128:	return 0xffffa0;
		case  256:	return 0xffff80;
		case  512:	return 0xffff00;
		case 1024:	return 0xffffb0;
		case 2048:	return 0x233333;
		case 4096:	return 0x783d72;
		case 8192:	return 0xd73762;
	}
}
static void cubie(int x,int y,int z)
{
	int min;
	int color;
	int count;
	if(haha->width < haha->height)min = haha->width;
	else min = haha->height;

	//
	if(z<16)count=0;
	else if(z<128)count=1;
	else if(z<1024)count=2;
	else if(z<16384)count=3;
	else count=4;

	//
	color = the2048_color(z);
	if( ( (haha->pixelformat)&0xffffffff) == 0x61626772)	//bgra->rgba
	{
		color	= 0xff000000
			+ ((color&0xff)<<16)
			+ (color&0xff00)
			+ ((color&0xff0000)>>16);
	}

	rect(
		x*min/4,
		y*min/4,
		((x+1)*min/4) - 1,
		((y+1)*min/4) - 1,
		color,
		0
	);

	if(z==0)return;
	printdecimal(
		x*(min/4)+min/10-count*16,
		y*(min/4)+min/20,
		4,
		z,
		0,
		0
	);
}
static void the2048_read_text()
{
	int x,y;
	int w = haha->width;
	int h = haha->height;
	char* p = (char*)(haha->pixelbuffer);
	int (*table)[4] = buffer + num*16*4;

	for(x=0;x<w * h;x++)p[x]=0x20;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			data2decimalstring(table[y][x], p + 4*w*y + 8*x);
		}
	}
}
static void the2048_read_html()
{
	int x,y;
	u32 color;
	char* p = (char*)(haha->pixelbuffer);
	int (*table)[4] = buffer + num*16*4;

	*(u32*)p = 0x6c6d7468;
	p += 0x1000;

	p += diary(
		p, 0x1000,
		"<style type=\"text/css\">"
		".rect{"
		"border:1px solid #000;"
		"position:absolute;"
		"color:#000;"
		"width:25%;"
		"height:25%;"
		"}"
		"</style>"
	);

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x] == 0)continue;

			color = the2048_color(table[y][x]);
			p += diary(
				p, 0x1000,
				"<div class=\"rect\" style=\""
				"left:%d%;"
				"top:%d%;"
				"background:#%06x;"
				"\">%d</div>",
				25*x, 25*y,
				color, table[y][x]
			);
		}
	}
}
static void the2048_read_pixel()
{
	int x,y;
	int (*table)[4] = buffer + num*16*4;

	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			cubie(x,y,table[y][x]);
		}
	}
}
static void the2048_read()
{
	u32 temp = (haha->pixelformat)&0xffffffff;
	//say("(@2048.read)temp=%x\n",temp);

	//text
	if(temp == 0x74786574)
	{
		the2048_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		the2048_read_html();
	}

	//pixel
	else
	{
		the2048_read_pixel();
	}
}




//斗兽场......
static void left2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	//4对祭品
	for(y=0;y<4;y++)
	{
		//前三个会去作死
		for(x=0;x<3;x++)
		{
		//前三个里面没死的才会去作死
		if(table[y][x]!=0)
		{
			//开始作死
			dst=x+1;
			for(;dst<4;dst++)
			{
				//空位置就找下一个
				if(table[y][dst] == 0)continue;

				//跟自己不一样，没法欺负就跑
				if(table[y][dst] != table[y][x])break;

				//找到相同的只能活一个，依次排队准备下次
				table[y][x]=2*table[y][x];	//吃掉别人
				table[y][dst]=0;		//扔尸体
				x=dst;		//别
				break;
			}
		}//if
		}//forx

		//活着的排好队
		dst=0;
		for(x=0;x<4;x++)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[y][dst]=temp;
			dst++;
		}
		}
	}//fory
}
static void right2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	for(y=0;y<4;y++)
	{
		for(x=3;x>0;x--)
		{
		if(table[y][x]!=0)
		{
			dst=x-1;
			for(;dst>=0;dst--)
			{
				if( table[y][dst] == 0 )continue;
				if( table[y][dst] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[y][dst]=0;
				x=dst;
				break;
			}
		}//if
		}//forx

		dst=3;
		for(x=3;x>=0;x--)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[y][dst]=temp;
			dst--;
		}
		}
	}//fory
}
static void up2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	for(x=0;x<4;x++)
	{
		for(y=0;y<3;y++)
		{
		if(table[y][x]!=0)
		{
			dst=y+1;
			for(;dst<4;dst++)
			{
				if( table[dst][x] == 0 )continue;
				if( table[dst][x] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[dst][x]=0;
				y=dst;
				break;
			}
		}//if
		}//fory

		dst=0;
		for(y=0;y<4;y++)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[dst][x]=temp;
			dst++;
		}
		}
	}//forx
}
static void down2048()
{
	int x,y;
	int dst,temp;
	int (*table)[4] = buffer + num*16*4;

	for(x=0;x<4;x++)
	{
		for(y=3;y>0;y--)
		{
		if(table[y][x]!=0)
		{
			dst=y-1;
			for(;dst>=0;dst--)
			{
				if( table[dst][x] == 0 )continue;
				if( table[dst][x] != table[y][x] )break;
				table[y][x]=2*table[y][x];
				table[dst][x]=0;
				y=dst;
				break;
			}
		}//if
		}//forx

		dst=3;
		for(y=3;y>=0;y--)
		{
		if(table[y][x]!=0)
		{
			temp=table[y][x];
			table[y][x]=0;
			table[dst][x]=temp;
			dst--;
		}
		}
	}//fory
}
static void new2048()
{
	int x,y;
	int who,temp;
	int (*table)[4] = buffer + num*16*4;

	//how many blank cubie
	who=0;
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			if(table[y][x]==0)who++;
		}
	}
	if(who==0)return;

	//where
	temp = getrandom();
	if(temp<0) temp = -temp;
	who = temp % who;

	//value
	temp = getrandom()&0x3;
	if(temp>=2)temp=4;
	else temp=2;

	//put
	for(y=0;y<4;y++)
	{
		for(x=0;x<4;x++)
		{
			//here=0 : i "can" put here
			if(table[y][x]==0)
			{
				//who=0 : i "want" put here
				if(who!=0)who--;
				else
				{
					table[y][x]=temp;
					return;
				}
			}//if(empty)
		}//for(x)
	}//for(y)
}
static void the2048_write(u64* who, u64* what, u64* key)
{
	//kbd
	u8 ch;
	int j;
	int* p;
	int* q;

	//
	ch = *(u8*)key;
	if(*what == 0x64626b)
	{
		//left,right,up,down
		if( (ch>=0x25) && (ch<=0x28) )
		{
			//
			p = buffer + 64*num;
			num = (num+1)%0x1000;
			q = buffer + 64*num;
			for(j=0;j<16;j++)q[j] = p[j];

			//
			if(ch == 0x25)left2048();
			else if(ch == 0x26)up2048();
			else if(ch == 0x27)right2048();
			else if(ch == 0x28)down2048();

			//new number?
			new2048();
		}
	}
	else if(*what == 0x72616863)
	{
		if(ch == 0x8)
		{
			if(num>0)num--;
		}
	}
}








static void the2048_list()
{
}
static void the2048_choose()
{
}








static void the2048_start()
{
	int j;
	char* p = buffer;
	for(j=0;j<0x4000;j++)p[j] = 0;

	//
	num = 0;
	new2048();
}
static void the2048_stop()
{
}
void the2048_create(void* base,void* addr)
{
	buffer = base + 0x300000;
	haha = addr;

	haha->type = 0x656d6167;
	haha->id = 0x38343032;
	haha->f2 = (u64)the2048_start;
	haha->f3 = (u64)the2048_stop;
	haha->f4 = (u64)the2048_list;
	haha->f5 = (u64)the2048_choose;
	haha->f6 = (u64)the2048_read;
	haha->f7 = (u64)the2048_write;
}
void the2048_delete()
{
}
