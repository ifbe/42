#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//检查
int istar(char*);	//压缩包
int iszip(char*);
int isfat(char*);	//文件系统
int isntfs(char*);
int isext(char*);
int ishfs(char*);
int isgpt(char*);	//分区表
int ismbr(char*);
//挂载
int mountext(char* src,char* dst);	//文件系统
int mountfat(char* src,char* dst);
int mounthfs(char* src,char* dst);
int mountntfs(char* src,char* dst);
int explaingpt(char* src,char* dst);	//分区表
int explainmbr(char* src,char* dst);

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
static int chosenfile=0;	//mount
static int dirdepth=0;		//ls,cd

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
	
	//是可执行文件
	if( iself(memaddr) !=0 )return ;	//'elf'
	if( ismacho(memaddr) !=0 )return ;	//'macho'
	if( ispe(memaddr) !=0 )return ;		//'pe'

	//是压缩包
	if( istar(memaddr) !=0 )return ;	//'tar'
	if( iszip(memaddr) !=0 )return ;	//'zip'
*/
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




//number=0:	解释分区表
//		写到[diskhome+0,diskhome+0xffff)位置空的地方
//number>0:	挂载对应分区
//		写到[diskhome+0,diskhome+0x10000)位置空的地方
int mount(QWORD number,char* dest)
{
        //读分区表
        if(temp==0x747067) explaingpt(datahome,diskhome);
        else if(temp==0x72626d) explainmbr(datahome,diskhome);
        else diary("i don't know it\n");

	//挂载
	temp = *(QWORD*)( diskhome+number*0x40 + 8 );   //type
	if(temp == 0x747865)            //'ext'
	{
		ret=mountext( diskhome+number*0x40 , diskhome );
	}
	else if(temp == 0x746166)       //'fat'
	{
		ret=mountfat( diskhome+number*0x40 , diskhome );
	}
	else if(temp == 0x736668)       //'hfs'
	{
		ret=mounthfs( diskhome+number*0x40 , diskhome );
	}
	else if(temp == 0x7366746e)     //'ntfs'
	{
		ret=mountntfs( diskhome+number*0x40 , diskhome );
	}
	if(ret<0)return;

	//拿到cd,load,explain等苦工的地址
	fscd = *(void**)( fshome+0x10 );
	fsload = *(void**)( fshome+0x50 );
	fsexplain = *(void**)( fshome+0x90 );
	diary("%llx,%llx,%llx\n",fscd,fsload,fsexplain);
	return;
}




//logical function
static int ls(char* name)
{
        //null:         just list
        int temp=0;
        if(name==0)
        {
                masterlist("file");
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




static int cd(char* arg1)
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




static int load(char* arg1)
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
        if(size>0x100000)diary("warning:large file\n");

        //1m,1m,1m的整块搞
        temp=0;
        for(;temp<( size&0xfffffff00000 );temp+=0x100000)
        {
                fsload(id,temp);
                mem2file(datahome,arg1,temp,0x100000);          //mem地址，file名字，文件内偏移，写入多
少字节
        }

        //最后的零头(要是size=1m的整数倍，就没有零头)
        if(temp<size)
        {
                fsload(id,temp);
                mem2file(datahome,arg1,temp,size%0x100000);             //mem地址，file名字，文件内偏移
，写入多少字节
        }
        return 0;
}




static int store(char* arg1)
{
        return 0;
}

