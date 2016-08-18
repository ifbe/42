#define QWORD unsigned long long
#define DWORD unsigned int
//
void printascii(int x1,int y1,int size,char ch,DWORD fg,DWORD bg);
void rectbody(int x1,int y1,int x2,int y2,DWORD color);
void rectframe(int x1,int y1,int x2,int y2,DWORD color);
void rect(int x1,int y1,int x2,int y2,DWORD body,DWORD frame);
void circlebody(int x,int y,int r,DWORD color);
void circleframe(int x,int y,int r,DWORD color);
void backgroundcolor();
//
void say(char*,...);




static struct temp{
        QWORD type;
        QWORD id;
        QWORD start;
        QWORD end;

        QWORD pixelbuffer;
        QWORD pixelformat;
        QWORD width;
        QWORD height;
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
	for(x=0;x<64;x++)
	{
		rectframe(
			x, x,
			width-64+x, height-64+x,
			0x040404*x
		);
	}
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			rectframe(
				64 + (width-64)*x/8, 64 + (height-64)*y/8,
				63 + (width-64)*(x+1)/8, 63 + (height-64)*(y+1)/8,
				0xffffff
			);

			printascii(
				64 + (width-64)*x/8, 64 + (height-64)*y/8,
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




static void keyboard_write()
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
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x6472616f6279656b;

	this[10]=(QWORD)keyboard_start;
	this[11]=(QWORD)keyboard_stop;
	this[12]=(QWORD)keyboard_list;
	this[13]=(QWORD)keyboard_change;
	this[14]=(QWORD)keyboard_read;
	this[15]=(QWORD)keyboard_write;
}
void keyboard_delete()
{
}
