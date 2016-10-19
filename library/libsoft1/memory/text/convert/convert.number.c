#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char*,...);




//anscii码转换成一个数字，比如anscii码串为0x36,0x33,转换后得到decimal=36
int decstr2data(u8* source,u64* data)
{
	*data=0;
	int i;
	for(i=0;i<20;i++)		//64bit的最大数为20个阿拉伯数字
	{
		//1.如果<0x20:		//返回取得的总数量
		if(source[i]<0x30)return i;
		if(source[i]>0x39)return i;

		//3.如果是正常值:	//先乘10，再加上这个值，然后搞下一个数
		*data=(*data)*10;
		*data+=source[i]-0x30;
	}
}
int data2decstr(u64 data,u8* string)
{
	unsigned long long temp;
	int i,count;

	count=0;	//至少1
	temp=(unsigned long long)data;
	while(1)
	{
		count++;

		if(temp<10)break;
		temp/=10;
	}

	temp=(unsigned long long)data;
	for(i=1;i<=count;i++)
	{
		string[count-i]=temp%10+0x30;
		temp/=10;
	}

	return count;
}




void double2decimalstring(double data,u8* string)
{
	double temp;
	int offset;
	int count;

	//符号部分
	offset=0;
	if(data<0)
	{
		string[0]='-';
		offset+=1;

		data=-data;
	}

	//整数部分
	offset+=data2decstr( (u64)data , string+offset );

	//小数点
	string[offset]='.';
	offset++;

	//小数部分
	temp=(double)(u64)data;
	temp=data-temp;

	if(temp<0.0000000000000000000000000000000001)
	{
		//特别小
		string[offset]=0x30;
		offset++;
	}
	else
	{
		//一般小数
		while(temp<0.1)
		{
			temp*=10;
			string[offset]=0x30;
			offset++;
		}
		temp=temp*10000000;
		count=data2decstr( (u64)temp , string+offset );
		offset+=count;
	}

	//0
	string[offset]=0;
}




//anscii码转换成一个16进制数字，比如anscii码串为0x36,0x61,转换后得到hex=0x6a
int hexstr2data(u8* source,u64* data)
{
	*data=0;
	int i;
	for(i=0;i<16;i++)
	{
		//say("%x\n",source[i]);
		//1.如果小于0x20，那就是结束了
		if(source[i]<=0x20) return i;

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
	return -999999;	//不会到这里
}




int data2hexstr(u64 data,u8* string)
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
	return i;
}




void datastr2hexstr(char* dst, char* src, int count)
{
	int j;
	char ch;

	for(j=0;j<count;j++)
	{
		ch = (src[j] & 0xf0) >> 4;
		ch += 0x30;
		if(ch > 0x39)ch += 0x27;
		dst[2*j] = ch;

		ch = src[j] & 0xf;
		ch += 0x30;
		if(ch > 0x39)ch += 0x27;
		dst[2*j+1] = ch;
	}
	dst[2*count] = 0;
}
