#include<actor.h>
u32 getrandom();
double squareroot(double);
double sine(double);
double cosine(double);
double tangent(double);
double arcsin(double);
double arccos(double);
double arctan2(double, double);
double power(double, double);
double ln(double);




void bgcolor(struct arena* win, u32 color)
{
	int x;
	u32* buf = (u32*)(win->buf);
	color |= 0xff000000;

	for(x=0; x<(win->w)*(win->h); x++)
	{
		buf[x] = color;
	}
}
void background1(struct arena* win)
{
	int x,y;
	int width = win->w;
	int height = win->h;
	u32* buf = (u32*)(win->buf);

	//用指定颜色清屏
	for(x=0;x<width*height;x++)
	{
		buf[x]=0xfff0f0f0;
	}

	//上下
	for(y=0;y<16;y++)
	{
		u32 color=0xff404040+(0x0b0b0b*y);

		//上，编译器会不会用rep stosd指令优化呢?
		u32* p = buf+y*width;
		for(x=y;x<width-y;x++)p[x]=color;

		//下
		p = buf+(height-1-y)*width;
		for(x=y;x<width-y;x++)p[x]=color;
	}
	//左右
	for(x=0;x<16;x++)
	{
		u32 color=0xff404040+(0x0b0b0b*x);

		for(y=x;y<height-x;y++)
		{
			buf[(y*width)+x]=color;
			buf[(y*width)+width-1-x]=color;
		}
	}
}



#define DIM 1024
#define _sq(x) ((x)*(x))
#define r(n)(getrandom()%n)
unsigned char RED1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2))*255);
}
unsigned char GREEN1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2-2*arccos(-1)/3))*255);
}
unsigned char BLUE1(int i,int j)
{
	return (char)(_sq(cosine(arctan2(j-512,i-512)/2+2*arccos(-1)/3))*255);
}
unsigned char RED2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):RED2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char GREEN2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):GREEN2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char BLUE2(int i,int j)
{
	static char c[1024][1024];
	return!c[i][j]?c[i][j]=!r(999)?r(256):BLUE2((i+r(2))%1024,(j+r(2))%1024):c[i][j];
}
unsigned char RED3(int i,int j)
{
	float x=0,y=0;int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return ln(k)*47;
}
unsigned char GREEN3(int i,int j)
{
	float x=0,y=0;
	int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return ln(k)*47;
}
unsigned char BLUE3(int i,int j)
{
	float x=0,y=0;int k;
	for(k=0;k++<256;)
	{
		float a=x*x-y*y+(i-768.0)/512;
		y=2*x*y+(j-512.0)/512;
		x=a;
		if(x*x+y*y>4)break;
	}
	return 128-ln(k)*23;
}
unsigned char RED4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*((n-80)/800)*((n-80)/800)*((n-80)/800);
}
unsigned char GREEN4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*power((n-80)/800,0.7);
}
unsigned char BLUE4(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b + j*8e-9 - 0.645411;
		a=c - d + i*8e-9 + 0.356888;
	}
	return 255*squareroot((n-80)/800);
}
unsigned char RED6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)((i+DIM)*s+y)%2+(int)((DIM*2-i)*s+y)%2)*127;
}
unsigned char GREEN6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)(5*((i+DIM)*s+y))%2+(int)(5*((DIM*2-i)*s+y))%2)*127;
}
unsigned char BLUE6(int i,int j)
{
	float s=3./(j+99);
	float y=(j+sine((i*i+_sq(j-700)*5)/100./DIM)*35)*s;
	return ((int)(29*((i+DIM)*s+y))%2+(int)(29*((DIM*2-i)*s+y))%2)*127;
}
void drawhaha1(struct arena* win)
{
	int x,y;
	int rr,gg,bb;
	int w = win->w;
	int h = win->h;
	u32* buf = (u32*)(win->buf);

	for(y=0;y<w;y++)
	{
		for(x=0;x<w;x++)
		{
			rr = RED4(x,y);
			gg = GREEN4(x,y);
			bb = BLUE4(x,y);
			buf[(y*w)+x] = 0xff000000 + (rr<<16) + (gg<<8) + bb;
		}
	}
}