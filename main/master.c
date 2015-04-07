#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include "struct.h"


static struct diskinfo* diskinfo;
static struct mytable* mytable;
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）

static BYTE buffer[128];//键盘输入专用




static int listdisk()
{
	int i;
	say("path            detail\n");
	for(i=0;i<0x40;i++)
	{
		if(diskinfo[i].path==0)break;
		say("%-16.16s    %-16.16s\n",&diskinfo[i].path,diskinfo[i].name);
	}
	say("\n");
}
static int listpartition()
{
	int i;
	say("partition             detail\n");
	for(i=0;i<0x40;i++)
	{
		if(mytable[i].startlba==0)break;
		say("%-16llx    %-16llx    %-16llx    %-16llx\n",
			mytable[i].startlba,mytable[i].endlba,mytable[i].type,mytable[i].name);
	}
	say("\n");
}
static int listfile()
{
	int i;
	say("name                special id          type                size\n");
	for(i=0;i<0x40;i++)
	{
		if(dir[i].name==0)break;
		say("%-16.16s    %-16llx    %-16llx    %-16llx\n",
			(char*)(&dir[i]),dir[i].specialid,dir[i].type,dir[i].size);
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




void main()
{
	//已申请到的内存在哪
	whereisdiskinfo(&diskinfo);
	whereisparttable(&mytable);
	whereisdir(&dir);
	//say("%llx,%llx,%llx\n",(QWORD)diskinfo,(QWORD)mytable,(QWORD)dir);

	//
	BYTE* arg0;
	BYTE* arg1;
	QWORD choose;
	QWORD value;
	while(1)
	{
		//1.等输入，再把这段里面所有的0x20变成0
		waitinput(buffer);
		buf2arg(buffer,&arg0,&arg1);
		//printmemory(buffer,0x80);


		//2.分析输入，要干什么
		if(*(DWORD*)arg1==0xffffffff){choose=value=0;}		//仅显示信息
		else
		{
			anscii2hex(arg1,&value);
			if(value<0x10)	choose=1;		//传进去0，1，2，3这种数字

			//
			else
			{
				choose=2;		//传进去的是字符串首地址
				value=(QWORD)arg1;
			}
		}
		say("%llx,%llx,%llx\n",arg0,arg1,value);


		//3.具体做什么事
		if(compare( arg0 , "exit" ) == 0)
		{
			break;
			//exit
		}
		else if(compare( arg0 , "ls" ) == 0)
		{
			listfile();
		}
		else if(compare( arg0 , "explain" ) == 0)
		{
			explain(value);
		}
		else if(compare( arg0 , "load" ) == 0)
		{
			load(arg1);			//读出文件(文件名字符串的地址)
		}
		else if(compare( arg0 , "cd" ) == 0)
		{
			cd(arg1);			//进入目录(目录名字符串的地址)
		}
		else if(compare( arg0 , "mount" ) == 0)
		{
			mount(choose,value);
		}
		else if(compare( arg0 , "disk" ) == 0)
		{
			disk(choose,value);
			mount(0,0);		//只检查分区表并打印
		}
		else
		{
			help();
		}

	}//while(1)循环
}
