#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
void backgroundcolor(DWORD);




DWORD* screenbuf=0;
static int width=0;
static int height=0;




static void keyboard_write(QWORD type,QWORD value)
{
	
}
static void keyboard_read()
{
	int x,y;
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








static void keyboard_start(QWORD size,void* addr)
{
	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	screenbuf=addr;
	width=size&0xffff;
	height=(size>>16)&0xffff;

	//
	backgroundcolor(0);
}
static void keyboard_stop()
{
}
void keyboard_init(char* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x64626b;
	this[2]=(QWORD)keyboard_start;
	this[3]=(QWORD)keyboard_stop;
	this[4]=(QWORD)keyboard_list;
	this[5]=(QWORD)keyboard_into;
	this[6]=(QWORD)keyboard_read;
	this[7]=(QWORD)keyboard_write;
}
void keyboard_kill()
{
}
