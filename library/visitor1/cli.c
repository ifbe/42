#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int command(char* buffer);
int eventread(u64* who, u64* what, u64* how);
//
int enter(char*);
int leave();
int birth();
int death();
//
void say(char*,...);




int main(int argc,char* argv[])
{
	//必须放第一个
	int ret;
	u64 who;
	u64 what;
	u64 how;

	//一个个解释传进来的东西:in=xxxx out=xxx type=xxxx what=what
	birth();
	for(ret=1;ret<argc;ret++)
	{
		say("%s\n",argv[ret]);
		command(argv[ret]);
	}

	//无限循环
	while(1)
	{
		//debug
		//say("here\n");

		//1.等输入，再把这段里面所有的0x20变成0
		eventread(&who, &what, &how);
		if(what==0)break;
		if(what != 0x727473)continue;

		//2.处理输入
		ret=command((char*)how);
	}

	//必须放在最后
	death();
	return 0;
}
