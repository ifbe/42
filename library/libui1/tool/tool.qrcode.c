#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//
void rectbody(int x1, int y1, int x2, int y2, DWORD color);
void backgroundcolor(DWORD);
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
			rectbody(x1, y1, x2, y2, color);
		}
//say("\n");
	}
}
static void qrcode_write()
{
}




static void qrcode_list()
{
}
static void qrcode_into()
{
	sidelength=49;
	qrcode_generate("haha",databuf,sidelength);
}




static void qrcode_start(QWORD size,void* addr)
{
	//
	width=size&0xffff;
	height=(size>>16)&0xffff;
	screenbuf=addr;
}
static void qrcode_stop()
{
}




void qrcode_init(void* base,void* addr)
{
	QWORD* this=(QWORD*)addr;
	this[0]=0x776f646e6977;
	this[1]=0x65646f637271;

	this[10]=(QWORD)qrcode_start;
	this[11]=(QWORD)qrcode_stop;
	this[12]=(QWORD)qrcode_list;
	this[13]=(QWORD)qrcode_into;
	this[14]=(QWORD)qrcode_read;
	this[15]=(QWORD)qrcode_write;

	databuf=(char*)(addr+0x300000);
}
void qrcode_kill()
{
}
