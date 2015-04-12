#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
#include "struct.h"


//3张表的位置
static struct diskinfo* diskinfo;
static struct mytable* mytable;
static struct dirbuffer* dir;	//dir=“目录名缓冲区”的内存地址（dir[0],dir[1],dir[2]是这个内存地址里面的第0，1，2字节快）
//log位置
static BYTE* logbuf;
//键盘输入
static BYTE buffer[128];//键盘输入专用
static bufcount=0;
//标签
static int tag=0;



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




void printlog()
{
	int x,y;
	//出
	for(y=0;y<30;y++)
	{
		//*(QWORD*)(logbuf+0x80*y)=0x0030313233343536;
		string(0,y,logbuf+0x80*y);
	}

	//入
	for(x=256;x<768;x++)
	{
		for(y=640-64;y<640-48;y++)
		{
			point(x,y,0x88888888);
		}
	}
	string(32,36,buffer);
}
void printdisk()
{
	disk(0,0);

	char* p;
	QWORD x,y;
//一.各种磁盘
	p=(char*)diskinfo;
	for(y=0;y<3;y++)
	{
		for(x=0;x<0x40;x++)
		{
			anscii(x,y,p[0x40*y+x]);
		}
	}
}
void printbitmap(QWORD start,QWORD end,QWORD typestr)
{
	int x;
	int y;

	//
	QWORD starty=start/1024*16;
	QWORD startx=start%1024;
	QWORD endy=starty+0xf;
	QWORD endx;
	QWORD color=start*0xfedcba/1024/36;
	QWORD nextline=(start/1024)*1024+1024;
	QWORD this=start;
	QWORD isfirst=1;
	while(1)		//一行行得
	{
		//得到这一轮的开始与结束
		starty=this/1024*16;
		startx=this%1024;
		endy=starty+15;
		if(end<nextline)
		{
			//这行就结束的话
			endx=end%1024;
		}
		else
		{
			endx=1023;
		}
		for(y=endy-16;y<endy;y++)
		{
			point(startx,y,0xff0000);
			point(endx,y,0xff0000);
		}
		say("(%x,%x)->(%x,%x)\n",startx,starty,endx,endy);

		//按照得到的坐标打印就行
		if(isfirst)
		{
			for(x=startx;x<endx;x++)
			{
				point(x,starty,0xff0000);
			}
			isfirst=0;
		}
		for(x=startx+1;x<endx;x++)
		{
			for(y=starty+1;y<=endy+1;y++)
			{
				point(x,y,color);
			}
		}

		//下一个start，或者已经结束了就break
		this=nextline;
		if(this>end)break;
		nextline+=1024;
		if(nextline>1024*36)break;
	}
	for(x=startx+1;x<endx;x++)
	{
		point(x,endy+1,0xff0000);
	}

	//
	//say("start:%x,end:%x,average:%x\n",start,end,(start+end)/2);
	x=((start+end)/2)%1024/8;
	y=(start+end)/2/1024;
	string(x,y,(char*)typestr);
}
void printpartition()
{
	//
	disk(1,1);
	mount(0,0);

	char* p;
	QWORD x,y;

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
		//say("%x\n",maxsector);
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
		QWORD start=((double)(startsec*1024*36)/(double)displaymax);
		//hexadecimal(start/8,20,startsec);
		QWORD end=((double)(endsec*1024*36)/(double)displaymax);
		//hexadecimal(end/8,21,endsec);

		say("[%llx,%llx],%s,[%x,%x]\n",startsec,endsec,typestr,start,end);

		//打印硬盘的大致图像
		printbitmap(start,end,typestr);
	}

}
void printfile()
{
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
}
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
	DWORD color;
	//color=0xff<<(tag*3);
	color=0x44444444;
	for(j=0;j<640-32;j++)
		for(i=0;i<1024;i++)
			point(i,j,color);
	for(j=640-32;j<640;j++)
		for(i=tag*160;i<tag*160+160;i++)
			point(i,j,color);

	//名称
	string(0,39,"journal");
	string(20,39,"disk");
	string(40,39,"partition");
	string(60,39,"file");
}
void printworld()
{
	//基本内容
	tagcontect();

	//具体内容
	if(tag==0) printlog();
	if(tag==1) printdisk();
	if(tag==2) printpartition();
	if(tag==3) printfile();

	//写屏
	writescreen();
}
void main()
{
	//已申请到的内存在哪
	whereisdiskinfo(&diskinfo);
	whereisparttable(&mytable);
	whereisdir(&dir);

	whereislogbuf(&logbuf);
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
				if( (y>640-32) && (x<640) )
				{
					tag=x/160;
				}
				break;
			}
		}
	}//while(1)
}
