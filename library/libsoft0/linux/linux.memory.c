#include<unistd.h>
#include<fcntl.h>
void say(char* fmt, ...);
static int _dev_mem=-1;
static unsigned long long currentmmap=0xffffffffffffffff;




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




void openmemory(char* what)
{
	//假装"打开多个"
	//_dev_mem[i]=open(what, O_RDWR|O_SYNC);
}
void closememory()
{
	//关掉一个
	//close(_dev_mem[i]);
}
void initmemory()
{
	_dev_mem=open("/dev/mem", O_RDWR|O_SYNC);
	if(_dev_mem==-1)
	{
		say("(failed,ignored)open /dev/mem\n");
	}
}
void killmemory()
{
	if(_dev_mem != -1)
	{
		close(_dev_mem);
	}
}
