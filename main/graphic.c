#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include "struct.h"



static struct diskinfo* diskinfo;
static struct mytable* mytable;
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）


static QWORD readbuffer;//读缓冲区的地址，缓冲区64k大小

static BYTE buffer[128];//键盘输入专用
static bufcount=0;

static QWORD explainfunc,cdfunc,loadfunc;//调用的cd和load函数所在的内存地址




void changedisk(QWORD in)
{
	if(*(DWORD*)in==0xffffffff)
	{
		disk(0xffffffffffffffff);
	}
	else
	{
		int i;
		anscii2hex(in,&i);
		if(i<0xa)
		{
			disk(i);
		}
		else disk(in);
	}
}
//if(in=0)：只读取分区表
//else if(收到的地址里面是0xffffffffffffffff)：只读取分区表
//else if(收到的地址里面是数字)：挂载这一分区
static int mount(QWORD in)
{
	if(in<0x80)
	{
		readdisk(readbuffer,0,0,64);
		explainparttable(readbuffer);
		return -1;
	}

	int i;
	anscii2hex(in,&i);
	if(i>0xa)
	{
		readdisk(readbuffer,0,0,64);
		explainparttable(readbuffer);
		return -2;
	}

	//得到编号，然后得到分区位置，然后挂载
	QWORD type=mytable[i].parttype;
	QWORD start=mytable[i].startlba;
	if( (start==0) )
	{
		say("impossible partition:%llx\n",i);
		return -3;
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
void help()
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
