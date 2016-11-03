//martin buttner
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void blur_gauss_x(u64 size,u64 radius,u8* src,u8* dst);
void blur_gauss_y(u64 size,u64 radius,u8* src,u8* dst);
void blur_gauss_xy(u64 size,u64 radius,u8* src,u8* dst);
void blur_gauss_2(u64 size,u64 radius,u8* src,u8* dst);
void blur_box(u64 size,u64 radius,u8* src,u8* dst);
double power(double,double);
//
void windowcreate();
void windowdelete();
void windowstart(void* addr,void* pixfmt, int width,int height);
void windowstop();
void windowread();
void windowwrite();
//
void eventwrite();
void eventread(u64*, u64*, u64*);




//
static u32 palette[1024*1024];
static u32 final[1024*1024];
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
void processmessage(u64 type,u64 key)
{
	int i;
	printf("%llx,%llx\n",type,key);

	if(type==0x72616863)             //'char'
	{
		//0
		if(key==0x30)
		{
			for(i=0;i<1024*1024;i++)
			{
				final[i]=palette[i];
			}
		}
		//1
		else if(key==0x31)
		{
			blur_gauss_x(0x04000400,9,(u8*)palette,(u8*)final);
		}
		//2
		else if(key==0x32)
		{
			blur_gauss_y(0x04000400,9,(u8*)palette,(u8*)final);
		}
		//3
		else if(key==0x33)
		{
			blur_gauss_xy(0x04000400,9,(u8*)palette,(u8*)final);
		}
		//4
		else if(key==0x34)
		{
			blur_gauss_2(0x04000400,9,(u8*)palette,(u8*)final);
		}
		//5
		else if(key==0x35)
		{
			blur_box(0x04000400,9,(u8*)palette,(u8*)final);
		}
	}
}
void main()
{
	//before
	int x,y;
	int r,g,b;
	char pixfmt[] = {"rgba8888"};

	//
	windowcreate();
	windowstart(final, pixfmt, 1024, 1024);

	//picture
	for(y=0;y<1024;y++)
	{
		for(x=0;x<1024;x++)
		{
			r=RED(x,y)&0xff;
			g=GREEN(x,y)&0xff;
			b=BLUE(x,y)&0xff;
			palette[y*1024+x] = 0xff000000 | (r<<16) | (g<<8) | b;
		}
	}
	processmessage(0x72616863,0x30);

	//forever
	u64 who;
	u64 type;
	u64 value;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		windowwrite();

		//2.等事件，是退出消息就退出
		eventread(&who, &type, &value);
		if( type==0 )break;
		if( (type==0x64626b)&&(value==0x1b))break;

		//3.处理事件
		processmessage(type, value);
	}

	//after
	windowstop();
	windowdelete();
}
