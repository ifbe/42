#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int arteryprompt();
int arterycommand(void*);
//
void eventread(u64* what, u64* who, u64* where, u64* when);
void eventwrite(int);
int birth();
int death();
//
void say(char*,...);




int main(int argc,char* argv[])
{
	int ret;
	u64 what, who, where, when;

	//必须放第一个
	birth();

	//一个个解释传进来的东西:in=xxxx out=xxx type=xxxx what=what
	what = 0xd;
	for(ret=1;ret<argc;ret++)
	{
		arterycommand( argv[ret] );
		arterycommand( &what );
	}

	//无限循环
	what = 0;
	while(1)
	{
		//1.显示位置
		arteryprompt();

		//2.等待输入
		eventread(&what, &who, &where, &when);
		if(who==0)break;
		arterycommand(&what);
	}

	//必须放在最后
	death();
	return 0;
}
