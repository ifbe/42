#include "libsoft.h"
#define	BSWAP_8(x)	((x) & 0xff)
#define	BSWAP_16(x)	((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define	BSWAP_32(x)	((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))
#define	BSWAP_64(x)	((BSWAP_32(x) << 32) | BSWAP_32((x) >> 32))
int cleverread(u64,u64,u64,	u8*,u64,u64);
int cleverwrite(u64,u64,u64,	u8*,u64,u64);




//系统或者各种东西提供好的memory，这几个变量仅仅记录了位置
static u8* fshome;
	static u8* pbr;
	static u8* catabuf;
static u8* dirhome;
static u8* datahome;
//
static u64 block0;		//分区开始扇区，每块多少扇区
static u64 blocksize;
static u64 catalogsector;	//catalog扇区位置，每个节点多少扇区
static u64 nodesize;
static u64 rootnode;		//记下几个节点号
static u64 firstleafnode;




//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//（只用来调试）解释index节点
void explainindexnode()
{
	//每个index和leaf节点尾巴上有，每个record的位置
	int temp=nodesize*0x200;
	while(1)
	{
		//每次进来-2
		temp-=2;

		//从节点尾巴上找到record在哪儿，指针=0，或者指针>本身位置，就退出
		int offset=BSWAP_16(*(u16*)(datahome+temp));
		if(offset==0)break;
		if(offset>=temp)break;

		//拿到keylength，如果这个位置是最后的空记录，就退出
		int keylen=BSWAP_16(*(u16*)(datahome+offset));
		if(keylen==0)break;

		//打印
		say("@%x,len=%x,key=",offset,keylen);

		//
		int i;
		for(i=0;i<8;i++)
		{
			say("%02x ",*(u8*)(datahome+offset+2+i));
		}
		u32 child=BSWAP_32(*(u32*)(datahome+offset+2+keylen));
		say(",child=%x\n",child);

	}
}
//（只用来调试）解释叶节点的record的data部分
void explainrecorddata(u64 offset)
{
	switch( *(u8*)(datahome+offset+1) )
	{
		case 1:			//folder
		{
			say("folder,");
			say("cnid=%x,",BSWAP_32(*(u32*)(datahome+offset+8)));
			break;
		}
		case 2:			//file
		{
			say("file,");
			say("cnid=%x,",BSWAP_32(*(u32*)(datahome+offset+8)));
			break;
		}
		case 3:			//folderthread
		{
			say("folderthread,");
			break;
		}
		case 4:			//filethread
		{
			say("filethread,");
			break;
		}
	}
	say("\n");
}
//（只用来调试）解释叶节点
void explainleafnode()
{
	//每个index和leaf节点尾巴上有，每个record的位置	//每个index和leaf节点尾巴上有，每个record的位置
	int temp=nodesize*0x200;
	while(1)
	{
		//每次进来-2
		temp-=2;

		//从节点尾巴上找到record在哪儿，指针=0，或者指针>本身位置，就退出
		int offset=BSWAP_16(*(u16*)(datahome+temp));
		if(offset==0)break;
		if(offset>=temp)break;

		//拿到keylength，如果这个位置是最后的空记录，就退出
		int keylen=BSWAP_16(*(u16*)(datahome+offset));
		if(keylen==0)break;

		//key第一部分，father
		u32 father=BSWAP_32(*(u32*)(datahome+offset+2));
		say("@%x,len=%x,father=%x,data:",offset,keylen,father);

		//key后面的data
		int i;
		for(i=0;i<8;i++)
		{
			say("%02x ",*(u8*)(datahome+offset+2+keylen+i));
		}
		say("\n");

		//目录和文件
		explainrecorddata(offset+2+keylen);

	}//while结束

}//函数结束


//（只用来调试）解释某个节点
static void hfs_explain(u64 number)
{
	say("%llx@%llx\n",number,catalogsector+nodesize*number);
/*	file_take(0, 0,
		"", (catalogsector+nodesize*number)*0x200,
		datahome, nodesize*0x200
	);*/
	printmemory(datahome,0x1000);

	//1.解释节点头
	u32 rightbrother=BSWAP_32(*(u32*)datahome);
	u32 leftbrother=BSWAP_32(*(u32*)(datahome+4));
	u8 type=*(u8*)(datahome+8);
	say("rightbro=%x,leftbro=%x,type:%x\n",rightbrother,leftbrother,type);

	//
	if(type==1)
	{
		//这是头节点
	}
	else if(type==0)
	{
		//index节点
		explainindexnode();
	}
	else if(type==0xff)
	{
		//叶节点
		explainleafnode();
	}
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------




//告诉我从哪个节点开始搜，你要哪一号cnid
//我会返回这一号cnid所在的叶子节点
u64 searchbtreeforcnid(u64 nodenum,u64 wantcnid)
{
	say("enter node:%llx\n",nodenum);

	//把指定节点读到内存,顺便看看这节点是啥类型
/*	file_take(0, 0,
		"", (catalogsector+nodenum*nodesize)*0x200,
		datahome, nodesize*0x200
	);*/
	u8 type=*(u8*)(datahome+8);

	//节点内每一个record找一遍，找本节点内第一个的偏移
	int totalrecords=BSWAP_16(*(u16*)(datahome+0xa));
	int temp=0;

	int candidatechosen=0;		//确认就选这个了
	int candidateoffset=-1;		//候选record的位置
	u64 candidatekey=0;		//候选record的key
	u64 candidatechild=0;		//候选record的child
	while(1)
	{
		//每次进来减1，下一个指针
		temp++;
		//这是最后一个但是不能确定，所以还要处理一下
		if(temp>totalrecords)
		{
			u64 temptempnodenum=BSWAP_32(*(u32*)datahome);
			if(temptempnodenum==0)break;

			//临时读下一个到datahome+0x8000那里(节点最大不超过0x8000吧)
			//读这个临时节点的第一个记录看看，能确定下来目前的最后一个record就是想要的
/*			file_take(0, 0,
				"", (catalogsector+temptempnodenum*nodesize)*0x200,
				datahome+0x8000, nodesize*0x200
			);*/
			u64 temptempkey=BSWAP_32(*(u32*)(datahome+0x8000+0x10));

			//
			//say("temptempnode:%x,temptempkey:%x\n",temptempnodenum,temptempkey);
			if(wantcnid<=temptempkey)
			{
				candidatechosen=1;			//候选者选中信号1
			}
		}


		//没有特权的，只能先成为候选人
		if(candidatechosen==0)
		{
			u16 offset=BSWAP_16(*(u16*)(datahome+nodesize*0x200-temp*2));
			u32 key=BSWAP_32(*(u32*)(datahome+offset+2));
			int keylen=BSWAP_16(*(u16*)(datahome+offset));
			//say("%x@%x,%x\n",key,nodenum,offset);


			//候选者
			if(key<wantcnid)
			{
				//比如节点内是1，2，3，64，88，1270，3355，而want=4
				//那么最开始1是候选者，又被更好的2取代，又被更好的3取代
				candidateoffset=offset;
				candidatekey=key;
				candidatechild=BSWAP_32(*(u32*)(datahome+offset+2+keylen));
				//say("candidatekey:%x,candidatechild:%x,@%x,%x\n",key,candidatechild,nodenum,offset);
			}
			else{candidatechosen=1;}			//候选者选中信号2
		}


		//有两种情况进入这里，1：这是节点内最后1个record，而下一个节点全部大于等于所要的
		//2：当前这个已经大于等于想要的，所以候选者直接被选中
		if(candidatechosen>0)
		{
			//say("the chosen one:%x,%x\n",nodenum,candidatekey);

			//有问题的候选者退出
			if(candidateoffset==-1)
			{
				//wantcnid=0所以它会比所有的都小，但是cnid不可能=0，所以返回失败 want
				return 0;
			}

			//看节点类型分别处理（头节点和位图节点不可能被传进来吧？）
			if(type==0xff)
			{
				//say("return %x\n",nodenum);
				return nodenum;
			}
			if(type==0)
			{
				//要是index节点，就找它儿子麻烦
				return searchbtreeforcnid(candidatechild,wantcnid);
			}
		}


	}//大while(1)循环

	return 0;	//不会到这里
}




static void explaindirectory(u64 nodenum,u64 wantcnid)
{
	u8* rdi=dirhome;
	int i;

	//清理内存
	for(i=0;i<0x100000;i++)
	{
		*(u8*)(rdi+i)=0;
	}

	//第一个是当前目录信息（也就是.）
	*(u64*)(rdi+0)=0x656c6966;	//type
	*(u64*)(rdi+0x8)='.';		//'.'
	*(u64*)(rdi+0x10)=wantcnid;	//cnid
	*(u64*)(rdi+0x18)=0;		//size
	*(u64*)(rdi+0x20)='.';	//full path
	rdi+=0x40;

	//这俩控制循环次数
	int totalrecords=BSWAP_16(*(u16*)(datahome+0xa));
	int temp=0;
	while(1)
	{
		//每次进来
		temp++;
		//这个结束了就继续下一个节点
		if(temp>totalrecords)
		{
			nodenum=BSWAP_32(*(u32*)datahome);
			if(nodenum==0)break;
			say("next node:%x\n",nodenum);

/*			file_take(0, 0,
				"", (catalogsector+nodenum*nodesize)*0x200,
				datahome, nodesize*0x200
			);*/
			totalrecords=BSWAP_16(*(u16*)(datahome+0xa));
			temp=0;
			continue;
		}




		//这以后所有的key都大于想要的就跳出循环
		u16 offset=BSWAP_16(*(u16*)(datahome+nodesize*0x200-temp*2));
		u32 key=BSWAP_32(*(u32*)(datahome+offset+2));
		if(key>wantcnid)break;
		if(key<wantcnid)continue;
		//say("key:%x,in:%llx\n",key,nodenum);

		int keylen=BSWAP_16(*(u16*)(datahome+offset));
		u64 filetype=BSWAP_16(*(u16*)(datahome+offset+2+keylen));

		//[0,0x7]:type
		if(filetype==2)
		{
			*(u64*)(rdi+0)=0x656c6966;	//'file'
		}
		else if(filetype==1)
		{
			*(u64*)(rdi+0)=0x726964;	//'dir'
		}
		else
		{
			*(u64*)(rdi+0)=filetype;
		}
		//[0x8,0xf]:subtype
		*(u64*)(rdi+0x8)=0;
		//[0x10,0x17]:cnid号
		*(u64*)(rdi+0x10)=BSWAP_32(*(u32*)(datahome+offset+2+keylen+0x8));
		//[0x18,0x1f]:size
		if(filetype==2)
		{
			*(u64*)(rdi+0x18)=BSWAP_64(*(u64*)(datahome+offset+2+keylen+0x58));
		}

		//[0x20,0x3f]:名字	//namelength=*(byte*)(rsi+6)
		i=BSWAP_16(*(u16*)(datahome+offset+6));
		//say("%x@%x\n",i,offset);
		i--;
		if(i>0x1b)i=0x1b;
		for(;i>=0;i--)
		{
			*(u8*)(rdi+0x20+i)=*(u8*)(datahome+offset+9+i*2);
		}
		//say("%s\n",(rdi+0x20));
		if(*(u32*)(rdi+0x20) == 0) *(u32*)(rdi+0x20)=0x3f3f3f3f;



		rdi+=0x40;
		if(rdi>=dirhome+0xfffc0)break;

	}//大while(1)循环

}//函数结束




//传进来的是：本文件父目录的cnid，本文件的cnid，节点号，想要的文件内部偏移
void explainfile(u64 fathercnid,u64 wantcnid,u64 nodenum,u64 wantwhere)
{
	int i;
	u8* rdi=datahome;

	//清理内存
	for(i=0;i<0x100000;i++)
	{
		*(u8*)(rdi+i)=0;
	}
	say("father:%x,self:%x,node:%x,want:%x\n",fathercnid,wantcnid,nodenum,wantwhere);


	//然后是后面的记录
/*	file_take(0, 0,
		"", (catalogsector+nodenum*nodesize)*0x200,
		catabuf, nodesize*0x200
	);
*/
	//
	int temp=nodesize*0x200;
	while(1)
	{
		//每次进来直接减2
		temp-=2;
		//say("oh\n");

		//这个节点结束了（1）
		u16 offset=BSWAP_16(*(u16*)(catabuf+temp));
		//say("oh 1:offset=%x\n",offset);
		if(offset>=temp)break;

		//出毛病了，到指针最后一个都没找着（2）
		int keylen=BSWAP_16(*(u16*)(catabuf+offset));
		//say("oh 2:keylen=%x\n",keylen);
		if(keylen==0)return;

		//比较fathercnid，要是不一样就跳过（3）
		u32 key=BSWAP_32(*(u32*)(catabuf+offset+2));
		//say("oh 3:key=%x\n",key);
		if(key!=fathercnid)continue;

		//要是这个record不是文件，那就再跳过（4）
		u8 filetype=*(u8*)(catabuf+offset+2+keylen+1);
		//say("oh 4:type=:%x\n",filetype);
		if(filetype!=2)continue;

		//再看看文件cnid对不对，不对还跳过（5）
		u32 thiscnid=BSWAP_32(*(u32*)(catabuf+offset+2+keylen+8));
		//say("oh 5:self:%x\n",thiscnid);
		if(thiscnid!=wantcnid)continue;

		//穿越重重阻碍，现在可以从datafork里面，找扇区号，和扇区数了
		say("oh i am here!\n");
		u8* forkaddr=catabuf+offset+2+keylen+0x58+0x10;
		u64 logicwhere=0;
		for(i=0;i<8;i++)
		{
			//读块号，没有这项就返回
			u64 fileblock=BSWAP_32(*(u32*)(forkaddr+i*0x50));
			if(fileblock==0)break;

			//读块数，是0就返回
			u64 count=BSWAP_32(*(u32*)(forkaddr+4+i*0x50));
			if(fileblock==0)break;

			//蛋又碎了，传进去的参数为：
			//前三个：这一块的物理扇区号，扇区数，逻辑位置，
			//后三个：目标位置，目标大小，需求位置
			cleverread
			(
				(fileblock*blocksize)*0x200,
				(count*blocksize)*0x200,
				logicwhere,

				datahome,0x80000,wantwhere
			);

			//下一块位置修改
			logicwhere+=count*blocksize*0x200;

		}//for循环
		break;

	}//大while(1)循环

}



static int hfs_list(u8* to)
{
	return 0;
}
static int hfs_choose(u64 id)
{
	//2.已经知道了目录的cnid号，那么需要从b树里面找到节点号和节点内偏移
	u64 foundnode;
	if(id==2)
	{
		//根肯定在最开始的地方，相当于稍微优化一下
/*		file_take(0, 0,
			"", (catalogsector+firstleafnode*nodesize)*0x200,
			datahome, nodesize*0x200
		);*/
		foundnode=firstleafnode;
	}
	else
	{
		foundnode=searchbtreeforcnid(rootnode,id);
		if( foundnode <= 0 )		//offset值不可能小于e
		{
			say("this cnid isn't in btree\n");
			return -2;
		}
		say("found:%llx@node:%llx\n",id,foundnode);
	}


	//3.既然上面找到了，那么就逐个翻译吧
	//（temp2那个返回值是为了省事给hfs_load函数准备的，但是hfs_cd只用它来判断搜索成功失败）
	explaindirectory(foundnode,id);
	return 1;
}
static void hfs_read(u64 id,u64 wantwhere)
{
	//搜索b树找它爸，没办法直接找到它！
	u64 fathercnid=*(u32*)(dirhome+0x10);
	u64 foundnode=searchbtreeforcnid(rootnode,fathercnid);
	if(foundnode <= 0)
	{
		say("not found\n");
		return;
	}
	say("found:%llx@node:%llx\n",id,foundnode);


	//3.从他爹开始，record，的data部分，的fork信息里面，找到东西
	explainfile(fathercnid,id,foundnode,wantwhere);
}
static void hfs_write()
{
	return;
}
int explainhfshead()
{
	u64 size,clumpsize,totalblock,sector,count;
	int i;
	u8* addr;

//---------------------第一次读，把分区头读进pbrbuffer------------------------
	blocksize=BSWAP_32( *(u32*)(pbr+0x428) )/0x200;
	say("blocksize:%x\n",blocksize);

	//0x470,0x4c0,0x510,0x560
	for(i=0;i<4;i++)
	{
		addr = pbr + 0x470 + (0x50*i);
		size = BSWAP_64(*(u64*)addr );
		clumpsize = BSWAP_32(*(u32*)(addr+0x8) );
		totalblock = BSWAP_32(*(u32*)(addr+0xc) );
		sector = block0+8*BSWAP_32(*(u32*)(addr+0x10) );
		count = blocksize*BSWAP_32(*(u32*)(addr+0x14) );

		if(i==0)
		{
			say("allocation\n");
		}
		else if(i==1)
		{
			say("extents overflow\n");
		}
		else if(i==2)
		{
			catalogsector=sector;
			say("catalog\n");
		}
		else if(i==3)
		{
			say("attribute\n");
		}
	}




//----------------第二次读，把分区头读进catabuf--------------
	//file_take(0, 0, "", catalogsector*0x200, catabuf, 0x1000);
	//printmemory(catabuf,0x200);

	//nodesize
	nodesize=BSWAP_16( *(u16*)(catabuf+0x20) );
	nodesize=nodesize/0x200;
	say("nodesize:%x\n",nodesize);

	//rootnode
	rootnode=BSWAP_32(*(u32*)(catabuf+0x10) );
	say("rootnode:%x\n",rootnode);

	//firstleafnode
	firstleafnode=BSWAP_32(*(u32*)(catabuf+0x18) );
	say("firstleafnode:%x\n",firstleafnode);



	return 1;
}








int check_hfs(u8* addr)
{
	u64 temp;

	temp=*(u16*)(addr+0x400);
	if( temp== 0x4442)
	{
		say("hfs\n");
		return 11;
	}
	else if( temp == 0x2b48 )
	{
		say("hfs+\n");
		return 22;
	}
	else if( temp == 0x5848 )
	{
		say("hfsx\n");
		return 33;
	}
	else return 0;
}




int hfsclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int hfsclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int hfsclient_discon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int hfsclient_linkup(struct halfrel* self, struct halfrel* peer)
{
	if(_src_ != self->flag)return 0;

	_obj* art = self->pchip;
/*
	int ret = file_take(art, _src_, "", 0, pbr, 0x1000);
	if(ret < 0x1000)return -1;

	ret = check_hfs(pbr);
	if(ret==0)return -2;

	//很多东西
	ret = explainhfshead();
	if(ret < 0)return ret;

	//进入根目录
	hfs_choose(2);*/
	return 0;
}
int hfsclient_delete(_obj* art)
{
	return 0;
}
int hfsclient_create(_obj* art)
{
	fshome = memorycreate(0x100000, 0);
		pbr = fshome+0x10000;
		catabuf = fshome+0x20000;
	dirhome = memorycreate(0x100000, 0);
	datahome = memorycreate(0x100000, 0);
	return 0;
}
