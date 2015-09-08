#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


//memory
static QWORD diskhome;		//
static QWORD fshome;		//fat表
	static QWORD fatbuffer;
static QWORD dirhome;		//目录专用
static QWORD datahome;		//一般使用

//disk
static QWORD firstsector;
static QWORD fat0;			//fat表所在扇区
static QWORD fatsize;		//fat表总共的扇区数量
static QWORD cluster0;		//0号簇所在扇区
static QWORD clustersize;	//每个簇的扇区数量


//用了别人的
void printmemory(QWORD addr,QWORD size);
void readmemory(QWORD rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void whereislogicworld(QWORD* in);
void holyshit(QWORD sector,QWORD count,QWORD logicpos,QWORD want,QWORD addr);
void say(char* fmt,...);




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
			//name
			j=0;
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
			//cluster
			QWORD temp;
			temp=((QWORD)(*(WORD*)(rsi+0x14)))<<16; //high
			temp+=(QWORD)(*(WORD*)(rsi+0x1a));  //low
			*(QWORD*)(rdi+0x10)=temp;
			//type
			rdi[0x20]=rsi[0xb];
			//size
			*(QWORD*)(rdi+0x30)=*(DWORD*)(rsi+0x1c);
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
}
//从收到的簇号开始一直读最多1MB，接收参数为目的内存地址，第一个簇号
static int fat16_data(QWORD dest,QWORD cluster)
{
	say("cluster:%x\n",cluster);

	QWORD rdi=dest;
	while(rdi<dest+0x100000)		//大于1M的不管
	{
		//判断退出
		if(cluster<2)break;
		if(cluster==0xfff7){say("bad cluster:%x\n",cluster);break;}
		if(cluster>=0xfff8)break;

		//读一个簇
		readmemory(rdi,cluster0+clustersize*cluster,0,clustersize);

		//准备下一个地址，找下一个簇，全部fat表在内存里不用担心
		rdi+=clustersize*0x200;
		cluster=(QWORD)(*(WORD*)(fatbuffer+2*cluster));
	}

	say("count:%x\n",rdi-dest);
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
	say("reading whole fat table\n");
	readmemory(fatbuffer,fat0,0,0x100);

	say("cd %x\n",fat0+fatsize*2);
	readmemory(datahome,fat0+fatsize*2,0,32);	//0x40000=0x20*0x200
	explaindirectory();

	say("\n");
}
static int fat16_cd(QWORD id)
{
	//清理
	int i;
	BYTE* memory=(BYTE*)(dirhome);
	for(i=0;i<0x40000;i++) memory[i]=0;

	//读取,转换
	fat16_data(datahome,id);
	explaindirectory();

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
	say("whatwewant:%x\n",whatwewant);
	readmemory(fatbuffer,fat0+(whatwewant/0x80),0,0x200);	//每扇区有0x200/4=0x80个，需要fat表所在位置往后
	firstincache=whatwewant;
}
static int fat32_explain()
{
}
//从收到的簇号开始一直读最多1MB，接收参数为目的内存地址，第一个簇号
static void fat32_data(QWORD dest,QWORD cluster)		//destine,clusternum
{
	say("cluster:%x\n",cluster);

	QWORD rdi=dest;
	while(rdi<dest+0x100000)
	{
		readmemory(rdi,cluster0+clustersize*cluster,0,clustersize);
		rdi+=clustersize*0x200;

		//检查缓冲，从检查完的缓冲区里面读一个cluster号
		checkcacheforcluster(cluster);
		cluster=(QWORD)(*(DWORD*)(fatbuffer+4*(cluster%0x10000)));

		//if(cluster<2){say("impossible cluster,bye!%x\n",cluster);return;}
		if(cluster<2)break;
		if(cluster>=0x0ffffff8)break;
		if(cluster==0x0ffffff7){say("bad cluster,bye!%x\n",cluster);break;}
	}
	say("count:%x\n",rdi-dest);
	say("\n");
}
//接收参数：文件名字符串，调用者要的文件内部偏移（以1M为单元）
static void fat32_load(QWORD id,QWORD offset)
{
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

	say("cd root:%x\n",cluster0+clustersize*2);
	readmemory(datahome,cluster0+clustersize*2,0,32);
	explaindirectory();
}
static int fat32_cd(QWORD id)
{
	//清理
	int i;
	BYTE* memory=(BYTE*)(dirhome);
	for(i=0;i<0x40000;i++) memory[i]=0;

	//读取，转换
	fat32_data(datahome,id);
	explaindirectory();

	return 1;
}







/*
void explainfat16head(QWORD in,QWORD out)
{
	//准备本程序需要的变量
	//QWORD firstsector=(QWORD)( *(DWORD*)(datahome+0x1c) );
	fatsize=(QWORD)( *(WORD*)(datahome+0x16) );
	*(QWORD*)(out)=0x16;
	*(QWORD*)(out+0x8)=0x17;
	*(QWORD*)(out+0x10)=fatsize;
	say("fatsize:%x\n",fatsize);

	fat0=(QWORD)( *(WORD*)(datahome+0xe) );
	*(QWORD*)(out+0x40)=0xe;
	*(QWORD*)(out+0x48)=0xf;
	*(QWORD*)(out+0x50)=fat0;
	fat0=firstsector + fat0;
	say("fat0:%x\n",fat0);

	clustersize=(QWORD)( *(BYTE*)(datahome+0xd) );
	*(QWORD*)(out+0x80)=0xd;
	*(QWORD*)(out+0x88)=0xd;
	*(QWORD*)(out+0x90)=clustersize;
	say("clustersize:%x\n",clustersize);

	cluster0=fat0+fatsize*2+32-clustersize*2;
	say("cluster0:%x\n",cluster0);
}
void explainfat32head(QWORD in,QWORD out)
{
	//准备本程序需要的变量
	//QWORD firstsector=(QWORD)( *(DWORD*)(datahome+0x1c) );
	fatsize=(QWORD)( *(DWORD*)(datahome+0x24) );
	*(QWORD*)(out)=0x24;
	*(QWORD*)(out+0x8)=0x27;
	*(QWORD*)(out+0x10)=fatsize;
	say("fatsize:%x\n",fatsize);

	fat0=(QWORD)( *(WORD*)(datahome+0xe) );
	*(QWORD*)(out+0x40)=0xe;
	*(QWORD*)(out+0x48)=0xf;
	*(QWORD*)(out+0x50)=fat0;
	fat0=firstsector + fat0;
	say("fat0:%x\n",fat0);

	clustersize=(QWORD)( *(BYTE*)(datahome+0xd) );
	*(QWORD*)(out+0x80)=0xd;
	*(QWORD*)(out+0x88)=0xd;
	*(QWORD*)(out+0x90)=clustersize;
	say("clustersize:%x\n",clustersize);

	cluster0=fat0+fatsize*2-clustersize*2;
	say("cluster0:%x\n",cluster0);
}*/
int mountfat(QWORD addr,QWORD which)
{
	//say("%llx\n",(QWORD)fat32_explain);
	//得到本分区的开始扇区位置，再得到3个buffer的位置
	diskhome=addr;
	fshome=addr+0x100000;
		fatbuffer=fshome+0x10000;
	dirhome=addr+0x200000;
	datahome=addr+0x300000;




	//读取pbr
	QWORD* this=(QWORD*)(diskhome+which*0x40);
	firstsector=this[0];
	readmemory(datahome,firstsector,0,1); //pbr
	if( *(WORD*)(datahome+0xb) !=0x200)	//检查分区问题，有就滚
	{
		say("not 512B per sector,bye!\n");
		return -1;
	}
	if( *(BYTE*)(datahome+0x10) != 2)
	{
		say("not 2 fat,bye!\n");
		return -2;
	}




	//检查fat版本
	int similarity=50;
	if( *(WORD*)(datahome+0x11) == 0) similarity++;         //fat32为0
	else similarity--;
	if( *(WORD*)(datahome+0x16) ==0) similarity++;         //fat32为0
	else similarity--;
	if(similarity==48)		//这是fat16
	{
		//上报3个函数的地址
		say("fat16\n");
		this[4]=(QWORD)fat16_explain;
		this[5]=(QWORD)fat16_cd;
		this[6]=(QWORD)fat16_load;

		//
		//explainfat16head(datahome,out);

		//change directory /
		fat16_root();
	}
	else if(similarity==52)		//这是fat32
	{
		//上报3个函数的地址
		say("fat32\n");
		this[4]=(QWORD)fat32_explain;
		this[5]=(QWORD)fat32_cd;
		this[6]=(QWORD)fat32_load;

		//
		//explainfat32head(datahome,out);

		//change directory /
		fat32_root();
	}
	else
	{
		say("seem not fatxx,bye!\n");
		return -3;
	}

	return 0;
}

