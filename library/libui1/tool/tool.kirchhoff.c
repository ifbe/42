#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void backgroundcolor();
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
struct circuit{
	u64 type;	//battery, resistor, ...
	u64 value;	//ohm, volt, ...
	struct circuit* father;
	struct circuit* left;
	struct circuit* right;
	struct circuit* children;
};




static void kirchhoff_read_html()
{
}
static void kirchhoff_read_pixel()
{
}
static void kirchhoff_read_text()
{
}
static void kirchhoff_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		kirchhoff_read_text();
	}

	//html
	else if( ( (haha->pixelformat)&0xffffffff) == 0x6c6d7468)
	{
		kirchhoff_read_html();
	}

	//pixel
	else
	{
		kirchhoff_read_pixel();
	}
}




static void kirchhoff_write(u64* who, u64* what, u64* how)
{
}




static void kirchhoff_list()
{
}
static void kirchhoff_change()
{
}




static void kirchhoff_start()
{
}
static void kirchhoff_stop()
{
}
void kirchhoff_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x74736574;
	this[1] = 0x686b;

	this[10]=(u64)kirchhoff_start;
	this[11]=(u64)kirchhoff_stop;
	this[12]=(u64)kirchhoff_list;
	this[13]=(u64)kirchhoff_change;
	this[14]=(u64)kirchhoff_read;
	this[15]=(u64)kirchhoff_write;
}
void kirchhoff_delete()
{
}
