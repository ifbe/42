#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void backgroundcolor();
//
void say(char*,...);




//显示
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

//元件
struct cell{
	int type;	//battery, resistor, ...
	int value;	//ohm, volt, ...
	int side1;	//1,2,3, ...
	int side2;	//1,2,3, ...
};




static void circuit_read_html()
{
}
static void circuit_read_pixel()
{
	backgroundcolor(0);
}
static void circuit_read_text()
{
}
static void circuit_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		circuit_read_text();
	}

	//html
	else if( ( (haha->pixelformat)&0xffffffff) == 0x6c6d7468)
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
