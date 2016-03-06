#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#define	BSWAP_8(x)	((x) & 0xff)
#define	BSWAP_16(x)	((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define	BSWAP_32(x)	((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))
#define	BSWAP_64(x)	((BSWAP_32(x) << 32) | BSWAP_32((x) >> 32))
//用了别人的
int cleverread(QWORD,QWORD,QWORD,	char*,QWORD,QWORD);
int readsystem(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void printmemory(char* addr,QWORD size);
void say(char* fmt,...);




//系统或者各种东西提供好的memory，这几个变量仅仅记录了位置
static char* fshome;
	static char* pbr;
	static char* catabuf;
static char* dirhome;
static char* datahome;

static QWORD block0;		//分区开始扇区，每块多少扇区
static QWORD blocksize;
static QWORD catalogsector;	//catalog扇区位置，每个节点多少扇区
static QWORD nodesize;
static QWORD rootnode;		//记下几个节点号
static QWORD firstleafnode;




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
		int offset=BSWAP_16(*(WORD*)(datahome+temp));
		if(offset==0)break;
		if(offset>=temp)break;

		//拿到keylength，如果这个位置是最后的空记录，就退出
		int keylen=BSWAP_16(*(WORD*)(datahome+offset));
		if(keylen==0)break;

		//打印
		say("@%x,len=%x,key=",offset,keylen);

		//
		int i;
		for(i=0;i<8;i++)
		{
			say("%.2x ",*(BYTE*)(datahome+offset+2+i));
		}
		DWORD child=BSWAP_32(*(DWORD*)(datahome+offset+2+keylen));
		say(",child=%x\n",child);

	}
}
//（只用来调试）解释叶节点的record的data部分
void explainrecorddata(QWORD offset)
{
	switch( *(BYTE*)(datahome+offset+1) )
	{
		case 1:			//folder
		{
			say("folder,");
			say("cnid=%x,",BSWAP_32(*(DWORD*)(datahome+offset+8)));
			break;
		}
		case 2:			//file
		{
			say("file,");
			say("cnid=%x,",BSWAP_32(*(DWORD*)(datahome+offset+8)));
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
		int offset=BSWAP_16(*(WORD*)(datahome+temp));
		if(offset==0)break;
		if(offset>=temp)break;

		//拿到keylength，如果这个位置是最后的空记录，就退出
		int keylen=BSWAP_16(*(WORD*)(datahome+offset));
		if(keylen==0)break;

		//key第一部分，father
		DWORD father=BSWAP_32(*(DWORD*)(datahome+offset+2));
		say("@%x,len=%x,father=%x,data:",offset,keylen,father);

		//key后面的data
		int i;
		for(i=0;i<8;i++)
		{
			say("%.2x ",*(BYTE*)(datahome+offset+2+keylen+i));
		}
		say("\n");

		//目录和文件
		explainrecorddata(offset+2+keylen);

	}//while结束

}//函数结束


//（只用来调试）解释某个节点
static void hfs_explain(QWORD number)
{
	say("%llx@%llx\n",number,catalogsector+nodesize*number);
	readsystem(datahome,catalogsector+nodesize*number,0,nodesize);	//0x1000
	printmemory(datahome,0x1000);

	//1.解释节点头
	DWORD rightbrother=BSWAP_32(*(DWORD*)datahome);
	DWORD leftbrother=BSWAP_32(*(DWORD*)(datahome+4));
	BYTE type=*(BYTE*)(datahome+8);
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
QWORD searchbtreeforcnid(QWORD nodenum,QWORD wantcnid)
{
	say("enter node:%llx\n",nodenum);

	//把指定节点读到内存,顺便看看这节点是啥类型
	readsystem(datahome,catalogsector+nodenum*nodesize,0,nodesize);
	BYTE type=*(BYTE*)(datahome+8);

	//节点内每一个record找一遍，找本节点内第一个的偏移
	int totalrecords=BSWAP_16(*(WORD*)(datahome+0xa));
	int temp=0;

	int candidatechosen=0;		//确认就选这个了
	int candidateoffset=-1;		//候选record的位置
	QWORD candidatekey=0;		//候选record的key
	QWORD candidatechild=0;		//候选record的child
	while(1)
	{
		//每次进来减1，下一个指针
		temp++;
		//这是最后一个但是不能确定，所以还要处理一下
		if(temp>totalrecords)
		{
			QWORD temptempnodenum=BSWAP_32(*(DWORD*)datahome);
			if(temptempnodenum==0)break;

			//临时读下一个到datahome+0x8000那里(节点最大不超过0x8000吧)
			//读这个临时节点的第一个记录看看，能确定下来目前的最后一个record就是想要的
			readsystem(datahome+0x8000,catalogsector+temptempnodenum*nodesize,0,nodesize);
			QWORD temptempkey=BSWAP_32(*(DWORD*)(datahome+0x8000+0x10));

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
			WORD offset=BSWAP_16(*(WORD*)(datahome+nodesize*0x200-temp*2));
			DWORD key=BSWAP_32(*(DWORD*)(datahome+offset+2));
			int keylen=BSWAP_16(*(WORD*)(datahome+offset));
			//say("%x@%x,%x\n",key,nodenum,offset);


			//候选者
			if(key<wantcnid)
			{
				//比如节点内是1，2，3，64，88，1270，3355，而want=4
				//那么最开始1是候选者，又被更好的2取代，又被更好的3取代
				candidateoffset=offset;
				candidatekey=key;
				candidatechild=BSWAP_32(*(DWORD*)(datahome+offset+2+keylen));
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




static void explaindirectory(QWORD nodenum,QWORD wantcnid)
{
	char* rdi=dirhome;
	int i;

	//清理内存
	for(i=0;i<0x100000;i++)
	{
		*(BYTE*)(rdi+i)=0;
	}

	//第一个是当前目录信息（也就是.）
	*(QWORD*)(rdi+0)=0x656c6966;	//type
	*(QWORD*)(rdi+0x8)='.';		//'.'
	*(QWORD*)(rdi+0x10)=wantcnid;	//cnid
	*(QWORD*)(rdi+0x18)=0;		//size
	*(QWORD*)(rdi+0x20)='.';	//full path
	rdi+=0x40;

	//这俩控制循环次数
	int totalrecords=BSWAP_16(*(WORD*)(datahome+0xa));
	int temp=0;
	while(1)
	{
		//每次进来
		temp++;
		//这个结束了就继续下一个节点
		if(temp>totalrecords)
		{
			nodenum=BSWAP_32(*(DWORD*)datahome);
			if(nodenum==0)break;
			say("next node:%x\n",nodenum);

			readsystem(datahome,catalogsector+nodenum*nodesize,0,nodesize);
			totalrecords=BSWAP_16(*(WORD*)(datahome+0xa));
			temp=0;
			continue;
		}




		//这以后所有的key都大于想要的就跳出循环
		WORD offset=BSWAP_16(*(WORD*)(datahome+nodesize*0x200-temp*2));
		DWORD key=BSWAP_32(*(DWORD*)(datahome+offset+2));
		if(key>wantcnid)break;
		if(key<wantcnid)continue;
		//say("key:%x,in:%llx\n",key,nodenum);

		int keylen=BSWAP_16(*(WORD*)(datahome+offset));
		QWORD filetype=BSWAP_16(*(WORD*)(datahome+offset+2+keylen));

		//[0,0x7]:type
		if(filetype==2)
		{
			*(QWORD*)(rdi+0)=0x656c6966;	//'file'
		}
		else if(filetype==1)
		{
			*(QWORD*)(rdi+0)=0x726964;	//'dir'
		}
		else
		{
			*(QWORD*)(rdi+0)=filetype;
		}
		//[0x8,0xf]:subtype
		*(QWORD*)(rdi+0x8)=0;
		//[0x10,0x17]:cnid号
		*(QWORD*)(rdi+0x10)=BSWAP_32(*(DWORD*)(datahome+offset+2+keylen+0x8));
		//[0x18,0x1f]:size
		if(filetype==2)
		{
			*(QWORD*)(rdi+0x18)=BSWAP_64(*(QWORD*)(datahome+offset+2+keylen+0x58));
		}

		//[0x20,0x3f]:名字	//namelength=*(byte*)(rsi+6)
		i=BSWAP_16(*(WORD*)(datahome+offset+6));
		//say("%x@%x\n",i,offset);
		i--;
		if(i>0x1b)i=0x1b;
		for(;i>=0;i--)
		{
			*(BYTE*)(rdi+0x20+i)=*(BYTE*)(datahome+offset+9+i*2);
		}
		//say("%s\n",(char*)(rdi+0x20));
		if(*(DWORD*)(rdi+0x20) == 0) *(DWORD*)(rdi+0x20)=0x3f3f3f3f;



		rdi+=0x40;
		if(rdi>=dirhome+0xfffc0)break;

	}//大while(1)循环

}//函数结束




//传进来的是：本文件父目录的cnid，本文件的cnid，节点号，想要的文件内部偏移
void explainfile(QWORD fathercnid,QWORD wantcnid,QWORD nodenum,QWORD wantwhere)
{
	int i;
	char* rdi=datahome;

	//清理内存
	for(i=0;i<0x100000;i++)
	{
		*(BYTE*)(rdi+i)=0;
	}
	say("father:%x,self:%x,node:%x,want:%x\n",fathercnid,wantcnid,nodenum,wantwhere);


	//然后是后面的记录
	readsystem(catabuf,catalogsector+nodenum*nodesize,0,nodesize);
	int temp=nodesize*0x200;
	while(1)
	{
		//每次进来直接减2
		temp-=2;
		//say("oh\n");

		//这个节点结束了（1）
		WORD offset=BSWAP_16(*(WORD*)(catabuf+temp));
		//say("oh 1:offset=%x\n",offset);
		if(offset>=temp)break;

		//出毛病了，到指针最后一个都没找着（2）
		int keylen=BSWAP_16(*(WORD*)(catabuf+offset));
		//say("oh 2:keylen=%x\n",keylen);
		if(keylen==0)return;

		//比较fathercnid，要是不一样就跳过（3）
		DWORD key=BSWAP_32(*(DWORD*)(catabuf+offset+2));
		//say("oh 3:key=%x\n",key);
		if(key!=fathercnid)continue;

		//要是这个record不是文件，那就再跳过（4）
		BYTE filetype=*(BYTE*)(catabuf+offset+2+keylen+1);
		//say("oh 4:type=:%x\n",filetype);
		if(filetype!=2)continue;

		//再看看文件cnid对不对，不对还跳过（5）
		DWORD thiscnid=BSWAP_32(*(DWORD*)(catabuf+offset+2+keylen+8));
		//say("oh 5:self:%x\n",thiscnid);
		if(thiscnid!=wantcnid)continue;

		//穿越重重阻碍，现在可以从datafork里面，找扇区号，和扇区数了
		say("oh i am here!\n");
		char* forkaddr=catabuf+offset+2+keylen+0x58+0x10;
		QWORD logicwhere=0;
		for(i=0;i<8;i++)
		{
			//读块号，没有这项就返回
			QWORD fileblock=BSWAP_32(*(DWORD*)(forkaddr+i*0x50));
			if(fileblock==0)break;

			//读块数，是0就返回
			QWORD count=BSWAP_32(*(DWORD*)(forkaddr+4+i*0x50));
			if(fileblock==0)break;

			//蛋又碎了，传进去的参数为：
			//前三个：这一块的物理扇区号，扇区数，逻辑位置，
			//后三个：目标位置，目标大小，需求位置
			cleverread
			(
				fileblock*blocksize,count*blocksize,logicwhere,
				datahome,0x80000,wantwhere
			);

			//下一块位置修改
			logicwhere+=count*blocksize*0x200;

		}//for循环
		break;

	}//大while(1)循环

}



static int hfs_ls(char* to)
{
	return 0;
}
static int hfs_cd(QWORD id)
{
	//2.已经知道了目录的cnid号，那么需要从b树里面找到节点号和节点内偏移
	QWORD foundnode;
	if(id==2)
	{
		//根肯定在最开始的地方，相当于稍微优化一下
		readsystem(datahome,catalogsector+firstleafnode*nodesize,0,nodesize);
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
static void hfs_load(QWORD id,QWORD wantwhere)
{
	//搜索b树找它爸，没办法直接找到它！
	QWORD fathercnid=*(DWORD*)(dirhome+0x10);
	QWORD foundnode=searchbtreeforcnid(rootnode,fathercnid);
	if(foundnode <= 0)
	{
		say("not found\n");
		return;
	}
	say("found:%llx@node:%llx\n",id,foundnode);


	//3.从他爹开始，record，的data部分，的fork信息里面，找到东西
	explainfile(fathercnid,id,foundnode,wantwhere);
}
static void hfs_store()
{
	return;
}
int explainhfshead()
{
	QWORD size,clumpsize,totalblock,sector,count;
	int i;
	char* addr;
	QWORD* dstqword=(QWORD*)fshome;


	//func cd
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0;
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=(QWORD)hfs_ls;
	dstqword[5]=(QWORD)hfs_cd;
	dstqword[6]=(QWORD)hfs_load;
	dstqword[7]=(QWORD)hfs_store;
	dstqword += 8;


//---------------------第一次读，把分区头读进pbrbuffer------------------------
	blocksize=BSWAP_32( *(DWORD*)(pbr+0x428) )/0x200;
	say("blocksize:%x\n",blocksize);

	//0x470,0x4c0,0x510,0x560
	for(i=0;i<4;i++)
	{
		addr=(char*)( pbr + 0x470 + (0x50*i) );
		size=BSWAP_64(*(QWORD*)addr );
		clumpsize=BSWAP_32(*(DWORD*)(addr+0x8) );
		totalblock=BSWAP_32(*(DWORD*)(addr+0xc) );
		sector=block0+8*BSWAP_32(*(DWORD*)(addr+0x10) );
		count=blocksize*BSWAP_32(*(DWORD*)(addr+0x14) );

		dstqword[0]=0x24;	//'$'
		if(i==0)
		{
			dstqword[1]=0x636f6c6c61;	//'alloc'
			say("allocation\n");
		}
		else if(i==1)
		{
			dstqword[1]=0x73746e65747865;	//'extents'
			say("extents overflow\n");
		}
		else if(i==2)
		{
			catalogsector=sector;
			dstqword[1]=0x676f6c61746163;	//'catalog'
			say("catalog\n");
		}
		else if(i==3)
		{
			dstqword[1]=0x6972747461;	//'attri'
			say("attribute\n");
		}
		dstqword[2]=0x470+(0x50*i);
		dstqword[3]=0x470+0x4f+(0x50*i);
		dstqword[4]=BSWAP_64(*(QWORD*)(addr+0) );
		dstqword[5]=BSWAP_64(*(QWORD*)(addr+8) );
		dstqword[6]=BSWAP_64(*(QWORD*)(addr+0x10) );
		dstqword[7]=BSWAP_64(*(QWORD*)(addr+0x18) );
		dstqword += 8;

		//dstqword[2]=sector;
		//dstqword[3]=sector+count;
		//dstqword[4]=size;
		//dstqword[5]=clumpsize;
		//dstqword[6]=totalblock;
		//dstqword += 8;
		//say("	size:%llx\n",size);
		//say("	clumpsize:%llx\n",clumpsize);
		//say("	totalblocks:%llx\n",totalblock);
		//say("	sector:%llx\n",sector);
		//say("	count:%llx\n",count);
	}




//----------------第二次读，把分区头读进catabuf--------------
	readsystem(catabuf,catalogsector,0,0x8);	//0x1000
	//printmemory(catabuf,0x200);

	//nodesize
	nodesize=BSWAP_16( *(WORD*)(catabuf+0x20) );
	nodesize=nodesize/0x200;
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x7a7365646f6e;	//'nodesz'
	dstqword[2]=0x21;
	dstqword[3]=0x20;
	dstqword[4]=nodesize;
	dstqword += 8;
	say("nodesize:%x\n",nodesize);

	//rootnode
	rootnode=BSWAP_32(*(DWORD*)(catabuf+0x10) );
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x646f6e746f6f72;	//'rootnod'
	dstqword[2]=0x13;
	dstqword[3]=0x10;
	dstqword[4]=rootnode;
	dstqword += 8;
	say("rootnode:%x\n",rootnode);

	//firstleafnode
	firstleafnode=BSWAP_32(*(DWORD*)(catabuf+0x18) );
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x306661656c;	//'leaf0'
	dstqword[2]=0x1b;
	dstqword[3]=0x18;
	dstqword[4]=firstleafnode;
	dstqword += 8;
	say("firstleafnode:%x\n",firstleafnode);



	return 1;
}








int ishfs(char* addr)
{
	QWORD temp;

	temp=*(WORD*)(addr+0x400);
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
int mounthfs(QWORD sector,char* addr)
{
	int ret;

	//得到本分区的开始扇区位置，再得到3个buffer的位置
	block0=sector;
	fshome=addr+0;
		pbr=fshome+0x10000;
		catabuf=fshome+0x20000;
	dirhome=addr+0x100000;
	datahome=addr+0x200000;

	//检查
	ret=readsystem(pbr,block0,0,0x8);	//0x1000
	ret=ishfs(pbr);
	if(ret==0)return -1;

	//很多东西
	ret=explainhfshead();
	if(ret<0)return ret;

	//进入根目录
	hfs_cd(2);
	return 1;
}
