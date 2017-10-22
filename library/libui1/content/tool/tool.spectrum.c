#include "actor.h"
#define PI 3.14159265358979323846264338327950288419716939937510582097494459230
#define tau PI*2
//libui1
void drawline(void*,
	int x1, int y1,
	int x2, int y2,
	u32 color);
//libsoft1
void fft(double* real, double* imag, int k);
void ifft(double* real, double* imag, int k);
double squareroot(double);
double cosine(double);
double sine(double);
double log2(double);
double lg(double);
double ln(double);
int sound_output(double*, double*, u16*);
int piano_freq(int);
//libsoft0
int startsound(int rate, int chan, void* buf, int max);
int stopsound();
u32 getrandom();




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
static int area=0;




static void spectrum_read_pixel(struct arena* win, struct actor* act, struct style* rel)
{
	double t,cc,ss;
	int x,y;
	int width = win->w;
	int height = win->h;
/*
	if(?????)
	{
		for(x=0;x<1024;x++)
		{
			if(pcmin[x]>32768)continue;
			y = pcmin[x] *height /maxpower /4;
			line(win,
				x*width/1024, (height/4) - y,
				x*width/1024, (height/4) + y,
				0xffffff
			);
		}
		return;
	}
*/
	for(x=0;x<512;x++)
	{
		t = x * tau /512.0;
		cc = cosine(t) * 256;
		ss = -sine(t) * 256;
		drawline(win,
			256 + (int)(cc * (1.0 - 2*power[x])),
			256 + (int)(ss * (1.0 - 2*power[x])),
			256 + (int)cc,
			256 + (int)ss,
			0xffffff
		);
	}
}
static void spectrum_read_html(struct arena* win, struct actor* act, struct style* rel)
{
}
static void spectrum_read_text(struct arena* win, struct actor* act, struct style* rel)
{
	int x,y;
	int w = win->w;
	int h = win->h;
	u8* p = (u8*)(win->buf);

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
static void spectrum_read_cli()
{
}
static void spectrum_read(struct arena* win, struct actor* act, struct style* rel)
{
	u64 fmt = win->fmt;

	//text
	if(fmt == 0x74786574)
	{
		spectrum_read_text(win, act, rel);
	}

	//html
	else if(fmt == 0x6c6d7468)
	{
		spectrum_read_html(win, act, rel);
	}

	//pixel
	else
	{
		spectrum_read_pixel(win, act, rel);
	}
}




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
static void spectrum_write(struct event* ev)
{
	int j;
	u64 type = ev->what;
	u64 key = ev->why;

	if(type==0x72616863)	//'char'
	{
		if( (key>=0x31) && (key<=0x39) )
		{
			key -= 0x31;
		}
		else if(key == '0')key = 9;
		else if(key == '-')key = 10;
		else if(key == '=')key = 11;
		else
		{
			if( (key>='a') && (key<='z') )
			{
				area = key - 'a';
			}
			return;
		}

		for(j=0;j<1024;j++)real[j]=imag[j]=0.0;

		j = piano_freq(area*12+key);
		say("%d->",j);
		j = (j*1024)/44100;
		say("%d\n",j);

		real[j]=real[1023-j]=65535;
		sound_output(real, imag, pcmout);
	}
	else if(type=='s')
	{
		//printmemory(pcmin, 16);
		spectrum_random();
	}
}




static void spectrum_list()
{
}
static void spectrum_into()
{
}
static void spectrum_start()
{
	int j;
	maxpower = 65536;

	//
	startsound(44100, 2, pcmin, 0x100000);
	spectrum_random();
}
static void spectrum_stop()
{
}
void spectrum_create(void* uibuf,void* addr)
{
	struct actor* p = addr;
	pcmin=(void*)(uibuf+0x200000);
	pcmout=(void*)(uibuf+0x280000);
	real=(double*)(uibuf+0x300000);
	imag=(double*)(uibuf+0x340000);
	power=(double*)(uibuf+0x380000);
	phase=(double*)(uibuf+0x3c0000);

	p->type = hex32('t', 'o', 'o', 'l');
	p->name = hex64('s', 'p', 'e', 'c', 't', 'r', 'u', 'm');

	p->start = (void*)spectrum_start;
	p->stop = (void*)spectrum_stop;
	p->list = (void*)spectrum_list;
	p->choose = (void*)spectrum_into;
	p->read = (void*)spectrum_read;
	p->write = (void*)spectrum_write;
}
void spectrum_delete()
{
}
