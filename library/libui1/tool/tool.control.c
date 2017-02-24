#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void printascii(
	int x1,int y1,int size,char ch,u32 fg,u32 bg);
void rectbody(
	int x1,int y1,int x2,int y2,u32 color);
void rectframe(
	int x1,int y1,int x2,int y2,u32 color);
void rect(
	int x1,int y1,int x2,int y2,u32 body,u32 frame);
void circlebody(
	int x,int y,int r,u32 color);
void circleframe(
	int x,int y,int r,u32 color);
void backgroundcolor(
	u64, u64, u64, u64,
	u32);
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




static int aaaa = 0;
static void keyboard()
{
	int x,y;
	int width = haha->width;
	int height = haha->height;

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
static void joystick()
{
	int j;
	int width = haha->width;
	int height = haha->height;

	for(j=0;j<32;j++)
	{
		rectframe(
			j, j,
			width-32+j, height-32+j,
			0x040404*j
		);
	}

	if(width > height)
	{
		j = height/16;

		rectframe(width*13/32,height*3/4,width/2,height*11/16, 0);
		rectframe(width/2,height*3/4,width*19/32,height*11/16, 0);

		circlebody(width/8, height/2, j, 0xff);
		circlebody(width/4, height/4, j, 0xff00);
		circlebody(width/4, height*3/4, j, 0xffff);
		circlebody(width*3/8, height/2, j, 0xff0000);

		circlebody(width*5/8, height/2, j, 0xff00ff);
		circlebody(width*3/4, height/4, j, 0xfedcba);
		circlebody(width*3/4, height*3/4, j, 0xabcdef);
		circlebody(width*7/8, height/2, j, 0xffff00);
	}
	else
	{
		j = width/16;

		rectframe(width/4,height*13/32,width*5/16,height/2, 0);
		rectframe(width/4,height/2,width*5/16,height*19/32, 0);

		circlebody(width/2, height/8, j, 0xff);
		circlebody(width/4, height/4, j, 0xffff);
		circlebody(width*3/4, height/4, j, 0xff00);
		circlebody(width/2, height*3/8, j, 0xff0000);

		circlebody(width/2, height*5/8, j, 0xff00ff);
		circlebody(width/4, height*3/4, j, 0xabcdef);
		circlebody(width*3/4, height*3/4, j, 0xfedcba);
		circlebody(width/2, height*7/8, j, 0xffff00);
	}
}
static void touchpad()
{
	int j;
	int width = haha->width;
	int height = haha->height;

	for(j=0;j<32;j++)
	{
		rectframe( 
			j, j,
			width-32+j, height-32+j,
			0x040404*j
		);
	}
}
static void control_read_text()
{
}
static void control_read_html()
{
}
static void control_read()
{
	//text
	if( ( (haha->format)&0xffffffff) == 0x74786574)
	{
		control_read_text();
	}

	//html
	else if( ( (haha->format)&0xffffffff) == 0x6c6d7468)
	{
		control_read_html();
	}

	//pixel
	else
	{
		backgroundcolor(
			haha->buffer, 0, haha->width, haha->height,
			0x444444
		);

		if(aaaa == 0)joystick();
		else if(aaaa == 1)keyboard();
		else if(aaaa == 2)touchpad();

		rectbody(0, 0, 64, 64, 0xffffff);
		rectbody(haha->width-64, 0, haha->width-1, 64, 0xffffff);
		rectbody(0, haha->height-64, 64, haha->height-1, 0xffffff);
	}
}




static void control_write(u64* who, u64* what, u64* how)
{
	int x,y;
	if(*what == 0x2d6d)
	{
		x = *(u16*)how;
		y = *(u16*)(how+2);

		if(y<64)
		{
			if(x<64)aaaa = 0;
			if(x>(haha->width-64))aaaa = 1;
		}
		if(y>(haha->height-64))
		{
			if(x<64)aaaa = 2;
		}
	}
}




static void control_list()
{
}
static void control_change()
{
}




static void control_start()
{
}
static void control_stop()
{
}
void control_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x6c6f72746e6f63;

	this[10]=(u64)control_start;
	this[11]=(u64)control_stop;
	this[12]=(u64)control_list;
	this[13]=(u64)control_change;
	this[14]=(u64)control_read;
	this[15]=(u64)control_write;
}
void control_delete()
{
}
