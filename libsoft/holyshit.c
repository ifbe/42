#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


void diary(char* fmt,...);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);




//------------------粗糙的打比方--------------------
//一个村庄（每个文件）
//被拆迁（文件系统处理）
//村里各户人家（组成文件那些字节字节字节字节字节...）
//分散到各种地方（硬盘上分区内第几扇区到第几扇区）
//调用这个蛋碎一地函数（这块实际上在哪儿，这块总共多少个扇区，这块逻辑上是哪儿，你要哪儿）
//就是跟老村联络员说（王二家现在住哪，总共多少人，王二拆迁前住在哪，要找村里哪几户人家）

//-------------------详细的抽象---------------------
//20厘米的尺子第3到11厘米涂上颜色，被不同大小得掰断成好几份
//分别是[0,2],[2,4],[4,8],[8,10],[10,14],[14,20]
//现在要把涂色的地方摆在(规定的地方)，没涂色的部分扔掉不管
//因为这把尺子每个碎块上面都有刻度，所以从0开始可以排好队等着搞
//[0,2],[14,20]:这两块都不要不管
//[2,4]:切出[3,4],放在(规定的地方),总共1cm
//[4,8]:全部放在(规定的地方+1cm),总共4cm
//[8,10]:全部放在(规定的地方+4cm),总共2cm
//[10,14]:切出[10,11],放在(规定的地方+7cm),总共1cm

//--------------------画图说事--------------------
//               |[wantwhere,wantwhere+1m]|
//1:             |                        | [where,]  //自己就能排除，不要麻烦这个函数
//2: [where,]    |                        |           //自己就能排除，不要麻烦这个函数
//3:         [---|--where,--]             |           //传进来一块，前面不要
//4:         [---|--where,----------------|----]      //传进来一块，前面不要，后面也不要
//5:             |  [where,]              |           //传进来一块，全要
//6:             |  [---where,------------|----]      //传进来一块，后面不要

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
	diary("sector:%llx,count:%llx,where:%llx\n",sector,count,where);
	diary("want:%llx,to:%llx\n",wantwhere,towhere);
	diary("rdi=%llx,rsi=%llx,rcx=%llx\n",rdi,rsi,rcx);
}
