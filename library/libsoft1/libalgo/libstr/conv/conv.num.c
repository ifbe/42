#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*, int);
void say(char*, ...);




int decstr2u16(u8* src, u32* dst)
{
	int j;
	u16 tmp = 0;
	for(j=0;j<5;j++){
		if(src[j]<0x30)break;
		if(src[j]>0x39)break;
		tmp = tmp*10;
		tmp += src[j]-0x30;
	}
	*dst = tmp;
	return j;
}
int decstr2u32(u8* src, u32* dst)
{
	int j;
	u32 tmp = 0;
	for(j=0;j<10;j++){
		if(src[j]<0x30)break;
		if(src[j]>0x39)break;
		tmp = tmp*10;
		tmp += src[j]-0x30;
	}
	*dst = tmp;
	return j;
}
int decstr2u64(u8* src, u64* dst)
{
	int j;
	u64 tmp = 0;
	for(j=0;j<20;j++){
		if(src[j]<0x30)break;
		if(src[j]>0x39)break;
		tmp = tmp*10;
		tmp += src[j]-0x30;
	}
	*dst = tmp;
	return j;
}
int decstr2data(u8* src, void* dst)
{
	int j;
	u64 data=0;
	for(j=0;j<20;j++)		//64bit的最大数为20个阿拉伯数字
	{
		//1.如果<0x20:		//返回取得的总数量
		if(src[j]<0x30)break;
		if(src[j]>0x39)break;

		//3.如果是正常值:	//先乘10，再加上这个值，然后搞下一个数
		data = data*10;
		data += src[j]-0x30;
	}
	//say("data=%llx\n", data);

	if(data>0xffffffff)*(u64*)dst = data;
	else if(data>0xffff)*(u32*)dst = data;
	else if(data>0xff)*(u16*)dst = data;
	else *(u8*)dst = data;
	return j;
}
int decstr2float(u8* src, float* data)
{
	int j;
	float flag;
	float temp;
	float asdf;

stage0:
	if('-' != src[0])flag = 1.0;
	else{
		flag = -1.0;
		src += 1;
	}

stage1:
	temp = 0.0;
	for(j=0;j<20;j++)
	{
		if((src[j] >= '0') && (src[j] <= '9'))
		{
			temp = (temp*10) + src[j] - 0x30;
		}
		else if(src[j] == '.')
		{
			j++;
			*data = temp;
			goto stage2;
		}
		else
		{
			*data = temp;
			goto byebye;
		}
	}

stage2:
	asdf = 0.1;
	for(;j<30;j++)
	{
		if((src[j] >= '0') && (src[j] <= '9'))
		{
			*data += (src[j]-0x30)*asdf;
			asdf *= 0.1;
		}
		else break;
	}

byebye:
	if(flag > 0.0){
		return j;
	}

	*data *= -1.0;
	return j+1;
}
int decstr2double(u8* src, double* data)
{
	int j;
	double flag;
	double temp;
	double asdf;

stage0:
	if('-' != src[0])flag = 1.0;
	else{
		flag = -1.0;
		src += 1;
	}

stage1:
	temp = 0.0;
	for(j=0;j<20;j++)
	{
		if((src[j] >= '0') && (src[j] <= '9'))
		{
			temp = (temp*10) + src[j] - 0x30;
		}
		else if(src[j] == '.')
		{
			j++;
			*data = temp;
			goto stage2;
		}
		else
		{
			*data = temp;
			goto byebye;
		}
	}

stage2:
	asdf = 0.1;
	for(;j<30;j++)
	{
		if((src[j] >= '0') && (src[j] <= '9'))
		{
			*data += (src[j]-0x30)*asdf;
			asdf *= 0.1;
		}
		else break;
	}

byebye:
	if(flag > 0.0){
		return j;
	}

	*data *= -1.0;
	return j+1;
}




int data2decstr(long long data, u8* str)
{
	int j,k,f;
	char temp[32];

	if(0 == data){str[0] = '0';return 1;}
	if(data > 0)f = 0;
	else{f = -1;data = -data;}

	k = 0;
	while(1)
	{
		if(0 == data)break;

		temp[k] = (data % 10) + 0x30;
		k++;
		data = data / 10;
	}
	if(f != 0){temp[k] = '-';k++;}

	for(j=k-1;j>=0;j--)
	{
		*str = temp[j];
		str++;
	}
	return k;
}
int float2decstr(float data, u8* str)
{
	float temp;
	int offset;
	int count;

	//符号部分
	offset=0;
	if(data<0)
	{
		str[0]='-';
		offset+=1;

		data=-data;
	}

	//整数部分
	offset+=data2decstr( (u64)data , str+offset );

	//小数点
	str[offset]='.';
	offset++;

	//小数部分
	temp=(float)(u64)data;
	temp=data-temp;

	if(temp<0.0000000000000000000000000000000001)
	{
		//特别小
		str[offset]=0x30;
		offset++;
	}
	else
	{
		//一般小数
		while(temp<0.1)
		{
			temp*=10;
			str[offset]=0x30;
			offset++;
		}
		temp=temp*10000000;
		count=data2decstr( (u64)temp , str+offset );
		offset+=count;
	}

	//0
	str[offset]=0;
	return offset;
}
int double2decstr(double data, u8* str)
{
	double temp;
	int offset;
	int count;

	//符号部分
	offset=0;
	if(data<0)
	{
		str[0]='-';
		offset+=1;

		data=-data;
	}

	//整数部分
	offset+=data2decstr( (u64)data , str+offset );

	//小数点
	str[offset]='.';
	offset++;

	//小数部分
	temp=(double)(u64)data;
	temp=data-temp;

	if(temp<0.0000000000000000000000000000000001)
	{
		//特别小
		str[offset]=0x30;
		offset++;
	}
	else
	{
		//一般小数
		while(temp<0.1)
		{
			temp*=10;
			str[offset]=0x30;
			offset++;
		}
		temp=temp*10000000;
		count=data2decstr( (u64)temp , str+offset );
		offset+=count;
	}

	//0
	str[offset]=0;
	return offset;
}




//
void hexstr2datastr(char* dst, char* src, int count)
{
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
int hexstr2data(u8* src,u64* data)
{
	int j;
	*data=0;
	for(j=0;j<16;j++)
	{
		//say("%x\n",src[j]);
		//1.如果小于0x20，那就是结束了
		if(src[j]<=0x20) return j;

		//2.如果大于0x80，那就返回错误
		if(src[j]>=0x80) return -1;

		//3.如果是正常值
		*data=(*data)<<4;
		if(src[j]>=0x61 && src[j]<=0x66)
		{
			*data+=src[j]-0x61+10;
		}
		else if(src[j]>=0x30 && src[j]<=0x39)
		{
			*data+=src[j]-0x30;
		}
		else return -2;
	}
	return -999999;	//不会到这里
}
int data2hexstr(u64 data,u8* str)
{
	int j,max;
	u64 temp;

	max = 0;
	temp = data;
	for(j=0;j<0x10;j++)
	{
		max++;
		temp >>= 4;
		if(temp == 0)break;
	}

	for(j=1;j<=max;j++)
	{
		temp = data&0xf;
		data >>= 4;

		temp+=0x30;
		if(temp>=0x3a)temp+=7;
		str[max-j] = temp;
	}
	return max;
}




//in:  3.66,1244,  1.4,   86.34     0.3,    7.7      ,5
//out: float vector
int parsefv(float* vec, int flen, u8* str, int slen)
{
	int j,k,cnt;
	j = k = cnt = 0;

	while(1){
		while(0x20 == str[j])j++;
		j += decstr2float(str+j, &vec[cnt]);

		for(k=0;k<16;k++){
			//normally finished
			if('\n' == str[j+k])return cnt+1;

			//havenot finished
			if(',' == str[j+k]){
				j += k+1;
				break;
			}
		}
		if(16 == k)return cnt+1;

		cnt += 1;
	}

	//something wrong
	return 0;
}