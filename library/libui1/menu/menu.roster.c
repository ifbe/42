#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printstring(int x,int y,int size,char* str,u32 fg,u32 bg);
void printascii(int x,int y,int size,char ch,u32 fg,u32 bg);
void rect(int x0,int y0,int x1,int y1,u32 body,u32 frame);
u32 getrandom();




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




static void roster_write(u64* who, u64* what, u64* value)
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
	u64* this = (u64*)addr;
	haha = addr;

	this[0]=0;
	this[1]=0x726574736f72;

	this[10]=(u64)roster_start;
	this[11]=(u64)roster_stop;
	this[12]=(u64)roster_list;
	this[13]=(u64)roster_into;
	this[14]=(u64)roster_read;
	this[15]=(u64)roster_write;
}
void roster_delete()
{
}
