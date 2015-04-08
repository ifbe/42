#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include "struct.h"



static struct diskinfo* diskinfo;
static struct mytable* mytable;
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）

static BYTE* buf1d;
static BYTE* buf2d;

static BYTE buffer[128];//键盘输入专用
static bufcount=0;



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




void d1()
{
}
void d2()
{
	//
	disk(1,1);
	mount(0,0);

	char* p;
	int x,y;

//每个分区里面的文件和文件夹
	p=(char*)dir;
	for(y=0;y<2;y++)
	{
		for(x=0;x<0x40;x++)
		{
			anscii(x,10+y,p[0x40*y+x]);
		}
	}

//仔细看每一个磁盘由很多分区构成
	//1.最大扇区号是几
	QWORD maxsector=0;
	p=(char*)mytable;
	for(x=0;x<16;x++)
	{
		QWORD temp=*(QWORD*)(p+x*0x40+0x10);
		if(temp == 0)break;
		if(temp>=maxsector)maxsector=temp;
	}
	//2.标记头尾
	p=(char*)mytable;
	for(y=0;y<8;y++)
	{
		hexadecimal(0,20+y,*(QWORD*)(p+0x40*y));
		hexadecimal(8,20+y,*(QWORD*)(p+0x40*y+8));
		for(x=0x10;x<0x20;x++)
		{
			anscii(x,20+y,p[0x40*y+x]);
		}
	}
	for(x=100;x<900;x++)
	{
		point(x+64,320,0xff0000);
	}

//各种磁盘
	p=(char*)diskinfo;
	for(y=0;y<3;y++)
	{
		for(x=0;x<0x40;x++)
		{
			anscii(x,30+y,p[0x40*y+x]);
		}
	}
}
static int tag=1;
void tagcontect()
{
	//画分隔线，写标签名
	int i,j;
	for(j=0;j<32;j++)
		for(i=0;i<1024;i++)
			point(i,j,0xffffffff);
	for(j=0;j<32;j++)
	{
		point(160,j,0);
		point(320,j,0);
	}

	//清屏
	DWORD color=0xff<<(tag*8);
	for(j=0;j<32;j++)
		for(i=tag*160;i<tag*160+160;i++)
			point(i,j,color);
	for(j=32;j<640;j++)
		for(i=0;i<1024;i++)
			point(i,j,color);

	//名称
	string(0,0,"journal");
	string(20,0,"graphic");
}
void printworld()
{
	//input
	writetitle(buffer);

	//基本内容
	tagcontect();

	//具体内容
	if(tag==0) d1();
	if(tag==1) d2();

	//写屏
	writescreen();
}
void main()
{
	//已申请到的内存在哪
	whereisdiskinfo(&diskinfo);
	whereisparttable(&mytable);
	whereisdir(&dir);
	//say("%llx,%llx,%llx\n",(QWORD)diskinfo,(QWORD)mytable,(QWORD)dir);


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
					tag=x/160;
				}
				break;
			}
		}
	}//while(1)
}
