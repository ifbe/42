#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int windowread();
int windowwrite(u64, u64, u64);
int eventread(u64* who, u64* what, u64* how);
int eventwrite(u64* who, u64* what, u64* how);
//
int birth();
int death();
//
void say(char*,...);




int main(int argc,char* argv[])
{
	int ret;
	u64 who, what, how;

	//必须放第一个
	birth();

	//一个个解释传进来的东西:in=xxxx out=xxx type=xxxx what=what
	for(ret=1;ret<argc;ret++)
	{
		say("%s\n",argv[ret]);
		windowwrite(0, 0x64626b, (u64)argv[ret]);
	}

	//无限循环
	how=0;
	while(1)
	{
		//1.显示位置
		windowread();

		//2.等待输入
		eventread(&who, &what, &how);
		if(what==0)break;

		//3.发送命令
		windowwrite(who, what, how);
	}

	//必须放在最后
	death();
	return 0;
}
