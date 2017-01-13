#define u8 unsigned char 
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
void fft(double* real, double* imag, int k);
void ifft(double* real, double* imag, int k);
//
int startsound(int rate, int chan, void* buf, int len);
int stopsound();
int readsound(void* buf, int len);
int writesound(void* buf, int len);
//
void sleep_us(int);
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
static void sound_write()
{
	int j;
	for(j=0;j<1024;j++)real[j]=imag[j]=0.0;
	real[64]=real[1023-64]=65535;
	real[128]=real[1023-128]=65535;
	real[256]=real[1023-256]=65535;

	ifft(real,imag,10);
	for(j=0;j<1024;j++)pcmout[j]=(u16)real[j];

	for(j=0;j<10*44100/1024;j++)
	{
		writesound(pcmout, 1024*2);
		sleep_us(20000);
	}
	say("3\n");
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