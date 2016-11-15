#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int arteryprompt();
int arterycommand(void*);
//
void eventwrite(u64 why, u64 what, u64 where, u64 when);
u64* eventread();
int birth();
int death();
//
void say(char*,...);




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
		//1.显示位置
		arteryprompt();

		//2.等待输入
		addr = eventread();
		if(addr == 0)break;
		if(addr[1] == 0)break;
		arterycommand(addr);
	}

	//必须放在最后
	death();
	return 0;
}
