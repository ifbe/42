#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


static QWORD buffer0;
static QWORD readbuffer;




//用了别人的
void explaingpt(QWORD rdi,QWORD rsi);
void explainmbr(QWORD rdi,QWORD rsi);

void say(char* str,...);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void whereisrealworld(QWORD* in);
void whereislogicworld(QWORD* in);




void inithello()
{
	//现实世界在哪里
	whereisrealworld(&buffer0);
	whereislogicworld(&readbuffer);
}




//from:
//只知道是一堆数据
//to:					//[+0,+ffff],每个0x40,总共0x400个
//[0,7]  		startlba
//[8,f]  		endlba
//[10,17]		type
//[18,1f]		name
//[20,27]		cdfunc
//[28,2f]		loadfunc
//[30,37]		explainfunc
//[38,3f]		unused
void explainparttable(QWORD from,QWORD to)
{
	//清理
	int i;
	BYTE* memory=(BYTE*)(to);
	for(i=0;i<0x10000;i++) memory[i]=0;

	//看看是什么类型，交给小弟处理
	if(*(QWORD*)(from+0x200)==0x5452415020494645)
	{
		explaingpt(from,to);
	}
	else
	{
		explainmbr(from,to);
	}
}
void explainprocess()
{
	say("reading process...\n");
}
void hello()		//你究竟是个什么？
{
	//读最开始的64个扇区（0x8000字节）来初步确定
	readmemory(readbuffer,0,0,64);
		//读不出来，可能是内存？

	if( *(WORD*)(readbuffer+0x1fe) == 0xaa55 )
	{
		//末尾有0x55，0xaa这个标志，就当成磁盘
		say("it's disk\n");
		explainparttable(readbuffer,buffer0);
	}
	else
	{
		say("don't know\n");
	}
}