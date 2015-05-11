#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long


//-----------------------------global variety--------------------------------
//[buffer0][buffer1][buffer2]......[buffer15]
static QWORD realworld;

	//1个硬盘(或者虚拟磁盘文件) = 很多个分区(ext/fat/hfs/ntfs)
	static QWORD buffer0;
	//1个分区(或者某格式的文件) = 很多个区域(头/索引区/数据区/尾)
	static QWORD buffer1;
	//1个索引(mft/inode/btnode) = 很多信息(名字，唯一id，时间，扇区位置等)
	static QWORD buffer2;
	//[+0x30000,+0xfffff]:未用
	static QWORD buffer3;

//[0m,1m)[1m,2m)[2m,3m)[3m,4m)
static QWORD logicworld;

	//往这儿读，只临时用一下（当心别人也用）
	static QWORD readbuffer;
	//名，id，种类，大小
	static QWORD dirbuffer;
	//缓存几千几万个fat/mft/btnode/inode
	static QWORD fsbuffer;
	//[0x300000,0x3fffff]:未用

//硬盘信息
static BYTE* diskinfo;

//log位置
static BYTE* logbuf;

//键盘输入
static BYTE buffer[128];//键盘输入专用
static bufcount=0;

//显示什么
static int tag=1;			//主体显示啥
static int complex=0;		//主体华丽程度
//---------------------------global variety----------------------------








//--------------------------------detail:log--------------------------------
void printlog0()
{
	//背景
	int x,y;
	unsigned int color,i=0;
	for(y=0;y<640-32;y++)
	{
		for(x=0;x<768;x++)
		{
			point(x,y,0xcccccccc);
		}
	}
	for(y=640-32;y<640-16;y++)
	{
		color=0xcc+0x11111100*((15-i)*3/4);		//绿
		for(x=0;x<32-i;x++)
		{
			point(x,y,color);
		}
		i++;
	}

	//内容
	QWORD offsety=*(DWORD*)(logbuf+0xffff0);
	int linenum=offsety/0x80;
	if(offsety<0x80*36)		//[0,0x80*35]
	{
		for(y=0;y<linenum;y++)
		{
			string(0,y,logbuf+0x80*y);
		}
	}
	else
	{
		for(y=0;y<36;y++)
		{
			string(0,y,logbuf+offsety+0x80*(y-36));
		}
	}

	//键盘输入区
	for(x=0;x<768-16;x++)
		for(y=640-64;y<640-48;y++)
			point(x,y,0xffffffff);
	string(0,36,buffer);

	//框框
	for(x=0;x<768;x++)
	{
		point(x,0,0xcc);
	}
	for(x=32;x<768;x++)
	{
		point(x,640-33,0xcc);
	}
	for(y=0;y<640-16;y++)
	{
		point(0,y,0xcc);
	}
	for(y=0;y<640-32;y++)
	{
		point(767,y,0xcc);
	}
	for(x=16;x<32;x++)
	{
		point(x,640-x,0xcc);
	}
}
void printlog1()
{
	
}
void printlog2()
{
	
}
//--------------------------------detail:log--------------------------------




//--------------------------------detail:disk--------------------------------
void printdisk0()
{
	//背景
	int x,y;
	unsigned int color,i=0;
	for(y=0;y<640-32;y++)
	{
		for(x=256;x<1024;x++)
		{
			point(x,y,0xcccccccc);
		}
	}
	for(y=640-32;y<640-16;y++)
	{
		color=0xcc00+0x11110011*((15-i)*3/4);		//绿
		for(x=1024-32+i;x<1024;x++)
		{
			point(x,y,color);
		}
		i++;
	}

	//内容
	char* p=(char*)diskinfo;
	for(i=0;i<10;i++)
	{
		if(*(DWORD*)(diskinfo+0x100*i) == 0)break;

		for(x=512;x<768;x++)
		{
			for(y=64*i;y<64*i+48;y++)
			{
				point(x,y,0xffffffff);
			}
		}
		string(0x40,4*i,diskinfo+0x100*i);
		string(0x40,4*i+1,diskinfo+0x100*i+0x80);
	}

	//选了谁
	for(x=512;x<768;x++)
	{
		for(y=640-64;y<640-32;y++)
		{
			point(x,y,0xffffffff);
		}
	}
	string(0x40,36,diskinfo);

	//框框
	for(x=256;x<1024;x++)
	{
		point(x,0,0xcc00);
	}
	for(x=256;x<1024-32;x++)
	{
		point(x,640-33,0xcc00);
	}
	for(y=0;y<640-32;y++)
	{
		point(256,y,0xcc00);
	}
	for(y=0;y<640-16;y++)
	{
		point(1023,y,0xcc00);
	}
	for(x=0;x<16;x++)
	{
		point(x+1024-32,x+640-32,0xcc00);
	}
}
void printdisk1()
{
}
void printdisk2()
{
	
}
//--------------------------------detail:disk--------------------------------




//------------------------------detail:partition-----------------------------
void printpartition0()
{
	int x,y;
	char* p=(char*)buffer0;
	for(y=0;y<20;y++)
	{
		if(*(QWORD*)(buffer0+y*0x40) == 0)break;
		hexadecimal(0,y,*(QWORD*)(buffer0+y*0x40));
		hexadecimal(0x8,y,*(QWORD*)(buffer0+y*0x40+0x8));
		hexadecimal(0x10,y,*(QWORD*)(buffer0+y*0x40+0x10));
		hexadecimal(0x18,y,*(QWORD*)(buffer0+y*0x40+0x18));
		hexadecimal(0x20,y,*(QWORD*)(buffer0+y*0x40+0x20));
		hexadecimal(0x28,y,*(QWORD*)(buffer0+y*0x40+0x28));
		hexadecimal(0x30,y,*(QWORD*)(buffer0+y*0x40+0x30));
		hexadecimal(0x38,y,*(QWORD*)(buffer0+y*0x40+0x38));
	}
}
void printbitmap(QWORD start,QWORD end,QWORD typestr)
{
	//
	QWORD starty=start/1024*16;
	QWORD startx=start%1024;
	QWORD endy=starty+0xf;
	QWORD endx;
	QWORD color=start*0xfedcba/1024/36;
	QWORD nextline=(start/1024)*1024+1024;
	QWORD this=start;
	QWORD isfirst=1;
	int x,y;

	//具体内容
	while(1)		//一行行得
	{
		//得到这一轮的开始与结束
		starty=this/1024*16;
		startx=this%1024;
		endy=starty+0x10;
		if(end<nextline)
		{
			//这行就结束的话
			endx=end%1024;
		}
		else
		{
			endx=1023;
		}
		say("(%x,%x)->(%x,%x)\n",startx,starty,endx,endy);

		//按照得到的坐标打印就行
		for(y=starty;y<endy;y++)			//左右边界线
		{
			point(startx,y,0xff0000);
			point(endx,y,0xff0000);
		}
		for(x=startx+1;x<endx;x++)		//体
		{
			for(y=starty;y<endy;y++)
			{
				point(x,y,color);
			}
		}
		if(isfirst)			//顶部边界线
		{
			for(x=startx;x<endx;x++)
			{
				point(x,starty,0xff0000);
			}
			isfirst=0;
		}

		//下一个start，或者已经结束了就break
		this=nextline;
		if(this>end)break;
		nextline+=1024;
		if(nextline>1024*36)break;
	}

	//框框
	for(x=startx+1;x<endx;x++)		//下部边界线
	{
		point(x,endy-1,0xff0000);
	}
	for(x=endx;x<1024;x++)
	{
		point(x,endy-16,0xff0000);
	}

	//字符串
	//say("start:%x,end:%x,average:%x\n",start,end,(start+end)/2);
	x=((start+end)/2)%1024/8;
	y=(start+end)/2/1024;
	string(x,y,(char*)typestr);
}
void printpartition1()
{
	int x,y;
	char* p=(char*)buffer0;
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
	p=(char*)buffer0;
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
void printpartition2()
{
	
}
//------------------------------detail:partition-----------------------------




//------------------------------detail:file-----------------------------
void printfile0()
{
	int x,y;
	char* p;

//三.每个分区里面的文件和文件夹
	p=(char*)dirbuffer;
	for(y=0;y<2;y++)
	{
		for(x=0;x<0x40;x++)
		{
			anscii(x,10+y,p[0x40*y+x]);
		}
	}
}
void printfile1()
{
	
}
void printfile2()
{
	
}
//------------------------------detail:file-----------------------------








//-------------------------------display-----------------------------------
void background()
{
	QWORD x,y;

	//清屏
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0x88888888);
		}
	}
}
void content()
{
//2：具体内容
	if(tag==0)
	{
		if(complex==0)
		{
			printlog0();
		}
		else if(complex==1)
		{
			printlog1();
		}
		else
		{
			printlog2();
		}
	}
	else if(tag==15)
	{
		if(complex==0)
		{
			printdisk0();
		}
		else if(complex==1)
		{
			printdisk1();
		}
		else
		{
			printdisk2();
		}
	}
	else if(tag==1)
	{
		if(complex==0)
		{
			printpartition0();
		}
		else if(complex==1)
		{
			printpartition1();
		}
		else
		{
			printpartition2();
		}
	}
	//if(tag==2) printfile();

}
void foreground()
{
	QWORD x,y;

	//[608,639]:低栏颜色
	for(y=640-16;y<640;y++)
		for(x=256;x<768;x++)
			point(x,y,0xffffffff);

	//低栏分界线
	for(y=640-16;y<640;y++)
		for(x=256;x<768;x+=64)
		point(x,y,0);

	//+涂黑选中项
	if( (tag>0) && (tag<15) )
	{
		for(y=640-16;y<640;y++)
			for(x=64*tag+192;x<64*tag+256;x++)
				point(x,y,0x44444444);
	}

	//+写标签名
	string(32,39,"/part");
	string(40,39,"/file");

	//4个角落
	for(x=0;x<16;x++)
	{
		point(x,0,0xffffff);
		point(x,15,0xffffff);
	}
	for(y=0;y<16;y++)
	{
		point(0,y,0xffffff);
		point(15,y,0xffffff);
	}

	//右上角
	for(x=1024-16;x<1024;x++)
		for(y=0;y<16;y++)
			point(x,y,0xff0000);

	//左下角
	for(x=0;x<16;x++)
		for(y=640-16;y<640;y++)
			point(x,y,0xff);

	//右下角
	for(x=1024-16;x<1024;x++)
		for(y=640-16;y<640;y++)
			point(x,y,0xff00);
}
//-------------------------------display-----------------------------------








//-------------------------process input--------------------------------
void clicked(int x,int y)
{
	say("mouse:(%d,%d)\n",x,y);

	//
	if(y>640-16)
	{
		if(x<16)
		{
			tag=0;
		}
		else if(x>1024-16)
		{
			tag=15;
		}
		else if( (x>256) && (x<768) )
		{
			tag=x/64-3;
		}
	}
}
void pressed(int key)
{
	say("keyboard:%x\n",key);
	if(key==0xd)
	{
		command(buffer);

		int i;
		bufcount=0;
		for(i=0;i<128;i++)
		{
			buffer[i]=0;
		}
	}
	else if(key==0x8)		//backspace
	{
		if(bufcount!=0)
		{
			bufcount--;
			buffer[bufcount]=0;
		}
	}
	else if(key==0x40000050)	//left	0x4b
	{
		if(tag>0)tag--;
	}
	else if(key==0x4000004f)	//right	0x4d
	{
		if(tag<3)tag++;
	}
	else
	{
		if(bufcount<0x80)
		{
			buffer[bufcount]=key&0xff;
			bufcount++;
		}
	}
}
//-------------------------process input--------------------------------








void main()
{
	initmaster();

	whereislogbuf(&logbuf);
	whereisdiskinfo(&diskinfo);

	whereisrealworld(&realworld);
	whereislogicworld(&logicworld);
	buffer0=realworld;
	buffer1=realworld+0x10000;
	buffer2=realworld+0x20000;
	buffer3=realworld+0x30000;
	readbuffer=logicworld;
	dirbuffer=logicworld+0x100000;
	fsbuffer=logicworld+0x200000;

	while(1)
	{
		//1.这次显示啥
		background();		//背景
		content();			//内容
		foreground();		//选项
		writescreen();		//写屏

		//2.等事件
		DWORD type=0;
		DWORD key=0;
		waitevent(&type,&key);
		//say("type=%x,key=%x\n",type,key);

		//3.干啥事
		switch(type)
		{
			case 0:return;
			case 1:				//键盘
			{
				if(key==0x1b)return;

				pressed(key);

				break;
			}
			case 2:				//鼠标
			{
				int x=key&0xffff;
				int y=(key>>16)&0xffff;

				if( (x>1024-16) && (y<16) )return;
				clicked(x,y);

				break;
			}
		}//switch
	}//while(1)
}
