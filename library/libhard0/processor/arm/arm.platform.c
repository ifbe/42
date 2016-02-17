#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void say(char* fmt , ...);
void printmemory(char*,int);




static int myownstr2data(char* source,QWORD* data)
{
	*data=0;
	int i;
	for(i=0;i<16;i++)
	{
		//say("%x\n",source[i]);
		//1.如果小于0x20，那就是结束了
		if(source[i]<=0x20) break;

		//2.如果大于0x80，那就返回错误
		if(source[i]>=0x80) return -1;

		//3.如果是正常值
		*data=(*data)<<4;
		if(source[i]>=0x61 && source[i]<=0x66)
		{
			*data+=source[i]-0x61+10;
		}
		else if(source[i]>=0x30 && source[i]<=0x39)
		{
			*data+=source[i]-0x30;
		}
		else return -2;
	}
	return -999999; //不会到这里
}
int platformread(char* arg)
{
	QWORD addr;

	//read memory
	addr = *(QWORD*)arg;
	addr &= 0xffffffffffff;
	if(addr==0x79726f6d656d)	//'memory'
	{
		if( arg[6] == '.' )	//memory.400000
		{
			myownstr2data(arg+7 , &addr);
			printmemory( (char*)addr , 0x200 );
			return 1;
		}
		else if( arg[6] == '[' )	//memory[from,to]
		{
			say("reading memory[?,?]\n");
			return 2;
		}
		return -100;
	}

	//read gpio
	addr = *(DWORD*)arg;
	if(addr==0x6f697067)
	{
		if( arg[4] == '.' )	//gpio.70
		{
			myownstr2data(arg+7 , &addr);
			say( "gpio.%x\n" , addr );
			return 3;
		}
		else if( arg[4] == '[' )	//gpio[cf8,cfb]
		{
			say("gpio[?,?]\n");
			return 4;
		}
		return -200;
	}

	return -10000;
}
int platformwrite(char* arg)
{
	QWORD addr,value;

	//write memory[x,y]=what
	value = *(QWORD*)arg;
	value &= 0xffffffffffffff;	//clear byte7 for cmp
	if(value==0x79726f6d656d)	//'memory'
	{
		myownstr2data(arg+7 , &addr);
		say("[%llx]\n" , addr);
		return 1;
	}

	return -10000;
}
