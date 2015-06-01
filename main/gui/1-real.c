#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long

static QWORD buffer0;

static int tag=0;
static int complex=0;		//主体华丽程度




void real0init()
{
	whereisrealworld(&buffer0);
}
void real0background()
{
	int x,y;
	unsigned int color,i=0;
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
		{
			point(x,y,0xe0e0e0);
		}
	}
	//上下
	for(y=0;y<16;y++)
	{
		color=(QWORD)y*0x0e0e0e;

		for(x=y;x<1024-y;x++)
		{
			point(x,y,color);
			point(x,639-y,color);
		}
	}
	//左右
	for(x=0;x<16;x++)
	{
		color=(QWORD)x*0x0e0e0e;

		for(y=x;y<640-x;y++)
		{
			point(x,y,color);
			point(1023-x,y,color);
		}
	}
	/*
	QWORD x,y;
	unsigned int color,i=0;
	for(y=0;y<640;y++)
	{
		for(x=0;x<1024;x++)
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
	*/
	//框框
	for(x=0;x<1024;x++)
	{
		point(x,0,0xcc);
		point(x,639,0xcc);
	}
	for(y=0;y<640;y++)
	{
		point(0,y,0xcc);
		point(1023,y,0xcc);
	}
	/*
	for(x=16;x<32;x++)
	{
		point(x,639-x,0xcc);
	}
	*/
/*
	//左上
	for(y=0;y<32;y++)
		for(x=0;x<32-y;x++)
			point(x,y,0);
	//右上
	for(y=0;y<32;y++)
		for(x=1024-32+y;x<1024;x++)
			point(x,y,0);
	//左下
	//for(x=0;x<32;x++)
	//	for(y=640-32;y<640;y++)
	//		point(x,y,0xff);
	//右下
	for(y=0;y<32;y++)
		for(x=1024-y;x<1024;x++)
			point(x,640-32+y,0);
*/
	//[608,639]:低栏颜色与低栏分界线
	//for(y=640-16;y<640;y++)
	//	for(x=256;x<768;x++)
	//		point(x,y,0xffffffff);
	for(y=640-16;y<640;y++)
		for(x=256;x<=768;x+=64)
			point(x,y,0);

	//+涂黑选中项
	for(y=640-16;y<640;y++)
		for(x=64*tag+256;x<64*tag+320;x++)
			point(x,y,0x44444444);

	//+写标签名
	string(24,39,"-");
	string(32,39,"   0    ");
	string(40,39,"   1    ");
	string(48,39,"   2    ");
	string(56,39,"   3    ");
	string(64,39,"   4    ");
	string(72,39,"   5    ");
	string(80,39,"   6    ");
	string(88,39,"   7    ");
	string(96,39,"       +");

	//框框
	for(x=0;x<1024;x++)
	{
		point(x,32,0xff);
		point(x,48,0xff);
		point(x,64,0xff);
		point(x,80,0xff);
		point(x,96,0xff);
		point(x,112,0xff);
		point(x,128,0xff);
	}
	for(y=0;y<512;y++)
	{
		point(128,y,0xff);
		point(256,y,0xff);
		point(384,y,0xff);
		point(512,y,0xff);
		point(640,y,0xff);
		point(768,y,0xff);
		point(896,y,0xff);
	}
	string(4+0,0,"start");
	string(4+16,0,"end");
	string(4+32,0,"type");
	string(4+48,0,"reserved");
	string(4+64,0,"explain");
	string(4+80,0,"cd");
	string(4+96,0,"load");
	string(4+112,0,"reserved");

	//右箭头
	for(x=1024-32;x<1024;x++)
		for(y=320-(1024-x)*4;y<320+(1024-x)*4;y++)
			point(x,y,0xffffffff);
	string(0x7c,20,"main");
}




void printpartition0()
{
	int x,y;

	for(y=0;y<20;y++)
	{
		if(*(QWORD*)(buffer0+y*0x40) == 0)break;
		hexadecimal(0,y+2,*(QWORD*)(buffer0+y*0x40));
		hexadecimal(16,y+2,*(QWORD*)(buffer0+y*0x40+0x8));
		string(32,y+2,buffer0+y*0x40+0x10);
		hexadecimal(48,y+2,*(QWORD*)(buffer0+y*0x40+0x18));
		hexadecimal(64,y+2,*(QWORD*)(buffer0+y*0x40+0x20));
		hexadecimal(80,y+2,*(QWORD*)(buffer0+y*0x40+0x28));
		hexadecimal(96,y+2,*(QWORD*)(buffer0+y*0x40+0x30));
		hexadecimal(112,y+2,*(QWORD*)(buffer0+y*0x40+0x38));
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
void real0()
{
	real0background();

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
void real0message(DWORD type,DWORD key)
{
	if(type==1)
	{
		if(key==0x25)	//left	0x4b
		{
			if(tag>0)tag--;
		}
		else if(key==0x27)	//right	0x4d
		{
			if(tag<7)tag++;
		}
	}
	if(type==2)
	{
		int x=key&0xffff;
		int y=(key>>16)&0xffff;
		if(y>320-128)
		{
			if(y<320+128)
			{
				if(x>1024-32)
				{
					killmehelpit(1,0);
					return;
				}
			}
		}
		if( (x>256) && (x<768) )
		{
			tag=x/64-4;		//比例尺
		}
	}
}