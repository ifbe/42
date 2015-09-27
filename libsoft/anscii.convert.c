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




//anscii码转换成一个数字，比如anscii码串为0x36,0x33,转换后得到decimal=36
int decstring2data(BYTE* source,QWORD* data)
{
	*data=0;
	int i;
	for(i=0;i<20;i++)		//64bit的最大数为20个阿拉伯数字
	{
		//1.如果大于0x80：		返回错误1
		if(source[i]>=0x80) return 0;

		//2.如果不是阿拉伯数字：	返回取得的总数量
		//（可能是小数点，结束符，算数符号等）
		if( (source[i]<0x30) | (source[i]>0x39) )
		{
			return i;
		}

		//3.如果是正常值：		先乘10，再加上这个值，然后搞下一个数
		*data=(*data)*10;
		*data+=source[i]-0x30;
	}
}




//anscii码转换成一个16进制数字，比如anscii码串为0x36,0x61,转换后得到hex=0x6a
int hexstring2data(BYTE* source,QWORD* data)
{
	*data=0;
	int i;
	for(i=0;i<8;i++)
	{
		//diary("%x\n",source[i]);
		//1.如果小于0x20，那就是结束了
		if(source[i]<=0x20) break;

		//2.如果大于0x80，那就返回错误
		if(source[i]>=0x80) return -1;

		//3.如果是正常值
		*data=(*data)<<4;
		if(source[i]>=0x61 && source[i]<=0x66)
		{
			*data+=source[i]-0x61+10;
		}
		else if(source[i]>=0x30 && source[i]<=0x39)
		{
			*data+=source[i]-0x30;
		}
		else return -2;
	}
}




int data2hexstring(QWORD data,BYTE* string)
{
	int i;
	unsigned char temp;
	for(i=0;i<0xf;i++)string[i]=0x20;
	string[15]=0x30;

	for(i=0;i<0x10;i++)
	{
		if(data==0)break;
		temp=data&0xf;
		data>>=4;

		temp+=0x30;
		if(temp>=0x3a)temp+=7;
		string[15-i]=temp;
	}
}




int data2decimalstring(QWORD data,BYTE* string)
{
    unsigned long long temp;
    int i,count;

	count=0;
    temp=(unsigned long long)data;
    while(1)
    {
		if(temp<10)break;
        temp/=10;
        count++;
    }

    temp=(unsigned long long)data;
	for(i=0;i<=count;i++)
	{
		string[count-i]=temp%10+0x30;
		temp/=10;
	}

	return count;
}
void double2decimalstring(double data,BYTE* string)
{
	double temp;
	int count;

	//整数部分
	count=data2decimalstring( (QWORD)data , string );

	//小数点
	string[count+1]='.';

	//小数部分
	temp=(double)(QWORD)data;
	temp=data-temp;
	temp=temp*10000000;
	data2decimalstring( (QWORD)temp , string + count+2 );
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