#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//libsoft1/
int mountext(char* src,char* dst);
int mountfat(char* src,char* dst);
int mounthfs(char* src,char* dst);
int mountntfs(char* src,char* dst);
int explaingpt(char* src,char* dst);
int explainmbr(char* src,char* dst);
int compare(char*,char*);	//base tool
int hexstring2data(char*,QWORD*);
int buf2arg(char*,BYTE**,BYTE**);
//libsoft0/
int listmemory();
int intomemory(char*);
int readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int writememory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count);
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count);
int say(char* str,...);		//+1
int diary(char* str,...);	//+2
int history(char* str,...);	//+3
int cleanmemory(char* addr,int count);
int printmemory(char* addr,int count);




//每个1M
static char* diskhome;		//+0m
static char* dirhome;		//+1m
static char* fshome;		//+2m
static char* datahome;		//+3m
//3大函数的位置
static char bowl[0x40];		//用来放三个函数的地址
int (*fsexplain)(QWORD id);		//((int (*)(QWORD))(fsexplain))(value);
int (*fscd)(QWORD id);		//((int (*)(QWORD))(fscd))(arg1);
int (*fsload)(QWORD id,QWORD part);	//((int (*)(QWORD,QWORD))(fsload))(arg1,temp*0x100000);




//自己读出来检查这是个什么玩意，不能相信分区表里面的类型
void hello()
{
	//读最开始的64个扇区（0x8000字节）来初步确定
	int ret=readmemory(datahome,0,0,64);
	if(ret<=0)
	{
		//读不出来，可能是内存？
		diary("it's memory?\n");
	}
	else if( *(WORD*)(datahome+0x1fe) == 0xaa55 )
	{
		//末尾有0x55，0xaa这个标志，这个是磁盘，或者要当成磁盘用

		//看看是什么类型，交给小弟处理
		QWORD temp=*(QWORD*)(datahome+0x200);
		if( temp == 0x5452415020494645 )
		{
			explaingpt(datahome,diskhome);
		}
		else
		{
			explainmbr(datahome,diskhome);
		}
	}
	else
	{
		//可能是zip,网络包,或者其他乱七八糟的结构
		diary("don't know\n");
	}
}
int directread(char* arg1)
{
	QWORD value;
	hexstring2data(arg1,&value);
	
	readmemory(datahome,value,0,1);
	printmemory(datahome,0x200);
	diary("above is:%x,%x\n",value,value+7);
}








int explain(char* arg1)
{
	QWORD value;
	hexstring2data(arg1,&value);

	diary("explainer@%llx\n",fsexplain);
	fsexplain(value);
}
void list(char* arg1)
{
	int i;
	char* targetaddr=diskhome;
	QWORD target=0,temp=0;
	//printmemory(diskhome,0x200);

	//想要什么
	if(arg1==0)target=0;
	else if( compare(arg1,"disk") == 0 )target=0x6b736964;	//'disk'
	else if( compare(arg1,"part") == 0 )target=0x2e2e2e6b736964;	//'disk...'
	else if( compare(arg1,"file") == 0 )
	{
		targetaddr=dirhome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(targetaddr,0x200);
	}
	else if( compare(arg1,"fs") == 0 )
	{
		targetaddr=fshome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(targetaddr,0x200);
	}

	//搜到就显示
	for(i=0; i<0x100; i++)
	{
		temp=*(QWORD*)( targetaddr + (i*0x40) );
		if(temp == 0)break;
		if( (target==0) | (temp == target) )
		{
			diary(
				"%d:	(%s,%s)	[%x,%x]	%s\n",
				i,
				targetaddr + (i*0x40),
				targetaddr + (i*0x40) + 8,
				*(QWORD*)(targetaddr + (i*0x40) + 0x10),
				*(QWORD*)(targetaddr + (i*0x40) + 0x18),
				targetaddr + (i*0x40) + 0x20
			);
		}
	}//for
}
void into(char* arg)
{
	int i;
	int number;
	QWORD temp;

	//如果传进来0，仅扫描所有硬盘
	if(arg == 0)
	{
		listmemory();
		printmemory(diskhome,0x200);
		return;
	}

	//如果传进来数量少，并且是纯数字
	if( (arg[1]==0) | (arg[2]==0) )
	{
		//拿数字
		number=0;
		for(i=0;i<10;i++)
		{
			if( arg[i]==0 )break;
			if( (arg[i] >= 0x30) | (arg[i] <= 0x39) )
			{
				number=10*number + arg[i] - 0x30;
			}
		}

		//挂载那个数字对应的分区
		if(number != 0)
		{
			//printmemory(diskhome + number*0x40,0x40);

			//更新一下种类
			temp = *(QWORD*)( diskhome+number*0x40 + 8 );	//type
			if(temp == 0x747865)		//'ext'
			{
				mountext( diskhome+number*0x40 , diskhome );
			}
			else if(temp == 0x746166)	//'fat'
			{
				mountfat( diskhome+number*0x40 , diskhome );
			}
			else if(temp == 0x736668)	//'hfs'
			{
				mounthfs( diskhome+number*0x40 , diskhome );
			}
			else if(temp == 0x7366746e)	//'ntfs'
			{
				mountntfs( diskhome+number*0x40 , diskhome );
			}

			//fsexplain=(void*)( this[4] );
			//fscd=(void*)( this[5] );
			//fsload=(void*)( this[6] );
			return;
		}
	}

	//如果
	cleanmemory(dirhome,0x300000);
	intomemory(arg);
	hello();
}








char* searchthis(char* name)
{
	char* temp=dirhome;
	for(;temp<dirhome+0x1000;temp+=0x40)
	{
		//diary("%llx,%llx\n",*(QWORD*)name,*(QWORD*)temp);
		if( compare( name , (char*)(temp+0x20) ) == 0 )
		{
			printmemory(temp,0x40);
			return temp;
		}
	}

	diary("file not found\n");
	return 0;
}
void ls(char* arg1)
{
	diary("id              size            type                name\n");
	char* addr=dirhome;
	for(;addr<dirhome+0x1000;addr+=0x40)
	{
		if(*(QWORD*)(addr+0x20)==0)break;
		diary
		(
			"%-12llx    %-12llx    %-12x    %-31.31s\n",
			*(QWORD*)(addr+0),	//type
			*(QWORD*)(addr+8),	//id
			//*(QWORD*)(addr+0x10),	//start
			*(QWORD*)(addr+0x18),	//end

			(char*)(addr+0x20)	//name
		);
	}
	//printmemory(dirhome,0x1000);
}
int cd(char* arg1)
{
	char* addr=searchthis(arg1);
	if( addr==0 )return -1;		//没找到

	//change directory
	fscd( *(QWORD*)(addr+0) );
}
int load(char* arg1)
{
	//寻找这个文件名，得到id，type，size
	char* addr=searchthis(arg1);
	if( addr==0 )return -1;

	QWORD id=*(QWORD*)(addr+0);
	QWORD size=*(QWORD*)(addr+8);
	if(size>0x100000)diary("warning:large file\n");

	QWORD temp=0;
	//1m,1m,1m的整块搞
	for(;temp<( size&0xfffffff00000 );temp+=0x100000)
	{
		fsload(id,temp);
		mem2file(datahome,arg1,temp,0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
	}
	//最后的零头(要是size=1m的整数倍，就没有零头)
	if(temp<size)
	{
		fsload(id,temp);
		mem2file(datahome,arg1,temp,size%0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
	}
}








void command(char* buffer)
{
//-----------------1.把收到的命令检查并翻译一遍-------------------
	BYTE* arg0;
	BYTE* arg1;
	buf2arg(buffer,&arg0,&arg1);
	if(arg0==0)return;




//----------------------2.实际的活都找专人来干-----------------------------
	//help
	if(compare( arg0 , "help" ) == 0)
	{
		diary("list ?          (list all known)\n");
		diary("into ?        (choose a disk)\n");
		diary("read ?          (hex print a physical sector)\n");
		diary("mount ?         (choose a partition)\n");
		diary("explain ?       (explain inode/cluster/cnid/mft)\n");
		diary("cd dirname      (change directory)\n");
		diary("load filename   (load this file)\n");
	}

	//the disk
	else if(compare( arg0 , "list" ) == 0)
	{
		list(arg1);
	}
	else if(compare( arg0 , "into" ) == 0)
	{
		into(arg1);
	}
	else if(compare( arg0 , "mount" ) == 0)
	{
		mount(arg1);
	}
	else if(compare( arg0 , "read" ) == 0)
	{
		directread(arg1);
	}
	else if(compare( arg0 , "write" ) == 0)
	{
		//dangerous
		//directread(arg1);
	}

	//the filesystem
	else if(compare( arg0 , "ls" ) == 0)
	{
		ls(arg1);
	}
	else if(compare( arg0 , "explain" ) == 0)
	{
		explain(arg1);
	}
	else if(compare( arg0 , "cd" ) == 0)
	{
		cd(arg1);
	}
	else if(compare( arg0 , "load" ) == 0)
	{
		load(arg1);
	}

}




void initmaster(char* world)
{
	diskhome=world+0;
	dirhome=world+0x100000;
	fshome=world+0x200000;
	datahome=world+0x300000;

	hello();
}
