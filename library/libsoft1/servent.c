#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//检查
int iself(char*);	//可执行文件
int ismacho(char*);
int ispe(char*);
int is7z(char*);	//压缩包
int istar(char*);
int iszip(char*);
int isfat(char*);	//文件系统
int isntfs(char*);
int isext(char*);
int ishfs(char*);
int isgpt(char*);	//分区表
int ismbr(char*);
//挂载
int mountext(QWORD src,char* dst);	//文件系统
int mountfat(QWORD src,char* dst);
int mounthfs(QWORD src,char* dst);
int mountntfs(QWORD src,char* dst);
int explaingpt(char* src,char* dst);	//分区表
int explainmbr(char* src,char* dst);
//读取
int mem2file(char* src,char* dest,QWORD ignore,int size);
int readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
int compare(char*,char*);
//日志
void cleanmemory(char*,int);
void printmemory(char*,int);
void say(char*,...);
void diary(char*,...);
void history(char*,...);




/*
mountpoint: struct{
	//.text
	'cd',		@cd,		//0
	'explain',	@explain,	//0x10
	'load',		@load,		//0x20
	'store',	@store,		//0x30

	//.data
	'where'		=0x?,		//0x80
	'where1'	=0x?,		//0x90
	'blocksz',	=0x200,		//0xa0
	'indexsz'	=0x200,		//0xb0
}
*/




//what
static char* diskhome;
static char* fshome;
static char* dirhome;
	static int dirdepth=0;		//ls,cd
static char* datahome;

//3大函数的位置
int (*fsexplain)(QWORD id);     //((int (*)(QWORD))(fsexplain))(value);
int (*fscd)(QWORD id);          //((int (*)(QWORD))(fscd))(arg1);
int (*fsload)(QWORD id,QWORD part);     //((int (*)(QWORD,QWORD))(fsload))(arg1,temp*0x100000);








//试毒员
QWORD prelibation(char* memaddr)
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




//number=0:
//		解释分区表
//		写到[diskhome+0,diskhome+0xffff)位置空的地方
//number>0:
//		挂载对应分区
//		写到[diskhome+0,diskhome+0x10000)位置空的地方
int hello(char* src)
{
	int ret;
	QWORD sector=0;
	QWORD type=0;


	//sector=
	if(src==0)sector=0;
	else sector=*(QWORD*)(src+0x10);


	//读[sector,sector+63](0x8000bytes)进内存，检查种类
	readmemory(datahome , sector , 0 , 64);
	type=prelibation(datahome);
	diary("%s\n",&type);


	//如果是分区表头，并且是文件头，那么解释分区表到diskhome
	if(type==0x747067)	//0 && gpt
	{
		if(sector==0)
		{
			explaingpt(datahome,diskhome);
		}
		return 0;
	}
	if(type==0x72626d)		//0 && mbr
	{
		if(sector==0)
		{
			explainmbr(datahome,diskhome);
		}
		return 0;
	}


	//否则无论是什么，都解释到fshome
	cleanmemory(fshome,0x300000);
	if(type == 0x747865)            //'ext'
	{
		ret=mountext(sector,fshome);
	}
	else if(type == 0x746166)       //'fat'
	{
		ret=mountfat(sector,fshome);
	}
	else if(type == 0x736668)       //'hfs'
	{
		ret=mounthfs(sector,fshome);
	}
	else if(type == 0x7366746e)     //'ntfs'
	{
		ret=mountntfs(sector,fshome);
	}

	//拿到cd,load,explain等苦工的地址
	fscd = *(void**)( fshome+0x10 );
	fsload = *(void**)( fshome+0x50 );
	fsexplain = *(void**)( fshome+0x90 );
	diary("%llx,%llx,%llx\n",fscd,fsload,fsexplain);
	return 1;
}




//logical function
int ls(char* name)
{
	//null:         just list
	int i,j,temp;
	char buf[16];
	if(name==0)
	{
		for(i=0; i<0x400; i++)		//0x40*0x400=0x10000
		{
			temp=*(QWORD*)( dirhome+(i*0x40) );
			if(temp == 0)break;

			//[+0]:type
			diary("(%-4s," , dirhome+(i*0x40) );
			//[+8]:id
			*(QWORD*)buf=*(QWORD*)(dirhome+(i*0x40)+0x8);
			temp=0;
			for(j=0;j<8;j++)
			{
				//[1,0x1f]:wrong
				if(buf[j]>0 && buf[j]<0x20) temp++;
				//[0x80,0xff]:wrong
				if(buf[j]>=0x80) temp++;
			}
			if(temp==0) diary("%4s)	",buf);
			else diary("%4llx)	",*(QWORD*)buf);

			//[+10]:start
			diary("[%-4llx,",*(QWORD*)(dirhome+(i*0x40)+0x10));
			//[+18]:end
			diary("%4llx]	",*(QWORD*)(dirhome+(i*0x40)+0x18));
			//[+20]:detail
			diary("{%-16s}	",dirhome+(i*0x40)+0x20);
			//which
			diary("<%d>\n",i);
		}//for

		return 1;
	}

	//else:         search+explain
	for(;temp<0x10000;temp+=0x40)
	{
		//diary("%llx,%llx\n",*(QWORD*)name,*(QWORD*)temp);
		if( compare( name , dirhome+temp+0x20 ) == 0 )
		{
			printmemory(dirhome+temp,0x40);
			//id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
			//explain(id);
			return temp/0x40;
		}
	}

	//failed
	diary("file not found\n");
	return -1;
}




int cd(char* arg1)
{
	int ret;
	QWORD id;

	//search
	ret=ls(arg1);
	if( ret<0 )return ret;          //没找到

	//change directory
	id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
	return fscd(id);
}




int load(char* arg1)
{
	//寻找这个文件名，得到id，type，size
	int ret;
	QWORD id;
	QWORD size;
	QWORD temp;

	ret=ls(arg1);
	if( ret==0 )return -1;

	id=*(QWORD*)(dirhome + 0x40*ret + 0x10);
	size=*(QWORD*)(dirhome + 0x40*ret + 0x18);
	if(size>0x100000)diary("id=%x,size=%x\n",id,size);

	//1m,1m,1m的整块搞
	temp=0;
	for(;temp<( size&0xfffffff00000 );temp+=0x100000)
	{
		//
		fsload(id,temp);

		//mem地址，file名字，文件内偏移，写入多少字节
		mem2file(datahome,arg1,temp,0x100000);
	}

	//最后的零头(要是size=1m的整数倍，就没有零头)
	if(temp<size)
	{
		//
		fsload(id,temp);

		//mem地址，file名字，文件内偏移，写入多少字节
		mem2file(datahome,arg1,temp,size%0x100000);
	}

	return 0;
}




int store(char* arg1)
{
        return 0;
}




//
void initservent(char* addr)
{
	diskhome=addr;
	fshome=addr+0x100000;
	dirhome=addr+0x200000;
	datahome=addr+0x300000;
}
