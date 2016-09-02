#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void rectbody(int x1, int y1, int x2, int y2, u32 color);
void backgroundcolor(u32);
void background1();
//
void arteryread(char* rdi,u64 rsi,u64 rcx);
void arterywrite(char* rdi,u64 rsi,u64 rcx);
void data2hexstring(u64,char*);
int compare(char*,char*);
//
unsigned int getrandom();
void printmemory(char*,int);
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

//before
static double* databuf=0;
static int maxpower;

//after
static double* real;		//8*2048=0x4000
static double* imag;		//8*2048=0x4000
static double* power;		//8*1024=0x2000
static double* phase;		//8*1024=0x2000




static void spectrum_write(u64* who, u64* a, u64* b)
{
	u64 type = *a;
	u64 key = *b;

	if(type==0x656c6966706f7264)		//'dropfile'
	{
	}
	if(type==0x64626b)			//'kbd'
	{
		if(key==0x25)			//left	0x4b
		{
		}
		else if(key==0x27)		//right	0x4d
		{
		}
		else if(key==0x26)		//up	0x4b
		{
		}
		else if(key==0x28)		//down	0x4d
		{
		}
	}
	else if(type==0x72616863)		//'char'
	{
		if(key==9)					//tab
		{
		}
		else if(key==0x8)			//backspace
		{
		}
		else if(key==0xd)			//enter
		{
		}
		else
		{
		}
	}
	else if(type==0x7466656C207A7978)		//'xyz left'
	{
		int i;
		for(i=0;i<2048;i++)
		{
			//real[i] = (double)(i*63);
			real[i] = (double)(getrandom()%maxpower);
		}
	}
	else if(type==0x6E6F7266207A7978)		//'xyz fron'
	{
	}
	else if(type==0x6B636162207A7978)		//'xyz back'
	{
	}
}
static void spectrum_read()
{
	int x,y;
	int width,height,min;

	width = haha->width;
	height = haha->height;
	if(width<height)min=width;
	else min=height;

	rectbody(0, 0, min, min, 0);
	for(x=0;x<1024;x++)
	{
		y = min - (int)(real[x] * (double)min / (double)maxpower);

		rectbody(x*min/1024, y, x*min/1024, min, 0xffffffff);
//say("%x,%x\n",leftupper,rightbottom);
	}
}




static void spectrum_list(u64* this)
{
}
static void spectrum_into()
{
}




void spectrum_start()
{
	int j;
	backgroundcolor(0);

	maxpower=32768;
	for(j=0;j<2048;j++)
	{
		real[j] = (double)(j*31);
		//real[j] = (double)(random()%maxpower);
	}
}
void spectrum_stop()
{
}
void spectrum_create(void* uibuf,void* addr)
{
	u64* this = (u64*)addr;
	haha = addr;

	this[0] = 0x6c6f6f74;
	this[1] = 0x6d75727463657073;

	this[10]=(u64)spectrum_start;
	this[11]=(u64)spectrum_stop;
	this[12]=(u64)spectrum_list;
	this[13]=(u64)spectrum_into;
	this[14]=(u64)spectrum_read;
	this[15]=(u64)spectrum_write;

	databuf=(double*)(uibuf+0x200000);
	real=(double*)(uibuf+0x300000);
	imag=(double*)(uibuf+0x340000);
	power=(double*)(uibuf+0x380000);
	phase=(double*)(uibuf+0x3c0000);
}
void spectrum_delete()
{
}
