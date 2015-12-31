#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
void diary(char* fmt,...);
void readmemory(char* rdi,QWORD rsi,QWORD rdx,QWORD rcx);
void printmemory(QWORD start,QWORD count);




int ismbr(char* addr)
{
        //第一个扇区末尾必须有0x55，0xaa这个标志
        QWORD temp=*(WORD*)(addr+0x1fe);
        if(temp != 0xaa55 ) return 0;

	//但是mbr没有特别的标志，只能勉强用55aa确定
	//所以只能放在check type的最后
	return 0x72626d;
}




//from：那一条长度为16B的项目的地址，to：希望的位置
static int mbrrecord(char* from,char* to)
{
	DWORD flag;		//(mbr+0x1be)+0
	DWORD type;		//(mbr+0x1be)+4
	DWORD start;		//(mbr+0x1be)+8
	DWORD size;		//(mbr+0x1be)+c
	QWORD* dst;
	type=*(BYTE*)(from+4);
	if(type==0)return 0;

	//类型，子类型，开始，结束
	dst=(QWORD*)to;
	start=*(DWORD*)(from+8);
	size=*(DWORD*)(from+0xc);
        dst[0]=0x74726170; 	//'part'
	dst[2]=start;		//start
	dst[3]=start + size - 1;	//end

	//拓展分区要递归
	if( (type==0x5) | (type==0xf) )
	{
		diary("extend@start\n");
		dst[1]=0x646e65747865;
		return 66666;
	}

	//其他普通分区
	if( (type==0x4) | (type==0x6) | (type==0xb) )
	{
		//diary("fat\n");
		dst[1]=0x746166;
	}
	else if( type==0x7 )
	{
		//diary("ntfs\n");
		dst[1]=0x7366746e;
	}
	else if( type==0x83 )
	{
		//diary("ext\n");
		dst[1]=0x747865;
	}
	else
	{
		//diary("unknown:%x\n",type);
		dst[1]=type;
	}
	return 0x10;	//这次翻译了多少
}




//mbr:			[+0x1be,+0x1fd],每个0x10,总共4个
//[+0]:活动标记
//[+0x1,+0x3]:开始磁头柱面扇区
//[+0x4]:分区类型
//[+0x5,+0x7]:结束磁头柱面扇区
//[+0x8,+0xb]:起始lba
//[+0xc,+0xf]:大小
void explainmbr(char* buffer,char* to)
{
	char* dst;
	QWORD* dstqword;

	int i,j,ret;
	QWORD temp;
	diary("mbr disk\n",0);

        //保留硬盘记录，覆盖新的分区记录
	dst=to;
	dstqword=(QWORD*)to;
	for(i=0;i<0x100;i++)  //0x100*0x40=0x4000=16k
	{
		temp=dstqword[i*8];
		if(temp == 0)break;
		if(temp == 0x74726170)break;
	}
	for(j=0x40*i; j<0x10000; j++)
	{
		dst[j] = 0;
	}
	dst+=0x40*i;
	dstqword=(QWORD*)dst;

	//放下第一个
        dstqword[0]=0x74726170; //'part'
        dstqword[1]=0x72626d;	//'mbr'
        dstqword[2]=0;
        dstqword[3]=0;
        dstqword[4]=0x7766554433221100;
        dstqword[5]=0xffeeddccbbaa9988;
        dstqword[6]=0x8899aabbccddeeff;
        dstqword[7]=0x0011223344556677;
        dst += 0x40;
        dstqword += 8;

	//首先是主分区，最多4个
	ret=mbrrecord(buffer+0x1be,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}

	ret=mbrrecord(buffer+0x1ce,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}

	ret=mbrrecord(buffer+0x1de,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}

	ret=mbrrecord(buffer+0x1ee,dst);
	if(ret>0)
	{
		dst+=0x40;
		dstqword+=8;
	}
}
/*
	//在这四个里面，寻找逻辑分区并解释
	QWORD offset=0;
	while(1)
	{
		QWORD type=*(QWORD*)(to+offset+0x10);
		if(type==0)break;		//到这就没东西了

		QWORD start=*(QWORD*)(to+offset);
		if( type==5 | type==0xf )
		{
			//diary("sector:%x\n",*(DWORD*)(to+offset));
			readmemory(buffer,*(DWORD*)(to+offset),0,1);
			//printmemory(buffer+0x1be,0x40);

			char* remember=(char*)dstqword;
			mbrrecord(buffer+0x1be,dstqword);
			if(remember!=(char*)dstqword)
			{
				*(DWORD*)(remember+0)+=start;
				//diary("1st:%x\n",*(DWORD*)remember);
			}

			remember=dstqword;
			mbrrecord(buffer+0x1ce,dstqword);
			if(remember!=dstqword)
			{
				*(DWORD*)(remember+0)+=start;
				//diary("2st:%x\n",*(DWORD*)remember);
			}
		}

		//轮到下一个了
		offset+=0x40;
	}//while
*/
/*
	//最后打印所有的
	offset=0;
	while(1)
	{
		QWORD addr=to+offset*0x40+0x10;
		QWORD type=*(QWORD*)addr;
		if(type==0)break;

		diary("%d:%s\n",offset,(char*)addr);

		offset++;
	}
*/
