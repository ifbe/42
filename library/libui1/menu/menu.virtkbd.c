#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printascii(int x,int y,int size,char ch,u32 fg,u32 bg);
void rect(int x0,int y0,int x1,int y1,u32 body,u32 frame);
void backgroundcolor(u32);
u32 getrandom();




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
static char table[8][8] = {
	'a','b','c','d','e','f','g','h',
	'i','j','k','l','m','n','o','p',
	'q','r','s','t','u','v','w','x',
	'y','z',' ',' ',' ',' ',' ',' ',
	'0','1','2','3','4','5','6','7',
	'8','9',' ',' ',' ',' ',' ',' ',
	'+','-','*','/',' ',' ',' ',' ',
	' ',' ',' ',' ',' ',' ',' ',' '
};




static int virtkbd_write(u64* type,u64* value)
{
	int x,y;

	if(*type == 0x7466656C207A7978)
	{
		x = (*value) & 0xffff;
		y = ( (*value) >> 16 ) & 0xffff;
		if(y < (haha->height)*3/4)return 0;

		*type = 0x72616863;
		*value = 'x';
		return 1;
	}

	return 0;
}
static void virtkbd_read()
{
	int x,y;
	int width = haha->width;
	int height = haha->height;

	//[a,z]
	for(y=0;y<8;y++)
	{
		for(x=0;x<8;x++)
		{
			rect(
				x*width/8,
				(height*3/4) + (y*height/32),
				(x+1)*width/8,
				(height*3/4) + (y+1)*height/32,
				0xccffcc,
				0x352614
			);

			printascii(
				x*width/8,
				(height*3/4) + (y*height/32),
				2,
				table[y][x],
				0x221133,
				0
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
