#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void rectbody(int x1, int y1, int x2, int y2, u32 color);
void line(int x1, int y1, int x2, int y2, u32 color);
void backgroundcolor(u32);
//
void fft(float* real, float* imag, float* sample, int k);
void ifft(float* real, float* imag, float* sample, int k);
u32 getrandom();
//
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
static float* databuf=0;
static int maxpower;

//after
static float* real;		//8*2048=0x4000
static float* imag;		//8*2048=0x4000
static float* power;		//8*1024=0x2000
static float* phase;		//8*1024=0x2000




//
void spectrum_random()
{
	int j;
	for(j=0;j<2048;j++)
	{
		real[j] = (float)(getrandom()%maxpower);
	}
}




static void spectrum_read_pixel()
{
	int x,y;
	int width = haha->width;
	int height = haha->height;

	rectbody(0, 0, width, height/2, 0);
	for(x=0;x<1024;x++)
	{
		y = (int)(real[x] * height / maxpower / 4);
		line(
			x*width/1024, (height/4) - y,
			x*width/1024, (height/4) + y,
			0xffffffff
		);
	}
}
static void spectrum_read_html()
{
}
static void spectrum_read_text()
{
	int x,y;
	int w = haha->width;
	int h = haha->height;
	u8* p = (u8*)(haha->pixelbuffer);

	for(x=0;x<w*h*4;x++)p[x]=0;
	for(x=0;x<w;x++)
	{
		y = h - (int)(real[x] * (float)h / (float)maxpower);
		for(;y<h;y++)
		{
			p[((y*w + x)<<2) + 3] =  0x2;
		}
	}
}
static void spectrum_read()
{
	u32 temp = (haha->pixelformat)&0xffffffff;

	//text
	if(temp == 0x74786574)
	{
		spectrum_read_text();
	}

	//html
	else if(temp == 0x6c6d7468)
	{
		spectrum_read_html();
	}

	//pixel
	else
	{
		spectrum_read_pixel();
	}
}
static void spectrum_write(u64* who, u64* a, u64* b)
{
	u64 type = *a;
	u64 key = *b;

	if(type==0x656c6966706f7264)		//'dropfile'
	{
	}
	if(type==0x64626b)			//'kbd'
	{
		spectrum_random();
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
	else if(type==0x2d6d)
	{
		spectrum_random();
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
	spectrum_random();
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

	databuf=(float*)(uibuf+0x200000);
	real=(float*)(uibuf+0x300000);
	imag=(float*)(uibuf+0x340000);
	power=(float*)(uibuf+0x380000);
	phase=(float*)(uibuf+0x3c0000);
}
void spectrum_delete()
{
}
