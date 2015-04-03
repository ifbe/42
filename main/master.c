#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


//每0x40字节存放分区的基本信息
struct diskinfo
{
	BYTE path[0x20];
	BYTE name[0x20];
};
static struct diskinfo* diskinfo;

//每0x40字节存放分区的基本信息
struct mytable
{
	QWORD startlba;	//[+0,0x7]:起始lba
	QWORD endlba;	//[+0x8,0xf]:末尾lba
	QWORD parttype;	//[+0x10,0x17]:分区类型anscii
	QWORD partname;	//[0x18,0x1f]:分区名字
	QWORD a[4];	//[0x20,0x3f]:unused
};
static struct mytable* mytable;

//每0x40个字节是一个当前目录表
struct dirbuffer
{
	QWORD name;	//[+0,0x7]:起始lba
	QWORD unused;
	QWORD specialid;	//[+0x20,0x2f]:分区类型anscii
	QWORD unused1;
	QWORD type;	//[+0x10,0x1f]:末尾lba
	QWORD unused2;
	QWORD size;	//[0x30,0x3f]:分区名字
	QWORD unused3;
};
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）

//调用的cd和load函数所在的内存地址
static QWORD explainfunc,cdfunc,loadfunc;

//读缓冲区的地址，缓冲区64k大小
static QWORD readbuffer;

//键盘输入专用
static BYTE buffer[128];



//[fedcba9876543210,ffffffffffffffff]，就只需要打印已经认出的分区
//[100,fedcba9876543210)，收到一个地址，那个地方是一堆数字
//[0,ff]，收到数字
static int mount(QWORD in)
{
	if(in >= 0xfedcba9876543210)
	{
		readdisk(readbuffer,0,0,64);
		explainparttable(readbuffer);
		return -1;
	}
	if(in > 0xff) return -2;

	//得到编号，然后得到分区位置，然后挂载
	QWORD type=mytable[in].parttype;
	QWORD start=mytable[in].startlba;
	if( (start==0) )
	{
		say("impossible partition:%llx\n",in);
		return;
	}
	if(type == 0x747865)
	{
		mountext(start,&explainfunc,&cdfunc,&loadfunc);
	}
	else if(type == 0x746166)
	{
		mountfat(start,&explainfunc,&cdfunc,&loadfunc);
	}
	else if(type == 0x736668)
	{
		mounthfs(start,&explainfunc,&cdfunc,&loadfunc);
	}
	else if(type == 0x7366746e)
	{
		mountntfs(start,&explainfunc,&cdfunc,&loadfunc);
	}
	return 1;
}
static int explain(QWORD addr)
{
	//接收到的anscii转数字
	QWORD number;
	anscii2hex(addr,&number);

	//解释(几号文件)
	((int (*)())(explainfunc))(number);
}
static int cd(addr)
{
	say("i am in\n");
	((int (*)())(cdfunc))(addr);
}
static int load(addr)
{
	//寻找这个文件名，主要为了得到size
	int i;
	for(i=0;i<0x40;i++)
	{
		if(compare(addr,(char*)(&dir[i]))==0)break;
	}
	if(i==0x40)
	{
		say("file not found\n");
		return -1;
	}
	say("%-16.16s    %-16llx    %-16llx    %-16llx\n",(char*)(&dir[i]),dir[i].specialid,dir[i].type,dir[i].size);

	//现在分段读取保存
	QWORD totalsize=dir[i].size;
	QWORD temp;
	if(totalsize>0x100000)say("warning:large file\n");
	for(temp=0;temp<totalsize/0x100000;temp++)
	{
		((int (*)())(loadfunc))(addr,temp*0x100000);			//
		mem2file(readbuffer,addr,temp*0x100000,0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
	}
	((int (*)())(loadfunc))(addr,temp*0x100000);			//
	mem2file(readbuffer,addr,temp*0x100000,totalsize%0x100000);		//mem地址，file名字，文件内偏移，写入多少字节
}
static int ls()
{
	int i;
	say("name                special id          type                size\n");
	for(i=0;i<0x40;i++)
	{
		if(dir[i].name==0)break;
		say("%-16.16s    %-16llx    %-16llx    %-16llx\n",(char*)(&dir[i]),dir[i].specialid,dir[i].type,dir[i].size);
	}
	say("\n");
}




void main()
{
	//已申请到的内存在哪
	getaddrofdiskinfo(&diskinfo);
	getaddrofparttable(&mytable);
	getaddrofdir(&dir);
	getaddrofbuffer(&readbuffer);
	say("%llx,%llx,%llx,%llx\n",(QWORD)diskinfo,(QWORD)mytable,(QWORD)dir,readbuffer);

	//初始化一下parttable.c需要的变量
	mount(0xffffffffffffffff);

	while(1)
	{
		//等输入
		waitinput(buffer);

		//把这段里面所有的0x20变成0
		QWORD first,second;
		buf2arg(buffer,&first,&second);
		//say("%llx,%llx\n",first,second);
		//printmemory(buffer,0x80);

		//判断
		if(compare( (char*)first , "exit" ) == 0)
		{
			break;
			//exit
		}
		else if(compare( (char*)first , "disk" ) == 0)
		{
			if(*(DWORD*)second==0xffffffff)
			{
				disk(0xffffffffffffffff);
			}
			else
			{
				int i;
				anscii2hex(second,&i);
				if(i<0xa)
				{
					disk(i);
				}
				else disk(second);
			}
			mount(0xffffffffffffffff);
		}
		else if(compare( (char*)first , "mount" ) == 0)
		{
			if(*(DWORD*)second==0xffffffff)
			{
				mount(0xffffffffffffffff);
			}
			else
			{
				int i;
				anscii2hex(second,&i);
				if(i<0xa)
				{
					mount(i);
				}
				else mount(second);
			}
		}
		else if(compare( (char*)first , "explain" ) == 0)
		{
			explain(second);
		}
		else if(compare( (char*)first , "cd" ) == 0)
		{
			//进入目录(目录名字符串的地址)
			cd(second);
		}
		else if(compare( (char*)first , "load" ) == 0)
		{
			//读出文件(文件名字符串的地址)
			load(second);
		}
		else if(compare( (char*)first , "ls" ) == 0)
		{
			ls();
		}
		else
		{
			say("disk                    (list disks)\n");
			say("disk ?                  (choose a disk)\n");
			say("disk ?:\\\\name.format    (use an image file as disk)\n");
			say("mount                   (list partitions)\n");
			say("mount ?                 (choose a partition)\n");
			say("explain ?               (explain inode/cluster/cnid/mft)\n");
			say("cd dirname              (change directory)\n");
			say("load filename           (load this file)\n");
		}

	}//while(1)循环
}
