#include<math.h>
#include<stdio.h>
#define QWORD unsigned long long
#define BYTE unsigned char
#define pi 3.1415926535897932384626433832795023841971693993751058209749445923078
static double gausstable[10][10];




//(e^(-x*x/(2*sigma^2)))/(sigma*sqrt(2*pi))
void blur_gauss_x(QWORD size,QWORD radius,char* src,char* dst)
{
	int x,y;
	int m,n;
	int width,height;
	double sigma;
	double temp;
	double r,g,b;

	//由公式:d = 6σ + 1,可以得到σ = r / 3.
	if(radius>9)radius=9;
	sigma=(double)radius;
	sigma/=3;
	printf("radius=%d,sigma=%lf\n",radius,sigma);

	//first build the "gauss table"
	for(x=0;x<=radius;x++)
	{
		temp  = (double)(-x*x);
		temp /= 2*sigma*sigma;
		gausstable[0][x]  = exp(temp);
		gausstable[0][x] /= 2*pi*sigma*sigma;
		printf("gausstable[%d]=%lf\n",x,gausstable[0][x]);
	}

	//开始
	width=size&0xffff;
	height=(size>>16)&0xffff;
	for(y=radius;y<height-radius;y++)
	{
		for(x=radius;x<width-radius;x++)
		{
			//process this pixel
			r=g=b=0.0;

			//<0
			for(m=-radius;m<0;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*y + (x+m) )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*y + (x+m) )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*y + (x+m) )<<2)+2];
			}//m
			//>0
			for(m=0;m<radius;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*y + (x+m) )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*y + (x+m) )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*y + (x+m) )<<2)+2];
			}//m

			//put the result
			//printf("(%d,%d):%lf,%lf,%lf\n",x,y,b,g,r);
			dst[((width*y+x)<<2)+0]=(int)b;
			dst[((width*y+x)<<2)+1]=(int)g;
			dst[((width*y+x)<<2)+2]=(int)r;
		}//x
	}//y
}
void blur_gauss_y(QWORD size,QWORD radius,char* src,char* dst)
{
	int x,y;
	int m,n;
	int width,height;
	double sigma;
	double temp;
	double r,g,b;

	//由公式:d = 6σ + 1,可以得到σ = r / 3.
	if(radius>9)radius=9;
	sigma=(double)radius;
	sigma/=3;
	printf("radius=%d,sigma=%lf\n",radius,sigma);

	//first build the "gauss table"
	for(y=0;y<=radius;y++)
	{
		temp  = (double)(-y*y);
		temp /= 2*sigma*sigma;
		gausstable[0][y]  = exp(temp);
		gausstable[0][y] /= 2*pi*sigma*sigma;
		printf("gausstable[%d]=%lf\n",y,gausstable[0][y]);
	}

	//开始
	width=size&0xffff;
	height=(size>>16)&0xffff;
	for(y=radius;y<height-radius;y++)
	{
		for(x=radius;x<width-radius;x++)
		{
			//process this pixel
			r=g=b=0.0;

			//<0
			for(m=-radius;m<0;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*(y+m) + x )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*(y+m) + x )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*(y+m) + x )<<2)+2];
			}//m
			//>0
			for(m=0;m<radius;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*(y+m) + x )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*(y+m) + x )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*(y+m) + x )<<2)+2];
			}//m

			//put the result
			//printf("(%d,%d):%lf,%lf,%lf\n",x,y,b,g,r);
			dst[((width*y+x)<<2)+0]=(int)b;
			dst[((width*y+x)<<2)+1]=(int)g;
			dst[((width*y+x)<<2)+2]=(int)r;
		}//x
	}//y
	
}
void blur_gauss_xy(QWORD size,QWORD radius,char* src,char* dst)
{
	int x,y;
	int m,n;
	int width,height;
	double sigma;
	double temp;
	double r,g,b;

	//由公式:d = 6σ + 1,可以得到σ = r / 3.
	if(radius>9)radius=9;
	sigma=(double)radius;
	sigma/=3;
	printf("radius=%d,sigma=%lf\n",radius,sigma);

	//first build the "gauss table"
	for(y=0;y<=radius;y++)
	{
		temp  = (double)(-y*y);
		temp /= 2*sigma*sigma;
		gausstable[0][y]  = exp(temp);
		gausstable[0][y] /= 2*pi*sigma*sigma;
		printf("gausstable[%d]=%lf\n",y,gausstable[0][y]);
	}

	//开始(x)
	width=size&0xffff;
	height=(size>>16)&0xffff;
	for(y=radius;y<height-radius;y++)
	{
		for(x=radius;x<width-radius;x++)
		{
			//process this pixel
			r=g=b=0.0;

			//<0
			for(m=-radius;m<0;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*y + x+m )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*y + x+m )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*y + x+m )<<2)+2];
			}//m
			//>0
			for(m=0;m<radius;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*y + x+m )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*y + x+m )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*y + x+m )<<2)+2];
			}//m

			//<0
			for(m=-radius;m<0;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*(y+m) + x )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*(y+m) + x )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*(y+m) + x )<<2)+2];
			}//m
			//>0
			for(m=0;m<radius;m++)
			{
				//
				temp=gausstable[0][m];
				//b
				b+=temp*(double)src[ ( (width*(y+m) + x )<<2)+0];
				//g
				g+=temp*(double)src[ ( (width*(y+m) + x )<<2)+1];
				//r
				r+=temp*(double)src[ ( (width*(y+m) + x )<<2)+2];
			}//m

			//put the result
			//printf("(%d,%d):%lf,%lf,%lf\n",x,y,b,g,r);
			dst[((width*y+x)<<2)+0]=(int)b;
			dst[((width*y+x)<<2)+1]=(int)g;
			dst[((width*y+x)<<2)+2]=(int)r;
		}//x
	}//y

}
//(e^(-(x*x+y*y)/(2*sigma^2)))/(2*pi*sigma*sigma)
void blur_gauss_2(QWORD size,QWORD radius,char* src,char* dst)
{
	int x,y;
	int m,n;
	int width,height;
	double sigma;
	double temp;
	double r,g,b;

	//由公式:d = 6σ + 1,可以得到σ = r / 3.
	if(radius>9)radius=9;
	sigma=(double)radius;
	sigma/=3;
	printf("radius=%d,sigma=%lf\n",radius,sigma);

	//first build the "gauss table"
	for(y=0;y<=radius;y++)
	{
		for(x=0;x<=radius;x++)
		{
			temp  = (double)(-x*x-y*y);
			temp /= 2*sigma*sigma;
			gausstable[y][x]  = exp(temp);
			gausstable[y][x] /= 2*pi*sigma*sigma;
			printf("gausstable[%d][%d]=%lf\n",y,x,gausstable[y][x]);
		}
	}

	//开始
	width=size&0xffff;
	height=(size>>16)&0xffff;
	for(y=radius;y<height-radius;y++)
	{
		for(x=radius;x<width-radius;x++)
		{
			//process this pixel
			r=g=b=0.0;

			//<0
			for(n=-radius;n<0;n++)
			{
				//<0
				for(m=-radius;m<0;m++)
				{
					//
					temp=gausstable[n][m];
					//b
					b+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+0];
					//g
					g+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+1];
					//r
					r+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+2];
				}

				//>0
				for(m=0;m<radius;m++)
				{
					//
					temp=gausstable[n][m];
					//b
					b+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+0];
					//g
					g+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+1];
					//r
					r+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+2];
				}//m
			}//n

			//>0
			for(n=0;n<radius;n++)
			{
				//<0
				for(m=-radius;m<0;m++)
				{
					//
					temp=gausstable[n][m];
					//b
					b+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+0];
					//g
					g+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+1];
					//r
					r+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+2];
				}

				//>0
				for(m=0;m<radius;m++)
				{
					//
					temp=gausstable[n][m];
					//b
					b+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+0];
					//g
					g+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+1];
					//r
					r+=temp*(double)src[ ( (width*(y+n) + (x+m) )<<2)+2];
				}//m
			}//n

			//put the result
			//printf("(%d,%d):%lf,%lf,%lf\n",x,y,b,g,r);
			dst[((width*y+x)<<2)+0]=(int)b;
			dst[((width*y+x)<<2)+1]=(int)g;
			dst[((width*y+x)<<2)+2]=(int)r;
/*
			//compiler bug?
			//can not use "n=-radius;n<radius;n++"
			for(n=-radius;n<radius;n++)
			{
				for(m=-radius;m<radius;m++)
				{
					printf("(%d,%d)(%d,%d)\n",x,y,m,n);
				}
			}
*/
		}//x
	}//y
}
//均值
void blur_box(QWORD size,QWORD radius,char* src,char* dst)
{
	
}
