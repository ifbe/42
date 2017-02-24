#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void rectframe(
	int x1, int y1, int x2, int y2, u32 color);
void line(
	int x1,int y1,int x2,int y2, u32 color);
void backgroundcolor(
	u64, u64, u64, u64,
	u32);
//
void say(char*,...);




//显示
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

//元件
struct cell{
	int type;	//battery, resistor, ...
	int value;	//ohm, volt, ...
	int side1;	//1,2,3, ...
	int side2;	//1,2,3, ...
};

//(batt, 5000mv), (0, 1)
static struct cell c1 = {'V', 5000, 0, 1};
//(resistor, 1000ohm), (1,0)
static struct cell c2 = {'R', 1000, 1, 0};




static void battery(int x, int y, struct cell* c)
{
	line(x-16, y-8, x+16, y-8, 0xffffffff);
	line(x-8, y+8, x+8, y+8, 0xffffffff);
}
static void resistor(int x, int y, struct cell* c)
{
	rectframe(x-8, y-16, x+8, y+16, 0xffffffff);
}
static void autowire(int x1, int y1, int x2, int y2)
{
	int j;
	if(x1<x2)
	{
		j=y1-100;
		if(j > y2-100)j=y2-100;
		line(x1, y1, x1, j, 0xffffffff);
		line(x2, y2, x2, j, 0xffffffff);
		line(x1, j, x2, j, 0xffffffff);
	}
	else
	{
		j=y1+100;
		if(j < y2+100)j=y2+100;
		line(x1, y1, x1, j, 0xffffffff);
		line(x2, y2, x2, j, 0xffffffff);
		line(x1, j, x2, j, 0xffffffff);
	}
}




static void circuit_read_html()
{
}
static void circuit_read_pixel()
{
	int battx = haha->width/3;
	int batty = haha->height/2;
	int resx = haha->width*2/3;
	int resy = haha->height/2;
	backgroundcolor(
		haha->buffer, 0, haha->width, haha->height,
		0
	);

	//5v battery
	battery(battx, batty, &c1);

	//1000ohm resistor
	resistor(resx, resy, &c2);

	//wire
	autowire(battx, batty-8, resx, resy-16);
	autowire(resx, resy+16, battx, batty+8);
}
static void circuit_read_text()
{
}
static void circuit_read()
{
	//text
	if( ( (haha->format)&0xffffffff) == 0x74786574)
	{
		circuit_read_text();
	}

	//html
	else if( ( (haha->format)&0xffffffff) == 0x6c6d7468)
	{
		circuit_read_html();
	}

	//pixel
	else
	{
		circuit_read_pixel();
	}
}




static void circuit_write(u64* who, u64* what, u64* how)
{
}




static void circuit_list()
{
}
static void circuit_change()
{
}




static void circuit_start()
{
}
static void circuit_stop()
{
}
void circuit_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x74736574;
	this[1] = 0x74697563726963;

	this[10]=(u64)circuit_start;
	this[11]=(u64)circuit_stop;
	this[12]=(u64)circuit_list;
	this[13]=(u64)circuit_change;
	this[14]=(u64)circuit_read;
	this[15]=(u64)circuit_write;
}
void circuit_delete()
{
}
