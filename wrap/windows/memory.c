#include<stdlib.h>
unsigned char* readbuf;		//读出来的数据存放的地址
unsigned char* dirbuf;		//存储翻译之后的(目录名,索引号)
unsigned char* fsbuf;		//文件系统读取代码需要的内部缓冲
__attribute__((constructor)) void initmemory()
{
	readbuf=(unsigned char*)malloc(0x100000);
	dirbuf=(unsigned char*)malloc(0x100000);
	fsbuf=(unsigned char*)malloc(0x100000);
}
__attribute__((destructor)) void freememory()
{
	free(readbuf);
	free(dirbuf);
	free(fsbuf);
}








void whereisbuffer(unsigned long long* p)
{
	*p=(unsigned long long)readbuf;
}
void whereisdir(unsigned long long* p)
{
	*p=(unsigned long long)dirbuf;
}
void whereisfsbuf(unsigned long long* p)
{
	*p=(unsigned long long)fsbuf;
}
void whereislogbuf(unsigned long long* p)
{
	*p=(unsigned long long)logbuf;
}
void whereisscreenbuf(unsigned long long* p)
{
	*p=(unsigned long long)screenbuf;
}