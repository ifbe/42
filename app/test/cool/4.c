//martin buttner
#include<stdio.h>
#include<math.h>
#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void initwindow();
void killwindow();
void writewindow(QWORD size,void* addr);
void uievent(QWORD* type,QWORD* key);




//
static DWORD palette[1024*1024];
void createppm(QWORD size,BYTE* palette,char* filename)
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
	return 255*pow((n-80)/800,3.);
}
unsigned char GREEN(int i,int j)
{
	
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b+j*8e-9-.645411;
		a=c-d+i*8e-9+.356888;
	}
	return 255*pow((n-80)/800,.7);
}
unsigned char BLUE(int i,int j)
{
	double a=0,b=0,c,d,n=0;
	while((c=a*a)+(d=b*b)<4&&n++<880)
	{
		b=2*a*b+j*8e-9-.645411;
		a=c-d+i*8e-9+.356888;
	}
	return 255*pow((n-80)/800,.5);
}





//
void main()
{
	//before
	int x,y;
	int r,g,b;
	initwindow();

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
	createppm(0x04000400,(BYTE*)palette,"4.ppm");

	//forever
	QWORD type=0;
	QWORD key=0;
	while(1)
	{
		//1.先在内存里画画，然后一次性写到窗口内
		writewindow(0x04000400,palette);

		//2.等事件，是退出消息就退出
		uievent(&type,&key);
		if( type==0 )break;
	}

	//after
	killwindow();
}
