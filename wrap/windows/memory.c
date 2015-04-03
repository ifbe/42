#include<stdlib.h>
unsigned char* fsbuf;		//文件系统读取代码需要的内部缓冲
unsigned char* dirbuf;		//存储翻译之后的(目录名,索引号)
unsigned char* readbuf;		//读出来的数据存放的地址
__attribute__((constructor)) void initmemory()
{
	fsbuf=(unsigned char*)malloc(0x100000);
	dirbuf=(unsigned char*)malloc(0x100000);
	readbuf=(unsigned char*)malloc(0x100000);
}
__attribute__((destructor)) void freememory()
{
	free(fsbuf);
	free(dirbuf);
	free(readbuf);
}








void getaddroffs(unsigned long long* p)
{
	*p=(unsigned long long)fsbuf;
}
void getaddrofdir(unsigned long long* p)
{
	*p=(unsigned long long)dirbuf;
}
void getaddrofbuffer(unsigned long long* p)
{
	*p=(unsigned long long)readbuf;
}