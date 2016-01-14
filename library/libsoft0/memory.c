#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//socket
void listsocket();
void intosocket();
void readsocket();
void writesocket();
//process
void listprocess();
void intoprocess();
void readprocess();
void writeprocess();
//file
void listfile(char* towhere);
void intofile(char* name);
void readfile(QWORD buf,QWORD sector,QWORD ignore,DWORD count);
void writefile(QWORD buf,QWORD sector,QWORD ignore,DWORD count);
void killfile();




//这个.c负责管理现在开了哪些东西
void initmemory(char* addr)
{
	initfile();
	initother();
}
void killmemory()
{
	killother();
	killfile();
}




void cleanmemory(char* addr,int size)
{
	//清理
	int i=0;
	for(i=0;i<size;i++) addr[i]=0;
}
void copymemory(char* src,char* dest,int size)
{
	int i;
	for(i=0;i<size;i++)
	{
		dest[i]=src[i];
	}
}




void listmemory(char* addr)				//ls?
{
	listfile(addr);
	//+memoryinfo
	//+listprocess
}
void intomemory(char* what)		//cd
{
	intofile(what);
}




void readmemory(QWORD buf,QWORD sector,QWORD ignore,DWORD count)		//read?
{
	ignore=ignore;		//kill compiler
	//if(memory)
	//

	//if(disk/file)
	readfile(buf,sector,0,count);

	//if(process)
	//

	//if(socket)
	//
}
void cleverread
(
	QWORD   sector, QWORD count,    QWORD where,
	QWORD destaddr, QWORD destsize, QWORD wantwhere
)
{
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
//来源物理扇区，来源扇区数量，来源代表哪里
//目的内存地址，目的字节数量，目的需要哪里
//               |[wantwhere,wantwhere+1m]|
//1:             |                        | [where,]	//自己就能排除
//2: [where,]    |                        |         	//自己就能排除
//3:         [---|--where,--]             |         	//传进来一块，前面不要
//4:         [---|--where,----------------|----]    	//传进来一块，前后不要
//5:             |  [where,]              |         	//传进来一块，全要
//6:             |  [---where,------------|----]    	//传进来一块，后面不要








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
void cleverwrite()
{
	
}
