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




static void keyboard_write(QWORD type,QWORD value)
{
	
}
static void keyboard_read()
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
static void keyboard_into()
{
}
static void keyboard_list()
{
}








static void keyboard_start()
{
}
static void keyboard_stop()
{
}
void keyboard_init(void* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0;
	this[1]=0x64626b;

	this[10]=(QWORD)keyboard_start;
	this[11]=(QWORD)keyboard_stop;
	this[12]=(QWORD)keyboard_list;
	this[13]=(QWORD)keyboard_into;
	this[14]=(QWORD)keyboard_read;
	this[15]=(QWORD)keyboard_write;
}
void keyboard_kill()
{
}
