#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//用了别人的
int systemread(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int systemwrite(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int cleverread(QWORD,QWORD,QWORD,	char*,QWORD,QWORD);
//
void printmemory(char* addr,int size);
void say(char* fmt,...);




//memory
static char* fshome;			//[0x100000,0x10ffff]
	static char* pbr;			//[0x110000,0x11ffff]
	static char* mft0;			//[0x120000,0x12ffff]
	static char* mftbuffer;		//[0x140000,0x1fffff]
static char* dirhome;			//[0x200000,0x2fffff]
static char* datahome;			//[0x300000,0x3fffff]
//disk
static QWORD ntfssector;
static QWORD clustersize;
static QWORD mftcluster;
//ntfs...............
static QWORD pwd[10];
static int ntfspwd;




//传进来那个run的地址，
//返回扇区号和扇区数量
void explainrun(char* runaddr,long long* offset,long long* count)
{
	//变量
	long long temp;
	int i;

	BYTE* run=(BYTE*)runaddr;
	BYTE data= run[0];
	BYTE low4bit=0xf & data;
	BYTE high4bit=data >>4;

	//簇数
	temp=0;
	for(i=low4bit;i>0;i--)
	{
		temp=(temp<<8) + run[i];
	}
	count[0] = temp*clustersize*0x200;

	//簇号
	i=low4bit+high4bit;
	if( (run[i] < 0x80) | (offset[0]==0) )	//正数 或者 传进来的是第一个run
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
	offset[0] = offset[0] + temp*clustersize;
}




//目的地是哪里，datarun那一串数字在哪里，你要的是哪里
void datarun(char* targetaddr,char* runaddr,QWORD want,QWORD max)
{
	//变量们
	long long offset=0;
	long long count=0;
	QWORD logicpos=0;
	max=max;		//kill warning currently

	//printmemory(runaddr,0x40);
	while(1)
	{
		BYTE data= *(BYTE*)runaddr;
		if(data == 0) break;
		if(want+0x80000<=logicpos)break;

		//拿到这一大块的扇区号和扇区数量
		explainrun(runaddr,&offset,&count);
		say("sector=%llx,count=%llx\n",ntfssector+offset,count);

		//要是这个不要，看看下一个怎么样
		if(logicpos+count>want)
		{
			//读进内存
			//传进去的参数为：这一块的物理扇区号，扇区数，逻辑位置，需求位置，目标位置
			//systemread(rdi,ntfssector+offset,diskaddr,count);
			cleverread
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
char* checkcacheformft(QWORD mftnum)
{
	//say("checkcacheformft:%x\n",mftnum);
	//内存里已经是这几个的话就直接返回
	//0xffffff00:0x100个,0x40000
	QWORD thistime=mftnum&0xffffffffffffff00;
	if(thistime==firstmftincache)
	{
		return mftbuffer+0x400*(mftnum % 0x100);		//0x40000/0x400=0x100个
	}

	//开始找那一块地址并且读取
	say("reloading mft:[%x,%x]\n",thistime,thistime+0xff);
	QWORD offset=*(WORD*)(mft0+0x14);
	while(1)
	{
		if(offset > 0x400) break;

		DWORD property= *(DWORD*)(mft0+offset);
		if(property == 0xffffffff)
		{
			//结束了，mft0里面没有80属性
			say("can't find bakcuped mft\n",0);
			break;
		}
		if(property==0x80)
		{
			//从mft0的datarun中读取我们要的部分mft
			//printmemory(mft0+offset,0x80);

			char* temp=mft0+offset;
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




//输入:(好看的数据)目标位置，(INDX里面诡异的数据)位置，字节数量
//返回:下一次翻译到哪里(现在解释到了哪)
char* explainindex(char* rdi,char* rsi,char* end)
{
	char* buffer=rdi;
	char* temp=rsi;
	int i;

	while(1)
	{
		if( temp >= end ) break;
		if( *(DWORD*)(temp+8) <= 0x18 ) break;

		QWORD mftnum=(*(QWORD*)temp)&0xffffffffffff;
		char* thismft=checkcacheformft(mftnum);
		QWORD offset=*(WORD*)(thismft+0x14);

		//[0x10,0x17]=mft号
		*(QWORD*)(buffer+0x10)=mftnum;
		while(1)
		{
			if(offset > 0x400) break;

			DWORD property= *(DWORD*)(thismft+offset);
			if(property == 0xffffffff)
			{
				//结束了，mft0里面没有80属性
				say("error mft\n",0);
				break;
			}
			if(property==0x30)
			{
				//从mft0的datarun中读取我们要的部分mft
				//printmemory(thismft+offset,0x60);
				char* property30body=thismft+offset;
				property30body += *(WORD*)(property30body+0x14);

				//[0,7]=type
				*(DWORD*)(buffer+0)=0x656c6966;		//'file'
				//*(DWORD*)(buffer+0x4)=*(DWORD*)(property30body+0x38);

				//[8,f]=subtype
				*(QWORD*)(buffer+0x8)=0;

				//[0x18,0x1f]=size
				*(QWORD*)(buffer+0x18)=*(QWORD*)(property30body+0x30);
				break;
			}
			//offset=下一个property地址
			offset += *(DWORD*)(thismft+offset+4);
		}

		//[0x20,0x3f]名字
		for(i=0;i<*(BYTE*)(temp+0x50);i++)
		{
			buffer[0x20+i]= *(char*)(temp+0x52+i*2);

			if(buffer[0x20+i]==0) break;
			if(i>=0x20) break;
		}
		say
		(
			"%llx,%llx,%llx,%s\n",
			*(QWORD*)buffer,*(QWORD*)(buffer+8),*(QWORD*)(buffer+0x10),buffer+0x20
		);

		//下一个文件
		temp+= *(WORD*)(temp+0x8);
		buffer+=0x40;
	}
	//printmemory(rdi,0x200);
	return buffer;
}




//0x10属性头：
//[0,3]:	属性类型	0x10
//[4,7]:	本属性长度
//[8]:		是否为常驻	0
//[9]:		属性名长度（0表示没有）		0
//[a,b]:	属性名开始偏移（没有属性名）	0
//[c,d]:	压缩加密稀疏标志：1=压缩，4000=加密，8000=稀疏
//[e,f]:	属性id
//[10,13]:	属性体长度
//[14,15]:	属性体开始偏移
//[16]:		索引标志
//[17]:		无意义
//0x10属性体：
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
//void explain10(char* add)
//{
//}




//0x20属性：
//*+0x00*/	ATTRIBUTE_TYPE AttributeType;
//*+0x04*/	uint16 Length;
//*+0x06*/	uchar NameLength;
//*+0x07*/	uchar NameOffset;
//*+0x08*/	uint64 StartVcn; // LowVcn
//*+0x10*/	uint64 FileReferenceNumber;
//*+0x08*/	uint16 AttributeNumber;
//*+0x0a*/	uint16 AlignmentOrReserved[3];
//void explain20(char* addr)
//{
//}




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
//void explain30(char* addr)
//{
//}




//0x80属性头：
//[0,3]:	属性类型	0x10
//[4,7]:	本属性长度
//[8]:		是否为常驻	0
//[9]:		属性名长度（0表示没有）		0
//[a,b]:	属性名开始偏移（没有属性名）	0
//[c,d]:	压缩加密稀疏标志：1=压缩，4000=加密，8000=稀疏
//[e,f]:	属性id
//[10,13]:	属性体长度
//[14,15]:	属性体开始偏移
//[16]:		索引标志
//[17]:		无意义
//0x80属性体：
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
void explain80(char* addr,QWORD want)	//file data
{
	if( addr[8] == 0 )
	{
		say("resident80@%x\n",addr);
		DWORD length = *(DWORD*)(addr+0x10);
		BYTE* rsi=(BYTE*)(addr + (QWORD)(*(DWORD*)(addr+0x14)) );
		BYTE* rdi=(BYTE*)datahome;
		DWORD i;
		for(i=0;i<length;i++) rdi[i]=rsi[i];

		return;
	}
	else
	{
		say("non resident80@%x\n",addr);
		datarun(datahome,addr + (*(WORD*)(addr+0x20)) ,want,0x80000);
	}
}




//0x90属性头：
//[0,3]:	属性类型	0x10
//[4,7]:	本属性长度
//[8]:		是否为常驻	0
//[9]:		属性名长度（0表示没有）		0
//[a,b]:	属性名开始偏移（没有属性名）	0
//[c,d]:	压缩加密稀疏标志：1=压缩，4000=加密，8000=稀疏
//[e,f]:	属性id
//[10,13]:	属性体长度
//[14,15]:	属性体开始偏移
//[16]:		索引标志
//[17]:		无意义
//0x90属性体：
void explain90(char* addr)	//index root
{
	addr += *(WORD*)(addr+0x14);	//现在addr=属性体地址=索引根地址
	addr+=0x10;			//现在addr=索引头地址

	QWORD size=(QWORD)( *(DWORD*)(addr+4) );
	addr+=0x10;			//现在addr=第一个索引项地址

	//剩下的事(这块以后要改，排序什么的)
	if( *(BYTE*)(addr+0xc) ==0 )	//是小索引
	{
		explainindex(dirhome,addr,addr+size);
	}
	else				//是大索引
	{
		explainindex(dirhome,addr,addr+size);
	}
}




//0xa0属性头：
//[0,3]:	属性类型	0x10
//[4,7]:	本属性长度
//[8]:		是否为常驻	0
//[9]:		属性名长度（0表示没有）		0
//[a,b]:	属性名开始偏移（没有属性名）	0
//[c,d]:	压缩加密稀疏标志：1=压缩，4000=加密，8000=稀疏
//[e,f]:	属性id
//[10,13]:	属性体长度
//[14,15]:	属性体开始偏移
//[16]:		索引标志
//[17]:		无意义
//0xa0属性体：
void explaina0(char* addr)	//index allocation
{
	//清理dirhome
	BYTE* memory;
	int i;
	memory=(BYTE*)(datahome);
	for(i=0;i<0x100000;i++) memory[i]=0;	//clear [1c0000,1ffff8]
	memory=(BYTE*)(dirhome);
	for(i=0;i<0x100000;i++) memory[i]=0;	//clear [1c0000,1ffff8]

	//读INDX进来
	datarun(datahome,addr + (*(QWORD*)(addr+0x20)) ,0 , 0x100000);
	//printmemory(datahome,0x1000);

	//解释INDX成易懂的格式：名字，编号，类型，大小
	char* p=datahome;
	char* rdi=dirhome;
	while( *(DWORD*)p ==0x58444e49 )	//INDX
	{
		say("INDX@%x,vcn:%x\n",p,*(QWORD*)(p+0x10));
		char* start=p + 0x18 + ( *(DWORD*)(p+0x18) );
		char* end=p + ( *(DWORD*)(p+0x1c) );

		rdi=explainindex(rdi,start,end);
		//printmemory(dirhome,0x200);
		p+=0x1000;
	}
}




//解释mft（mft号，要这个文件里面第几个字节开始的一段）
//如果是文件夹自动做文件夹的事，如果是文件自动读取指定位置
//mft记录：
//*+0x00*/ uint32 Type;        // 固定值'FILE'
//*+0x04*/ uint16 UsaOffset;   // 更新序列号偏移, 与操作系统有关
//*+0x06*/ uint16 UsaCount;    // 固定列表大小Size in words of Update Sequence Number&Array(S)
//*+0x08*/ uint64 Lsn;         // 日志文件序列号(LSN)
//*+0x10*/ uint16 SequenceNumber;   // 序列号(用于记录文件被反复使用的次数)
//*+0x12*/ uint16 LinkCount;        // 硬连接数
//*+0x14*/ uint16 AttributeOffset;  // 第一个属性偏移
//*+0x16*/ uint16 Flags;            // falgs, 00=删除文件,01=正常文件,02=删除目录,03=正常目录
//*+0x18*/ uint32 BytesInUse;       // 文件记录实时大小(字节) 当前MFT表项长度,到FFFFFF的长度+4
//*+0x1C*/ uint32 BytesAllocated;   // 文件记录分配大小(字节)
//*+0x20*/ uint64 BaseFileRecord;   // = 0 基础文件记录 File reference to the base FILE record
//*+0x28*/ uint16 NextAttributeNumber; // 下一个自由ID号
//*+0x2A*/ uint16 Pading;           // 边界
//*+0x2C*/ uint32 MFTRecordNumber;  // windows xp中使用,本MFT记录号
//*+0x30*/ uint32 MFTUseFlags;      // MFT的使用标记
static char here[1024];
void explainmft(QWORD mftnum,QWORD want)
{
	//具体不用管，知道返回值是所求MFT的地址就行
	int i;
	char* mft=checkcacheformft(mftnum);
	if( *(DWORD*)mft !=0x454c4946 )
	{
		say("[mft]wrong:%x\n",mftnum);
		return;
	}

	//mft会被改掉，所以把当前的复制一份到自己家处理
	for(i=0;i<1024;i++)
	{
		here[i] = mft[i];
	}

	//有一个property解释要给property
	int offset=*(WORD*)(here+0x14);
	while(1)
	{
		if(offset > 0x400) break;

		DWORD property= *(DWORD*)(here+offset);
		if(property == 0xffffffff) break;

		switch(property)
		{
			case 0x10:{	//standard information	//权限，时间，硬链接数量
				say("10@%x\n",offset);
				break;
			}
			case 0x20:{	//attribute list//需要多个文件记录时用来描述属性列表
				say("20@%x\n",offset);
				break;
			}
			case 0x30:{	//unicode name//文件名，unicode
				say("30@%x\n",offset);
				break;
			}
			case 0x40:{	//object id	//早起ntfs1.2中为卷版本
				say("40@%x\n",offset);
				break;
			}
			case 0x50:{	//security descriptor
				say("50@%x\n",offset);
				break;
			}
			case 0x60:{	//volume name	//只存在于$volume中
				say("60@%x\n",offset);
				break;
			}
			case 0x70:{	//volume information	//只存在于$volume中
				say("70@%x\n",offset);
				break;
			}
			case 0x80:{	//data
				say("80@%x\n",offset);
				explain80(here+offset,want);
				break;
			}
			case 0x90:{	//index root
				say("90@%x\n",offset);
				explain90(here+offset);
				break;
			}
			case 0xa0:	//index allocation
			{
				say("a0@%x\n",offset);
				explaina0(here+offset);
				break;
			}
			case 0xb0:{	//bitmap
				say("b0@%x\n",offset);
				break;
			}
			case 0xc0:{	//reparse point	//早期ntfs1.2中为符号链接
				say("c0@%x\n",offset);
				break;
			}
			case 0xd0:{	//ea information	//扩充属性信息
				say("d0@%x\n",offset);
				break;
			}
			case 0xe0:{	//ea			//扩充属性
				say("e0@%x\n",offset);
				break;
			}
			case 0xf0:{	//property set		//早期ntfs1.2中才有
				say("f0@%x\n",offset);
				break;
			}
			case 0x100:{	//logged utility stream	//efs加密属性
				say("100@%x\n",offset);
				break;
			}
			default:{
				say("unknown@%x\n",offset);
				break;
			}
		}
		offset += *(DWORD*)(here+offset+4);

	}//while1
}//explainmft








static int ntfs_ls(char* to)
{
	return 0;
}
static int ntfs_cd(QWORD id)
{
	int i=0;
	for(i=0;i<0x10000;i++)dirhome[i]=0;

	explainmft(id,0);
	if(ntfspwd<10) ntfspwd++;
	pwd[ntfspwd]=id;
	return 1;
}
static int ntfs_load(QWORD id,QWORD offset)
{
	explainmft(id,offset);
	if(ntfspwd<10) ntfspwd++;
	pwd[ntfspwd]=id;
	return 1;
}
static int ntfs_store(QWORD id)
{
	return 1;
}
int explainntfshead()
{
	int i;
	QWORD* dstqword=(QWORD*)fshome;

	//clean
	for(i=0;i<0x10000;i++)fshome[i]=0;

	//func ls.cd,load,store
	dstqword[0]=0x636e7566;		//'func'
	dstqword[1]=0;
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=(QWORD)ntfs_ls;
	dstqword[5]=(QWORD)ntfs_cd;
	dstqword[6]=(QWORD)ntfs_load;
	dstqword[7]=(QWORD)ntfs_store;
	dstqword += 8;

	//[d,d]
	clustersize=(QWORD)( *(BYTE*)(pbr+0xd) );
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x7a73756c63;	//'clusz'
	dstqword[2]=0xd;
	dstqword[3]=0xd;
	dstqword[4]=clustersize;
	dstqword += 8;
	say("clustersize:%x\n",clustersize);

	//[0x30,0x37]
	mftcluster= *(QWORD*)(pbr+0x30);
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x756c6374666d;	//'mftclu'
	dstqword[2]=0x30;
	dstqword[3]=0x37;
	dstqword[4]=mftcluster;
	dstqword += 8;
	say("mftcluster:%x\n",mftcluster);

	//[0x44,0x44]
	QWORD indexsize=(QWORD)( *(BYTE*)(pbr+0x44) );
	indexsize=clustersize * indexsize;
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x7a737865646e69;	//'indexsz'
	dstqword[2]=0x30;
	dstqword[3]=0x37;
	dstqword[4]=indexsize;
	dstqword += 8;
	say("indexsize:%x\n",indexsize);

	//保存开头几个mft,然后开始	//32个扇区=16个mft=0x4000
	systemread(mft0,ntfssector+mftcluster*clustersize,0,32);
	//printmemory(mft0,0x400);		//	$Mft
	//printmemory(mft0+0x400*5,0x400);	//	.
	//printmemory(mft0+0x400*7,0x400);	//	$Boot

	return 1;
}








int isntfs(char* addr)
{
	QWORD temp;

	//0x55,0xaa
	temp=*(WORD*)(addr+0x1fe);
	if( temp != 0xaa55 ) return 0;

	//0x4e,0x54,0x46,0x53
	temp=*(DWORD*)(addr+3);
	if( temp != 0x5346544e ) return 0;

	//
	return 0x666666;
}
//描述地址，状态机地址
int mountntfs(QWORD sector,char* addr)
{
	int ret=0;

	//得到本分区的开始扇区位置，再得到3个buffer的位置
	ntfssector=sector;
	fshome=addr+0;	//func,struture,...
		pbr=fshome+0x10000;		//pbr
		mft0=fshome+0x20000;	//mft0
		mftbuffer=fshome+0x40000;	//mftn
	dirhome=addr+0x100000;
	datahome=addr+0x200000;

	//读PBR，检查失败就返回
	ret=systemread(pbr,ntfssector,0,1);
	ret=isntfs(pbr);
	if(ret==0)return -1;

	//解释分区头(拿出并保存几个重要变量)
	ret=explainntfshead();
	if(ret < 0)return ret;

	//cd /
	firstmftincache=0xffffffff;
	pwd[0]=5;
	ntfspwd=0;
	ntfs_cd(5);

	return 1;
}
