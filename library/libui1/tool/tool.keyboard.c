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
int xsize=0;
int ysize=0;




static void keyboard_write(QWORD type,QWORD value)
{
	
}
static void keyboard_read()
{
	int x,y;
	for(y=0;y<ysize;y++)
	{
		for(x=0;x<xsize;x++)
		{
			screenbuf[y*xsize + x]=0;
		}
	}

	//横线
	for(y=ysize/8;y<ysize*15/16;y+=ysize/8)
	{
		for(x=0;x<xsize;x++)
		{
			screenbuf[y*xsize+x]=0xffffffff;
		}
	}

	//竖线
	for(x=xsize/8;x<xsize*15/16;x+=xsize/8)
	{
		for(y=0;y<=ysize;y++)
		{
			screenbuf[y*xsize+x]=0xffffffff;
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
	xsize=size&0xffff;
	ysize=(size>>16)&0xffff;
//say("%d,%d\n",xsize,ysize);
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
