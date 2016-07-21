#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//
void rectbody(DWORD xyz1,DWORD xyz2,DWORD color);
void backgroundcolor(DWORD);
//
void background_start(QWORD size,void* addr);
void shape_start(QWORD size,void* addr);
void ascii_start(QWORD size,void* addr);
void unicode_start(QWORD size,void* addr);
//
void qrcode_generate(char* src,char* dst,int sidelength);
void say(char*,...);




//
static int width;
static int height;
static DWORD* screenbuf;
//
static int sidelength;
static char* databuf;




static void qrcode_list()
{
}
static void qrcode_into()
{
}
static void qrcode_read()
{
	DWORD color;
	int min;
	int x,y,x1,y1,x2,y2;

	if(width<height)min=width;
	else min=height;

	backgroundcolor(0);
	for(y=0;y<sidelength;y++)
	{
		for(x=0;x<sidelength;x++)
		{
			x1=x*min/sidelength;
			y1=y*min/sidelength;
			x2=(x+1)*min/sidelength-1;
			y2=(y+1)*min/sidelength-1;
			if( databuf[(y*sidelength)+x] == 0 )color=0;
			else color=0xffffffff;
//say("%d ",databuf[(y*sidelength)+x]);
			rectbody(x1+(y1<<16), x2+(y2<<16), color);
		}
//say("\n");
	}
}
static void qrcode_write()
{
}
static void qrcode_start(QWORD size,void* addr)
{
	ascii_start(size,addr);
	unicode_start(size,addr);
	background_start(size,addr);
	shape_start(size,addr);

	//
	width=size&0xffff;
	height=(size>>16)&0xffff;
	screenbuf=addr;

	sidelength=49;
	qrcode_generate("haha",databuf,sidelength);
}
static void qrcode_stop()
{
}
void qrcode_init(void* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x65646f637271;
	this[2]=(QWORD)qrcode_start;
	this[3]=(QWORD)qrcode_stop;
	this[4]=(QWORD)qrcode_list;
	this[5]=(QWORD)qrcode_into;
	this[6]=(QWORD)qrcode_read;
	this[7]=(QWORD)qrcode_write;

	databuf=(char*)(addr+0x300000);
}
void qrcode_kill()
{
}
