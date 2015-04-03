#include<stdlib.h>

int width=640;
int height=640;
unsigned int* mypixel;

unsigned char* readbuf;		//读出来的数据存放的地址
unsigned char* dirbuf;		//存储翻译之后的(目录名,索引号)
unsigned char* fsbuf;		//文件系统读取代码需要的内部缓冲


__attribute__((constructor)) void initmemory()
{
	mypixel=(unsigned int*)malloc(width*height*4);
	readbuf=(unsigned char*)malloc(0x100000);
	dirbuf=(unsigned char*)malloc(0x100000);
	fsbuf=(unsigned char*)malloc(0x100000);
}
__attribute__((destructor)) void destorymemory()
{
	free(mypixel);
	free(readbuf);
	free(dirbuf);
	free(fsbuf);
}




void getaddrofbuffer(unsigned long long* p)
{
	*p=(unsigned long long)readbuf;
}
void getaddrofdir(unsigned long long* p)
{
	*p=(unsigned long long)dirbuf;
}
void getaddroffs(unsigned long long* p)
{
	*p=(unsigned long long)fsbuf;
}
void point(int x,int y,int color)
{
	mypixel[y*width+x]=color;
}
void draw(int x,int y,int color)
{
	point(x+(width/2),(height/2)-y-1,color);
}