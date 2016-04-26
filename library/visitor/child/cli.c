#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
int command(char* buffer);
int waitevent(QWORD*,QWORD*);
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
	birth();

	//一个个解释传进来的东西:in=xxxx out=xxx type=xxxx what=what
	for(ret=1;ret<argc;ret++)
	{
		say("%s\n",argv[ret]);
		command(argv[ret]);
	}

	//无限循环
	QWORD first;
	QWORD second;
	while(1)
	{
		//debug
		//say("here\n");

		//1.等输入，再把这段里面所有的0x20变成0
		waitevent(&first,&second);
		if(first==0)break;
		if(first != 0x727473)continue;

		//2.处理输入
		ret=command((char*)second);
	}

	//必须放在最后
	death();
	return 0;
}
