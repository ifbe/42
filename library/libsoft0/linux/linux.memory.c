#include<unistd.h>
#include<fcntl.h>
static int _dev_mem=-1;
static unsigned long long currentmmap;




void listmemory()
{
	//列出物理地址对应的设备：memory/mmiodev/reserved
}
void intomemory()
{
	//换位置
}
void readmemory(char* addr,int count)
{
}
void writememory(char* addr,int count)
{
}




void openmemory()
{
	//假装"打开多个"
}
void closememory()
{
	//关掉一个
}
void initmemory()
{
	_dev_mem=open("/dev/mem", O_RDWR|O_SYNC);
}
void killmemory()
{
	close(_dev_mem);
}
