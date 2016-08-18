#define QWORD unsigned long long
#define DWORD unsigned int
//
void rectbody(int x1,int y1,int x2,int y2,DWORD color);
void rectframe(int x1,int y1,int x2,int y2,DWORD color);
void rect(int x1,int y1,int x2,int y2,DWORD body,DWORD frame);
void circlebody(int x,int y,int r,DWORD color);
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




static void joystick_read_html()
{
}
static void joystick_read_pixel()
{
	int j;
	int width = haha->width;
	int height = haha->height;

	backgroundcolor(0x444444);
	for(j=0;j<64;j++)
	{
		rectframe(
			j, j,
			width-64+j, height-64+j,
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

		circlebody(width*5/8, height/2, j, 0x546974);
		circlebody(width*3/4, height/4, j, 0x987126);
		circlebody(width*3/4, height*3/4, j, 0x123465);
		circlebody(width*7/8, height/2, j, 0x846962);
	}
	else
	{
		j = width/16;

		rectframe(width*3/4,height*13/32,width*11/16,height/2, 0);
		rectframe(width*3/4,height/2,width*11/16,height*19/32, 0);

		circlebody(width/2, height/8, j, 0xff);
		circlebody(width/4, height/4, j, 0xff00);
		circlebody(width*3/4, height/4, j, 0xffff);
		circlebody(width/2, height*3/8, j, 0xff0000);

		circlebody(width/2, height*5/8, j, 0x078473);
		circlebody(width/4, height*3/4, j, 0x436593);
		circlebody(width*3/4, height*3/4, j, 0x352856);
		circlebody(width/2, height*7/8, j, 0x749272);
	}
}
static void joystick_read_text()
{
}
static void joystick_read()
{
	//text
	if( ( (haha->pixelformat)&0xffffffff) == 0x74786574)
	{
		joystick_read_text();
	}

	//html
	else if( ( (haha->pixelformat)&0xffffffff) == 0x6c6d7468)
	{
		joystick_read_html();
	}

	//pixel
	else
	{
		joystick_read_pixel();
	}
}




static void joystick_write()
{
}




static void joystick_list()
{
}
static void joystick_change()
{
}




static void joystick_start()
{
}
static void joystick_stop()
{
}
void joystick_create(void* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x6b63697473796f6a;

	this[10]=(QWORD)joystick_start;
	this[11]=(QWORD)joystick_stop;
	this[12]=(QWORD)joystick_list;
	this[13]=(QWORD)joystick_change;
	this[14]=(QWORD)joystick_read;
	this[15]=(QWORD)joystick_write;
}
void joystick_delete()
{
}
