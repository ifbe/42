#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//用了别人的
int readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void printmemory(char* addr,QWORD size);
void diary(char* fmt,...);




//memory
static char* fshome;		//fat表
	static char* pbr;
	static char* fatbuffer;
static char* dirhome;		//目录专用
static char* datahome;		//一般使用

//disk
static QWORD firstsector;
static QWORD fat0;			//fat表所在扇区
static QWORD fatsize;		//fat表总共的扇区数量
static QWORD cluster2;		//2号簇所在扇区
static QWORD clustersize;	//每个簇的扇区数量




//[0,0xf]:name
//[0x10,0x1f]:time?
//[0x20,0x2f]:firstcluster
//[0x30,0x3f]:size
static void explaindirectory()
{
	int i,j;
	BYTE* rsi=(BYTE*)(datahome);
	BYTE* rdi=(BYTE*)(dirhome);

	for(i=0;i<0x4000;i++) rdi[i]=0;

	while(rsi<(BYTE*)(datahome+0x2000) )
	{
		if( rsi[0xb] !=0xf ){		//fat ignore
		if( rsi[0] !=0xe5 ){		//not deleted
		if( *(QWORD*)rsi !=0 ){		//have name
			QWORD temp;

			//[0x10,0x17]:type
			temp=rsi[0xb];
			if(temp==0x10)
			{
				*(QWORD*)(rdi+0)=0x726964;    //'dir'
			}
			else
			{
				*(QWORD*)(rdi+0)=0x656c6966+(temp<<32);
			}

			//[0x10,0x17]:cluster
			temp=((QWORD)(*(WORD*)(rsi+0x14)))<<16; //high
			temp+=(QWORD)(*(WORD*)(rsi+0x1a));  //low
			*(QWORD*)(rdi+0x10)=temp;

			//[0x18,0x1f]:size
			*(QWORD*)(rdi+0x18)=*(DWORD*)(rsi+0x1c);

			//[0x20,0x3f]:name
			j=0x20;
			for(i=0;i<8;i++)
			{
				if(rsi[i]!=0x20)
				{
					rdi[j]=rsi[i];
					if( (rdi[j]>='A') && (rdi[j]<='Z') )rdi[j]+=0x20;
					j++;
				}
			}
			if(rsi[8]!=0x20)
			{
				rdi[j]='.';
				j++;
			}
			for(i=8;i<11;i++)
			{
				if(rsi[i]!=0x20)
				{
					rdi[j]=rsi[i];
					if( (rdi[j]>='A') && (rdi[j]<='Z') )rdi[j]+=0x20;
					j++;
				}
			}
			//
			rdi+=0x40;
		}
		}
		}
		rsi+=0x20;
	}
}







static int fat16_explain()
{
	return 1;
}
//从收到的簇号开始一直读最多1MB，接收参数为目的内存地址，第一个簇号
static int fat16_data(char* dest,QWORD cluster)
{
	diary("cluster:%x\n",cluster);

	char* rdi=dest;
	while(rdi<dest+0x80000)		//大于1M的不管
	{
		//判断退出
		diary("->%x\n",cluster);
		if(cluster<2)break;
		if(cluster==0xfff7){diary("bad cluster:%x\n",cluster);break;}
		if(cluster>=0xfff8)break;

		//读一个簇
		readmemory(rdi,cluster2+clustersize*(cluster-2),0,clustersize);

		//准备下一个地址，找下一个簇，全部fat表在内存里不用担心
		rdi+=clustersize*0x200;
		cluster=(QWORD)(*(WORD*)(fatbuffer+2*cluster));
	}

	diary("count:%x\n",rdi-dest);
	return rdi-dest;
}
//接收参数：文件名字符串，调用者要的文件内部偏移（以1M为单元）
static void fat16_load(QWORD id,QWORD offset)
{
//从首簇开始，沿着fat的链表，慢慢挪，直到得到调用者要求的位置对应的簇号
	QWORD cluster=id;
	QWORD temp=0;
	while(1)
	{
		//就是这里，就从这个簇开始
		if(temp==offset)break;
		
		//准备下一个地址，找下一个簇，全部fat表在内存里不用担心
		temp+=clustersize*0x200;
		cluster=(QWORD)(*(WORD*)(fatbuffer+2*cluster));
	}

//然后读
	fat16_data(datahome,cluster);
}
static void fat16_root()
{
	//清理内存
	BYTE* memory=(BYTE*)(datahome);
	int i;
	for(i=0;i<0x40000;i++) memory[i]=0;

	//fat16的
	//文件分配表区最多0xffff个簇记录*每个记录占2个字节<=0x20000=0x100个扇区
	//而数据区最大0xffff个簇记录*每簇0x8000字节(?)<=0x80000000=2G=0x400000个扇区
	diary("reading whole fat table\n");
	readmemory(fatbuffer,fat0,0,0x100);
	//printmemory(fatbuffer,0x1000);

	diary("cd %x\n",fat0+fatsize*2);
	readmemory(datahome,fat0+fatsize*2,0,32);	//0x40000=0x20*0x200
	explaindirectory();

	diary("\n");
}
static int fat16_cd(QWORD id)
{
	//清理
	int i;
	for(i=0;i<0x40000;i++) datahome[i]=0;

	//读取,转换
	if(id<2)id=2;
	fat16_data(datahome,id);
	explaindirectory();

	return 1;
}









//fat32的fat表很大，不能像fat16那样直接全部存进一块32K的内存里
//所以每次查表的时候，要把，被查的表项目，所在的64K块，搬进内存
//0x40000=0x200个扇区=0x10000个簇记录
//所以要读的扇区为：[whatwewant,whatwewant+0x1ff](whatwewant=fat0扇区+(cluster/0x10000)*0x200)
//举例子：
//请求cluster=   0x777，如果内存里就是这第   0大块就返回，否则要把       0号到  0xffff号扔进内存然后记下当前clustercurrent=0
//请求cluster= 0x13578，如果内存里就是这第   1大块就返回，否则要把 0x10000号到 0x1ffff号扔进内存然后记下当前clustercurrent=0x10000
//请求cluster= 0x20000，如果内存里就是这第   2大块就返回，否则要把 0x20000号到 0x2ffff号扔进内存然后记下当前clustercurrent=0x20000
//请求cluster=0x613153，如果内存里就是这第0x61大块就返回，否则要把0x610000号到0x61ffff号扔进内存然后记下当前clustercurrent=0x610000
static QWORD firstincache;
static void checkcacheforcluster(QWORD cluster)
{
	//现在的就是我们要的，就直接返回
	QWORD whatwewant=cluster&0xffffffffffff0000;
	if(firstincache == whatwewant) return;

	//否则，从这个开始，读0xffff个，再记下目前cache里面第一个
	diary("whatwewant:%x\n",whatwewant);
	readmemory(fatbuffer,fat0+(whatwewant/0x80),0,0x200);	//每扇区有0x200/4=0x80个，需要fat表所在位置往后
	firstincache=whatwewant;
}
static int fat32_explain()
{
	return 1;
}
//从收到的簇号开始一直读最多1MB，接收参数为目的内存地址，第一个簇号
static void fat32_data(char* dest,QWORD cluster)		//destine,clusternum
{
	diary("cluster:%x\n",cluster);

	char* rdi=dest;
	while(rdi<dest+0x80000)
	{
		readmemory(rdi,cluster2+clustersize*(cluster-2),0,clustersize);
		rdi+=clustersize*0x200;

		//检查缓冲，从检查完的缓冲区里面读一个cluster号
		checkcacheforcluster(cluster);
		cluster=(QWORD)(*(DWORD*)(fatbuffer+4*(cluster%0x10000)));

		//if(cluster<2){diary("impossible cluster,bye!%x\n",cluster);return;}
		if(cluster<2)break;
		if(cluster>=0x0ffffff8)break;
		if(cluster==0x0ffffff7){diary("bad cluster,bye!%x\n",cluster);break;}
	}
	diary("count:%x\n",rdi-dest);
}
//接收参数：文件名字符串，调用者要的文件内部偏移（以1M为单元）
static void fat32_load(QWORD id,QWORD offset)
{
	if(offset>0x100000)offset=0x100000;
	fat32_data(datahome,id);
}
static void fat32_root()
{
	int i;
	BYTE* memory=(BYTE*)(datahome);
	for(i=0;i<0x40000;i++) memory[i]=0;

	//fat32
	//文件分配表区总共可以有0xffffffff个*每个簇记录占4个字节<=0x400000000=16G=0x2000000个扇区
	//数据区总共0xffffffff个簇*每簇512k<=0x20000000000=2T=0x100000000个扇区
	firstincache=0xffffffff;		//绝对会读一块
	checkcacheforcluster(0);

	diary("cd root:%x\n",cluster2);
	readmemory(datahome,cluster2,0,32);
	explaindirectory();
}
static int fat32_cd(QWORD id)
{
	//清理
	int i;
	for(i=0;i<0x10000;i++) datahome[i]=0;

	//读取，转换
	if(id<2)
	{
		fat32_root();
		return 1;
	}

	fat32_data(datahome,id);
	explaindirectory();
	return 2;
}







void explainfat16head()
{
	//准备本程序需要的变量
	//QWORD firstsector=(QWORD)( *(DWORD*)(pbr+0x1c) );
	QWORD* dstqword=(QWORD*)fshome;

	//func cd
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0x6463;             //'cd'
	dstqword[2]=(QWORD)fat16_cd;
	dstqword += 8;

	//func load
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0x64616f6c;         //'load'
	dstqword[2]=(QWORD)fat16_load;
	dstqword += 8;

	//func explain
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0x6e69616c707865;           //'explain'
	dstqword[2]=(QWORD)fat16_explain;
	dstqword += 8;

	fat0=(QWORD)( *(WORD*)(pbr+0xe) );
	fat0=firstsector + fat0;
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x30746166;		//'fat0'
	dstqword[2]=0xe;
	dstqword[3]=0xf;
	dstqword[4]=fat0;
	dstqword += 8;
	diary("fat0@%x\n",fat0);

	fatsize=(QWORD)( *(WORD*)(pbr+0x16) );
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x657a6973746166;	//'fatsize'
	dstqword[2]=0x16;
	dstqword[3]=0x17;
	dstqword[4]=fatsize;
	dstqword += 8;
	diary("fatsize:%x\n",fatsize);

	cluster2=fat0+fatsize*2+32;
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x32756c63;		//'clu2'
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=cluster2;
	dstqword += 8;
	diary("cluster2@%x\n",cluster2);

	clustersize=(QWORD)( *(BYTE*)(pbr+0xd) );
	dstqword[0]=0x7366;		//'fs'
	dstqword[1]=0x657a6973756c63;	//'clusize'
	dstqword[2]=0xd;
	dstqword[3]=0xd;
	dstqword[4]=clustersize;
	dstqword += 8;
	diary("clustersize:%x\n",clustersize);
}
void explainfat32head()
{
	//准备本程序需要的变量
	//QWORD firstsector=(QWORD)( *(DWORD*)(pbr+0x1c) );
	QWORD* dstqword=(QWORD*)fshome;

	//func cd
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0x6463;             //'cd'
	dstqword[2]=(QWORD)fat32_cd;
	dstqword += 8;

	//func load
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0x64616f6c;         //'load'
	dstqword[2]=(QWORD)fat32_load;
	dstqword += 8;

	//func explain
	dstqword[0]=0x636e7566;         //'func'
	dstqword[1]=0x6e69616c707865;           //'explain'
	dstqword[2]=(QWORD)fat32_explain;
	dstqword += 8;

	fat0=(QWORD)( *(WORD*)(pbr+0xe) );
	fat0=firstsector + fat0;
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x30746166;         //'fat0'
	dstqword[2]=0xe;
	dstqword[3]=0xf;
	dstqword[4]=fat0;
	dstqword += 8;
	diary("fat0@%x\n",fat0);

	fatsize=(QWORD)( *(DWORD*)(pbr+0x24) );
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x657a6973746166;   //'fatsize'
	dstqword[2]=0x24;
	dstqword[3]=0x27;
	dstqword[4]=fatsize;
	dstqword += 8;
	diary("fatsize:%x\n",fatsize);

	cluster2=fat0+fatsize*2;
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x32756c63;         //'clu0'
	dstqword[2]=0;
	dstqword[3]=0;
	dstqword[4]=cluster2;
	dstqword += 8;
	diary("cluster2@%x\n",cluster2);

	clustersize=(QWORD)( *(BYTE*)(pbr+0xd) );
	dstqword[0]=0x7366;             //'fs'
	dstqword[1]=0x657a6973756c63;   //'clusize'
	dstqword[2]=0xd;
	dstqword[3]=0xd;
	dstqword[4]=clustersize;
	dstqword += 8;
	diary("clustersize:%x\n",clustersize);
}
int isfat(char* addr)
{
	int version=24;
	QWORD temp;
	//printmemory(addr,0x200);

	//0x55,0xaa
	temp=*(WORD*)(addr+0x1fe);
	if( temp != 0xaa55 ) return 0;

	//512 bytes per sector
	temp=*(WORD*)(addr+0xb);
	if( temp !=0x200 )
	{
		//diary("not 512B per sector,bye!\n");
		return 0;
	}

	//totally 2 fat tables
	temp=*(BYTE*)(addr+0x10);
	if( temp != 2 )
	{
		//diary("not 2 fat,bye!\n");
		return 0;
	}

	//fat32 or fat16
	if( *(WORD*)(addr+0x11) == 0) version+=4;         //fat32为0
	else version-=4;
	if( *(WORD*)(addr+0x16) ==0) version+=4;         //fat32为0
	else version-=4;

	//version
	if(version==32)return 32;
	else if(version==16)return 16;
	else return 0;
}

//1:那一条0x40字节的地址，2:可以用的8m内存的地址
int mountfat(QWORD sector,char* addr)
{
	int ret;
	firstsector=sector;
	//diary("%llx\n",(QWORD)fat32_explain);

	//得到本分区的开始扇区位置，再得到3个buffer的位置
	fshome=addr+0;
		pbr=fshome+0x10000;
		fatbuffer=fshome+0x20000;
	dirhome=addr+0x100000;
		//rootdir
		//dirdepth1
		//dirdepth2
		//dirdepth3
		//dirdepth4
		//......
	datahome=addr+0x200000;

	//读取pbr，检查种类和版本
	ret=readmemory(pbr,firstsector,0,1); //pbr
	ret=isfat(pbr);
	if(ret==16)		//这是fat16
	{
		//上报3个函数的地址
		explainfat16head();

		//change directory /
		fat16_root();

		return 0;
	}
	else if(ret==32)		//这是fat32
	{
		//上报3个函数的地址
		explainfat32head();

		//change directory /
		fat32_root();

		return 0;
	}
	else
	{
		diary("wrong fat\n");
		return -1;
	}
}
