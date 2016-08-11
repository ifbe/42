#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void backgroundcolor(DWORD);




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




static void keyboard_write(QWORD type,QWORD value)
{
	
}
static void keyboard_read()
{
	int x,y;
	int width,height;
	DWORD* screenbuf;

	screenbuf = (DWORD*)(haha->pixelbuffer);
	width = haha->width;
	height = haha->height;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			screenbuf[y*width + x]=0;
		}
	}

	//横线
	for(y=height/8;y<height*15/16;y+=height/8)
	{
		for(x=0;x<width;x++)
		{
			screenbuf[y*width+x]=0xffffffff;
		}
	}

	//竖线
	for(x=width/8;x<width*15/16;x+=width/8)
	{
		for(y=0;y<=height;y++)
		{
			screenbuf[y*width+x]=0xffffffff;
		}
	}
}
static void keyboard_into()
{
}
static void keyboard_list()
{
}








static void keyboard_start()
{
	backgroundcolor(0);
}
static void keyboard_stop()
{
}
void keyboard_init(void* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x64626b;

	this[10]=(QWORD)keyboard_start;
	this[11]=(QWORD)keyboard_stop;
	this[12]=(QWORD)keyboard_list;
	this[13]=(QWORD)keyboard_into;
	this[14]=(QWORD)keyboard_read;
	this[15]=(QWORD)keyboard_write;
}
void keyboard_kill()
{
}
