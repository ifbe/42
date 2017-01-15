#define u8 unsigned char 
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void fft(double* real, double* imag, int k);
void ifft(double* real, double* imag, int k);
int decstr2data(u8* src,u64* dst);
int hexstr2data(u8* src,u64* dst);
//
int startsound(int rate, int chan, void* buf, int len);
int stopsound();
int readsound(void* buf, int len);
int writesound(void* buf, int len);
//
void sleep_us(int);
void printmemory(void*, int);
void say(void*, ...);




//
static u16* pcmin;
static u16* pcmout;
static double* real;
static double* imag;




//
void sound_explain(u64* p)
{
}




//
static void sound_list()
{
}
static void sound_choose()
{
}
static void sound_read()
{
}
static void sound_write(char* buf)
{
	int j,k;
	u64 dat;
	if(buf == 0)dat=64;
	else
	{
		decstr2data(buf, &dat);
		dat=dat*1024/44100;
	}

	for(j=0;j<1024;j++)real[j]=imag[j]=0.0;
	real[dat]=real[1023-dat]=65535;

	ifft(real,imag,10);
	for(j=0;j<1024;j++)
	{
		real[j]*=32;
		if(real[j]>65535.0)pcmout[j]=65535;
		else pcmout[j]=(u16)real[j];
	}
	//printmemory(pcmout,2048);

	for(j=1;j<44100/1024;j++)
	{
		for(k=0;k<1024;k++)
		{
			pcmout[j*1024+k]=pcmout[k];
		}
	}

	writesound(pcmout, 44100*2);
}
static void sound_start()
{
	startsound(44100, 2, pcmin, 0x40000);
}
static void sound_stop()
{
	stopsound();
}
void sound_create(void* w, u64* p)
{
	p[0]=0x776f6c66;	//type
	p[1]=0x646e756f73;	//id

	p[10]=(u64)sound_start;
	p[11]=(u64)sound_stop;
	p[12]=(u64)sound_list;
	p[13]=(u64)sound_choose;
	p[14]=(u64)sound_read;
	p[15]=(u64)sound_write;

	pcmin = w + 0x300000;
	pcmout = w + 0x340000;
	real=(double*)(w+0x380000);
	imag=(double*)(w+0x3c0000);
}
void sound_delete()
{
}