#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//用了别人的
void printmemory(QWORD addr,QWORD size);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void whereislogicworld(QWORD* in);
void holyshit(QWORD,QWORD,QWORD,QWORD,QWORD,QWORD);
void diary(char* fmt,...);




//memory
static QWORD diskhome;
static QWORD fshome;
	static QWORD inodebuffer;	//故意空格的
static QWORD dirhome;
static QWORD datahome;

//disk
static QWORD block0;
static QWORD blocksize;
static QWORD groupsize;
static QWORD inodepergroup;
static QWORD inodesize;




//输入值：请求的组号
//返回值：这个组里面inode表的第一个block号
static BYTE blockrecord[512];
static QWORD whichblock(QWORD groupnum)
{
	//group descriptor从哪个扇区开始
	QWORD sector;
	if(blocksize==2) sector=block0+4;
	else sector=block0+blocksize;

	//从这个扇区起往后多少扇区
	sector+=groupnum/(0x200/0x20);

	//肯定在这个扇区里面
	readmemory((QWORD)blockrecord,sector,0,1);

	//每0x20描述一个组，一个扇区有16个组的信息
	QWORD addr=(QWORD)blockrecord+8+(groupnum*0x20)%0x200;
	return *(DWORD*)addr;
}




//文件->一家人，inode->户口本里的记录，这个函数->管理员
//这个函数作用：每次指名要一个记录，管理员就从户口本里翻到那儿([0x400n+1号,0x400n+1号])再指给请求者看
//比如0x3352号:先读入[0x3001,0x3400]这1024个记录
//再比如0x182304号:读入[0x182001,182400]这1024个记录
//注意1:不管inodesize是120(0x400*120)还是128(0x400*128)还是256(0x400*256)，算出的值都能被0x200(一个扇区)整除
//注意2:每个组的inode数量一般为8192，是0x400的倍数
//注意3:inode首个的编号是1不是0
static QWORD firstinodeincache;
static QWORD checkcacheforinode(QWORD wanted)
{
	//内存里已经是这几个的话就直接返回
	QWORD inodeoffset=(wanted-1)%0x400;
	if(wanted-inodeoffset == firstinodeincache)
	{
		return inodebuffer+inodeoffset*inodesize;
	}


	//不是就麻烦了
	QWORD rdi=inodebuffer;
	QWORD this=wanted-inodeoffset;
	while(1)
	{
		//inode分散在各个group里面，可能需要从不同地方收集
		QWORD groupnum=(this-1)/inodepergroup;		//算出此inode在第几块组
		QWORD groupoffset=(this-1)%inodepergroup;			//在这一组内的偏移（几个）

		//算：哪个扇区,几个扇区
		QWORD where;
		where=block0+blocksize*whichblock(groupnum);	//这一组第一个inode所在扇区，不会出问题
		where+=groupoffset*inodesize/0x200;		//注意！！！！inodepergroup奇葩时这里出问题
		QWORD count;
		count=inodepergroup-groupoffset;	//这一group内剩余多少个
		if(count>0x400)count=0x400;			//这一组里剩余的太多的话，多余的不要

		//read inode table
		//diary("inode:%x@%x\n",this,where);
	    readmemory(rdi,where,0,count*inodesize/0x200);	//注意！！！！inodepergroup奇葩时这里出问题

		//读满0x400个inode就走人
		rdi+=count*inodesize;				//注意！！！！inodepergroup奇葩时这里出问题
		this+=count;
		if(rdi-inodebuffer>=0x40000) break;
	}
	//mem2file(inodebuffer,"after.bin",0,0x40000);
	firstinodeincache=wanted-inodeoffset;
	return inodebuffer+inodeoffset*inodesize;
}




//输入值：要搞的inode号，这个inode对应的文件内部偏移
//返回值：<=0就是失败
//作用：从offset开始能读多少读多少(1M以内)
static int explaininode(QWORD inode,QWORD wantwhere)
{
	//函数调用之后,rsi为所请求的inode的内存地址，
	QWORD rsi=checkcacheforinode(inode);

	//检查是不是软链接
	WORD temp=(*(WORD*)rsi)&0xf000;
	if(temp == 0xa000)
	{
		diary((char*)(rsi+0x28));
		diary("(soft link)\n");
		return -1;
	}

	if(*(WORD*)(rsi+0x28) == 0xf30a)	//ext4用新方式
	{
		//读extend头，拿点重要的变量
					//*(WORD*)(rsi+0x28)		//这是个标志，等于0xf30a说明用extend方式
		QWORD numbers=*(WORD*)(rsi+0x28+2);		//有效项个数
					//*(WORD*)(rsi+0x28+4);		//项中的存储容量，4
					//*(WORD*)(rsi+0x28+6);		//树的深度
					//*(DWORD*)(rsi+0x28+8);	//树的代数

		//为循环准备变量，清空内存，给读取的数据空间
		rsi=rsi+0x28;		//加完以后指向ext4_extend头
		int i;
		QWORD rdi;
		for(rdi=datahome;rdi<datahome+0x100000;rdi++)
		{
			*(BYTE*)rdi=0;
		}
		for(i=0;i<numbers;i++)
		{
			//ext4_extend体结构如下:
			//[0,3]第一个逻辑块
			//[4,5]数据块个数
			//[6,7]数据块高16位
			//[8,b]数据块低32位
			//每个ext4_extend体，大小为12B
			rsi+=12;

			//逻辑上，这是文件的第几块
			QWORD aaaaa=( *(DWORD*)rsi )*blocksize*0x200;		//逻辑上，这块的字节位置
			if(wantwhere+0x100000<=aaaaa)break;		//结束了

			//总共多少个扇区
			QWORD count=( *(WORD*)(rsi+4) )*blocksize;			//多少个扇区
			if(aaaaa+count*0x200<=wantwhere)continue;			//还没到

			//实际上，从第几个扇区开始
			QWORD sector=*(WORD*)(rsi+6);	//从extent体里得到高16位实际块号
			sector=(sector<<32) + ( *(DWORD*)(rsi+8) );	//加上低32位实际块号
			sector*=blocksize;	//乘以每块多少扇区，现在sector=分区内偏移多少个扇区
			sector+=block0;		//加上分区相对硬盘开始多少个扇区

			//蛋碎了，拼回来。。。传进去的参数为：
			//前三个；这一块的物理扇区号，扇区数，逻辑位置
			//后三个：目标位置，目标大小，需求位置
			holyshit
			(
				sector,count,aaaaa,
				datahome,0x80000,wantwhere
			);
		}

		return 1;
	}
	else		//ext2，3用老的直接间接块的方式
	{
	diary("old@%x\n",rsi);
	diary("{\n");
/*
	rsi+=0x28;
	for(i=0;i<8;i++)
	{
		if(*(DWORD*)rsi != 0)
		{
			QWORD temp;
			diary("    pointer@%x\n",rsi);

			temp=block0+(*(DWORD*)rsi)*blocksize;
			diary("sector:%x\n",temp);

		        readmemory(rdi,temp,0,blocksize);
			rdi+=0x200*blocksize;
		}

		rsi+=4;
	}
*/
	return 2;
	}
}


static void explaindirectory()
{
	QWORD rdi=dirhome;
	QWORD rsi=datahome;
	int i;

	for(i=0;i<0x100000;i++)
	{
		*(BYTE*)(rdi+i)=0;
	}
	while(1)
	{
		//几种情况结束的
		if(rdi>=dirhome+0x100000)break;
		if(rsi>=datahome+0x100000)break;
		if(*(DWORD*)rsi == 0)break;
		if(*(WORD*)(rsi+4) == 0)break;
		//diary("%x\n",*(WORD*)(rsi+4));
		//printmemory(rsi,0x10);

		//[0,7]inode号
		QWORD thisinode=*(DWORD*)rsi;
		*(QWORD*)(rdi+0)=thisinode;

		//[8,f]:size，ext的目录表里面没有文件大小，需要到inode表里面寻找
		QWORD inodeaddr=checkcacheforinode(thisinode);
		*(QWORD*)(rdi+8)=*(DWORD*)(inodeaddr+4);

		//[0x10,0x17]:type
		*(QWORD*)(rdi+0x10)=*(BYTE*)(rsi+7);

		//[0x20,0x3f]:名字
		i=0;
		for(i=0;i<*(BYTE*)(rsi+6);i++)	//namelength=*(byte*)(rsi+6)
		{
			*(BYTE*)(rdi+0x20+i)=*(BYTE*)(rsi+8+i);
		}

		//最后指向下一个
		rsi+=*(WORD*)(rsi+4);
		rdi+=0x40;
	}
}


static int ext_explain(QWORD inode)
{
	diary("inode %x\n",inode);

	//函数调用之后,rsi为所请求的inode的内存地址，
	QWORD rsi=checkcacheforinode(inode);

	//打印整个inode方便调试，这步可注释掉
	printmemory(rsi,inodesize);
}


static int ext_cd(QWORD id)
{
	//开搞
	int result=explaininode(id,0);
	if(result>0)explaindirectory();
	return 1;
}


static void ext_load(QWORD id,QWORD offset)
{
	explaininode(id,offset);
}








void explainexthead(QWORD pbr,QWORD fshome)
{
	//变量们
	blocksize=*(DWORD*)(pbr+0x418);
	*(QWORD*)(fshome)=0x418;
	*(QWORD*)(fshome+8)=0x41b;
	*(QWORD*)(fshome+0x10)=blocksize;
	blocksize=( 1<<(blocksize+10) )/0x200;		//每块多少扇区
	diary("sectorperblock:%x\n",blocksize);

	//每组多少扇区
	groupsize=*(DWORD*)(pbr+0x420);
	*(QWORD*)(fshome+0x40)=0x420;
	*(QWORD*)(fshome+0x48)=0x423;
	*(QWORD*)(fshome+0x50)=groupsize;
	groupsize=groupsize*blocksize;
	diary("sectorpergroup:%x\n",groupsize);

	//每组多少个inode
	inodepergroup=*(DWORD*)(pbr+0x428);
	*(QWORD*)(fshome+0x80)=0x428;
	*(QWORD*)(fshome+0x88)=0x42b;
	*(QWORD*)(fshome+0x90)=inodepergroup;
	diary("inodepergroup:%x\n",inodepergroup);

	//每inode多少字节
	inodesize=*(WORD*)(pbr+0x458);
	*(QWORD*)(fshome+0xc0)=0x458;
	*(QWORD*)(fshome+0xc8)=0x459;
	*(QWORD*)(fshome+0xd0)=inodesize;
	diary("byteperinode:%x\n",inodesize);
}
int mountext(QWORD addr,QWORD which)
{
	//得到本分区的开始扇区位置，再得到3个buffer的位置
	diskhome=addr;
	fshome=addr+0x100000;
		inodebuffer=fshome+0x10000;
	dirhome=addr+0x200000;
	datahome=addr+0x300000;

	//返回cd和load函数的地址
	block0=*(QWORD*)(diskhome+0x40*which+0);
	*(QWORD*)(diskhome+0x40*which+0x20)=(QWORD)ext_explain;
	*(QWORD*)(diskhome+0x40*which+0x28)=(QWORD)ext_cd;
	*(QWORD*)(diskhome+0x40*which+0x30)=(QWORD)ext_load;

	//读分区前8扇区，检查magic值
	readmemory(datahome,block0,0,0x8);	//0x1000
	if( *(WORD*)(datahome+0x438) != 0xef53 ) return -1;
	explainexthead(datahome,fshome);

	//cd /
	firstinodeincache=0xffffffff;
	ext_cd(2);

	return 0;
}
