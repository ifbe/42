#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void yuyv2rgba(
	u8* src, int w1, int h1, 
	u8* dst, int w2, int h2
);
//
void startvision();
void stopvision();
//libboot
void printmemory(void*, int);
void say(void*, ...);




//
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
//
static u8* vision = 0;




static void camera_list()
{
}
static void camera_into()
{
}




void camera_read_text()
{
}
void camera_read_html()
{
}
void camera_read_pixel()
{
	int j;
	u8* screen = (u8*)haha->buffer;
	if(vision == 0)return;

	for(j=0;j<640*480;j++)vision[j*2]=256-vision[j*2];
	yuyv2rgba(
		vision, 640, 480,
		screen, haha->width, haha->height
	);
	vision = 0;
}
static void camera_read()
{
        u32 temp = (haha->format)&0xffffffff;

        //text
        if(temp == 0x74786574)
        {
                camera_read_text();
        }

        //html
        else if(temp == 0x6c6d7468)
        {
                camera_read_html();
        }

        //pixel
        else
        {
                camera_read_pixel();
        }
}
static void camera_write(u64* who, u64* a, u64* b)
{
	u64 type = *a;
	u64 key = *b;

	if(type==0x72616863)	//'char'
	{
	}
	else if(type=='v')
	{
		vision = (void*)key;
		printmemory(vision+0xfff, 16);
	}
}




static void camera_start()
{
	startvision();
}
static void camera_stop()
{
	stopvision();
}
void camera_create(void* base,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x6172656d6163;

	this[10]=(u64)camera_start;
	this[11]=(u64)camera_stop;
	this[12]=(u64)camera_list;
	this[13]=(u64)camera_into;
	this[14]=(u64)camera_read;
	this[15]=(u64)camera_write;
}
void camera_delete()
{
}
