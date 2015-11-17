#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


//memory
static QWORD diskhome;			//[0x000000,0x0fffff]
static QWORD fshome;			//[0x100000,0x10ffff]
	static QWORD mft0;			//[0x110000,0x11ffff]
	static QWORD mftbuffer;		//[0x120000,0x1fffff]
static QWORD dirhome;			//[0x200000,0x2fffff]
static QWORD datahome;			//[0x300000,0x3fffff]

//disk
static QWORD ntfssector;
static QWORD clustersize;
static QWORD mftcluster;

//ntfs...............
static QWORD pwd[10];
static int ntfspwd;


//用了别人的
void printmemory(QWORD addr,QWORD size);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void whereislogicworld(QWORD* in);
void holyshit(QWORD,QWORD,QWORD,QWORD,QWORD,QWORD);
void diary(char* fmt,...);




//传进来那个run的地址，
//返回扇区号和扇区数量
void explainrun(QWORD runaddr,long long* offset,long long* count)
{
	//变量
	BYTE* run=(char*)runaddr;
	BYTE data= run[0];
	BYTE low4bit=0xf & data;
	BYTE high4bit=data >>4;
	int i;
	long long temp;

	//簇数
	temp=0;
	for(i=low4bit;i>0;i--)
	{
		temp=(temp<<8)+(QWORD)run[i];
	}
	//diary("runcount:%x\n",temp);
	*count=temp*clustersize*0x200;

	//簇号
	i=low4bit+high4bit;
	if( (run[i] < 0x80) | (*offset==0) )	//正数 或者 传进来的是第一个run
	{
		temp=(long long)run[i];		//0x68变成0x0000000000000068
	}
	else					//负数 且 不是第一个run
	{
		temp=(long long)(char)run[i];	//0x98变成0xffffffffffffff98
	}

	for(i=low4bit+high4bit-1;i>low4bit;i--)
	{
		temp=(temp<<8)+(long long)run[i];
	}

	//从pbr开始偏移的扇区数
	*offset=*offset + temp*clustersize;
}




//目的地是哪里，datarun那一串数字在哪里，你要的是哪里
void datarun(QWORD targetaddr,QWORD runaddr,QWORD want,QWORD max)
{
	//变量们
	long long offset=0;
	QWORD count=0;
	QWORD logicpos=0;

	//printmemory(dirhome,0x40);
	//每个run来一次
	while(1)
	{
		BYTE data= *(BYTE*)runaddr;
		//diary("%x\n",data);
		if(data == 0) break;

		//这个以及后面再也没有想要的了
		//diary("%x,%x\n",want+0x100000,logicpos);
		if(want+0x80000<=logicpos)break;

		//拿到这一大块的扇区号和扇区数量
		explainrun(runaddr,&offset,&count);
		printmemory(runaddr,0x10);
		diary("sector=%x,count=%x\n",ntfssector+offset,count);

		//要是这个不要，看看下一个怎么样
		if(logicpos+count>want)
		{
			//读进内存
			//传进去的参数为：这一块的物理扇区号，扇区数，逻辑位置，需求位置，目标位置
			//readmemory(rdi,ntfssector+offset,diskaddr,count);
			holyshit
			(
				ntfssector+offset,count/0x200,logicpos,
				targetaddr,0x80000,want
			);
		}
		//准备下一轮
		runaddr= runaddr + 1 + (QWORD)(data & 0xf) + (QWORD)(data >> 4);
		logicpos+=count;
	}
	//printmemory(dirhome,0x40);
}




//保证包含mftnum的那个1M大小的数据块在我们定义的1M大小的缓冲区里
static QWORD firstmftincache;
QWORD checkcacheformft(QWORD mftnum)
{
	//diary("checkcacheformft:%x\n",mftnum);
	//内存里已经是这几个的话就直接返回
	//0xffffff00:0x100个,0x40000
	QWORD thistime=mftnum&0xffffffffffffff00;
	if(thistime==firstmftincache)
	{
		return mftbuffer+0x400*(mftnum % 0x100);		//0x40000/0x400=0x100个
	}

	//开始找那一块地址并且读取
	diary("reloading mft:[%x,%x]\n",thistime,thistime+0xff);
	QWORD offset=*(WORD*)(mft0+0x14);
	while(1)
	{
		if(offset > 0x400) break;

		DWORD property= *(DWORD*)(mft0+offset);
		if(property == 0xffffffff)
		{
			//结束了，mft0里面没有80属性
			diary("can't find bakcuped mft\n",0);
			break;
		}
		if(property==0x80)
		{
			//从mft0的datarun中读取我们要的部分mft
			//printmemory(mft0+offset,0x80);

			QWORD temp=(QWORD)mft0+offset;
			temp+=(*(QWORD*)(temp+0x20));
			datarun(mftbuffer,temp,thistime*0x400,0x80000);
			firstmftincache=thistime;
			break;
		}

		//offset=下一个property地址
		offset += *(DWORD*)(mft0+offset+4);
	}
	return mftbuffer+0x400*(mftnum % 0x100);		//0x40000/0x400=0x100个
}




void explain80(QWORD addr,QWORD want)	//file data
{
	if( *(BYTE*)(addr+8) == 0 )
	{
		diary("resident80@%x\n",addr);
		DWORD length = *(DWORD*)(addr+0x10);
		BYTE* rsi=(BYTE*)(addr + (QWORD)(*(DWORD*)(addr+0x14)) );
		BYTE* rdi=(BYTE*)datahome;
		int i;
		for(i=0;i<length;i++) rdi[i]=rsi[i];

		return;
	}
	else
	{
		diary("non resident80@%x\n",addr);
		datarun(datahome,addr + (*(WORD*)(addr+0x20)) ,want,0x80000);
	}
}




//输入:(好看的数据)目标位置，(INDX里面诡异的数据)位置，字节数量
//返回:下一次翻译到哪里(现在解释到了哪)
QWORD explainindex(QWORD rdi,QWORD rsi,QWORD rcx)
{
	BYTE* buffer=(BYTE*)rdi;
	QWORD temp=rsi;
	int i;

	while(1)
	{
		if( rsi+rcx <= temp) break;
		if( *(DWORD*)(temp+8) <= 0x18 ) break;

		QWORD mftnum=(*(QWORD*)temp)&0xffffffffffff;
		QWORD thismft=checkcacheformft(mftnum);
		QWORD offset=*(WORD*)(thismft+0x14);

		//[0,7]=mft号
		*(QWORD*)(buffer+0)=mftnum;
		while(1)
		{
			if(offset > 0x400) break;

			DWORD property= *(DWORD*)(thismft+offset);
			if(property == 0xffffffff)
			{
				//结束了，mft0里面没有80属性
				diary("error mft\n",0);
				break;
			}
			if(property==0x30)
			{
				//从mft0的datarun中读取我们要的部分mft
				//printmemory(thismft+offset,0x60);

				QWORD property30body=thismft+offset;
				property30body += *(WORD*)(property30body+0x14);

				//[8,f]=size
				*(QWORD*)(buffer+8)=*(QWORD*)(property30body+0x30);

				//[10,17]=type
				*(QWORD*)(buffer+0x10)=*(DWORD*)(property30body+0x38);
				break;
			}
			//offset=下一个property地址
			offset += *(DWORD*)(thismft+offset+4);
		}

		//[0x20,0x3f]名字
		for(i=0;i<*(BYTE*)(temp+0x50);i++)
		{
			buffer[0x20+i]= *(BYTE*)(temp+0x52+i*2);

			if(buffer[0x20+i]==0) break;
			if(i>=0x20) break;
		}
		diary
		(
			"%llx,%llx,%llx,%s\n",
			*(QWORD*)buffer,*(QWORD*)(buffer+8),*(QWORD*)(buffer+0x10),buffer+0x20
		);

		//下一个文件
		temp+= *(WORD*)(temp+0x8);
		buffer+=0x40;
	}
	//printmemory(rdi,0x200);
	return (QWORD)buffer;
}


void explain90(QWORD addr)	//index root
{
	diary("90@%x\n",addr);

	addr += *(WORD*)(addr+0x14);	//现在addr=属性体地址=索引根地址

	addr+=0x10;			//现在addr=索引头地址
	QWORD size=(QWORD)( *(DWORD*)(addr+4) );

	addr+=0x10;			//现在addr=第一个索引项地址

	//剩下的事(这块以后要改，排序什么的)
	if( *(BYTE*)(addr+0xc) ==0 )	//是小索引
	{
		explainindex(dirhome,addr,size);
	}
	else				//是大索引
	{
		explainindex(dirhome,addr,size);
	}
}


void explaina0(QWORD addr)	//index allocation
{
	//清理dirhome
	BYTE* memory;
	int i;
	memory=(BYTE*)(datahome);
	for(i=0;i<0x100000;i++) memory[i]=0;	//clear [1c0000,1ffff8]
	memory=(BYTE*)(dirhome);
	for(i=0;i<0x100000;i++) memory[i]=0;	//clear [1c0000,1ffff8]

	//读INDX进来
	diary("a0@%x{\n",addr);
	datarun(datahome,addr + (*(QWORD*)(addr+0x20)) ,0 , 0x100000);
	//printmemory(datahome,0x1000);

	//解释INDX成易懂的格式：名字，编号，类型，大小
	QWORD p=datahome;
	QWORD rdi=dirhome;
	while( *(DWORD*)p ==0x58444e49 )	//INDX
	{
		diary("INDX@%x,vcn:%x\n",p,*(QWORD*)(p+0x10));
		QWORD start=p + 0x18 + ( *(DWORD*)(p+0x18) );
		QWORD count=p + ( *(DWORD*)(p+0x1c) )-start;

		rdi=explainindex(rdi,start,count);
		//printmemory(dirhome,0x200);
		p+=0x1000;
	}

	diary("}\n",0);
}




//解释mft（mft号，要这个文件里面第几个字节开始的一段）
//如果是文件夹自动做文件夹的事，如果是文件自动读取指定位置
//mft记录：
//*+0x00*/	uint32 Type;            // 固定值'FILE'
//*+0x04*/	uint16 UsaOffset;       // 更新序列号偏移, 与操作系统有关
//*+0x06*/	uint16 UsaCount;        // 固定列表大小Size in words of Update Sequence Number & Array (S)
//*+0x08*/  uint64 Lsn;             // 日志文件序列号(LSN)
//*+0x10*/  uint16  SequenceNumber;   // 序列号(用于记录文件被反复使用的次数)
//*+0x12*/  uint16  LinkCount;        // 硬连接数
//*+0x14*/  uint16  AttributeOffset;  // 第一个属性偏移
//*+0x16*/  uint16  Flags;            // falgs, 00表示删除文件,01表示正常文件,02表示删除目录,03表示正常目录
//*+0x18*/  uint32  BytesInUse;       // 文件记录实时大小(字节) 当前MFT表项长度,到FFFFFF的长度+4
//*+0x1C*/  uint32  BytesAllocated;   // 文件记录分配大小(字节)
//*+0x20*/  uint64  BaseFileRecord;   // = 0 基础文件记录 File reference to the base FILE record
//*+0x28*/  uint16  NextAttributeNumber; // 下一个自由ID号
//*+0x2A*/  uint16  Pading;           // 边界
//*+0x2C*/  uint32  MFTRecordNumber;  // windows xp中使用,本MFT记录号
//*+0x30*/  uint32  MFTUseFlags;      // MFT的使用标记

//0x10属性：
//*+0x00*/	uint64 CreationTime;         // 创建时间
//*+0x08*/	uint64 ChangeTime;           // 修改时间
//*+0x10*/	uint64 LastWriteTime;        // 最后写入时间
//*+0x18*/	uint64 LastAccessTime;       // 最后访问时间
//*+0x20*/	uint32 FileAttribute;        // 文件属性
//*+0x24*/	uint32 AlignmentOrReserved[3];  // 
//#if 0
//*+0x28*/	uint32 QuotaId;
//*+0x2c*/	uint32 SecurityId;
//*+0x30*/	uint64 QuotaCharge;
//*+0x38*/	USN Usn;
//#endif

//0x20属性：
//*+0x00*/	ATTRIBUTE_TYPE AttributeType;
//*+0x04*/	uint16 Length;
//*+0x06*/	uchar NameLength;
//*+0x07*/	uchar NameOffset;
//*+0x08*/	uint64 StartVcn; // LowVcn
//*+0x10*/	uint64 FileReferenceNumber;
//*+0x08*/	uint16 AttributeNumber;
//*+0x0a*/	uint16 AlignmentOrReserved[3];

//0x30属性：
//*+0x00*/	uint64 DirectoryFile:48;    // 父目录记录号(前个字节)
//*+0x06*/  uint64 ReferenceNumber:16;  // +序列号(与目录相关)
//*+0x08*/	uint64 CreationTime;        // 文件创建时间
//*+0x10*/	uint64 ChangeTime;          // 文件修改时间        
//*+0x18*/	uint64 LastWriteTime;       // MFT更新的时间
//*+0x20*/	uint64 LastAccessTime;      // 最后一次访问时间
//*+0x28*/	uint64 AllocatedSize;       // 文件分配大小
//*+0x30*/	uint64 DataSize;            // 文件实际大小
//*+0x38*/	uint32 FileAttributes;      // 标志,如目录\压缩\隐藏等
//*+0x3C*/	uint32 AlignmentOrReserved; // 用于EAS和重解析
//*+0x40*/	uchar NameLength;      // 以字符计的文件名长度,没字节占用字节数由下一字节命名空间确定
       	// 文件名命名空间, 0 POSIX大小写敏感,1 win32空间,2 DOS空间, 3 win32&DOS空间
//*+0x41*/	uchar NameType;        
//*+0x42*/	wchar Name[1];         // 以Unicode方式标识的文件名

//80属性：
//*+0x00*/	ATTRIBUTE Attribute;  
//*+0x10*/	uint64 StartVcn;     // LowVcn 起始VCN  起始簇号
//*+0x18*/	uint64 LastVcn;      // HighVcn  结束VCN  结束簇号
//*+0x20*/	uint16 RunArrayOffset;    // 数据运行的偏移
//*+0x22*/	uint16 CompressionUnit;   // 压缩引擎
//*+0x24*/	uint32  Padding0;       // 填充
//*+0x28*/	uint32  IndexedFlag;    // 为属性值分配大小(按分配的簇的字节数计算)
//*+0x30*/	uint64 AllocatedSize;   // 属性值实际大小
//*+0x38*/	uint64 DataSize;     // 属性值压缩大小
//*+0x40*/	uint64 InitializedSize;   // 实际数据大小
//*+0x48*/	uint64 CompressedSize;    // 压缩后大小
static BYTE explaining[1024];
void explainmft(QWORD mftnum,QWORD want)
{
	//具体不用管，知道返回值是所求MFT的地址就行
	QWORD mft=checkcacheformft(mftnum);

	//mft会被改掉，所以把当前的复制一份到自己家处理
	int i;
	for(i=0;i<1024;i++)
	{
		explaining[i]=*(BYTE*)(mft+i);
	}
	mft=(QWORD)explaining;

    //不对就滚
	if( *(DWORD*)mft !=0x454c4946 )
	{
		diary("[mft]wrong:%x\n",mftnum);
		return;
	}

	//有一个property解释要给property
	QWORD offset=mft + ( *(WORD*)(mft+0x14) );
	while(1)
	{
		if(offset > mft + 0x400) break;

		DWORD property= *(DWORD*)offset;
		if(property == 0xffffffff) break;

		switch(property)
		{
			case 0x10:{	//standard information
				diary("10@%x\n",offset);
				break;
			}
			case 0x20:{	//attribute list
				diary("20@%x\n",offset);
				break;
			}
			case 0x30:{	//unicode name
				diary("30@%x\n",offset);
				break;
			}
			case 0x40:{	//object id
				diary("40@%x\n",offset);
				break;
			}
			case 0x50:{	//security descriptor
				diary("50@%x\n",offset);
				break;
			}
			case 0x60:{	//volume name
				diary("60@%x\n",offset);
				break;
			}
			case 0x70:{	//volumn information
				diary("70@%x\n",offset);
				break;
			}
			case 0x80:{	//data
				explain80(offset,want);
				break;
			}
			case 0x90:{	//index root
				explain90(offset);
				break;
			}
			case 0xa0:	//index allocation
			{
				explaina0(offset);
				break;
			}
			case 0xb0:{	//bitmap
				diary("b0@%x\n",offset);
				break;
			}
			case 0xc0:{	//reparse point
				diary("c0@%x\n",offset);
				break;
			}
			case 0xd0:{	//ea information
				diary("d0@%x\n",offset);
				break;
			}
			case 0xe0:{	//ea
				diary("e0@%x\n",offset);
				break;
			}
			case 0xf0:{	//property set
				diary("f0@%x\n",offset);
				break;
			}
			case 0x100:{	//logged utility stream
				diary("100@%x\n",offset);
				break;
			}
			default:{
				diary("unknown@%x\n",offset);
				break;
			}
		}
		offset += *(DWORD*)(offset+4);
	}
	diary("\n");
}


static void ntfs_explain(QWORD mftnum)
{
	diary("mft %x\n",mftnum);
	QWORD mft=checkcacheformft(mftnum);
	printmemory(mft,0x400);
}


static int ntfs_cd(QWORD id)
{
	explainmft(id,0);
	if(ntfspwd<10) ntfspwd++;
	pwd[ntfspwd]=id;
}


static int ntfs_load(QWORD id,QWORD offset)
{
	explainmft(id,offset);
	if(ntfspwd<10) ntfspwd++;
	pwd[ntfspwd]=id;
}








void explainntfshead(QWORD in,QWORD fshome)
{
	clustersize=(QWORD)( *(BYTE*)(in+0xd) );
	*(QWORD*)(fshome)=0xd;
	*(QWORD*)(fshome+8)=0xd;
	*(QWORD*)(fshome+0x10)=clustersize;
	diary("clustersize:%x\n",clustersize);

	mftcluster= *(QWORD*)(in+0x30);
	*(QWORD*)(fshome+0x40)=0x30;
	*(QWORD*)(fshome+0x48)=0x37;
	*(QWORD*)(fshome+0x50)=mftcluster;
	diary("mftcluster:%x\n",mftcluster);

	QWORD indexsize=(QWORD)( *(BYTE*)(in+0x44) );
	*(QWORD*)(fshome+0x80)=0x44;
	*(QWORD*)(fshome+0x88)=0x44;
	*(QWORD*)(fshome+0x90)=indexsize;
	indexsize=clustersize * indexsize;
	diary("indexsize:%x\n",indexsize);
}
int mountntfs(QWORD addr,QWORD which)
{
	//得到本分区的开始扇区位置，再得到3个buffer的位置
	diskhome=addr;
	dirhome=addr+0x100000;
	fshome=addr+0x200000;
		mft0=fshome+0x10000;
		mftbuffer=fshome+0x20000;
	datahome=addr+0x300000;

	//上报3个函数的地址
	QWORD* this=(QWORD*)(diskhome+which*0x40);
	ntfssector=this[0];
	this[4]=(QWORD)ntfs_explain;
	this[5]=(QWORD)ntfs_cd;
	this[6]=(QWORD)ntfs_load;
	diary("ntfs sector:%x\n",ntfssector);

	//读PBR，失败就返回,成功就解释分区头(拿出并保存几个重要变量)
	readmemory(datahome,ntfssector,0,1);
	if( *(DWORD*)(datahome+3) != 0x5346544e ) return -1;
	explainntfshead(datahome,fshome);

	//保存开头几个mft
	readmemory((QWORD)mft0,ntfssector+mftcluster*clustersize,0,2);
	//printmemory(mft0,0x400);

	//cd /
	firstmftincache=0xffffffff;		//no mft cache yet
	pwd[0]=5;
	ntfspwd=0;
	ntfs_cd(5);

	return 0;
}
