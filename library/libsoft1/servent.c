#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




//试毒员
QWORD prelibation(char* memaddr)
{
	//读最开始的64个扇区（0x8000字节）来初步确定
	int ret=readmemory(datahome,0,0,64);
	if(ret<=0)
	{
		//读不出来，可能是内存？
		diary("it's memory?\n");
	}
	else if( *(WORD*)(datahome+0x1fe) == 0xaa55 )
	{
		//末尾有0x55，0xaa这个标志，这个是磁盘，或者要当成磁盘用

		//看看是什么类型，交给小弟处理
		QWORD temp=*(QWORD*)(datahome+0x200);
		if( temp == 0x5452415020494645 )
		{
			explaingpt(datahome,diskhome);
		}
		else
		{
			explainmbr(datahome,diskhome);
		}
	}
	else
	{
		//可能是zip,网络包,或者其他乱七八糟的结构
		diary("don't know\n");
	}
}




void mount(char* arg)
{
	for(i=0;i<10;i++)
	{
		if( arg[i]==0 )break;
		if( (arg[i] >= 0x30) | (arg[i] <= 0x39) )
		{
			number=10*number + arg[i] - 0x30;
		}
	}
	if(number == 0)return;
	//printmemory(diskhome + number*0x40,0x40);

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
