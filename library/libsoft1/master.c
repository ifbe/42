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
int buf2arg(char*,char**,char**);
//libsoft0/
int listmemory();
int intomemory(char*);
int readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int writememory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int mem2file(char* memaddr,char* filename,QWORD offset,QWORD count);
int file2mem(char* memaddr,char* filename,QWORD offset,QWORD count);
int platformread(char*);
int platformwrite(char*);
int say(char* str,...);		//+1
int diary(char* str,...);	//+2
int history(char* str,...);	//+3
int cleanmemory(char* addr,int count);
int printmemory(char* addr,int count);




//每个1M
static char* diskhome;		//+0m
static char* fshome;		//+1m
static char* dirhome;		//+2m
static char* datahome;		//+3m
//3大函数的位置
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








//basic function
static int masterread(char* arg1)
{
	QWORD value;

	//nothing specified
	if(arg1==0)
	{
		diary("masterread@%llx,world@%llx\n",masterread,diskhome);
		return -1;
	}

	//what is it
	value=0;
	if( arg1[0] < '0' )value++;	//[0,0x2f]:wrong
	if( arg1[0] > 'f' )value++;	//[0x67,0xff]:wrong
	if( arg1[0] > '9' )		//[0x3a,0x60]:wrong
	{
		if( arg1[0] < 'a' )value++;
	}

	//default,read chosen memory/port/disk/socket...
	if(value==0)
	{
		hexstring2data(arg1,&value);
		readmemory(datahome,value,0,1);
		printmemory(datahome,0x200);
		diary("above is:%llx\n",value);
		return 0;
	}

	else return platformread(arg1);
}
static int masterwrite(char* arg1)
{
	QWORD value;

	//nothing specified
	if(arg1==0)
	{
		diary("masterwrite@%llx,world@%llx\n",masterwrite,diskhome);
		return -1;
	}

	//what is it
	value=0;
	if( arg1[0] < '0' )value++;	//[0,0x2f]:wrong
	if( arg1[0] > 'f' )value++;	//[0x67,0xff]:wrong
	if( arg1[0] > '9' )		//[0x3a,0x60]:wrong
	{
		if( arg1[0] < 'a' )value++;
	}

	//"read memory.400000"
	if(value==0)
	{
		diary("no!i won't write!\n");
		return 0;
	}

	else return platformwrite(arg1);
}
static void masterlist(char* arg1)
{
	int i;
	char* targetaddr=diskhome;
	QWORD target=0,temp=0;
	//printmemory(diskhome,0x200);

	//想要什么
	if(arg1==0)target=0;
	else if( compare(arg1,"disk") == 0 )target=0x6b736964;	//'disk'
	else if( compare(arg1,"part") == 0 )target=0x74726170;	//'part'
	else if( compare(arg1,"func") == 0 )
	{
		targetaddr=fshome;
		target=0x636e7566;	//'func'
	}
	else if( compare(arg1,"fs") == 0 )
	{
		targetaddr=fshome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(targetaddr,0x200);
	}
	else if( compare(arg1,"file") == 0 )
	{
		targetaddr=dirhome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(targetaddr,0x200);
	}
	else if( compare(arg1,"0") == 0 )
	{
		targetaddr=diskhome;
		target=0;
	}
	else if( compare(arg1,"1") == 0 )
	{
		targetaddr=fshome;
		target=0;
	}
	else if( compare(arg1,"2") == 0 )
	{
		targetaddr=dirhome;
		target=0;
	}
	else if( compare(arg1,"3") == 0 )
	{
		targetaddr=datahome;
		target=0;
	}

	//搜到就显示
	for(i=0; i<0x400; i++)		//0x40*0x400=0x10000
	{
		temp=*(QWORD*)( targetaddr + (i*0x40) );
		if(temp == 0)break;
		if( (target==0) | (temp == target) )
		{
			//[+0]:type,[+8]:id,[+10]:start,[+18]:end
			//[+20]:detail
			diary(
			"(%-4s,%4llx)	[%-4llx,%4llx]	{%-32s}<%d>\n",

			targetaddr + (i*0x40),				//0
			*(QWORD*)(targetaddr + (i*0x40) + 0x8),		//8
			*(QWORD*)(targetaddr + (i*0x40) + 0x10),	//10
			*(QWORD*)(targetaddr + (i*0x40) + 0x18),	//18
			targetaddr + (i*0x40) + 0x20,			//20
			i						//which

			);//diary
		}//if
	}//for
}//masterlist
static void masterinto(char* arg)
{
	int i=0;
	int ret=0;
	int number=0;
	QWORD temp=0;

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

			//挂载
			temp = *(QWORD*)( diskhome+number*0x40 + 8 );	//type
			if(temp == 0x747865)		//'ext'
			{
				ret=mountext( diskhome+number*0x40 , diskhome );
			}
			else if(temp == 0x746166)	//'fat'
			{
				ret=mountfat( diskhome+number*0x40 , diskhome );
			}
			else if(temp == 0x736668)	//'hfs'
			{
				ret=mounthfs( diskhome+number*0x40 , diskhome );
			}
			else if(temp == 0x7366746e)	//'ntfs'
			{
				ret=mountntfs( diskhome+number*0x40 , diskhome );
			}
			if(ret<0)return;

			//拿到cd,load,explain等苦工的地址
			fscd = *(void**)( fshome+0x10 );
			fsload = *(void**)( fshome+0x50 );
			fsexplain = *(void**)( fshome+0x90 );
			diary("%llx,%llx,%llx\n",fscd,fsload,fsexplain);
			return;
		}
	}

	//如果
	cleanmemory(diskhome+0x100000,0x200000);
	intomemory(arg);
	hello();
}








//filesystem function
static int ls(char* name)
{
	//ls
	int temp=0;
	if(name==0)
	{
		masterlist("file");
		return 1;
	}

	//search
	for(;temp<0x10000;temp+=0x40)
	{
		//diary("%llx,%llx\n",*(QWORD*)name,*(QWORD*)temp);
		if( compare( name , dirhome+temp+0x20 ) == 0 )
		{
			printmemory(dirhome+temp,0x40);

			//id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
			//explain(id);

			return temp/0x40;
		}
	}

	//failed
	diary("file not found\n");
	return -1;
}
static int cd(char* arg1)
{
	int ret;
	QWORD id;

	//search
	ret=ls(arg1);
	if( ret<0 )return ret;		//没找到

	//change directory
	id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
	return fscd(id);
}
static int load(char* arg1)
{
	//寻找这个文件名，得到id，type，size
	int ret;
	QWORD id;
	QWORD size;
	QWORD temp;

	ret=ls(arg1);
	if( ret==0 )return -1;

	id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
	size=*(QWORD*)(dirhome + 0x40*ret + 0x18);
	if(size>0x100000)diary("warning:large file\n");

	//1m,1m,1m的整块搞
	temp=0;
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
	return 0;
}
static int explain(char* arg1)
{
	QWORD value;
	hexstring2data(arg1,&value);

	diary("explainer@%llx\n",fsexplain);
	return fsexplain(value);
}








void command(char* buffer)
{
//-----------------1.把收到的命令检查并翻译一遍-------------------
	char* arg0;
	char* arg1;
	buf2arg(buffer,&arg0,&arg1);
	if(arg0==0)return;




//----------------------2.实际的活都找专人来干-----------------------------
	//help
	int ret=compare( arg0 , "help" );
	if(ret==0)
	{
		//normal
		diary("list ?		(list all known)\n");
		diary("into ?		(choose a disk)\n");
		diary("read ?		(hex print a physical sector)\n");
		diary("write ?		(no)\n");
		diary("ls ?		(list file)\n");
		diary("cd ?		(change directory)\n");
		diary("load ?		(load this file)\n");
		diary("store ?		(store this file)\n");
		return;
	}

	//
	ret=compare( arg0 , "list" );
	if(ret==0)
	{
		masterlist(arg1);
		return;
	}
	ret=compare( arg0 , "into" );
	if(ret==0)
	{
		masterinto(arg1);
		return;
	}
	ret=compare( arg0 , "read" );
	if(ret==0)
	{
		masterread(arg1);
		return;
	}
	ret=compare( arg0 , "write" );	//dangerous
	if(ret==0)
	{
		masterwrite(arg1);
		return;
	}
	ret=compare( arg0 , "ls");
	if(ret==0)
	{
		ls(arg1);
		return;
	}
	ret=compare( arg0 , "cd" );
	if(ret==0)
	{
		cd(arg1);
		return;
	}
	ret=compare( arg0 , "load" );
	if(ret==0)
	{
		load(arg1);
		return;
	}
	//ret=compare( arg0 , "store" );	//dangerous
	//if(ret==0)
	//{
		//store(arg1);
		//return;
	//}
}




void initmaster(char* world)
{
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	hello();
}
