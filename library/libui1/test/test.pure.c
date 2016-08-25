#define u64 unsigned long long
#define u32 unsigned int
//
void hexadecimal(int x,int y,u64 in);
void printhexadecimal(int x, int y, int size, u64 in, u32 fg, u32 bg);
//
void printmemory(char*,int);
void say(char*,...);




//
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
	u32 color;
	u32* screenbuf;

	screenbuf = (u32*)(haha->pixelbuffer);

	color=0xff000000;
	if((flag&0x1) == 0x1)color |= 0xff;
	if((flag&0x2) == 0x2)color |= 0xff00;
	if((flag&0x4) == 0x4)color |= 0xff0000;

	for(y=0;y<(haha->height);y++)
	{
		for(x=0;x<(haha->width);x++)
		{
			screenbuf[(haha->width)*y + x] = color;
		}
	}

	printhexadecimal(0, 0, 4, color & 0xffffff, 0x87654321,0xfedcba98);
}
static void pure_write(u64* type,u64* key)
{
	if( *(unsigned int*)type == 0x207A7978 )
	{
		flag = (flag+1)&0x7;
	}
}




static void pure_start()
{
}
static void pure_stop()
{
}
void pure_create(void* uibuf,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0]=0x776f646e6977;
	this[1]=0x65727570;

	this[10]=(u64)pure_start;
	this[11]=(u64)pure_stop;
	this[12]=(u64)pure_list;
	this[13]=(u64)pure_into;
	this[14]=(u64)pure_read;
	this[15]=(u64)pure_write;
}
void pure_delete()
{
}

