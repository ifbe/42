#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//soft.artery
int arteryprompt();
int arterycommand(void*);
void motion_explain(void*);
void network_explain(void*);
void sound_explain(void*);
void vision_explain(void*);
//boot
void printmemory(char*, int);
void say(char*, ...);
//event
u64* eventread();
void eventwrite(u64 why, u64 what, u64 where, u64 when);
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
		//1.界面显示
		arteryprompt();

again:
		//2.事件等待
		addr = eventread();
		if(addr == 0)break;		//error
		if(addr[1] == 0)break;		//exit


		//3.事件解释
		if((addr[1]&0xff) == 'p')	//touch
		{
			motion_explain(addr);
		}
		else if((addr[1]&0xff) == 'n')	//net
		{
			network_explain(addr);
		}
		else if((addr[1]&0xff) == 's')	//sound
		{
			sound_explain(addr);
		}
		else if((addr[1]&0xff) == 'v')	//vision
		{
			vision_explain(addr);
		}
		if(addr[1] != 0x64626b)goto again;


		//4.界面改变
		arterycommand(addr);
	}

	//必须放在最后
	death();
	return 0;
}
