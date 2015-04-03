#include <stdio.h>
#include <stdlib.h>


int width=640;
int height=640;
unsigned int* mypixel;


__attribute__((constructor)) void initpixel()
{
	mypixel=(unsigned int*)malloc(width*height*4);
}
__attribute__((destructor)) void destorypixel()
{
	free(mypixel);
}




//inner
//outer




void point(int x,int y,int color)
{
	mypixel[y*width+x]=color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}