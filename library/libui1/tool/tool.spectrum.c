#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230
#define tau PI*2
//
void rectbody(int x1, int y1, int x2, int y2, u32 color);
void line(int x1, int y1, int x2, int y2, u32 color);
void backgroundcolor(u32);
//
void fft(double* real, double* imag, int k);
void ifft(double* real, double* imag, int k);
double squareroot(double);
double cosine(double);
double sine(double);
double log2(double);
double lg(double);
double ln(double);
//
int sound_output(double*, double*, u16*);
//
int startsound(int rate, int chan, void* buf, int max);
int stopsound();
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
static int maxpower;
static u16* pcmin;
static u16* pcmout;

//after
static double* real;
static double* imag;
static double* power;
static double* phase;

//
static u16 tone[8]={200,256,288,320,341,384,427,480};




//
void spectrum_random()
{
	int j;
	for(j=0;j<1024;j++)
	{
		real[j] = (double)pcmin[j] / 65536.0;
		imag[j] = 0.0;
	}

	fft(real, imag, 10);
	for(j=0;j<1024;j++)
	{
		//say("%lf	%lf\n", real[j], imag[j]);
		power[j]=squareroot(real[j]*real[j] + imag[j]*imag[j]) / 1024;
	}
	//say("%lf,%lf,%lf,%lf\n",power[0],power[1],power[2],power[3]);
}
static void spectrum_write(u64* who, u64* a, u64* b)
{
	int j;
	u64 type = *a;
	u64 key = *b;

	if(type==0x72616863)	//'char'
	{
		if(key<=0x30)return;
		if(key>=0x38)return;
		key -= 0x30;

		for(j=0;j<1024;j++)real[j]=imag[j]=0.0;
		j=tone[key]*1024/44100;
		real[j]=real[1023-j]=65535;
		sound_output(real, imag, pcmout);
	}
	else if(type==0x2d6d)
	{
		spectrum_random();
	}
	else if(type=='s')
	{
		spectrum_random();
	}
}




static void spectrum_read_pixel()
{
	int x,j,k;
	int width = haha->width;
	int height = haha->height;
	backgroundcolor(0);

	for(x=0;x<1024;x++)
	{
		j = pcmin[x] *height /maxpower /4;
		k = pcmin[x]*256/maxpower;
		if(k<0)k=-k;
		line(
			x*width/1024, (height/4) - j,
			x*width/1024, (height/4) + j,
			0x010101 * k
		);
	}
	for(x=0;x<512;x++)
	{
		j = (int)(power[x]*height);
		line(
			x*width/512, height - j,
			x*width/512, height,
			0xffffff
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
		y = h - (int)(real[x] * (double)h / (double)maxpower);
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




static void spectrum_list(u64* this)
{
}
static void spectrum_into()
{
}




void spectrum_start()
{
	int j;
	maxpower = 65536;
	backgroundcolor(0);

	//
	startsound(44100, 2, pcmin, 0x100000);

	//
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

	pcmin=(void*)(uibuf+0x200000);
	pcmout=(void*)(uibuf+0x280000);

	real=(double*)(uibuf+0x300000);
	imag=(double*)(uibuf+0x340000);
	power=(double*)(uibuf+0x380000);
	phase=(double*)(uibuf+0x3c0000);
}
void spectrum_delete()
{
}
