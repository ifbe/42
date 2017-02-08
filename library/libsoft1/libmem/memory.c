#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
//
int data_create(void*,void*);
int data_delete();
int filesystem_create(void*,void*);
int filesystem_delete();
int sec_create(void*,void*);
int sec_delete();
int text_create(void*,void*);
int text_delete();
//
int cmp(void*,void*);
int hexstr2data(u8*,u64*);
//
int readfile(u8* file, u8* mem, u64 offset, u64 count);
int writefile(u8* file, u8* mem, u64 offset, u64 count);
int startfile();
int stopfile();
//
int printmemory(void* addr, int count);
int say(void* str, ...);




//
static u8* guys=0;
static u8* fshome=0;
static u8* dirhome=0;
static u8* datahome=0;
//
static int guystack[10];
static int guyrsp = 0;




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
*/
	//什么都不像，返回失败
	return 0;	//'unknown'
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
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
	if(where<want)	     //3和4
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
	readfile(0, rdi, rsi, rcx);
}




/*
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		|[want     ,     want+1m]|
		|			| [where,]    //不要
 [where, ]      |			|	     //不要
	    [---|--where,--]	     |	     //要后面
	    [---|--where,----------------|----]	//要中间
		|  [where,    ]	  |	     //全要
		|  [---where,------------|----]	//要前面
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
(内存这一小块是逻辑上的哪) -> (设备这一小块想要逻辑上的哪)
(来源，数量，是哪) -> (目的，数量，要哪)
*/
void cleverwrite(u8* src, u64 count, u64 where, u64 dst, u64 size, u64 want)
{
}




//
static int memory_list(u8* arg1)
{
	int j;
	u8* p;
	for(j=1;j<0x1000/0x80;j++)
	{
		p = guys + (j*0x80);
		if(p[0] == 0)say("%s/\n", p+8);
		else say("	%s\n", p+8);
	}
	return 0;
}
static int memory_choose(u8* arg)
{
	return 0;
}
static int memory_read(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int memory_write(u8* mem, u8* file, u64 addr, u64 count)
{
	return 0;
}
static int memory_start(u8* p)
{
	return startfile(p);
}
static int memory_stop()
{
	return stopfile();
}
int memory_create(u8* softaddr,u64* p)
{
	u8* q;

	//(自己)4块区域，每块1兆
	guys = softaddr;
	fshome = softaddr+0x100000;
	dirhome = softaddr+0x200000;
	datahome = softaddr+0x300000;

	//
	p[0]=0;
	p[1]=0x79726f6d656d;

	p[10]=(u64)memory_start;
	p[11]=(u64)memory_stop;
	p[12]=(u64)memory_list;
	p[13]=(u64)memory_choose;
	p[14]=(u64)memory_read;
	p[15]=(u64)memory_write;

	//
	q=(u8*)p+0x80;

	data_create(softaddr, q);
	q+=0x80;

	filesystem_create(softaddr, q);
	q+=0x80;

	sec_create(softaddr, q);
	q+=0x80;

	text_create(softaddr, q);
	q+=0x80;

	return q-(u8*)p;
}
int memory_delete()
{
	data_delete();
	filesystem_delete();
	sec_delete();
	text_delete();
	return 0;
}
