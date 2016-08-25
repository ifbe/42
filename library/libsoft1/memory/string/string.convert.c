#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char*,...);




//
static char buf[0x1000];




//比如aa zec，得到aa\0ec
void blank2zero(u64* name)
{
	int i;
	u64 temp;
	for(i=0;i<8;i++)
	{
		temp = ( (*name)>>(i*8) )&0xff;
		if(temp == 0x20)
		{
			*name -= ((u64)0x20)<<(i*8);
		}
	}
}




//比如ac\0be，得到ac be
void zero2blank(u64* name)
{
	int i;
	u64 temp;
	for(i=0;i<8;i++)
	{
		temp = ( (*name)>>(i*8) )&0xff;
		if(temp == 0)
		{
			*name += ((u64)0x20)<<(i*8);
		}
	}
}




//比如abcefg，变成ABCEFG
void small2capital(u64* name)
{
	int i;
	u64 temp;
	for(i=0;i<8;i++)
	{
		temp = ( (*name) >> (i*8) )&0xff;
		if(temp>='a' && temp<='z')
		{
			*name -= ( (u64)0x20 )<<(i*8);
		}
	}
}




//anscii码转换成一个数字，比如anscii码串为0x36,0x33,转换后得到decimal=36
int decstring2data(u8* source,u64* data)
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




//anscii码转换成一个16进制数字，比如anscii码串为0x36,0x61,转换后得到hex=0x6a
int hexstring2data(u8* source,u64* data)
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




int data2hexstring(u64 data,u8* string)
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




int data2decimalstring(u64 data,u8* string)
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
	offset+=data2decimalstring( (u64)data , string+offset );

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
		count=data2decimalstring( (u64)temp , string+offset );
		offset+=count;
	}

	//0
	string[offset]=0;
}





/*
(buf,size) -> (argc,argv)
*/
void buf2arg(u8* buf,int max,int* argc,u8** argv)
{
	int i;
	int count=0;
	int splited=0;
	argv[0]=0;

	//
	for(i=0;i<max;i++)
	{
		//finished
		if( buf[i] == 0 )break;

		//blank
		if( buf[i] <= 0x20 )
		{
			buf[i]=0;
			splited=1;
			continue;
		}

		//new?
		if(splited != 0)
		{
			count++;
			if(count>=7)break;

			argv[count]=0;
			splited=0;
		}

		//new!
		if( argv[count]==0 )
		{
			argv[count]=buf+i;
		}
	}//for

	//result
	count+=1;
	argv[count]=0;
	*argc=count;

/*
	//debug
	say("count=%x\n",count);
	for(i=0;i<count;i++)
	{
		say("%x=%s\n",i,argv[i]);
	}
*/
}




void buf2addrport(u8* pp,int max,u8* addr,int* port)
{
	int ii;
	u64 data=0;

	for(ii=0;ii<max;ii++)
	{
		if(pp[ii]==0)break;
		else if(pp[ii]==':')break;
		else addr[ii]=pp[ii];
	}

	if(pp[ii]!=':')
	{
		addr[0]=0;
		port[0]=0;
	}
	else
	{
		addr[ii]=0;

		decstring2data(pp+ii+1,&data);
		*port=(int)data;
	}
}




/*
"card=wlan0" -> "card" , "wlan0"
"user=name" -> "user" , "name"
*/
int buf2optval(u8* pp,int max,u8** type,u8** name)
{
	int ii;
	for(ii=0;ii<max;ii++)
	{
		if(pp[ii]==0)break;
		if(pp[ii]=='=')break;
	}

	if( pp[ii] != '=' )
	{
		*type=0;
		*name=0;
	}
	else
	{
		pp[ii]=0;

		*type=pp;
		*name=pp+ii+1;
	}
}




/*
"1.txt" -> "txt"
"2.html" -> "html"
*/
int buf2suffix(u8* p,u8** suffix)
{
	int i=0;
	int tail=0;
	if(p==0)return 0;

	for(i=0;i<256;i++)
	{
		//all possible '.'
		if( p[i] == '.' )
		{
			tail=i;
		}

		//finished
		else if( p[i] == 0 )
		{
			//".gitignore" is not an extension
			if(tail==0)
			{
				*suffix=0;
			}
			else
			{
				*suffix = p + tail;
			}
			break;
		}
	}

	//say("suffix=%s\n",(char*)suffix);
	return 1;
}




/*
"file://code/readme.txt" -> 'file' , "code/readme.txt"
"http://192.168.1.1/index.html" -> 'http' , "192.168.1.1/index.html"
*/
int buf2typename(u8* p,int max,u64* type,u8** name)
{
	int i=0;
	int j=0;
	int tail=0;
	if(p==0)return 0;

	*type=0;
	*name=p;
	for(i=0;i<max;i++)
	{
		//finished
		if( p[i] == 0)break;

		//"://"
		if( p[i] == ':' )
		{
			if( (p[i+1]=='/') && (p[i+2]=='/') )
			{
				for(j=i-1;j>=0;j--)
				{
					*type = (*type) << 8;
					*type += p[j];
				}
				*name=p+i+3;
				break;
			}
		}
	}

	//say("type=%s,name=%s\n",(char*)type,*name);
	return 1;
}




char* buf2folder(char* p)
{
	int j = 0;
	int k = -1;
	while(1)
	{
		if(j>0x1000)break;
		if(p[j] == 0)break;
		if(p[j] == '/')k=j;

		buf[j]=p[j];
		j++;
	}

	if(k<0)return p;
	if(k==0)return "/";

	buf[k]=0;
	return buf;
}




char* buf2filename(char* p)
{
        int j=0;
        int k=0;
        while(1)
        {
                if(p[j] == 0)break;
                if(p[j] == '/')k=j+1;

                j++;
        }
        return p+k;
}
