#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//soft.net
int net_read(void*);
int net_write(void*);
//soft.artery
int arteryprompt();
int arterycommand(void*);
//boot
void printmemory(char*, int);
void say(char*, ...);
//event
u64* eventread();
void eventwrite(u64 why, u64 what, u64 where, u64 when);
//world
int birth();
int death();




int main(int argc,char* argv[])
{
	int ret;
	u64* addr;

	//必须放第一个
	birth();

	//一个个解释传进来的东西:in=xxxx out=xxx type=xxxx what=what
	for(ret=1;ret<argc;ret++)
	{
		arterycommand( argv[ret] );
		arterycommand( "\n" );
	}

	//无限循环
	while(1)
	{
		//1.世界显示
		arteryprompt();

		//2.等待输入
		addr = eventread();

		//3.特殊的自己处理
		if(addr == 0)break;		//错误
		if(addr[1] == 0)break;		//退出
		if((addr[1]&0xff) == 'n')	//网络
		{
			net_read(addr);
			continue;
		}

		//4.世界改变
		arterycommand(addr);
	}

	//必须放在最后
	death();
	return 0;
}
