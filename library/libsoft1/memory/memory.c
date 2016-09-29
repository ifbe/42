#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//检查
int iself(char*);	//可执行文件
int ismacho(char*);
int ispe(char*);
int is7z(char*);	//压缩包
int iscpio(char*);
int isgz(char*);
int istar(char*);
int iszip(char*);
int isfat(char*);	//文件系统
int isntfs(char*);
int isext(char*);
int ishfs(char*);
int isgpt(char*);	//分区表
int ismbr(char*);
//
int code_create(char*,char*);
int code_delete();
int fs_create(char*,char*);
int fs_delete();
int pt_create(char*,char*);
int pt_delete();
//
int readfile(u8* mem, u8* file, u64 offset, u64 count);
int writefile(u8* mem, u8* file, u64 offset, u64 count);
int hexstring2data(char*,u64*);
int compare(char*,char*);	//base tool
//
int printmemory(char* addr,int count);
int say(char* str,...);		//+1




//
static char* diskhome=0;
static char* fshome=0;
static char* dirhome=0;
static char* datahome=0;




//试毒员
u64 prelibation(char* memaddr)
{
/*
	//是视频文件
	if( ismp4(memaddr) !=0 )return ;	//'mp4'
	if( isrmvb(memaddr) !=0 )return ;	//'rmvb'

	//是音乐文件
	if( ismp3(memaddr) !=0 )return ;	//'mp3'
	if( iswav(memaddr) !=0 )return ;	//'wav'

	//是图片
	if( isjpeg(memaddr) !=0 )return ;	//'jpeg'
	if( ispng(memaddr) !=0 )return ;	//'png'

	//办公文件
	if( isdoc(memaddr) !=0 )return ;	//'doc'
	if( ispdf(memaddr) !=0 )return ;	//'pdf'

	//3d模型

	//网络协议包
	if( isethernet(memaddr) !=0 )return ;	//'ethernet'
	if( isarp(memaddr) !=0 )return ;	//'arp'
	if( isudp(memaddr) !=0 )return ;	//'udp'
	if( istcp(memaddr) !=0 )return ;	//'tcp'
*/
	//是可执行文件
	if( iself(memaddr) !=0 )return 0x666c65;	//'elf'
	if( ismacho(memaddr) !=0 )return 0x6f6863616d;	//'macho'
	if( ispe(memaddr) !=0 )return 0x6570;		//'pe'

	//是压缩包
	if( is7z(memaddr) !=0 )return 0x7a37;		//'7z'
	if( iscpio(memaddr) !=0 )return 0x6f697063;	//'cpio'
	if( isgz(memaddr) !=0 )return 0x7a67;		//'gz'
	if( istar(memaddr) !=0 )return 0x726174;	//'tar'
	if( iszip(memaddr) !=0 )return 0x70697a;	//'zip'

	//是文件系统
	if( isfat(memaddr) !=0 )return 0x746166;	//'fat'
	if( isntfs(memaddr) !=0 )return 0x7366746e;	//'ntfs'
	if( isext(memaddr) !=0 )return 0x747865;	//'ext'
	if( ishfs(memaddr) !=0 )return 0x736668;	//'hfs'

	//是分区表头
	//if( isapm(memaddr) !=0)return ;	//'apm'	//apple partition map
	//if( isbsd(memaddr) !=0)return ;	//'bsd'	//bsd label
	if( isgpt(memaddr) !=0 )return 0x747067;	//'gpt'
	if( ismbr(memaddr) !=0 )return 0x72626d;	//'mbr',特殊,只能放最后

	//什么都不像，返回失败
	return 0;	//'unknown'
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                |[want     ,     want+1m]|
                |                        | [where,]    //不要
 [where, ]      |                        |             //不要
            [---|--where,--]             |             //要后面
            [---|--where,----------------|----]        //要中间
                |  [where,    ]          |             //全要
                |  [---where,------------|----]        //要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(设备这一小块是逻辑上的哪) -> (内存这一小块想要逻辑上的哪)
(扇区，数量，是哪) -> (内存，数量，要哪)
*/
void cleverread(u64 src, u64 count, u64 where, u8* dst, u64 size, u64 want)
{
	u8* rdi=0;    //内存地址
	u64 rsi=0;    //扇区号
	u64 rcx=0;    //扇区数量

	//改rdi,rsi,rcx数值
	if(where<want)             //3和4
	{
		rdi = dst;
		rsi = src+(want-where);
		if(where+count <= want+size)
		{
			rcx = count-(want-where);
		}
		else
		{
			rcx = size;
		}
	}
	else
	{
		rdi = dst+(where-want);
		rsi = src;
		if(where+count <= want+size)
		{
			rcx = count;
		}
		else
		{
			rcx = want+size-where;
		}
	}

/*
	say(
		"(%llx,%llx,%llx)->(%llx,%llx,%llx)\n",
		src,count,where,    dst,size,want
	);
	say(
		"rdi=%llx,rsi=%llx,rcx=%llx\n",
		rdi,rsi,rcx
	);
*/
	readfile(rdi, 0, rsi, rcx);
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
                |[want     ,     want+1m]|
                |                        | [where,]    //不要
 [where, ]      |                        |             //不要
            [---|--where,--]             |             //要后面
            [---|--where,----------------|----]        //要中间
                |  [where,    ]          |             //全要
                |  [---where,------------|----]        //要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(内存这一小块是逻辑上的哪) -> (设备这一小块想要逻辑上的哪)
(来源，数量，是哪) -> (目的，数量，要哪)
*/
void cleverwrite(u8* src, u64 count, u64 where, u64 dst, u64 size, u64 want)
{
}




//physical function
static int memory_list(char* arg1)
{
	u64 temp=0;
	int i,j;
	char buf[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	u64 target=0;
	char* addr=diskhome;
	//printmemory(diskhome,0x200);

	//想要什么
	if(arg1==0)target=0;
	else if( compare(arg1,"disk") == 0 )target=0x6b736964;	//'disk'
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
		temp=*(u64*)( addr + (i*0x40) );
		if(temp == 0)break;
		if( (target==0) | (temp == target) )
		{
			//[+0]:type
			say("(%-4s," , addr+(i*0x40) );

			//[+8]:id
			*(u64*)buf=*(u64*)(addr+(i*0x40)+0x8);
			temp=0;
			for(j=0;j<8;j++)
			{
				//[1,0x1f]:wrong
				if(buf[j]>0 && buf[j]<0x20) temp++;

				//[0x80,0xff]:wrong
				if(buf[j]>=0x80) temp++;
			}
			if(temp==0) say("%4s)	",buf);
			else say("%4llx)	",*(u64*)buf);

			//[+10]:start
			say("[%-4llx,",*(u64*)(addr+(i*0x40)+0x10));

			//[+18]:end
			say("%4llx]	",*(u64*)(addr+(i*0x40)+0x18));

			//[+20]:detail
			say("{%-16s}	",addr+(i*0x40)+0x20);

			//which
			say("<%x>\n",i);
		}//if
	}//for

	return 1;
}//memory_list
static int memory_choose(char* arg)
{
}
static int memory_read(char* arg1)
{
	u64 value;

	//nothing specified
	if(arg1==0)
	{
		say("read@%llx,world@%llx\n",memory_read,diskhome);
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
		readfile(datahome, 0, value*0x200, 0x200);
		printmemory(datahome,0x200);
		say("above is:%llx\n",value);
	}

	return 0;
}
static int memory_write(char* arg1)
{
	u64 value;

	//nothing specified
	if(arg1==0)
	{
		say("write@%llx,world@%llx\n",memory_write,diskhome);
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
		say("dangerous,bye\n");
	}

	return 0;
}








int memory_create(char* world,u64* p)
{
	char* q=(char*)p;

	//(自己)4块区域，每块1兆
	diskhome=world+0;
	fshome=world+0x100000;
	dirhome=world+0x200000;
	datahome=world+0x300000;

	//
	code_create(world,q);
	q+=0x40;

	fs_create(world,q);
	q+=0x40;

	pt_create(world,q);
	q+=0x40;

	return q-(char*)p;
}
int memory_delete()
{
	code_delete();
	fs_delete();
	pt_delete();
}
