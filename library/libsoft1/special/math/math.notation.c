#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(char*,int);
void say(char*,...);




//得到haha：1<haha<10
//得到counter：十的多少次方
void kexuejishufa(double* haha,int* counter)
{
	//[ -0.00000000001 , 0.000000000000001 ]:    return 0 , 0
	if( (haha[0]>-0.0000000001) && (haha[0]<0.0000000001) )
	{
		haha[0] = 1.00;
		counter[0] = -100000;
		return;
	}

	//取符号
	int fuhao=1.00;
	if(haha[0]<0)
	{
		fuhao = -1.00;
		haha[0] = -haha[0];
	}

	//[-1,1]:
	if( haha[0] < 1.00 )
	{
		while( haha[0] < 1.00 )
		{
			//say("%lf,%d\n",haha[0],counter[0]);
			haha[0] *= 10.00;
			counter[0] --;
		}
	}

	//(-?,-1]&&[1,?):
	else
	{
		while( haha[0] > 10.00 )
		{
			//say("%lf,%d\n",haha[0],counter[0]);
			haha[0] /= 10.00;
			counter[0] ++;
		}
	}

	//还原符号
	haha[0] *= fuhao;
}
double huanyuan(double in,int count)
{
	if(count<0)
	{
		while(count<0)
		{
			in /= 10;
			count++;
		}
	}

	if(count>0)
	{
		while(count>0)
		{
			in *= 10;
			count--;
		}
	}

	return in;
}




//2	7	->	return 3
//0.019495	0.019777	->	return 0.0195
double beautifulbetween(double first,double second)
{
	int fuhao = 1;
	int exp1=0,exp2=0;
	double data1,data2,result;




	//错误返回两个平均值	//保证了下面first<=second
	if(first>=second)return first;




	//包含原点就返回0	//保证了下面first<second<0或者0<first<second
	//-9	38	->	return 0
	if(first<=0 && second>=0)return 0;




	//取出符号		//保证了下面0<first<second
	if(first<0)
	{
		fuhao = -1;
		first = -first;
		second = -second;
	}




	//科学记数法表示两个值，上几步保证了1 < data < 10
	data1=first;
	data2=second;
	exp1=0;
	exp2=0;
	kexuejishufa(&data1,&exp1);
	kexuejishufa(&data2,&exp2);




	//如果不在同一数量级返回大的取整(上面保证了exp1 <= exp2 !!!!!!!)
	//0.58	17.4	->	return 10
	if(exp1 < exp2)
	{
		result=huanyuan( 10 , exp1 );
		return result * fuhao;			//符号！
	}




	//同一数量级就麻烦了(举例: 3.141592653 , 3.141598888888888 -> return 3.141593 )
	int aa=0,bb=0,count=0;
	while(1)
	{
		//出错返回
		if(count>100)return first;




		//取出整数，剪掉整数
		aa=(int)data1;
		data1 -= (double)aa;

		bb=(int)data2;
		data2 -= (double)bb;




		//不相等就对了，把剩下的数剪掉完事
		if(aa != bb)
		{
			result=second + huanyuan(1-data1 , exp1-count);
			return result * fuhao;
		}




		//这次不行，再来一瓶
		data1 *= 10;
		data2 *=10;
		count ++;
	}
}
