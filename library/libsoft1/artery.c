#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//
void readfile(char* buf,QWORD sector,QWORD disk,DWORD count);
void readmemory(char* buf,QWORD sector,QWORD disk,DWORD count);
//
void initbinary(char*);
void initfilesystem(char*);




void rawread(char* buf,QWORD sector,QWORD disk,DWORD count)
{
	readfile(buf,sector,disk,count);
}
void rawwrite(char* buf,QWORD sector,QWORD disk,DWORD count)
{
}




void cleverread(QWORD sector,	QWORD count,	QWORD where,
		BYTE* destaddr,	QWORD destsize,	QWORD wantwhere)
{

        BYTE* rdi=0;    //关键:读到哪儿
        QWORD rsi=0;    //读哪号扇区
        QWORD rcx=0;    //读几个扇区

        //改rdi,rsi,rcx数值
        if(where<wantwhere)             //3和4
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

        rawread(rdi,rsi,0,rcx);
        //say("sector:%llx,count:%llx,where:%llx\n",sector,count,where);
        //say("want:%llx,to:%llx\n",wantwhere,destaddr);
        //say("rdi=%llx,rsi=%llx,rcx=%llx\n",rdi,rsi,rcx);
}
//来源物理扇区，来源扇区数量，来源代表哪里
//目的内存地址，目的字节数量，目的需要哪里
//               |[wantwhere,wantwhere+1m]|
//1:             |                        | [where,]    //自己就能排除
//2: [where,]    |                        |             //自己就能排除
//3:         [---|--where,--]             |             //传进来一块，前面不要
//4:         [---|--where,----------------|----]        //传进来一块，前后不要
//5:             |  [where,]              |             //传进来一块，全要
//6:             |  [---where,------------|----]        //传进来一块，后面不要
void cleverwrite()
{
}








//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void openartery(char* what)
{
	//选一个操作者
}
void closeartery()
{
}
void initartery(char* world)
{
	bin_init(world);	//1
        fs_init(world);		//2
}
void killartery()
{
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
