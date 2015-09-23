#include <windows.h>
#define QWORD unsigned long long




void diary(char* fmt,...);
void target(QWORD);




void explainarg()
{
	//拿到进程的输入arg,决定默认打开谁
	char* inputarg=GetCommandLine();
	diary("arg:%s\n",inputarg);

	//
	int i;
	int signal=0;
	while(1)
	{
		if(inputarg[i]==0)break;
		else if(inputarg[i]==0x22)
		{
			//记录引号的个数，引号出现在arg0
			signal++;
		}
		else if(inputarg[i]==0x20)
		{
			//碰到两次引号之后出现空格，要注意了不出意外就是后面一个！
			//要是多次空格保持不变0xff就行
			if(signal==2)signal=0xff;
		}
		else
		{
			//不是引号不是空格的正常字符 && 此时有信号
			//就能开干了
			if(signal==0xff)break;
		}

		i++;
	}
	if(inputarg[i]==0)
	{
		//"d:\code\file\a.exe"
		//比如上面这种，就默认打开扫描到的第一个磁盘
		target(0);
	}
	else
	{
		//"d:\code\file\a.exe" d:\code\1.txt
		//比如上面这种，就默认打开d:\code\1.txt
		target( (QWORD)(inputarg+i) );
	}
}