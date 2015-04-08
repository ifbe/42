#include<stdlib.h>

int width=1024;
int height=640;
unsigned int* mypixel;
unsigned char* buf2d;


__attribute__((constructor)) void init2d()
{
	mypixel=(unsigned int*)malloc(width*height*4);
	buf2d=(unsigned char*)malloc(1024*1024*4);
}
__attribute__((destructor)) void destory2d()
{
	free(mypixel);
	free(buf2d);
}




void whereisscreenbuf(unsigned long long* p)
{
	*p=(unsigned long long)mypixel;
}
void whereisbuf2d(unsigned long long* p)
{
	*p=(unsigned long long)buf2d;
}


void point(int x,int y,int color)
{
	mypixel[y*width+x]=color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}