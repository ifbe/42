#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void rect(int x0,int y0,int x1,int y1,u32 body,u32 frame);
void printascii(int x,int y,int size,char ch,u32 fg,u32 bg);
//
u32 getrandom();
void say(char*,...);




static struct temp{
        u64 type;
        u64 id;
        u64 start;
        u64 end;

        u64 buffer;
        u64 format;
        u64 width;
        u64 height;
}*haha;
static char table[8][8] = {
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z',' ',' ',' ',' ',0x8,0xd,
	'0','1','2','3','4','5','6','7',
	'8','9',' ',' ',' ',' ',' ',' ',
	'+','-','*','/',' ',' ',' ',' ',
	'=',' ',' ',' ',' ',' ',' ',' '
};
static int arealeft = 64;
static int areatop = 672;
static int arearight = 960;
static int areabottom = 960;	//max=1024=2^10




static int virtkbd_write(u64* who, u64* what, u64* value)
{
	int x,y;
	int width = haha->width;
	int height = haha->height;

	if(*what == 0x2d6d)
	{
		x = (*value) & 0xffff;
		x = (x<<10)/width;
		if(x < arealeft)return 1;
		if(x > arearight)return 1;

		y = ( (*value) >> 16 ) & 0xffff;
		y = (y<<10)/height;
		if(y < areatop)return 1;
		if(y > areabottom)return 1;

		x = 8*(x-arealeft)/(arearight-arealeft);
		y = 8*(y-areatop)/(areabottom-areatop);
		say("==%d,%d\n",x,y);

		*what = 0x72616863;
		*value = table[y][x];
	}

	return 1;
}
static void virtkbd_read()
{
	int x,y;
	int left,top,right,bottom;
	int width = haha->width;
	int height = haha->height;

	//[a,z]
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			left = (arealeft + x*(arearight-arealeft)/8)*width/1024;
			top = (areatop + y*(areabottom-areatop)/8)*height/1024;
			right = (arealeft + (x+1)*(arearight-arealeft)/8)*width/1024;
			bottom = (areatop + (y+1)*(areabottom-areatop)/8)*height/1024;
			//say("====%d,%d,%d,%d\n",left,top,right,bottom);

			rect(	left, top, right, bottom,
				0xccffcc, 0x752614
			);

			printascii(	left, top,
					2, table[y][x],
					0x221133, 0
			);
		}
	}
}
static void virtkbd_into()
{
}
static void virtkbd_list()
{
}








static void virtkbd_start()
{
}
static void virtkbd_stop()
{
}
void virtkbd_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0]=0;
	this[1]=0x64626b74726976;

	this[10]=(u64)virtkbd_start;
	this[11]=(u64)virtkbd_stop;
	this[12]=(u64)virtkbd_list;
	this[13]=(u64)virtkbd_into;
	this[14]=(u64)virtkbd_read;
	this[15]=(u64)virtkbd_write;
}
void virtkbd_delete()
{
}
