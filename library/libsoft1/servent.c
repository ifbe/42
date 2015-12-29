#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
//返回相似度，越像越接近0x7fffffff，确定不是就返回0
int isgpt();
int ismbr();
int isfat();
int isntfs();
int isext();
int ishfs();
//int is...();


/*
check
explain
cd
load
store
*/




//试毒员
QWORD prelibation(char* memaddr)
{
	//一个个去问
	if( isgpt(memaddr) )return 0x747067;		//'gpt'

	if( isfat(memaddr) )return 0x746166;		//'fat'
	if( isntfs(memaddr) )return 0x7366746e;	//'ntfs'
	if( isext(memaddr) )return 0x747865;		//'ext'
	if( ishfs(memaddr) )return 0x736668;		//'hfs'

	if( ismbr(memaddr) )return 0x72626d;		//'mbr'

	//什么都不像，返回失败
	return 0;	//'unknown'
}
/*
	//末尾有0x55，0xaa这个标志 -> 分区表头或windows系列文件系统
	QWORD temp=*(WORD*)(datahome+0x1fe);
	if(temp == 0xaa55 )
	{
		//第二个扇区开头是"EFI PART" -> 是gpt分区表头
		temp=*(QWORD*)(datahome+0x200);
		if( temp == 0x5452415020494645 )
		{
			return 0x747067;	//'gpt'
		}

	}
*/




//能挂的肯定是已经认出来的，认出来的就有个id
int mount(QWORD number)
{
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
