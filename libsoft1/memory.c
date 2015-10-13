#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


void diary(char* fmt,...);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);




void cleanmemory(QWORD addr,QWORD size)
{
	//清理
	BYTE* memory=(BYTE*)addr;
	int i=0;
	for(i=0;i<size;i++) memory[i]=0;
}




//debug用，打印从addr开始的总共size个字节
void printmemory(QWORD addr,QWORD size)
{
	BYTE* printaddr=(BYTE*)addr;
	int i,j;

	//打印的很少就不显示这些了
	if(size>0x10)
	{
		diary("[----addr----]");
		for(i=0;i<=0xf;i++)
		{
			diary("%2x ",i);
		}
		diary("[----anscii----]\n");
	}

	//2222222222222222222222222
	for(j=0;j<size/16;j++)
	{
		if(j%16 == 0)diary("@%-13llx",addr+j*16);
		else diary("+%-13x",j*16);

		for(i=0;i<=0xf;i++)
		{
			diary("%.2x ",printaddr[16*j+i]);
		}
		for(i=0;i<=0xf;i++)
		{
			unsigned char ch=printaddr[16*j+i];
			if( (ch>=0x80)|(ch<=0x20) )ch=0x20;
			diary("%c",ch);
		}
		diary("\n",printaddr[16*j+15]);
	}
}




//               |[wantwhere,wantwhere+1m]|
//1:             |                        | [where,]  //自己就能排除，不要麻烦这个函数
//2: [where,]    |                        |           //自己就能排除，不要麻烦这个函数
//3:         [---|--where,--]             |           //传进来一块，前面不要
//4:         [---|--where,----------------|----]      //传进来一块，前面不要，后面也不要
//5:             |  [where,]              |           //传进来一块，全要
//6:             |  [---where,------------|----]      //传进来一块，后面不要
//来源物理扇区，来源扇区数量，来源代表哪里
//目的内存地址，目的字节数量，目的需要哪里
void holyshit(QWORD sector,QWORD count,QWORD where,QWORD wantwhere,QWORD towhere)
{
	//关键:读到哪儿，读哪号扇区，读几个扇区
	QWORD rdi=0;
	QWORD rsi=0;
	QWORD rcx=0;

	//改rdi,rsi,rcx数值
	if(where<wantwhere)		//3和4
	{
		rdi=towhere;
		rsi=sector+(wantwhere-where)/0x200;
		if(where+count*0x200<=wantwhere+0x100000)
		{
			rcx=count-(wantwhere-where)/0x200;
		}
		else
		{
			rcx=0x100000/0x200;
		}
	}
	else
	{
		rdi=towhere+(where-wantwhere);
		rsi=sector;
		if(where+count*0x200<=wantwhere+0x100000)
		{
			rcx=count;
		}
		else
		{
			rcx=(wantwhere+0x100000-where)/0x200;
		}
	}

	readmemory(rdi,rsi,0,rcx);
	//diary("sector:%llx,count:%llx,where:%llx\n",sector,count,where);
	//diary("want:%llx,to:%llx\n",wantwhere,towhere);
	//diary("rdi=%llx,rsi=%llx,rcx=%llx\n",rdi,rsi,rcx);
}
