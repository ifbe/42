#define QWORD unsigned long long
#define DWORD unsigned int
//
void hexadecimal(int x,int y,QWORD in);
void printhexadecimal(int x, int y, int size, QWORD in, DWORD fg, DWORD bg);
//
void printmemory(char*,int);
void say(char*,...);




//
static DWORD* screenbuf;
static char* pixelformat;
static int width;
static int height;
//
static int flag=0;




static void pure_list()
{
}
static void pure_into()
{
}
static void pure_read()
{
	int x,y;
	DWORD color=0xff000000;

	if((flag&0x1) == 0x1)color |= 0xff;
	if((flag&0x2) == 0x2)color |= 0xff00;
	if((flag&0x4) == 0x4)color |= 0xff0000;

	for(y=0;y<height;y++)
	{
		for(x=0;x<width;x++)
		{
			screenbuf[y*width + x] = color;
		}
	}

	printhexadecimal(0, 0, 4, color & 0xffffff, 0x87654321,0xfedcba98);
}
static void pure_write(QWORD type,QWORD key)
{
	if( (type&0xffffffff) == 0x207A7978 )
	{
		flag = (flag+1)&0x7;
	}
}




static void pure_start(QWORD size,void* addr)
{
	int i;

	//
	screenbuf=addr;
	width=size&0xffff;
	height=(size>>16)&0xffff;
}
static void pure_stop()
{
}
void pure_init(char* uibuf,char* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x65727570;

	this[10]=(QWORD)pure_start;
	this[11]=(QWORD)pure_stop;
	this[12]=(QWORD)pure_list;
	this[13]=(QWORD)pure_into;
	this[14]=(QWORD)pure_read;
	this[15]=(QWORD)pure_write;
}
void pure_kill()
{
}

