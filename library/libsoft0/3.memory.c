#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




void listmemory()				//ls?
{
	filelist();
}
void intomemory(char* what)		//cd
{
	filetarget(what);
}
void cleanmemory(QWORD addr,QWORD size)
{
	//清理
	BYTE* memory=(BYTE*)addr;
	int i=0;
	for(i=0;i<size;i++) memory[i]=0;
}




void writememory()
{
	//if memory
	//

	//if file/disk
	//

	//if(process)
	//

	//if socket
	//
}
void readmemory(QWORD buf,QWORD sector,QWORD ignore,DWORD count)		//read?
{
	//if(memory)
	//

	//if(disk/file)
	fileread(buf,sector,0,count);

	//if(process)
	//

	//if(socket)
	//
}
void holyshit(
	QWORD   sector, QWORD count,    QWORD where,			//来源物理扇区，来源扇区数量，来源代表哪里
	QWORD destaddr, QWORD destsize, QWORD wantwhere)		//目的内存地址，目的字节数量，目的需要哪里
{
	//               |[wantwhere,wantwhere+1m]|
	//1:             |                        | [where,]	//自己就能排除，不要麻烦这个函数
	//2: [where,]    |                        |         	//自己就能排除，不要麻烦这个函数
	//3:         [---|--where,--]             |         	//传进来一块，前面不要
	//4:         [---|--where,----------------|----]    	//传进来一块，前面不要，后面也不要
	//5:             |  [where,]              |         	//传进来一块，全要
	//6:             |  [---where,------------|----]    	//传进来一块，后面不要
	QWORD rdi=0;	//关键:读到哪儿
	QWORD rsi=0;	//读哪号扇区
	QWORD rcx=0;	//读几个扇区

	//改rdi,rsi,rcx数值
	if(where<wantwhere)		//3和4
	{
		rdi=destaddr;
		rsi=sector+(wantwhere-where)/0x200;
		if(where+count*0x200<=wantwhere+destsize)
		{
			rcx=count-(wantwhere-where)/0x200;
		}
		else
		{
			rcx=destsize/0x200;
		}
	}
	else
	{
		rdi=destaddr+(where-wantwhere);
		rsi=sector;
		if(where+count*0x200<=wantwhere+destsize)
		{
			rcx=count;
		}
		else
		{
			rcx=(wantwhere+destsize-where)/0x200;
		}
	}

	readmemory(rdi,rsi,0,rcx);
	//diary("sector:%llx,count:%llx,where:%llx\n",sector,count,where);
	//diary("want:%llx,to:%llx\n",wantwhere,destaddr);
	//diary("rdi=%llx,rsi=%llx,rcx=%llx\n",rdi,rsi,rcx);
}




initmemory()
{
	//拿到文件名
	p=explainarg();
	intomemory( p );
}