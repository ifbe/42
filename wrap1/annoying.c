#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




//------------------粗糙的打比方--------------------
//一个村庄（每个文件）
//被拆迁（文件系统处理）
//村里各户人家（组成文件那些字节字节字节字节字节...）
//分散到各种地方（硬盘上分区内第几扇区到第几扇区）
//调用这个蛋碎一地函数（这块实际上在哪儿，这块总共多少个扇区，这块逻辑上是哪儿，你要哪儿）
//就是跟老村联络员说（王二家现在住哪，总共多少人，王二拆迁前住在哪，要找村里哪几户人家）

//-------------------详细的抽象---------------------
//20厘米的尺子第3到11厘米涂上颜色，被不同大小得掰断成好几份
//分别是[0,2],[2,4],[4,8],[8,10],[10,14],[14,20]
//现在要把涂色的地方摆在(规定的地方)，没涂色的部分扔掉不管
//因为这把尺子每个碎块上面都有刻度，所以从0开始可以排好队等着搞
//[0,2],[14,20]:这两块都不要不管
//[2,4]:切出[3,4],放在(规定的地方),总共1cm
//[4,8]:全部放在(规定的地方+1cm),总共4cm
//[8,10]:全部放在(规定的地方+4cm),总共2cm
//[10,14]:切出[10,11],放在(规定的地方+7cm),总共1cm

//--------------------画图说事--------------------
//               |[wantwhere,wantwhere+1m]|
//1:             |                        | [where,]  //自己就能排除，不要麻烦这个函数
//2: [where,]    |                        |           //自己就能排除，不要麻烦这个函数
//3:         [---|--where,--]             |           //传进来一块，前面不要
//4:         [---|--where,----------------|----]      //传进来一块，前面不要，后面也不要
//5:             |  [where,]              |           //传进来一块，全要
//6:             |  [---where,------------|----]      //传进来一块，后面不要

void holyshit(sector,count,where,wantwhere,towhere)
{
	//关键:读到哪儿，读哪号扇区，读几个扇区
	QWORD rdi=0;
	QWORD rsi=0;
	QWORD rcx=0;

	//改rdi,rsi,rcx数值
	if(where<wantwhere)		//3和4
	{
		rdi=towhere;
		rsi=sector+(wantwhere-where)/0x200;
		if(where+count*0x200<=wantwhere+0x100000)
		{
			rcx=count-(wantwhere-where)/0x200;
		}
		else
		{
			rcx=0x100000/0x200;
		}
	}
	else
	{
		rdi=towhere+(where-wantwhere);
		rsi=sector;
		if(where+count*0x200<=wantwhere+0x100000)
		{
			rcx=count;
		}
		else
		{
			rcx=(wantwhere+0x100000-where)/0x200;
		}
	}

	say("sector:%x,count:%x,where:%x,want:%x,to:%x,rdi=%x,rsi=:%x,rcx=%x\n",sector,count,where,wantwhere,towhere,rdi,rsi,rcx);
	readdisk(rdi,rsi,0,rcx);
}




int compare(unsigned char* first,unsigned char* second)
{
	int i;
	//say("%s,%s\n",first,second);
	for(i=0;i<16;i++)
	{
		if( (first[i]==0) && (second[i]==0) )return 0;		//比较完毕，相同
		if(first[i]!=second[i])return -1;
	}
}




//debug用，打印从addr开始的总共size个字节
void printmemory(QWORD addr,QWORD size)
{
	BYTE* printaddr=(BYTE*)addr;
	int i,j;

	//1111111111111111111111
	say("[----addr----]");
	for(i=0;i<=0xf;i++)
	{
		say("%2x ",i);
	}
	say("[----anscii----]\n");

	//2222222222222222222222222
	for(j=0;j<size/16;j++)
	{
		if(j%16 == 0)say("%-14llx",addr+j*16);
		else say("+%-13x",j*16);

		for(i=0;i<=0xf;i++)
		{
			say("%.2x ",printaddr[16*j+i]);
		}
		for(i=0;i<=0xf;i++)
		{
			unsigned char ch=printaddr[16*j+i];
			if( (ch>=0x80)|(ch<=0x20) )ch=0x20;
			say("%c",ch);
		}
		say("\n",printaddr[16*j+15]);
	}
}




//比如aa zec，得到aa\0ec
void blank2zero(QWORD* name)
{
        int i;
		QWORD temp;
        for(i=0;i<8;i++)
        {
                temp = ( (*name)>>(i*8) )&0xff;
		if(temp == 0x20)
                {
                        *name -= ((QWORD)0x20)<<(i*8);
                }
        }

}
//比如ac\0be，得到ac be
void zero2blank(QWORD* name)
{
        int i;
	QWORD temp;
        for(i=0;i<8;i++)
        {
                temp = ( (*name)>>(i*8) )&0xff;
		if(temp == 0)
                {
                        *name += ((QWORD)0x20)<<(i*8);
                }
        }

}
//比如abcefg，变成ABCEFG
void small2capital(QWORD* name)
{
        int i;
	QWORD temp;
        for(i=0;i<8;i++)
        {
                temp = ( (*name) >> (i*8) )&0xff;
                if(temp>='a' && temp<='z')
                {
                        *name -= ( (QWORD)0x20 )<<(i*8);
                }
        }
}
//比如0x36，0x38,0x37,0x39,转换后得到data=0x39373836
void str2data(BYTE* str,QWORD* data)
{
	*data=0;
	int i;
	for(i=0;i<8;i++)
	{
		if(str[i]<=0x20) break;
		*data+=((QWORD)str[i])<<(i*8);
	}
}
//anscii码转换成一个数字，比如anscii码串为0x36,0x33,转换后得到decimal=32
void anscii2dec(BYTE* second,QWORD* decimal)
{
	*decimal=0;
	int i;
	for(i=0;i<8;i++)
	{
		if(second[i]<=0x20) break;
		//say("%x\n",second[i]);
		*decimal=(second[i]-0x30)+(*decimal)*10;
	}
}
//anscii码转换成一个16进制数字，比如anscii码串为0x36,0x61,转换后得到hex=0x6a
void anscii2hex(BYTE* second,QWORD* hex)
{
	*hex=0;
	int i;
	for(i=0;i<8;i++)
	{
		if(second[i]<=0x20) break;
		//say("%x\n",second[i]);
		*hex=(*hex)<<4;
		if(second[i]>=0x61 && second[i]<=0x66)
		{
			*hex+=second[i]-0x61+10;
		}
		else if(second[i]>=0x30 && second[i]<=0x39)
		{
			*hex+=second[i]-0x30;
		}
	}
}
void buf2arg(BYTE* buffer,QWORD* first,QWORD* second)
{
	//开始的时候尾巴处写满0xffffffff,然后first和second都指向尾巴
	*(QWORD*)(buffer+0x78)=0xffffffffffffffff;
	*first=*second=(QWORD)(buffer+0x78);

	//
	int i;
	int howmany=0;
	for(i=0;i<0x78;i++)
	{
		if( buffer[i] <= 0x20 )
		{
			buffer[i]=0;
		}
		if( buffer[i] > 0x20 )
		{
			if(i == 0)
			{
				*first=(QWORD)&buffer[i];
				//say("%llx\n",*first);
				howmany++;
			}
			else
			{
				if( buffer[i-1] == 0 )
				{
					if( howmany == 0 )
					{
						*first=(QWORD)&buffer[i];
						howmany++;
					}
					else if( howmany == 1 )
					{
						say("here:%x\n",howmany);
						*second=(QWORD)&buffer[i];
					}
				}//if前一个是0

			}//else这不是buffer里面第一个

		}//if这个是正常字符

	}//最外面的for循环
}