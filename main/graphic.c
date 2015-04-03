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
	unsigned long long name;	//[+0,0x7]:起始lba
	unsigned long long unused;
	unsigned long long specialid;	//[+0x20,0x2f]:分区类型anscii
	unsigned long long unused1;
	unsigned long long type;	//[+0x10,0x1f]:末尾lba
	unsigned long long unused2;
	unsigned long long size;	//[0x30,0x3f]:分区名字
	unsigned long long unused3;
};
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）

//读缓冲区的地址，缓冲区64k大小
static QWORD readbuffer;

//键盘输入专用
static BYTE buffer[128];
static bufcount=0;

//调用的cd和load函数所在的内存地址
static QWORD explainfunc,cdfunc,loadfunc;




//[fedcba9876543210,ffffffffffffffff]，就只需要打印已经认出的分区
//[0,fedcba9876543210)，收到一个地址，那个地方是一堆数字
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




//
static int tag=0;
void printworld()
{
	writetitle(buffer);
	//清屏，上面一排，分隔线
	int i,j;
	for(i=0;i<640;i++)
		for(j=0;j<32;j++)
			point(i,j,0xffffffff);
	for(j=0;j<32;j++)
	{
		point(80,j,0);
		point(160,j,0);
		point(240,j,0);
		point(320,j,0);
	}

	//本程序的大概标签式结构，标签含义，标签内的颜色
	DWORD color=0xfeffefcf>>(tag*5);
	for(i=tag*80;i<tag*80+80;i++)
		for(j=0;j<32;j++)
			point(i,j,color);
	for(i=0;i<640;i++)
		for(j=32;j<640;j++)
			point(i,j,color);
	string(0,0,"1.disk");
	string(10,0,"2.part");
	string(20,0,"3.file");
	string(30,0,"4.detail");

	//写屏
	writescreen();
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
		//1.这次显示啥
		printworld();

		//2.等事件
		DWORD type=0;
		DWORD key=0;
		waitevent(&type,&key);
		//say("%x\n",key);

		//3.干啥事
		switch(type)
		{
			case 0:return;
			case 1:
			{
				if(key==0x1b)return;
				else if(key==0x8)
				{
					if(bufcount!=0)
					{
						bufcount--;
						buffer[bufcount]=0;
					}
				}
				else
				{
					buffer[bufcount]=key&0xff;
					bufcount++;
				}
				break;
			}
			case 3:
			{
				int x=key&0xffff;
				int y=(key>>16)&0xffff;
				say("(%d,%d)\n",x,y);

				//
				if( (y<48) && (x<320) )
				{
					tag=x/80;
				}
				break;
			}
		}
	}//while(1)
}
