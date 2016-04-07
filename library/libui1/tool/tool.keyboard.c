#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
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
	for(y=0;y<768;y++)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0;
		}
	}

	//横线
	for(y=64;y<=768-64;y+=64)
	{
		for(x=0;x<1024;x++)
		{
			screenbuf[y*1024+x]=0xffffffff;
		}
	}

	//竖线
	for(x=128;x<1024;x+=128)
	{
		for(y=64;y<=768-64;y++)
		{
			screenbuf[y*1024+x]=0xffffffff;
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
