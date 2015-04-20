#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//from:
//只知道是一堆数据


//to:					[+0,+ffff],每个0x40,总共0x400个
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