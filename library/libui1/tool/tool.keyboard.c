#define u64 unsigned long long
#define u32 unsigned int
//
void printascii(int x1,int y1,int size,char ch,u32 fg,u32 bg);
void rectbody(int x1,int y1,int x2,int y2,u32 color);
void rectframe(int x1,int y1,int x2,int y2,u32 color);
void rect(int x1,int y1,int x2,int y2,u32 body,u32 frame);
void circlebody(int x,int y,int r,u32 color);
void circleframe(int x,int y,int r,u32 color);
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




static void keyboard_read_html()
{
}
static void keyboard_read_pixel()
{
	int x,y;
	int width = haha->width;
	int height = haha->height;

	backgroundcolor(0x444444);
	for(x=0;x<32;x++)
	{
		rectframe(
			x, x,
			width-32+x, height-32+x,
			0x040404*x
		);
	}
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			rectframe(
				32 + (width-32)*x/8, 32 + (height-32)*y/8,
				31 + (width-32)*(x+1)/8, 31 + (height-32)*(y+1)/8,
				0xffffff
			);

			printascii(
				32 + (width-32)*x/8, 32 + (height-32)*y/8,
				4, 'a',
				0, 0xffffffff
			);
		}
	}
}
static void keyboard_read_text()
{
}
static void keyboard_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		keyboard_read_text();
	}

	//html
	else if( ( (haha->pixelformat)&0xffffffff) == 0x6c6d7468)
	{
		keyboard_read_html();
	}

	//pixel
	else
	{
		keyboard_read_pixel();
	}
}




static void keyboard_write(u64* type,u64* key)
{
}




static void keyboard_list()
{
}
static void keyboard_change()
{
}




static void keyboard_start()
{
}
static void keyboard_stop()
{
}
void keyboard_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x6472616f6279656b;

	this[10]=(u64)keyboard_start;
	this[11]=(u64)keyboard_stop;
	this[12]=(u64)keyboard_list;
	this[13]=(u64)keyboard_change;
	this[14]=(u64)keyboard_read;
	this[15]=(u64)keyboard_write;
}
void keyboard_delete()
{
}
