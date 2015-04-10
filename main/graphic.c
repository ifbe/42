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
void printbitmap(QWORD start,QWORD end,QWORD typestr)
{	//point(x,320+y,color)

	int x;
	int y;
	QWORD linecount=0;
	QWORD color=start;
	QWORD startx,starty,endx,endy;
	while(1)		//一行行得
	{
		//得到这一轮的开始与结束
		starty=320+start/1024*16;
		endy=starty+16;
		startx=start%1024;
		if(end<linecount*1024)
		{
			//这行就结束的话
			endx=end%1024;
		}
		else
		{
			endx=1023;
		}

		//按照得到的坐标打印就行
		for(x=startx;x<=endx;x++)
		{
			for(y=starty;y<endy;y++)
			{
				point(x,y,color);
			}
		}

		//下一个start，或者已经结束了就break
		linecount++;
		start=linecount*1024;
		if(start>end)break;
	}

	//
}
void d2()
{
	//
	disk(1,1);
	mount(0,0);

	char* p;
	QWORD x,y;
//三.每个分区里面的文件和文件夹
	p=(char*)dir;
	for(y=0;y<2;y++)
	{
		for(x=0;x<0x40;x++)
		{
			anscii(x,10+y,p[0x40*y+x]);
		}
	}

//二.仔细看每一个磁盘由很多分区构成
	//1.最大扇区号是几
	p=(char*)mytable;
	QWORD temp;

	QWORD maxsector=0;
	for(x=0;x<16;x++)
	{
		temp=*(QWORD*)(p+x*0x40+8);
		if(temp == 0)break;
		if(temp>maxsector)maxsector=temp;
	}
	say("%x\n",maxsector);
	QWORD displaymax=1;
	for(x=0;x<64;x++)
	{
		say("%x\n",maxsector);
		if(maxsector <= 0)break;
		else
		{
			maxsector>>=1;
			displaymax<<=1;
		}
	}
	say("%x\n",displaymax);

	//3.标记头尾
	p=(char*)mytable;
	for(y=0;y<8;y++)
	{
		//算出开始扇区结束扇区
		QWORD startsec=*(QWORD*)(p+0x40*y);
		if(startsec==0)break;
		QWORD endsec=*(QWORD*)(p+0x40*y+8);
		QWORD typestr=(QWORD)&p[0x40*y+0x10];

		//
		QWORD start=((double)(startsec*10240)/(double)displaymax);
		hexadecimal(start/8,20,startsec);
		QWORD end=((double)(endsec*10240)/(double)displaymax);
		hexadecimal(end/8,21,endsec);

		say("[%llx,%llx],%s,[%x,%x]\n",startsec,endsec,typestr,start,end);

		//打印硬盘的大致图像
		printbitmap(start,end,typestr);
	}

//一.各种磁盘
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
	for(j=640-32;j<640;j++)
		for(i=0;i<1024;i++)
			point(i,j,0xffffffff);
	for(j=0;j<640-32;j++)
	{
		point(160,j,0);
		point(320,j,0);
	}

	//清屏
	DWORD color=0xff<<(tag*8);
	for(j=0;j<640-32;j++)
		for(i=0;i<1024;i++)
			point(i,j,color);
	for(j=640-32;j<640;j++)
		for(i=tag*160;i<tag*160+160;i++)
			point(i,j,color);

	//名称
	string(0,39,"journal");
	string(20,39,"graphic");
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
				if( (y>640-32) && (x<320) )
				{
					tag=x/160;
				}
				break;
			}
		}
	}//while(1)
}
