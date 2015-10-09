#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
int decstring2data(BYTE* source,QWORD* data);
void printmemory(char*,int);




struct mathnode{

        DWORD type;
        DWORD up;
        DWORD left;
        DWORD right;
        union{
                char datasize[16];
                double floatpoint;
                unsigned long long integer;
        };
};




static double stack2[20];
static int count2=0;




static void initstack2()
{
	count2=20;
}
static int push(double data)
{
	//say("push %llf\n",data);

	//满栈
	if(count2==0)return 0;

	//count2-1(rsp-8)，然后放下这个数字
	count2--;
	stack2[count2]=data;
	return 1;
}
static int pop(double* dest)
{
	//空栈
	if(count2>=20)return 0;

	//拿出当前数字，然后count2+1(rsp+8)
	dest[0]=stack2[count2];
	count2++;

	//say("pop %llf\n",dest[0]);
	return 1;
}




/*
double _pow(double a, double b) {
    double c = 1;
    for (int i=0; i<b; i++)
        c *= a;
    return c;
}

double _fact(double x) {
    double ret = 1;
    for (int i=1; i<=x; i++) 
        ret *= i;
    return ret;
}

double _sin(double x) {
    double y = x;
    double s = -1;
    for (int i=3; i<=100; i+=2) {
        y+=s*(_pow(x,i)/_fact(i));
        s *= -1;
    }  
    return y;
}
double _cos(double x) {
    double y = 1;
    double s = -1;
    for (int i=2; i<=100; i+=2) {
        y+=s*(_pow(x,i)/_fact(i));
        s *= -1;
    }  
    return y;
}
double _tan(double x) {
     return (_sin(x)/_cos(x));  
}




double cosine(double x)
{
    double ret=0,item=1.0,temp;
    int n=0,i,sign=1;
    if(x>2*3.1415||x<-2*3.1415){x-=((int)(x/(2*3.1415)))*(2*3.1415);}

    do{
        temp=item;
        for(i=1;i<=2*n;i++)temp/=i;
        ret+=sign*temp;
        item*=x*x;
        sign *=-1;
        n++;
      }while (temp>1e-10);
return ret;
}




double sine(double x)
{
    int m = 1,i;
    double temp,ret = 0.0;
    if(x>2*3.1415||x<-2*3.1415){x-=((int)(x/(2*3.1415)))*(2*3.1415);}

    do{
        i=0;
        if (m%2 == 0){temp= -1.0;}
        else{temp= 1.0;}
        for(i=1;i<=2*m-1;i++){temp = temp * x/i;}
        ret+= temp;
        m++;
    }while (temp<-.0000005||temp>0.0000005);
return ret;
}




double squareroot(double x)
{
    double temp,ret;
    if(x<0){return 0;}

    temp=0.5*(1+x);
    ret=0.5*(temp+x/temp);
    while(1)
    {
	if( (ret-temp) >= 1e-10 ) break;
	if( (temp-ret) >= 1e-10 ) break;
        temp=ret;
        ret=0.5*(temp+x/temp);
    }
return ret;
}
*/








double floor(double x)
{
	register long double value;

	__volatile unsigned short int cw;
	__volatile unsigned short int cwtmp;
	__asm __volatile("fnstcw %0":"=m"(cw));
	cwtmp = (cw & 0xf3ff) | 0x0400;

	__asm __volatile("fldcw %0"::"m"(cwtmp));
	__asm __volatile("frndint":"=t"(value):"0"(x));
	__asm __volatile("fldcw %0"::"m"(cw));
	return value;
}




double squareroot(double x)
{
	double result;
	__asm __volatile__( "fsqrt"
	                    : "=t"(result)
	                    : "0"(x)
	);
	return result;
}
/*
float squareroot(float val)
{
    float result;
    __asm__ __volatile__ ( "fld %1;"
                           "fsqrt;"
                           "fstp %0;" 
                           : "=g" (result) 
                           : "g" (val)
    );
    return result ;
}
*/




double sine(double x)
{
	double result;
	__asm __volatile__( "fsin"
	                    : "=t"(result)
	                    : "0"(x)
	);
	return result;
}
/* Convert angle from degrees to radians and then calculate sin value */
/*
float sine(float degree)
{
    float result, two_right_angles = 180.0f ;
    __asm__ __volatile__ ( "fld %1;"
                           "fld %2;"
                           "fldpi;"
                           "fmul;"
                           "fdiv;"
                           "fsin;"
                           "fstp %0;"
                           : "=g" (result) 
                           : "g" (two_right_angles), "g" (degree)
    );
    return result ;
}
*/




double cosine(double x)
{
	double result;
	__asm __volatile__( "fcos"
	                    : "=t"(result)
	                    : "0"(x)
	);
	return result;
}
/* Convert angle from degrees to radians and then calculate cos value */
/*
float cosine( float degree )
{
    float result, two_right_angles = 180.0f, radians ;
    __asm__ __volatile__ ( "fld %1;"
                           "fld %2;"
                           "fldpi;"
                           "fmul;"
                           "fdiv;"
                           "fstp %0;" 
                           : "=g" (radians)
                           : "g"(two_right_angles), "g" (degree)
    );

    __asm__ __volatile__ ( "fld %1;"
                           "fcos;"
                           "fstp %0;" 
                           : "=g" (result)
                           : "g" (radians)
    );
    return result ;
}
*/




double myexp(double value, int exp)
{
	double temp, texp, temp2;
	texp = exp;

	__asm ("fscale " 
		: "=u" (temp2), "=t" (temp) 
		: "0" (texp), "1" (value)
	);
	return (temp);
}




double log2(double x)
{
	//return 1.00;		//........
    register double result;
    __asm __volatile__ ("fld1; fxch; fyl2x" 	//use instrucion,not algorithm
                      : "=t" (result) 		//output
                      : "0" (x) 		//input
                      : "st(1)");		//clobbered register
    return result;
}
double lg(double x)
{
	return log2(x)/log2(10);
}
double ln(double x)
{
	return log2(x) / log2(2.7182818284590452353602874713526624977572470936);
}
/*
double log(double what,double y)	//y=10^x	->	x=log(10,y)
{
	return log2(y)/log2(what);
}
*/




double calculator(char* postfix,QWORD x,QWORD y)
{
	int source=0;
	int count;
	QWORD data;
	double first,second,temp;

	initstack2();

	while(1)
	{
		//第1种：常量
		if( ( postfix[source] >= '0' ) && ( postfix[source] <= '9' ) )
		{
			//先拿整数部分
			count = decstring2data( postfix+source , &data );
			source += count;
			first = (double)data;

			//检查有没有小数部分有就加上
			if(postfix[source] == '.')
			{
				//say(".@%d\n",source);
				source++;
				count=decstring2data( postfix+source , &data );

				if(count>0)
				{
					source += count;
					temp = (double)data;

					while(1)
					{
						temp /= 10.00;

						count--;
						if(count==0)break;
					}

					//加上小数
					first+=temp;
				}

			}

			//保存这个double值
			push(first);
		}//是数字




		//第2种：变量
		else if(postfix[source]=='x')
		{
			//push((double)x);
			source++;
		}
		else if(postfix[source]=='y')
		{
			//push((double)y);
			source++;
		}
		else if(postfix[source]=='z')
		{
			//push((double)z);
			source++;
		}




		//第3种：单字节符号
		else if(postfix[source] == '+')
		{
			pop(&second);
			pop(&first);		//注意，栈，先进后出
			temp = first + second;
			push(temp);

			source++;
		}
		else if(postfix[source] == '-')
		{
			pop(&second);
			pop(&first);
			temp=first-second;
			push(temp);

			source++;
		}
		else if(postfix[source] == '*')
		{
			pop(&second);
			pop(&first);
			temp=first*second;
			push(temp);

			source++;
		}
		else if(postfix[source] == '/')
		{
			pop(&second);
			pop(&first);
			temp=first/second;
			push(temp);

			source++;
		}
		else if(postfix[source] == '^') 	//指数		x^y
		{
			pop(&second);
			pop(&first);

			temp=1.00;
			data=(QWORD)(second+0.000001);
			if(data!=0) while(1)
			{
				temp*=first;
				data--;
				if(data==0)break;
			}
			push(temp);

			source++;
		}
		else if(postfix[source] == '%') 	//取余		x%y
		{
			source++;
		}
		else if(postfix[source] == '!') 	//阶乘		x!
		{
			source++;
		}




		//第4种：多字节符号
		else if(postfix[source] == 'l')		//对数		xlogy
		{
			source++;
		}
		else if(postfix[source] == 's')
		{
			//根号		ysqrty
			//正弦		sinx
			source++;		//下一个
		}
		else if(postfix[source] == 'c') 	//余弦		cosx
		{
			source++;		//下一个
		}
		else if(postfix[source] == 't') 	//正切		tanx
		{
			source++;		//下一个
		}
		else
		{
			source++;			//其他不认识的不管，不加会死这儿
		}




		//检查退出while循环
		if(source>=128)break;
		if(postfix[source]==0)break;




	}//while结束

	pop(&temp);
	return temp;
}




double sketchpad(struct mathnode* node,double x,double y)
{
	int source=1;
	double first,second,temp;
	double result1,result2;
	result1=0;
	while(1)
	{
		if( node[source].type == 0x33323130 )	//0123...
		{
			push(node[source].floatpoint);
		}
		else if( node[source].type == 'x' )
		{
			push(x);
		}
		else if( node[source].type == 'y' )
		{
			push(y);
		}
		else if( node[source].type == 0x2f2a2d2b )		//+-*/...
		{
			if( node[source].integer == '+' )
			{
				pop(&second);
				pop(&first);            //注意，栈，先进后出
				push(first+second);
			}
			if( node[source].integer == '-' )
			{
				pop(&second);
				pop(&first);            //注意，栈，先进后出
				push(first-second);
			}
			if( node[source].integer == '*' )
			{
				pop(&second);
				pop(&first);            //注意，栈，先进后出
				push(first*second);
			}
			if( node[source].integer == '/' )
			{
				pop(&second);
				pop(&first);            //注意，栈，先进后出
				push(first/second);
			}
		}
		else if( node[source].type == 0x736f63 )	//cos
		{
			pop(&first);
			push( cosine(first) );
		}
		else if( node[source].type == 0x6e6973 )	//sin
		{
			pop(&first);
			push( sine(first) );
		}
		else if( node[source].type == 0x3d)	//=
		{
			pop(&result1);
		}
		else if(node[source].type == 0)
		{
			pop(&result2);
			result2-=result1;
			break;
		}

		//
		source++;
	}

	return result2;
}
