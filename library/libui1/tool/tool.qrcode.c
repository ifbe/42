#define u64 unsigned long long
#define u32 unsigned int
#define u16 unsigned short
#define u8 unsigned char
//
void rectbody(int x1, int y1, int x2, int y2, u32 color);
void backgroundcolor(u32);
//
void qrcode_generate(char* src,char* dst,int sidelength);
void say(char*,...);




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 pixelbuffer;
        u64 pixelformat;
        u64 width;
        u64 height;
}*haha;

//
static int sidelength;
static char* databuf;




static void qrcode_read()
{
	u32 color;
	int x,y,x1,y1,x2,y2;
	int width,height,min;

	width = haha->width;
	height = haha->height;
	if(width < height)min = width;
	else min = height;

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
static void qrcode_write(u64* type,u64* key)
{
}




static void qrcode_list()
{
}
static void qrcode_into()
{
}




static void qrcode_start()
{
	sidelength=49;
	qrcode_generate("haha",databuf,sidelength);
}
static void qrcode_stop()
{
}




void qrcode_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x65646f637271;

	this[10]=(u64)qrcode_start;
	this[11]=(u64)qrcode_stop;
	this[12]=(u64)qrcode_list;
	this[13]=(u64)qrcode_into;
	this[14]=(u64)qrcode_read;
	this[15]=(u64)qrcode_write;

	databuf=(char*)(addr+0x300000);
}
void qrcode_delete()
{
}
