#include "libsoft.h"
int cleverread(u64, u64, u64, u8*, u64, u64);
int cleverwrite(u64, u64, u64, u8*, u64, u64);




//memory
static u8* fshome;
	static u8* pbr;		//+0x10000
	static u8* inodebuffer;	//+0x20000
static u8* dirhome;
static u8* datahome;

//disk
static u64 block0;
static u64 blocksize;
static u64 groupsize;
static u64 inodepergroup;
static u64 inodesize;
//
static u64 firstinodeincache;
static u8 blockrecord[512];
struct perfs{
	//@[1m,2m): todo: filenodes
	u8 datahome[0x100000];

	//@[512k,1m): todo: dirtree
	u8 dirhome[0x80000];

	//@[256k,512k)
	u8 fatbuffer[0x40000];

	//@[128k,256k)
	u8 pbrbuffer[0x20000];

}__attribute__((packed));




//输入值：请求的组号
//返回值：这个组里面inode表的第一个block号
static u64 whichblock(u64 groupnum)
{
	//group descriptor从哪个扇区开始
	u64 sector;
	if(blocksize==2) sector=block0+4;
	else sector=block0+blocksize;

	//从这个扇区起往后多少扇区
	sector+=groupnum/(0x200/0x20);

	//肯定在这个扇区里面
	//file_take(0, 0, "", sector*0x200, blockrecord, 0x200);

	//每0x20描述一个组，一个扇区有16个组的信息
	u8* addr=blockrecord+8+(groupnum*0x20)%0x200;
	return *(u32*)addr;
}




//文件->一家人，inode->户口本里的记录，这个函数->管理员
//这个函数作用：每次指名要一个记录，管理员就从户口本里翻到那儿([0x400n+1号,0x400n+1号])再指给请求者看
//比如0x3352号:先读入[0x3001,0x3400]这1024个记录
//再比如0x182304号:读入[0x182001,182400]这1024个记录
//注意1:不管inodesize是120(0x400*120)还是128(0x400*128)还是256(0x400*256)，算出的值都能被0x200(一个扇区)整除
//注意2:每个组的inode数量一般为8192，是0x400的倍数
//注意3:inode首个的编号是1不是0
static u8* checkcacheforinode(u64 wanted)
{
	//内存里已经是这几个的话就直接返回
	u64 inodeoffset=(wanted-1)%0x400;
	if(wanted-inodeoffset == firstinodeincache)
	{
		return inodebuffer+inodeoffset*inodesize;
	}


	//不是就麻烦了
	u8* rdi=inodebuffer;
	u64 this=wanted-inodeoffset;
	while(1)
	{
		//inode分散在各个group里面，可能需要从不同地方收集
		u64 groupnum=(this-1)/inodepergroup;		//算出此inode在第几块组
		u64 groupoffset=(this-1)%inodepergroup;	//在这一组内的偏移（几个）

		//算：哪个扇区,几个扇区
		u64 where;
		where=block0+blocksize*whichblock(groupnum);//这一组第一个inode所在扇区，不会出问题
		where+=groupoffset*inodesize/0x200;	//注意inodepergroup奇葩时这里出问题
		u64 count;
		count=inodepergroup-groupoffset;	//这一group内剩余多少个
		if(count>0x400)count=0x400;	//这一组里剩余的太多的话，多余的不要

		//read inode table
		//logtoall("inode:%x@%x\n",this,where);
		//注意inodepergroup奇葩时这里出问题
		//file_take(0, 0, "", where*0x200, rdi, count*inodesize*0x200);

		//读满0x400个inode就走人
		rdi+=count*inodesize;		//注意inodepergroup奇葩时这里出问题
		this+=count;
		if(rdi-inodebuffer>=0x40000) break;
	}
	firstinodeincache=wanted-inodeoffset;
	return inodebuffer+inodeoffset*inodesize;
}




//输入值：要搞的inode号，这个inode对应的文件内部偏移
//返回值：<=0就是失败
//作用：从offset开始能读多少读多少(1M以内)
static int explaininode(u64 inode,u64 wantwhere)
{
	//函数调用之后,rsi为所请求的inode的内存地址，
	u8* rsi=checkcacheforinode(inode);

	//检查是不是软链接
	u16 temp=(*(u16*)rsi)&0xf000;
	if(temp == 0xa000)
	{
		logtoall("soft link:%x\n",rsi+0x28);
		return -1;
	}

	if(*(u16*)(rsi+0x28) == 0xf30a)	//ext4用新方式
	{
		//读extend头，拿点重要的变量
		//*(u16*)(rsi+0x28)		//这是个标志，等于0xf30a说明用extend方式
		int numbers=*(u16*)(rsi+0x28+2);		//有效项个数
			//*(u16*)(rsi+0x28+4);		//项中的存储容量，4
			//*(u16*)(rsi+0x28+6);		//树的深度
			//*(u32*)(rsi+0x28+8);	//树的代数

		//为循环准备变量，清空内存，给读取的数据空间
		int i;
		u8* rdi;
		rsi=rsi+0x28;		//加完以后指向ext4_extend头
		for(rdi=datahome;rdi<datahome+0x100000;rdi++)
		{
			rdi[0] = 0;
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
			u64 aaaaa=( *(u32*)rsi )*blocksize*0x200;	//逻辑上位置
			if(wantwhere+0x100000<=aaaaa)break;		//结束了

			//总共多少个扇区
			u64 count=( *(u16*)(rsi+4) )*blocksize;		//多少个扇区
			if(aaaaa+count*0x200<=wantwhere)continue;	//还没到

			//实际上，从第几个扇区开始
			u64 sector=*(u16*)(rsi+6);	//从extent体里得到高16位实际块号
			sector=(sector<<32) + ( *(u32*)(rsi+8) );	//加上低32位实际块号
			sector*=blocksize;	//乘以每块多少扇区，现在sector=分区内偏移多少个扇区
			sector+=block0;		//加上分区相对硬盘开始多少个扇区

			//前三个；这一块的物理扇区号，扇区数，逻辑位置
			//后三个：目标位置，目标大小，需求位置
			cleverread
			(
				sector*0x200, count*0x200, aaaaa,
				datahome,0x100000,wantwhere
			);
		}

		return 1;
	}
	else		//ext2，3用老的直接间接块的方式
	{
	logtoall("old@%x\n",rsi);
/*
	rsi+=0x28;
	for(i=0;i<8;i++)
	{
		if(*(u32*)rsi != 0)
		{
			u64 temp;
			logtoall("    pointer@%x\n",rsi);

			temp=block0+(*(u32*)rsi)*blocksize;
			logtoall("sector:%x\n",temp);

			//file_take(0, temp*0x200, rdi, blocksize*0x200);
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
	int i;
	u8* rdi=dirhome;
	u8* rsi=datahome;

	for(i=0;i<0x100000;i++)
	{
		*(u8*)(rdi+i)=0;
	}
	while(1)
	{
		//几种情况结束的
		if(rdi>=dirhome+0x100000)break;
		if(rsi>=datahome+0x100000)break;
		if(*(u32*)rsi == 0)break;
		if(*(u16*)(rsi+4) == 0)break;
		//logtoall("%x\n",*(u16*)(rsi+4));
		//printmemory(rsi,0x10);

		//[0,0x7]:type
		u8 type=*(u8*)(rsi+7);
		if(type==1)
		{
			*(u64*)rdi=0x656c6966;	//'file'
		}
		else if(type==2)
		{
			*(u64*)rdi=0x726964;		//'dir'
		}
		else if(type==7)
		{
			*(u64*)rdi=0x6b6e696c;	//'link'
		}
		else
		{
			*(u64*)rdi=0x66+(type<<8);
		}

		//[0x10,0x17]inode号
		u64 thisinode=*(u32*)rsi;
		*(u64*)(rdi+0x10)=thisinode;

		//[0x18,0x1f]:size，ext的目录表里面没有文件大小，需要到inode表里面寻找
		u8* inodeaddr=checkcacheforinode(thisinode);
		*(u64*)(rdi+0x18)=*(u32*)(inodeaddr+4);

		//[0x20,0x3f]:名字
		i = *(u8*)(rsi+6);
		if(i>0x1f)
		{
			*(u8*)(rdi+0x3f)=0;
			*(u8*)(rdi+0x3e)='.';
			*(u8*)(rdi+0x3d)='.';
			*(u8*)(rdi+0x3c)='.';
			i=0x1b;
		}
		else
		{
			*(u8*)(rdi+0x20+i)=0;
			i--;
		}

		for(;i>=0;i--)
		{
			*(u8*)(rdi+0x20+i)=*(u8*)(rsi+8+i);
		}

		//最后指向下一个
		rsi+=*(u16*)(rsi+4);
		rdi+=0x40;
	}
}
int explainexthead(u8* pbr)
{
	//变量们
	blocksize=*(u32*)(pbr+0x418);
	blocksize=( 1<<(blocksize+10) )/0x200;		//每块多少扇区
	logtoall("sectorperblock:%x\n",blocksize);

	//每组多少扇区
	groupsize=*(u32*)(pbr+0x420);
	groupsize=groupsize*blocksize;
	logtoall("sectorpergroup:%x\n",groupsize);

	//每组多少个inode
	inodepergroup=*(u32*)(pbr+0x428);
	logtoall("inodepergroup:%x\n",inodepergroup);

	//每inode多少字节
	inodesize=*(u16*)(pbr+0x458);
	logtoall("byteperinode:%x\n",inodesize);

	return 1;
}
int check_ext(u8* addr)
{
	//0x53,0xef
	u64 temp=*(u16*)(addr+0x438);
	if( temp != 0xef53 ) return 0;

	//maybe ext
	return 4;
}




static int ext_list(u8* to)
{
	return 1;
}
static int ext_choose(u64 id)
{
	//开搞
	int ret;
	for(ret=0;ret<0x10000;ret++)dirhome[ret]=0;

	ret=explaininode(id,0);
	if(ret>0)explaindirectory();
	return 1;
}
static int ext_read(u64 id,u64 offset,u64 size)
{
	explaininode(id,offset);
	return 1;
}
static int ext_write(u64 id,u64 offset,u64 size)
{
	return 1;
}
static int ext_start(u64 sector)
{
	block0 = sector;

	return 0;
}




static int extclient_ontake(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int cmd, u8* buf, int len)
{
	logtoall("@extclient_ontake\n");
	return 0;
}
static int extclient_ongive(_obj* art,void* foot, _syn* stack,int sp, p64 arg, int idx, u8* buf, int len)
{
	return 0;
}
int extclient_attach(struct halfrel* self, struct halfrel* peer)
{
	logtoall("@extclient_attach\n");
	if(_src_ != self->foottype)return 0;

	_obj* art = self->pchip;
	if(0 == art)return 0;

	struct perfs* per = art->priv_ptr;
	if(0 == per)return 0;

	u8* pbr = per->pbrbuffer;
	int ret = take_data_from_peer(art,_src_, 0,0, 0,_pos_, pbr,0x1000);
	if(ret < 0x200){
		logtoall("fail@read:%d\n",ret);
		return 0;
	}

	//读分区前8扇区，检查magic值
	//ret = file_take(0, 0, "", block0*0x200, pbr, 0x1000);
	ret = check_ext(pbr);
	if(ret == 0)return -1;

	//读出关键数据
	ret = explainexthead(pbr);
	if(ret < 0)return ret;

/*
	//cd /
	firstinodeincache = 0xffffffff;
	ext_choose(2);
*/
	return 0;
}
int extclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




static int extclient_reader(_obj* art,int xxx, void* arg,int cmd, void* buf,int len)
{
	logtoall("@extclient_reader\n");
	return 0;
}
static int extclient_writer(_obj* art,int xxx, void* arg,int cmd, void* buf,int len)
{
	return 0;
}
int extclient_create(_obj* art)
{
	logtoall("@extclient_create\n");

	struct perfs* per = memoryalloc(0x200000, 0);
	art->priv_ptr = per;

	art->onreader = (void*)extclient_reader;
	art->onwriter = (void*)extclient_writer;
	art->ongiving = (void*)extclient_ongive;
	art->ontaking = (void*)extclient_ontake;
	return 0;
}
int extclient_delete(_obj* art)
{
	return 0;
}
