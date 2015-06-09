#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long




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
/*
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
*/
//anscii码转换成一个数字，比如anscii码串为0x36,0x33,转换后得到decimal=36
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
int anscii2hex(BYTE* source,QWORD* hex)
{
	*hex=0;
	int i;
	for(i=0;i<8;i++)
	{
		//say("%x\n",source[i]);
		//1.如果小于0x20，那就是结束了
		if(source[i]<=0x20) break;

		//2.如果大于0x80，那就返回错误
		if(source[i]>=0x80) return -1;

		//3.如果是正常值
		*hex=(*hex)<<4;
		if(source[i]>=0x61 && source[i]<=0x66)
		{
			*hex+=source[i]-0x61+10;
		}
		else if(source[i]>=0x30 && source[i]<=0x39)
		{
			*hex+=source[i]-0x30;
		}
	}
}
void buf2arg(BYTE* buffer,QWORD* first,QWORD* second)
{
	*first=*second=0;

	//
	int i;
	int howmany=0;
	for(i=0;i<0x80;i++)
	{
		if( buffer[i] <= 0x20 )buffer[i]=0;
		if( buffer[i] > 0x20 )
		{
			if(i == 0)		//buffer里第一个，不可能是second
			{
				*first=(QWORD)&buffer[i];
				howmany++;
			}
			else
			{
				if( buffer[i-1] == 0 )	//前面有0说明这是边界
				{
					if( howmany == 0 )
					{
						*first=(QWORD)&buffer[i];
						howmany++;
					}
					else if( howmany == 1 )
					{
						*second=(QWORD)&buffer[i];
					}
				}
			}

		}//if这个是正常字符

	}//最外面的for循环
}