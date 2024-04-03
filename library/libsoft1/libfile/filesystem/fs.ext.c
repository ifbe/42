#include "libsoft.h"




void ext_cleverread(_obj* art,
	u64 src, u64 count, u64 where,
	u8* buf, u64 len, u64 want)
{
	u8* rdi = 0;    //内存地址
	u64 rsi = 0;    //扇区号
	u64 rcx = 0;    //扇区数量

	//改rdi,rsi,rcx数值
	if(where<want)	     //3和4
	{
		rdi = buf;
		rsi = src+(want-where);
		if(where+count <= want+len)
		{
			rcx = count-(want-where);
		}
		else
		{
			rcx = len;
		}
	}
	else
	{
		rdi = buf+(where-want);
		rsi = src;
		if(where+count <= want+len)
		{
			rcx = count;
		}
		else
		{
			rcx = want+len-where;
		}
	}
/*
	logtoall(
		"(%llx,%llx,%llx)->(%llx,%llx,%llx)\n",
		src,count,where,    buf,len,want
	);
	logtoall(
		"rdi=%llx,rsi=%llx,rcx=%llx\n",
		rdi,rsi,rcx
	);
*/
	//readfile(0, 0, "", rsi, rdi, rcx);
	take_data_from_peer(art,_src_, 0,0, rsi,_pos_, rdi, rcx);
}



/*
//memory
static u8* fshome;
	static u8* pbr;		//+0x10000
	static u8* inodebuffer;	//+0x20000
static u8* dirhome;
static u8* datahome;
*/
//
#define datahomesize 0x100000
#define dirhomesize 0x80000
#define inodebuffersize 0x40000
#define pbrbuffersize 0x20000
#define blockrecordsize 512
struct perfs{
	//@[1m,2m): todo: filenodes
	u8 datahome[datahomesize];

	//@[512k,1m): todo: dirtree
	u8 dirhome[dirhomesize];

	//@[256k,512k)
	u8 inodebuffer[inodebuffersize];

	//@[128k,256k)
	u8 pbrbuffer[pbrbuffersize];

	//cached
	u8 blockrecord[blockrecordsize];
	u64 firstinodeincache;

	//info
	u64 block0;
	u64 blocksize;
	u64 groupsize;
	u64 inodepergroup;
	u64 byteperinode;
}__attribute__((packed));




//输入值：请求的组号
//返回值：这个组里面inode表的第一个block号
static u64 whichblock(_obj* art, u64 groupnum)
{
	struct perfs* per = art->priv_ptr;

	//group descriptor从哪个扇区开始
	u64 sector;
	if(per->blocksize==2)sector = per->block0+4;
	else sector = per->block0 + per->blocksize;
	sector += groupnum/(0x200/0x20);
	logtoall("whichblock: sector=%llx\n", sector);

	//肯定在这个扇区里面
	//file_take(0, 0, "", sector*0x200, blockrecord, 0x200);
	take_data_from_peer(art,_src_, 0,0, sector*0x200,_pos_, per->blockrecord, 0x200);
	//printmemory(per->blockrecord, 0x200);

	//每0x20描述一个组，一个扇区有16个组的信息
	u8* addr = per->blockrecord + 8 + (groupnum*0x20)%0x200;
	logtoall("whichblock: addr=%p, block=%llx\n", addr, *(u32*)addr);
	return *(u32*)addr;
}




//read[0x400*n,(n+1)*0x400), for example:
//0x3352: read[0x3001,0x3400]
//0x182304: read[0x182001,182400]
//注意1: 不管inodesize是120(0x400*120)还是128(0x400*128)还是256(0x400*256)，算出的值都能被0x200(一个扇区)整除
//注意2: 每个组的inode数量一般为8192，是0x400的倍数
//注意3: inode首个的编号是1不是0
static u8* checkcacheforinode(_obj* art, u64 wanted)
{
	struct perfs* per = art->priv_ptr;

	//skip read if already in there
	u64 inodeoffset=(wanted-1)%0x400;
	logtoall("checkcacheforinode: wanted=%llx, inodeoffset=%llx， firstinodeincache=%llx\n", wanted, inodeoffset, per->firstinodeincache);
	if(wanted-inodeoffset == per->firstinodeincache)
	{
		logtoall("checkcacheforinode: no need to read\n");
		return per->inodebuffer + inodeoffset * per->byteperinode;
	}

	u8* rdi = per->inodebuffer;
	u64 this = wanted - inodeoffset;
	while(1)
	{
		//inode in which group
		u64 groupnum = (this-1)/per->inodepergroup;
		//offs in this group
		u64 groupoffset = (this-1)%per->inodepergroup;
		logtoall("checkcacheforinode: groupnum=%llx,groupoffs=%llx\n", groupnum, groupoffset);

		//which sector
		u64 where;
		where = per->block0 + per->blocksize * whichblock(art, groupnum);
		where += groupoffset * per->byteperinode/0x200;
		//howmany sector(current skip when more than 0x400)
		u64 count;
		count = per->inodepergroup - groupoffset;
		if(count > 0x400)count = 0x400;
		logtoall("checkcacheforinode: where=%llx,count=%llx\n", where, count);

		//read inode table
		//file_take(0, 0, "", where*0x200, rdi, count*byteperinode*0x200);
		int ret = take_data_from_peer(art, _src_, 0, 0, where*0x200, _pos_, rdi, per->byteperinode*count);

		//next(currently no more than 0x40000byte)
		rdi += count * per->byteperinode;
		this += count;
		if(rdi - per->inodebuffer >= 0x40000) break;
	}
	per->firstinodeincache = wanted-inodeoffset;
	logtoall("checkcacheforinode: set firstinodeincache=%llx, wanted=%llx,inodeoffset=%llx\n", per->firstinodeincache, wanted, inodeoffset);
	return per->inodebuffer + inodeoffset * per->byteperinode;
}




//输入值：要搞的inode号，这个inode对应的文件内部偏移
//返回值：<=0就是失败
//作用：从offset开始能读多少读多少(1M以内)
static int explaininode(_obj* art, u64 inode,u64 wantwhere)
{
	logtoall("explaininode: inode=%llx,wantwhere=%llx\n", inode, wantwhere);
	struct perfs* per = art->priv_ptr;

	//函数调用之后,rsi为所请求的inode的内存地址，
	u8* rsi = checkcacheforinode(art, inode);
	//printmemory(rsi, 0x200);

	//检查是不是软链接
	u16 temp = (*(u16*)rsi)&0xf000;
	if(temp == 0xa000)
	{
		logtoall("explaininode: soft link:%x\n",rsi+0x28);
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
		for(rdi=per->datahome;rdi<per->datahome+0x100000;rdi++)
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
			u64 aaaaa=( *(u32*)rsi )*per->blocksize*0x200;	//逻辑上位置
			if(wantwhere+0x100000<=aaaaa)break;		//结束了

			//总共多少个扇区
			u64 count=( *(u16*)(rsi+4) )*per->blocksize;		//多少个扇区
			if(aaaaa+count*0x200<=wantwhere)continue;	//还没到

			//实际上，从第几个扇区开始
			u64 sector=*(u16*)(rsi+6);	//从extent体里得到高16位实际块号
			sector=(sector<<32) + ( *(u32*)(rsi+8) );	//加上低32位实际块号
			sector*=per->blocksize;	//乘以每块多少扇区，现在sector=分区内偏移多少个扇区
			sector+=per->block0;		//加上分区相对硬盘开始多少个扇区
			logtoall("explaininode: sector=%llx,count=%llx,aaaaa=%llx,want=%llx\n", sector, count, aaaaa, wantwhere);

			//byteoffs, bytecount, logicpos
			//targetaddr，targetlen, targetpos
			ext_cleverread
			(
				art,
				sector*0x200, count*0x200, aaaaa,
				per->datahome,0x100000,wantwhere
			);
		}

		return 1;
	}
	else		//ext2，3用老的直接间接块的方式
	{
	logtoall("explaininode: old@%x\n",rsi);
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


static void explaindirectory(_obj* art)
{
	struct perfs* per = art->priv_ptr;

	int i;
	u8* rdi = per->dirhome;
	u8* rsi = per->datahome;

	for(i=0;i<dirhomesize;i++)
	{
		*(u8*)(rdi+i)=0;
	}
	while(1)
	{
		if(rdi >= per->dirhome+dirhomesize)break;
		if(rsi >= per->datahome+datahomesize)break;
		if(*(u32*)rsi == 0)break;
		if(*(u16*)(rsi+4) == 0)break;
		logtoall("%x\n",*(u16*)(rsi+4));
		printmemory(rsi,0x10);

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
		logtoall("explaindirectory: type=%llx\n", *(u64*)rdi);

		//[0x10,0x17]inode号
		u64 thisinode=*(u32*)rsi;
		*(u64*)(rdi+0x10)=thisinode;
		logtoall("explaindirectory: inode=%llx\n", thisinode);

		//[0x18,0x1f]:size，ext的目录表里面没有文件大小，需要到inode表里面寻找
		u8* inodeaddr=checkcacheforinode(art, thisinode);
		*(u64*)(rdi+0x18)=*(u32*)(inodeaddr+4);
		logtoall("explaindirectory: size=%llx\n", *(u32*)(inodeaddr+4));

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
		logtoall("explaindirectory: name=%s\n", rdi+0x20);

		//最后指向下一个
		rsi+=*(u16*)(rsi+4);
		rdi+=0x40;
	}
}
int explainexthead(_obj* art, u8* pbr)
{
	struct perfs* per = art->priv_ptr;

	//sector per block
	per->blocksize = *(u32*)(pbr+0x418);
	per->blocksize = ( 1<<(per->blocksize+10) )/0x200;
	logtoall("sectorperblock:%x\n",per->blocksize);

	//sector per group
	per->groupsize = *(u32*)(pbr+0x420);
	per->groupsize = per->groupsize * per->blocksize;
	logtoall("sectorpergroup:%x\n", per->groupsize);

	//inode per group
	per->inodepergroup = *(u32*)(pbr+0x428);
	logtoall("inodepergroup:%x\n", per->inodepergroup);

	//byte per inode
	per->byteperinode = *(u16*)(pbr+0x458);
	logtoall("byteperinode:%x\n", per->byteperinode);

	per->block0 = 0;
	per->firstinodeincache = 0xffffffff;

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




static int ext_read(_obj* art, u64 id,u64 offset,u64 size)
{
	explaininode(art, id, offset);
	return 1;
}
static int ext_enterfolder(_obj* art, u64 id)
{
	struct perfs* per = art->priv_ptr;

	int ret;
	for(ret=0;ret<0x10000;ret++)per->dirhome[ret]=0;

	ret = explaininode(art, id, 0);
	if(ret>0)explaindirectory(art);
	return 1;
}
static int ext_cd(_obj* art, char* path)
{
	if(path[0] == '/'){
		ext_enterfolder(art, 2);
	}
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
	ret = explainexthead(art, pbr);
	if(ret < 0)return ret;

	return 0;
}
int extclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




static int extclient_reader(_obj* art,int xxx, void* arg,int cmd, void* buf,int len)
{
	logtoall("@extclient_reader\n");
	ext_cd(art, (void*)arg);
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
