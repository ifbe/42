#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//servent
QWORD prelibation(char*);
int mount(char*);
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
//libboot1
int cleanmemory(char* addr,int count);
int printmemory(char* addr,int count);
int say(char* str,...);		//+1
int diary(char* str,...);	//+2
int history(char* str,...);	//+3
//libboot0
int platformread(char*);
int platformwrite(char*);




//每个1M
static char* diskhome;		//+0m
static char* fshome;		//+1m
static char* dirhome;		//+2m
static char* datahome;		//+3m
//3大函数的位置
int (*fsexplain)(QWORD id);	//((int (*)(QWORD))(fsexplain))(value);
int (*fscd)(QWORD id);		//((int (*)(QWORD))(fscd))(arg1);
int (*fsload)(QWORD id,QWORD part);	//((int (*)(QWORD,QWORD))(fsload))(arg1,temp*0x100000);




//physical function
static void masterlist(char* arg1)
{
	QWORD temp=0;
	int i,j;
	char buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	QWORD target=0;
	char* addr=diskhome;
	//printmemory(diskhome,0x200);

	//想要什么
	if(arg1==0)target=0;
	else if( compare(arg1,"disk") == 0 )target=0x6b736964;	//'disk'
	else if( compare(arg1,"part") == 0 )target=0x74726170;	//'part'
	else if( compare(arg1,"func") == 0 )
	{
		addr=fshome;
		target=0x636e7566;	//'func'
	}
	else if( compare(arg1,"fs") == 0 )
	{
		addr=fshome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(addr,0x200);
	}
	else if( compare(arg1,"file") == 0 )
	{
		addr=dirhome;
		target=0;
		//target=0x656c6966;		//'file'
		//printmemory(addr,0x200);
	}
	else if( compare(arg1,"0") == 0 )
	{
		addr=diskhome;
		target=0;
	}
	else if( compare(arg1,"1") == 0 )
	{
		addr=fshome;
		target=0;
	}
	else if( compare(arg1,"2") == 0 )
	{
		addr=dirhome;
		target=0;
	}
	else if( compare(arg1,"3") == 0 )
	{
		addr=datahome;
		target=0;
	}

	//搜到就显示
	for(i=0; i<0x400; i++)		//0x40*0x400=0x10000
	{
		temp=*(QWORD*)( addr + (i*0x40) );
		if(temp == 0)break;
		if( (target==0) | (temp == target) )
		{
			//[+0]:type
			diary("(%-4s," , addr+(i*0x40) );

			//[+8]:id
			*(QWORD*)buf=*(QWORD*)(addr+(i*0x40)+0x8);
			temp=0;
			for(j=0;j<8;j++)
			{
				//[1,0x1f]:wrong
				if(buf[j]>0 && buf[j]<0x20) temp++;

				//[0x80,0xff]:wrong
				if(buf[j]>=0x80) temp++;
			}
			if(temp==0) diary("%4s)	",buf);
			else diary("%4llx)	",*(QWORD*)buf);

			//[+10]:start
			diary("[%-4llx,",*(QWORD*)(addr+(i*0x40)+0x10));

			//[+18]:end
			diary("%4llx]	",*(QWORD*)(addr+(i*0x40)+0x18));

			//[+20]:detail
			diary("{%-16s}	",addr+(i*0x40)+0x20);

			//which
			diary("<%d>\n",i);
		}//if
	}//for
}//masterlist
static void masterinto(char* arg)
{
	int ret=0;
	QWORD temp=0;

	//1.如果传进来0，仅扫描所有硬盘
	if(arg == 0)
	{
		listmemory();
		printmemory(diskhome,0x200);
		return;
	}

	//2.如果指向的第一个字节是'/'，说明要一个我们自己的单位
	if(arg[0]=='/')
	{
		return;
	}

	//3.其他情况
	//先清空内存
	cleanmemory(diskhome+0x100000,0x300000);

	//选中这个“东西”
	intomemory(arg);

	//读最开始64个扇区(0x200*0x40=0x8000)
	ret=readmemory(datahome,0,0,64);

	//喊仆人来检查“东西”种类
	temp=prelibation(datahome);

	//读分区表
	if(temp==0x747067) explaingpt(datahome,diskhome);
	else if(temp==0x72626d) explainmbr(datahome,diskhome);
	else diary("i don't know it\n");
}
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








//logical function
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
		//checktype
		if(arg1 == 0)
		{
			//physical(master)
			diary("help ?		(list all known)\n");
			diary("list ?		(list all known)\n");
			diary("into ?		(choose a disk)\n");
			diary("read ?		(hex print a sector)\n");
			diary("write ?		(no)\n\n");

			//logical(servent)
			diary("mount ?		(no)\n");
			diary("ls ?		(list file)\n");
			diary("cd ?		(change directory)\n");
			diary("load ?		(load this file)\n");
			diary("store ?		(store this file)\n");
			return;
		}

		//explain memory
		else
		{
			char* value;
			hexstring2data(arg1,&value);
			prelibation(value);
		}
	}
	//physical 1
	ret=compare( arg0 , "list" );
	if(ret==0)
	{
		masterlist(arg1);
		return;
	}
	//physical 2
	ret=compare( arg0 , "into" );
	if(ret==0)
	{
		masterinto(arg1);
		return;
	}
	//physical 3
	ret=compare( arg0 , "read" );
	if(ret==0)
	{
		masterread(arg1);
		return;
	}
	//physical 4
	ret=compare( arg0 , "write" );	//dangerous
	if(ret==0)
	{
		masterwrite(arg1);
		return;
	}




	//logical 0
	ret=compare( arg0 , "mount");
	if(ret==0)
	{
		QWORD value;
		hexstring2data(arg1,value);
		mount(value);
		return;
	}
	//logical 1
	ret=compare( arg0 , "ls");
	if(ret==0)
	{
		ls(arg1);
		return;
	}
	//logical 2
	ret=compare( arg0 , "cd" );
	if(ret==0)
	{
		cd(arg1);
		return;
	}
	//logical 3
	ret=compare( arg0 , "load" );
	if(ret==0)
	{
		load(arg1);
		return;
	}
	//logical 4
	//ret=compare( arg0 , "store" );	//very dangerous
	//if(ret==0)
	//{
		//store(arg1);
		//return;
	//}
}




void initmaster(char* world)
{
	//4块区域，每块1兆
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//最开始看着谁
	masterinto(diskhome+0x20);
}
