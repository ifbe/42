#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
int command(char* buffer);
int initall();
int cleanall();
//
int waitinput(char*);
void say(char*,...);




static char buffer[128];
int main(int argc,char* argv[])
{
	//必须放第一个
	int ret;
	initall();

	//无限循环
	while(1)
	{
		//1.等输入，再把这段里面所有的0x20变成0
		waitinput(buffer);

		//2.处理输入
		ret=command(buffer);
		if(ret==0)break;
	}

	//必须放在最后
	cleanall();
	return 0;
}
