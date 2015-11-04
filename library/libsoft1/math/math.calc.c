#define BYTE unsigned char
#define WORD unsigned short
#define DWORD unsigned int
#define QWORD unsigned long long
double cosine(double);
double sine(double);
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
