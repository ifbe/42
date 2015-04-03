#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#define	BSWAP_8(x)	((x) & 0xff)
#define	BSWAP_16(x)	((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define	BSWAP_32(x)	((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))
#define	BSWAP_64(x)	((BSWAP_32(x) << 32) | BSWAP_32((x) >> 32))


//系统或者各种东西提供好的memory，这几个变量仅仅记录了位置
static QWORD readbuffer;
static QWORD directorybuffer;
static QWORD catalogbuffer;
//分区开始扇区，每块多少扇区
static QWORD block0;
static QWORD blocksize;
//catalog扇区位置，每个节点多少扇区
static QWORD catalogsector;
static QWORD nodesize;
//记下几个节点号
static QWORD rootnode;
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
		int offset=BSWAP_16(*(WORD*)(readbuffer+temp));
		if(offset==0)break;
		if(offset>=temp)break;

		//拿到keylength，如果这个位置是最后的空记录，就退出
		int keylen=BSWAP_16(*(WORD*)(readbuffer+offset));
		if(keylen==0)break;

		//打印
		say("@%x,len=%x,key=",offset,keylen);

		//
		int i;
		for(i=0;i<8;i++)
		{
			say("%.2x ",*(BYTE*)(readbuffer+offset+2+i));
		}
		DWORD child=BSWAP_32(*(DWORD*)(readbuffer+offset+2+keylen));
		say(",child=%x\n",child);

	}
}
//（只用来调试）解释叶节点的record的data部分
explainrecorddata(QWORD offset)
{
	switch( *(BYTE*)(readbuffer+offset+1) )
	{
		case 1:			//folder
		{
			say("folder,");
			say("cnid=%x,",BSWAP_32(*(DWORD*)(readbuffer+offset+8)));
			break;
		}
		case 2:			//file
		{
			say("file,");
			say("cnid=%x,",BSWAP_32(*(DWORD*)(readbuffer+offset+8)));
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
explainleafnode()
{
	//每个index和leaf节点尾巴上有，每个record的位置	//每个index和leaf节点尾巴上有，每个record的位置
	int temp=nodesize*0x200;
	while(1)
	{
		//每次进来-2
		temp-=2;

		//从节点尾巴上找到record在哪儿，指针=0，或者指针>本身位置，就退出
		int offset=BSWAP_16(*(WORD*)(readbuffer+temp));
		if(offset==0)break;
		if(offset>=temp)break;

		//拿到keylength，如果这个位置是最后的空记录，就退出
		int keylen=BSWAP_16(*(WORD*)(readbuffer+offset));
		if(keylen==0)break;

		//key第一部分，father
		DWORD father=BSWAP_32(*(DWORD*)(readbuffer+offset+2));
		say("@%x,len=%x,father=%x,data:",offset,keylen,father);

		//key后面的data
		int i;
		for(i=0;i<8;i++)
		{
			say("%.2x ",*(BYTE*)(readbuffer+offset+2+keylen+i));
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
	readdisk(readbuffer,catalogsector+nodesize*number,0,nodesize);	//0x1000
	printmemory(readbuffer,0x1000);

	//1.解释节点头
	DWORD rightbrother=BSWAP_32(*(DWORD*)readbuffer);
	DWORD leftbrother=BSWAP_32(*(DWORD*)(readbuffer+4));
	BYTE type=*(BYTE*)(readbuffer+8);
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
	readdisk(readbuffer,catalogsector+nodenum*nodesize,0,nodesize);
	BYTE type=*(BYTE*)(readbuffer+8);

	//节点内每一个record找一遍，找本节点内第一个的偏移
	int totalrecords=BSWAP_16(*(WORD*)(readbuffer+0xa));
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
			QWORD temptempnodenum=BSWAP_32(*(DWORD*)readbuffer);
			if(temptempnodenum==0)break;

			//临时读下一个到readbuffer+0x8000那里(节点最大不超过0x8000吧)
			//读这个临时节点的第一个记录看看，能确定下来目前的最后一个record就是想要的
			readdisk(readbuffer+0x8000,catalogsector+temptempnodenum*nodesize,0,nodesize);
			QWORD temptempkey=BSWAP_32(*(DWORD*)(readbuffer+0x8000+0x10));

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
			WORD offset=BSWAP_16(*(WORD*)(readbuffer+nodesize*0x200-temp*2));
			DWORD key=BSWAP_32(*(DWORD*)(readbuffer+offset+2));
			int keylen=BSWAP_16(*(WORD*)(readbuffer+offset));
			//say("%x@%x,%x\n",key,nodenum,offset);


			//候选者
			if(key<wantcnid)
			{
				//比如节点内是1，2，3，64，88，1270，3355，而want=4
				//那么最开始1是候选者，又被更好的2取代，又被更好的3取代
				candidateoffset=offset;
				candidatekey=key;
				candidatechild=BSWAP_32(*(DWORD*)(readbuffer+offset+2+keylen));
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

}




static void explaindirectory(QWORD nodenum,QWORD wantcnid)
{
	QWORD rdi=directorybuffer;
	int i;

	//清理内存
	for(i=0;i<0x100000;i++)
	{
		*(BYTE*)(rdi+i)=0;
	}

	//第一个是当前目录信息（也就是.）
	*(QWORD*)rdi='.';		//1.name
	*(QWORD*)(rdi+0x10)=wantcnid;		//2.cnid
	rdi+=0x40;

	//这俩控制循环次数
	int totalrecords=BSWAP_16(*(WORD*)(readbuffer+0xa));
	int temp=0;
	while(1)
	{
		//每次进来
		temp++;
		//这个结束了就继续下一个节点
		if(temp>totalrecords)
		{
			nodenum=BSWAP_32(*(DWORD*)readbuffer);
			if(nodenum==0)break;
			say("next node:%x\n",nodenum);

			readdisk(readbuffer,catalogsector+nodenum*nodesize,0,nodesize);
			totalrecords=BSWAP_16(*(WORD*)(readbuffer+0xa));
			temp=0;
			continue;
		}




		//这以后所有的key都大于想要的就跳出循环
		WORD offset=BSWAP_16(*(WORD*)(readbuffer+nodesize*0x200-temp*2));
		DWORD key=BSWAP_32(*(DWORD*)(readbuffer+offset+2));
		if(key>wantcnid)break;
		if(key<wantcnid)continue;
		//say("key:%x,in:%llx\n",key,nodenum);

		int keylen=BSWAP_16(*(WORD*)(readbuffer+offset));
		//1.名字
		WORD namelen=BSWAP_16(*(WORD*)(readbuffer+offset+6));
		//say("%x@%x\n",namelen,offset);
		if(namelen>=0xf)namelen=0xf;
		i=0;
		for(i=0;i<namelen;i++)	//namelength=*(byte*)(rsi+6)
		{
			*(BYTE*)(rdi+i)=*(BYTE*)(readbuffer+offset+9+i*2);
			//say("%c",*(BYTE*)(readbuffer+offset+9+i*2));
		}
		//2.cnid号
		*(QWORD*)(rdi+0x10)=BSWAP_32(*(DWORD*)(readbuffer+offset+2+keylen+0x8));
		//3.type
		QWORD filetype=BSWAP_16(*(WORD*)(readbuffer+offset+2+keylen));
		*(QWORD*)(rdi+0x20)=filetype;
		//4.size
		if(filetype==2)
		{
			*(QWORD*)(rdi+0x30)=BSWAP_64(*(QWORD*)(readbuffer+offset+2+keylen+0x58));
		}




		if(*(DWORD*)rdi == 0) *(DWORD*)rdi=0x3f3f3f3f;
		rdi+=0x40;
		if(rdi>=directorybuffer+0xfffc0)break;

	}//大while(1)循环

}//函数结束
//所谓cd，就是把fathercnid=want的那些记录，翻译成容易看懂的格式：名字，id，种类，大小
static int hfs_cd(BYTE* addr)
{
	//1.检查文件夹名字来寻找cnid，如果没有这个文件夹那么不管了返回
	QWORD* table=(QWORD*)(directorybuffer);	//一定要括号
	QWORD wantcnid=0;
	int i;
	if(addr[0]=='/')
	{
		//1，2这种肯定在第一个叶节点（吧）
		wantcnid=2;
	}
	else
	{
		for(i=0;i<0x200;i+=4)
		{
			if( compare(&table[i],addr) == 0 )
			{
				wantcnid=table[i+2];
				break;
			}
		}
	}
	if(wantcnid==0)
	{
		say("not found:%s\n",addr);
		return -1;
	}




	//2.已经知道了目录的cnid号，那么需要从b树里面找到节点号和节点内偏移
	QWORD foundnode;
	if(wantcnid==2)
	{
		//根肯定在最开始的地方，相当于稍微优化一下
		readdisk(readbuffer,catalogsector+firstleafnode*nodesize,0,nodesize);
		foundnode=firstleafnode;
	}
	else
	{
		foundnode=searchbtreeforcnid(rootnode,wantcnid);
		if( foundnode <= 0 )		//offset值不可能小于e
		{
			say("this cnid isn't in btree\n");
			return -2;
		}
		say("found:%llx@node:%llx\n",wantcnid,foundnode);
	}




	//3.既然上面找到了，那么就逐个翻译吧
	//（temp2那个返回值是为了省事给hfs_load函数准备的，但是hfs_cd只用它来判断搜索成功失败）
	explaindirectory(foundnode,wantcnid);
}




//传进来的是：本文件父目录的cnid，本文件的cnid，节点号，想要的文件内部偏移
void explainfile(QWORD fathercnid,QWORD wantcnid,QWORD nodenum,QWORD wantwhere)
{
	readdisk(catalogbuffer,catalogsector+nodenum*nodesize,0,nodesize);
	QWORD rdi=readbuffer;
	int i;
	//清理内存
	for(i=0;i<0x100000;i++)
	{
		*(BYTE*)(rdi+i)=0;
	}
	say("father:%x,self:%x,node:%x,want:%x\n",fathercnid,wantcnid,nodenum,wantwhere);


	//然后是后面的记录
	int temp=nodesize*0x200;
	while(1)
	{
		//每次进来直接减2
		temp-=2;
		//say("oh\n");

		//这个节点结束了（1）
		WORD offset=BSWAP_16(*(WORD*)(catalogbuffer+temp));
		//say("oh 1:offset=%x\n",offset);
		if(offset>=temp)break;

		//出毛病了，到指针最后一个都没找着（2）
		int keylen=BSWAP_16(*(WORD*)(catalogbuffer+offset));
		//say("oh 2:keylen=%x\n",keylen);
		if(keylen==0)return;

		//比较fathercnid，要是不一样就跳过（3）
		DWORD key=BSWAP_32(*(DWORD*)(catalogbuffer+offset+2));
		//say("oh 3:key=%x\n",key);
		if(key!=fathercnid)continue;

		//要是这个record不是文件，那就再跳过（4）
		BYTE filetype=*(BYTE*)(catalogbuffer+offset+2+keylen+1);
		//say("oh 4:type=:%x\n",filetype);
		if(filetype!=2)continue;

		//再看看文件cnid对不对，不对还跳过（5）
		DWORD thiscnid=BSWAP_32(*(DWORD*)(catalogbuffer+offset+2+keylen+8));
		//say("oh 5:self:%x\n",thiscnid);
		if(thiscnid!=wantcnid)continue;

		//穿越重重阻碍，现在可以从datafork里面，找扇区号，和扇区数了
		say("oh i am here!\n");
		QWORD forkaddr=catalogbuffer+offset+2+keylen+0x58+0x10;
		QWORD logicwhere=0;
		for(i=0;i<8;i++)
		{
			//读块号，没有这项就返回
			QWORD fileblock=BSWAP_32(*(DWORD*)(forkaddr+i*0x50));
			if(fileblock==0)break;

			//读块数，是0就返回
			QWORD count=BSWAP_32(*(DWORD*)(forkaddr+4+i*0x50));
			if(fileblock==0)break;

			//蛋又碎了，传进去的参数为：这一块的物理扇区号，扇区数，逻辑位置，需求位置，目标位置
			holyshit(fileblock*blocksize,count*blocksize,logicwhere,wantwhere,readbuffer);

			//下一块位置修改
			logicwhere+=count*blocksize*0x200;

		}//for循环
		break;

	}//大while(1)循环

}
static int hfs_load(BYTE* addr,QWORD wantwhere)
{
	//1.在自己的表里面搜索到这个文件名对应的cnid号
	QWORD* table=(QWORD*)(directorybuffer);	//一定要括号
	QWORD wantcnid=0;
	int i;
	for(i=0;i<0x200;i+=4)
	{
		if( compare(&table[i],addr) == 0 )
		{
			wantcnid=table[i+2];
			break;
		}
	}
	if(wantcnid==0)
	{
		say("not found:%s\n",addr);
		return -1;
	}




	//2.搜索b树搜索key，只能找它爸，没办法直接找到它！
	QWORD fathercnid=*(DWORD*)(directorybuffer+0x10);
	QWORD foundnode=searchbtreeforcnid(rootnode,fathercnid);
	if(foundnode <= 0)
	{
		say("not found\n");
		return;
	}
	say("found:%llx@node:%llx\n",wantcnid,foundnode);




	//3.从这个节点开始，record，的data部分，的fork信息里面，找到东西
	explainfile(fathercnid,wantcnid,foundnode,wantwhere);
}




void printhead()
{
	QWORD sector;
	say("allocation\n");
	say("    size:%llx\n",BSWAP_64(*(QWORD*)(readbuffer+0x470) ) );
	say("    clumpsize:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x478) ) );
	say("    totalblocks:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x47c) ) );
	sector=block0+8*BSWAP_32(*(DWORD*)(readbuffer+0x480) );
	say("    sector:%llx,%lld\n",sector,sector);
	say("    count:%llx\n",blocksize*BSWAP_32(*(DWORD*)(readbuffer+0x484) ) );
	say("extents overflow\n");
	say("    size:%llx\n",BSWAP_64(*(QWORD*)(readbuffer+0x4c0) ) );
	say("    clumpsize:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x4c8) ) );
	say("    totalblocks:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x4cc) ) );
	sector=block0+8*BSWAP_32(*(DWORD*)(readbuffer+0x4d0) );
	say("    sector:%llx,%lld\n",sector,sector);
	say("    count:%llx\n",blocksize*BSWAP_32(*(DWORD*)(readbuffer+0x4d4) ) );
	say("catalog\n");
	say("    size:%llx\n",BSWAP_64(*(QWORD*)(readbuffer+0x510) ) );
	say("    clumpsize:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x518) ) );
	say("    totalblocks:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x51c) ) );
	sector=block0+8*BSWAP_32(*(DWORD*)(readbuffer+0x520) );
	say("    sector:%llx,%lld\n",sector,sector);
	say("    count:%llx\n",blocksize*BSWAP_32(*(DWORD*)(readbuffer+0x524) ) );
	say("attribute\n");
	say("    size:%llx\n",BSWAP_64(*(QWORD*)(readbuffer+0x560) ) );
	say("    clumpsize:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x568) ) );
	say("    totalblocks:%llx\n",BSWAP_32(*(DWORD*)(readbuffer+0x56c) ) );
	sector=block0+8*BSWAP_32(*(DWORD*)(readbuffer+0x570) );
	say("    sector:%llx,%lld\n",sector,sector);
	say("    count:%llx\n",blocksize*BSWAP_32(*(DWORD*)(readbuffer+0x574) ) );
}
int mounthfs(QWORD sector,QWORD* explainfunc,QWORD* cdfunc,QWORD* loadfunc)
{
	block0=sector;

	//返回cd和load函数的地址
	*explainfunc=(QWORD)hfs_explain;
	*cdfunc=(QWORD)hfs_cd;
	*loadfunc=(QWORD)hfs_load;

	//准备好可用的内存地址
	getaddrofbuffer(&readbuffer);
	getaddrofdir(&directorybuffer);
	getaddroffs(&catalogbuffer);

	//读分区前8扇区，总共0x1000字节(其实只要分区内2号和3号扇区)
	readdisk(readbuffer,sector,0,0x8);	//0x1000
	printmemory(readbuffer+0x400,0x200);
	printhead();
	if( *(WORD*)(readbuffer+0x400) == 0x2b48 )say("hfs+\n");
	else if( *(WORD*)(readbuffer+0x400) == 0x5848 )say("hfsx\n");
	blocksize=BSWAP_32( *(DWORD*)(readbuffer+0x428) )/0x200;
	catalogsector=block0+8*BSWAP_32(*(DWORD*)(readbuffer+0x520) );
	say("blocksize:%x\n",blocksize);
	say("catalogsector:%llx\n",catalogsector);

	//读catalog的第0个node，得到nodesize
	readdisk(readbuffer,catalogsector,0,0x8);	//0x1000
	nodesize=BSWAP_16( *(WORD*)(readbuffer+0x20) )/0x200;
	rootnode=BSWAP_32(*(DWORD*)(readbuffer+0x10) );
	firstleafnode=BSWAP_32(*(DWORD*)(readbuffer+0x18) );
	say("nodesize:%x\n",nodesize);
	say("rootnode:%x\n",rootnode);
	say("firstleafnode:%x\n",firstleafnode);

	hfs_cd("/");
	return 0;
}
