#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void printascii(int x,int y,int size,char ch,DWORD fg,DWORD bg);
void rect(int x0,int y0,int x1,int y1,DWORD body,DWORD frame);
void backgroundcolor(DWORD);
DWORD getrandom();




static struct temp{
        QWORD type;
        QWORD id;
        QWORD start;
        QWORD end;

        QWORD pixelbuffer;
        QWORD pixelformat;
        QWORD width;
        QWORD height;
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




static void virtkbd_write(QWORD type,QWORD value)
{
	
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
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0;
	this[1]=0x64626b74726976;

	this[10]=(QWORD)virtkbd_start;
	this[11]=(QWORD)virtkbd_stop;
	this[12]=(QWORD)virtkbd_list;
	this[13]=(QWORD)virtkbd_into;
	this[14]=(QWORD)virtkbd_read;
	this[15]=(QWORD)virtkbd_write;
}
void virtkbd_delete()
{
}
