#include<stdlib.h>

int width=1024;
int height=640;
unsigned int* mypixel;


__attribute__((constructor)) void init2d()
{
	mypixel=(unsigned int*)malloc(width*height*4);
}
__attribute__((destructor)) void destory2d()
{
	free(mypixel);
}




void whereisscreenbuf(unsigned long long* p)
{
	*p=(unsigned long long)mypixel;
}


void point(int x,int y,int color)
{
	mypixel[y*width+x]=color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}