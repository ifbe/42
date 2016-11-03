//martin buttner
#include<stdio.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
double power(double,double);
void createwindow();
void deletewindow();
void writewindow(u64 size,void* addr);
void uievent(u64* type,u64* key);




//
static u32 palette[1024*1024];
void createppm(u64 size,u8* palette,char* filename)
{
	int x,y;
	FILE* fp=fopen(filename,"w");
	fprintf(fp,"P6\n%d %d\n255\n",1024,1024);
	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			fwrite((char*)palette+y*4096+x*4+2,1,1,fp);
			fwrite((char*)palette+y*4096+x*4+1,1,1,fp);
			fwrite((char*)palette+y*4096+x*4,1,1,fp);
		}
	}
	fclose(fp);
}




//
#define _sq(x) ((x)*(x))
unsigned char RED(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b+j*8e-9-.645411;
		a=c-d+i*8e-9+.356888;
	}
	return 255*power((n-80)/800,3.);
}
unsigned char GREEN(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b+j*8e-9-.645411;
		a=c-d+i*8e-9+.356888;
	}
	return 255*power((n-80)/800,.7);
}
unsigned char BLUE(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b+j*8e-9-.645411;
		a=c-d+i*8e-9+.356888;
	}
	return 255*power((n-80)/800,.5);
}





//
void main()
{
	//before
	int x,y;
	int r,g,b;
	createwindow();

	//picture
	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			r=RED(x,y)&0xff;
			g=GREEN(x,y)&0xff;
			b=BLUE(x,y)&0xff;
			palette[y*1024+x]=(b)+(g<<8)+(r<<16);
		}
	}
	createppm(0x04000400,(u8*)palette,"4.ppm");

	//forever
	u64 type=0;
	u64 key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		writewindow(0x04000400,palette);

		//2.等事件，是退出消息就退出
		uievent(&type,&key);
		if( type==0 )break;
		if( (type==0x64626b)&&(key==0x1b))break;
	}

	//after
	deletewindow();
}
