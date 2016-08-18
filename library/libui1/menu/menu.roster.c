#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void printstring(int x,int y,int size,char* str,DWORD fg,DWORD bg);
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




static void roster_write(QWORD type,QWORD value)
{
	
}
static void roster_read()
{
	int x,y;
	int width = haha->width;
	int height = haha->height;

	rect(
		0,	0,
		width/4,	height/4,
		0x554455,	0xff00ff
	);
	printstring(      0,	0,4,"game",0,0xffffff);

	rect(
		width/4,	0,
		width/2,	height/4,
		0x998899,	0xff00ff
	);
	printstring(width/4,	0,4,"test",0,0xffffff);

	rect(
		width/2,	0,
		width*3/4,	height/4,
		0x887788,	0xff00ff
	);
	printstring(width/2,	0,4,"tool",0,0xffffff);

	rect(
		width*3/4,	0,
		width,	height/4,
		0xccbbcc,	0xff00ff
	);
	printstring(width*3/4,	0,4,"haha",0,0xffffff);
}
static void roster_into()
{
}
static void roster_list()
{
}








static void roster_start()
{
}
static void roster_stop()
{
}
void roster_create(void* base,void* addr)
{
	QWORD* this = (QWORD*)addr;
	haha = addr;

	this[0]=0;
	this[1]=0x726574736f72;

	this[10]=(QWORD)roster_start;
	this[11]=(QWORD)roster_stop;
	this[12]=(QWORD)roster_list;
	this[13]=(QWORD)roster_into;
	this[14]=(QWORD)roster_read;
	this[15]=(QWORD)roster_write;
}
void roster_delete()
{
}
